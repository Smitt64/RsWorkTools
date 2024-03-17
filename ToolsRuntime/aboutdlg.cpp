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
#include <QDir>

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

    void ReadVersionsTitles()
    {
        QDir current = QDir::current();
        current.cd("changelog");

        for (auto fi : current.entryInfoList(QStringList() << "com.rs.*.xml"))
        {
            QDomDocument doc(fi.absoluteFilePath());
            QString body = toolReadTextFileContent(fi.absoluteFilePath());
            doc.setContent(body);

            QDomElement docElem = doc.documentElement();
            m_Projects.insert(docElem.attribute("project"), fi);
        }
    }

    void RenderHtmlForProject(const QString &project)
    {
        if (project.isEmpty() || !m_Projects.contains(project))
            return;

        QString projects;
        QString body_template = toolReadTextFileContent(":/about/index_template.html");
        QString version_template = toolReadTextFileContent(":/about/index_template.html");

        QString add_template = toolReadTextFileContent(":/about/add_template.html");
        QString fix_template = toolReadTextFileContent(":/about/fixed_template.html");

        QMapIterator<QString,QFileInfo> iter(m_Projects);
        while(iter.hasNext())
        {
            iter.next();

            if (project == iter.key())
                projects += QString("<li><a class=\"active\" href=\"#%1\">%1</a></li>").arg(iter.key());
            else
                projects += QString("<li><a href=\"#%1\">%1</a></li>").arg(iter.key());
        }

        body_template = body_template.replace("{%projects%}", projects);
        body_template = body_template.replace("{%Project%}", project);

        QDomDocument doc;
        QString body = toolReadTextFileContent(m_Projects[project].absoluteFilePath());
        doc.setContent(body);

        QString version;
        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();
        while(!n.isNull())
        {
            QDomElement e = n.toElement();
            version += QString("<h2 class=\"hs-docs-heading\"><code>v%1</code></h2>").arg(e.attribute("ver"));

            QDomNode fix = e.firstChild();
            while(!fix.isNull())
            {
                QDomElement efix = fix.toElement();

                if (efix.tagName() == "fix")
                    version += fix_template.arg(efix.text());
                else if (efix.tagName() == "add")
                    version += add_template.arg(efix.text());

                fix = fix.nextSibling();
            }

            n = n.nextSibling();
        }

        QString content_divider = QString("<div class=\"content-divider\">%1</div>").arg(version);
        body_template = body_template.replace("{%content_divider%}", content_divider);

        QDir tmp = QDir::temp();
        QFile tempfile(tmp.absoluteFilePath(QString("%1.html").arg(project)));
        if (tempfile.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&tempfile);
            stream.setCodec("Utf-8");
            stream << body_template.toUtf8();
            tempfile.close();

            m_WebView->load(tempfile.fileName());
        }
    }

    AboutDlg *q_ptr;

    QStandardItemModel *pComponentsModel;
    QWebEngineView *m_WebView;
    QLocale currentLocale;
    QMap<QString,QFileInfo> m_Projects;

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
    d->ReadVersionsTitles();
    QFile file(config);

    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject obj = doc.object();

        QString tmpl = "<html><head/><body><p><span style=\"font-size:14pt; font-weight:600;\">%1 %2</span></p></body></html>";
        QString title = tmpl.arg(obj["application"].toString(), QString(" %1"));

#ifdef _DEBUG
        title = title.arg("(DEBUG)");
#else
        title = title.arg("");
#endif

        ui->applicationTitle->setText(title);
        ui->label->setPixmap(QPixmap(obj["logo"].toString()));
        ui->description->setText(obj["description"].toString());
        d->RenderHtmlForProject(obj["initalproject"].toString());

        file.close();
    }


    connect(d->m_WebView, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
}

AboutDlg::~AboutDlg()
{
    delete ui;
}

void AboutDlg::urlChanged(const QUrl &url)
{
    Q_D(AboutDlg);
    QString fragment = url.fragment();

    if (fragment.isEmpty())
        return;

    d->m_WebView->blockSignals(true);
    d->RenderHtmlForProject(fragment);
    d->m_WebView->blockSignals(false);
}
