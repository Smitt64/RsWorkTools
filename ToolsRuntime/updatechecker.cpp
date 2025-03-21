// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "updatechecker.h"
#include "toolsruntime.h"
#include <QProcess>
#include <QFile>
#include <QTimer>
#include <QLocale>
#include <QTime>
#include <QThread>
#include <QDomDocument>
#include <QDebug>
#include <QRegExp>
#include <QSettings>
#include <QStandardItemModel>

#define UpdateChecker15Min 900000

class UpdateCheckerPrivate
{
    Q_DECLARE_PUBLIC(UpdateChecker)
public:
    UpdateCheckerPrivate(UpdateChecker *obj)
    {
        q_ptr = obj;
        m_Interval = UpdateChecker15Min;
        m_CheckUpdate = 1;
    }

    UpdateChecker *q_ptr;

    int m_Interval;
    QString m_Program;
    QAtomicInt m_Interrupt, m_CheckUpdate;
};

UpdateChecker::UpdateChecker(QObject *parent)
    : QObject{parent},
    d_ptr(new UpdateCheckerPrivate(this))
{
    Q_D(UpdateChecker);

    qRegisterMetaType<CheckUpdateData>();
    qRegisterMetaType<CheckDataList>();
}

UpdateChecker::~UpdateChecker()
{
    delete d_ptr;
}

void UpdateChecker::setSettings(QSettings *prm)
{
    Q_D(UpdateChecker);
    prm->beginGroup("Update");
    if (!prm->value("Autocheck", true).toBool())
        d->m_CheckUpdate = 0;

    QTime Interval = prm->value("Interval", QTime(0, 15)).toTime();
    d->m_Interval = Interval.msecsSinceStartOfDay();
    prm->endGroup();
}

void UpdateChecker::setProgramName(const QString &name)
{
    Q_D(UpdateChecker);

    QString path = toolFullFileNameFromDir(name);

    if (!path.isEmpty() && QFile::exists(path))
        d->m_Program = path;
}

void UpdateChecker::setInterval(int msec)
{
    Q_D(UpdateChecker);
    d->m_Interval = msec;
}

void UpdateChecker::requestInterruption()
{
    Q_D(UpdateChecker);
    d->m_Interrupt.testAndSetOrdered(0, 1);
}

void UpdateChecker::setCheckUpdateFlag(bool value)
{
    Q_D(UpdateChecker);

    if (value)
        d->m_CheckUpdate.testAndSetOrdered(0, 1);
    else
        d->m_CheckUpdate.testAndSetOrdered(1, 0);
}

void UpdateChecker::MakeUpdateModel(QStandardItemModel **model, const CheckDataList &lst, QObject *parent)
{
    (*model) = new QStandardItemModel(parent);
    (*model)->setHorizontalHeaderLabels({tr("Наименование"),
                                      tr("Версия"),
                                      tr("Размер")});

    for(const CheckUpdateData &item : lst)
    {
        QStandardItem *name = new QStandardItem();
        name->setText(item.name);

        QStandardItem *version = new QStandardItem();
        version->setText(item.version);

        QStandardItem *size = new QStandardItem();
        size->setText(item.sizeString);

        (*model)->appendRow({name, version, size});
    }
}

void UpdateChecker::checkUpdate(CheckDataList *updatedata)
{
    Q_D(UpdateChecker);

    QScopedPointer<QProcess> process;
    process.reset(new QProcess());

    QLocale currentLocale = QLocale::system();
    process->setProgram(d->m_Program);
    process->setArguments(QStringList() << "check-updates");

    process->start();
    process->waitForStarted();
    process->waitForFinished();

    QString output = QString::fromLocal8Bit(process->readAllStandardOutput());
    qInfo(logUpdate()) << "StandardOutput:" << output;

    if (output.contains("Critical") || output.contains("Warning"))
    {
        QStringList lst = output.split(QRegExp("Critical|Warning"));
        for (const QString &str : qAsConst(lst))
        {
            if (str.front() == ':')
            {
                CheckUpdateData data;
                data.size = -1;

                int pos = str.indexOf(':');
                data.name = str.mid(pos + 1).trimmed();
                data.name = data.name.remove(QRegExp("\\[\\d+\\]")).simplified();

                if (str.contains("Preparing meta"))
                {
                    pos = data.name.indexOf("Preparing meta");
                    data.name = data.name.mid(0, pos);
                }

                updatedata->push_back(data);
            }
        }

        emit checkFinished(false, *updatedata);
    }
    else if (output.contains("There are currently no updates available"))
        emit checkFinished(false);
    else
    {
        const QString metaString = "Extracting meta information...";

        int pos = output.indexOf(metaString);
        if (pos != -1)
        {
            QString xmlcontent = output.mid(pos + metaString.length() + 1).simplified();
            //qDebug() << xmlcontent;
            QDomDocument doc;
            if (doc.setContent(xmlcontent))
            {
                QDomElement docElem = doc.documentElement();
                QDomNode n = docElem.firstChild();
                while(!n.isNull())
                {
                    QDomElement e = n.toElement();
                    if(!e.isNull())
                    {
                        CheckUpdateData data;
                        data.id = e.attribute("id");
                        data.name = e.attribute("name");
                        data.version = e.attribute("version");
                        data.size = e.attribute("size").toLongLong();
                        data.sizeString = currentLocale.formattedDataSize(data.size, 2, QLocale::DataSizeTraditionalFormat);
                        updatedata->push_back(data);
                    }
                    n = n.nextSibling();
                }
            }
        }
        emit checkFinished(true, *updatedata);
    }
}

void UpdateChecker::run()
{
    Q_D(UpdateChecker);
    if (d->m_Program.isEmpty())
        return;

    QScopedPointer<QProcess> process;
    process.reset(new QProcess());

    QLocale currentLocale = QLocale::system();
    process->setProgram(d->m_Program);
    process->setArguments(QStringList() << "check-updates");

    QTime lastStartTime = QTime::currentTime();
    forever
    {
        QThread::sleep(10);
        QTime curTime = QTime::currentTime();
        int secsTo = lastStartTime.msecsTo(curTime);
        if (secsTo >= d->m_Interval && d->m_CheckUpdate == 1)
        {
            lastStartTime = QTime::currentTime();
            emit checkStarted();

            CheckDataList updatedata;
            checkUpdate(&updatedata);
        }
        else
        {
            if (d->m_CheckUpdate == 0)
                lastStartTime = QTime::currentTime();
        }

        if (d->m_Interrupt.fetchAndAddAcquire(0) == 1)
            break;
    }
}
