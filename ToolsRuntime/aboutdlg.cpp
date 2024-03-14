#include "aboutdlg.h"
#include "ui_aboutdlg.h"
#include "toolsruntime.h"
#include <Windows.h>
#include <QStandardItemModel>
#include <QWebEngineView>
#include <QDomDocument>
#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

class AboutDlgPrivate
{
    Q_DECLARE_PUBLIC(AboutDlg)
public:
    AboutDlgPrivate(AboutDlg *obj)
    {
        q_ptr = obj;
    }

    QString GetVersionNumberString()
    {
        QString versionNumberString;
        char moduleName[MAX_PATH + 1];
        GetModuleFileNameA(NULL, moduleName, MAX_PATH);

        DWORD dummyZero;
        DWORD versionSize = GetFileVersionInfoSizeA(moduleName, &dummyZero);

        if(!versionSize)
            return QString();

        void* pVersion = malloc(versionSize);
        if(!pVersion)
            return QString();

        if(!GetFileVersionInfoA(moduleName, NULL, versionSize, pVersion))
        {
            free(pVersion);
            return QString();
        }

        UINT length;
        VS_FIXEDFILEINFO* pFixInfo;
        if (VerQueryValueA(pVersion, "\\", (LPVOID*)&pFixInfo, &length))
        {
            versionNumberString = QString("%1.%2.%3.%4")
                                      .arg((pFixInfo->dwFileVersionMS >> 16) & 0xffff)
                                      .arg((pFixInfo->dwFileVersionMS >>  0) & 0xffff)
                                      .arg((pFixInfo->dwFileVersionLS >> 16) & 0xffff)
                                      .arg((pFixInfo->dwFileVersionLS >>  0) & 0xffff);
        }
        free(pVersion);

        return versionNumberString;
    }

    void ReadComponents()
    {
        Q_Q(AboutDlg);

        QString filename = toolFullFileNameFromDir("components.xml");
        if (QFile::exists(filename))
        {
            QDomDocument doc("components");
            QFile file(filename);
            if (!file.open(QIODevice::ReadOnly))
            {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
                q->ui->tabWidget->setTabVisible(1, false);
#else
                q->ui->tabWidget->removeTab(1);
#endif
                return;
            }

            if (!doc.setContent(&file))
            {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
                q->ui->tabWidget->setTabVisible(1, false);
#else
                q->ui->tabWidget->removeTab(1);
#endif
                file.close();
                return;
            }
            file.close();

            QDomElement docElem = doc.documentElement();
            QDomNode n = docElem.firstChild();
            while(!n.isNull())
            {
                QDomElement e = n.toElement();
                if(!e.isNull())
                {
                    if (e.tagName() == "Package")
                        ReadComponentPackage(&e);
                }
                n = n.nextSibling();
            }
        }
        else
        {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
            q->ui->tabWidget->setTabVisible(1, false);
#else
            q->ui->tabWidget->removeTab(1);
#endif
        }
    }

    void ReadComponentPackage(QDomElement *Elem)
    {
        QMap<QString,QString> Package;

        QDomNode n = Elem->firstChild();
        while(!n.isNull())
        {
            QDomElement e = n.toElement();
            if(!e.isNull())
                Package.insert(e.tagName(), e.text());

            n = n.nextSibling();
        }

        QStandardItem *TitleItem = new QStandardItem();
        TitleItem->setText(Package["Title"]);

        QStandardItem *NameItem = new QStandardItem();
        NameItem->setText(Package["Name"]);

        QStandardItem *VersionItem = new QStandardItem();
        VersionItem->setText(Package["Version"]);

        qint64 Size = Package["Size"].toLongLong();
        QStandardItem *SizeItem = new QStandardItem();
        SizeItem->setText(currentLocale.formattedDataSize(Size, 2, QLocale::DataSizeTraditionalFormat));

        QDate InstallDate = QDate::fromString(Package["InstallDate"], Qt::ISODate);
        QString datesString = InstallDate.toString(Qt::SystemLocaleShortDate);

        if (!Package["LastUpdateDate"].isEmpty())
        {
            QDate LastUpdateDate = QDate::fromString(Package["LastUpdateDate"], Qt::ISODate);
            datesString += QString("/%1")
                               .arg(LastUpdateDate.toString(Qt::SystemLocaleShortDate));
        }

        QStandardItem *DatesItem = new QStandardItem();
        DatesItem->setText(datesString);

        pComponentsModel->appendRow(QList<QStandardItem*>()
                                    << TitleItem
                                    << NameItem
                                    << VersionItem
                                    << SizeItem
                                    << DatesItem);
    }

    AboutDlg *q_ptr;

    QStandardItemModel *pComponentsModel;
    QWebEngineView *m_WebView;
    QLocale currentLocale;
};

AboutDlg::AboutDlg(const QString &config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg),
    d_ptr(new AboutDlgPrivate(this))
{
    Q_D(AboutDlg);
    ui->setupUi(this);

    d->currentLocale = QLocale::system();
    d->pComponentsModel = new QStandardItemModel(this);
    d->pComponentsModel->setHorizontalHeaderLabels(QStringList()
                                                << tr("Компонент")
                                                << tr("ID")
                                                << tr("Версия")
                                                << tr("Размер")
                                                << tr("Установлен/Обновлен"));

    QHBoxLayout *pLayout = new QHBoxLayout(ui->tab);
    pLayout->setMargin(0);
    d->m_WebView = new QWebEngineView(ui->tab);

    ui->tab->setLayout(pLayout);
    ui->tab->layout()->addWidget(d->m_WebView);
    ui->labelBased->setText(tr("<b>Версия Qt: </b>%1").arg(QT_VERSION_STR));

    ui->tabWidget->setCurrentIndex(0);
    ui->componentsView->setModel(d->pComponentsModel);
    ui->componentsView->header()->resizeSection(0, 200);
    ui->componentsView->header()->resizeSection(1, 250);

    ui->labelVersion->setText(tr("<b>Версия: </b>%1").arg(d->GetVersionNumberString()));

    d->ReadComponents();

    QFile file(config);

    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject obj = doc.object();

        QString tmpl = "<html><head/><body><p><span style=\"font-size:14pt; font-weight:600;\">%1 %2</span></p></body></html>";
        ui->applicationTitle->setText(tmpl.arg(obj["application"].toString(), QString(" %1")));
        ui->label->setPixmap(QPixmap(obj["logo"].toString()));
        ui->description->setText(obj["description"].toString());

        file.close();
    }
}

AboutDlg::~AboutDlg()
{
    delete ui;
}
