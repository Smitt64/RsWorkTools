#include "updatechecker.h"
#include "toolsruntime.h"
#include <QProcess>
#include <QFile>
#include <QTimer>
#include <QLocale>
#include <QTime>
#include <QThread>
#include <QDomDocument>

#define UpdateChecker15Min 120000
//900000

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
    QAtomicInt m_Interrupt, m_CheckUpdate;
    QScopedPointer<QProcess> process;
};

UpdateChecker::UpdateChecker(QObject *parent)
    : QObject{parent},
    d_ptr(new UpdateCheckerPrivate(this))
{
    Q_D(UpdateChecker);
    d->process.reset(new QProcess());

    qRegisterMetaType<CheckUpdateData>();
    qRegisterMetaType<CheckDataList>();
}

UpdateChecker::~UpdateChecker()
{

}

void UpdateChecker::setProgramName(const QString &name)
{
    Q_D(UpdateChecker);

    QString path = toolFullFileNameFromDir(name);

    if (!path.isEmpty() && QFile::exists(path))
        d->process->setProgram(path);
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

void UpdateChecker::run()
{
    Q_D(UpdateChecker);
    if (d->process->program().isEmpty())
        return;

    QLocale currentLocale = QLocale::system();

    d->process->setArguments(QStringList() << "check-updates");

    QTime lastStartTime = QTime::currentTime();
    forever
    {
        QThread::sleep(5);
        QTime curTime = QTime::currentTime();
        int secsTo = lastStartTime.msecsTo(curTime);
        if (secsTo >= d->m_Interval && d->m_CheckUpdate == 1)
        {
            lastStartTime = QTime::currentTime();
            emit checkStarted();

            d->process->start();
            d->process->waitForFinished();
            QString output = QString::fromLocal8Bit(d->process->readAllStandardOutput());

            if (output.contains("Critical"))
            {
                CheckDataList updatedata;
                QStringList lst = output.split("Critical");
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
                        updatedata.push_back(data);
                    }
                }

                emit checkFinished(false, updatedata);
            }
            else if (output.contains("There are currently no updates available"))
                emit checkFinished(false);
            else
            {
                const QString metaString = "Extracting meta information...";
                CheckDataList updatedata;

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
                                updatedata.push_back(data);
                            }
                            n = n.nextSibling();
                        }
                    }
                }
                emit checkFinished(true, updatedata);
            }
        }

        if (d->m_Interrupt.fetchAndAddAcquire(0) == 1)
            break;
    }
}
