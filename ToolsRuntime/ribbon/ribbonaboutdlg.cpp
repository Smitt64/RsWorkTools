#include "ribbonaboutdlg.h"
#include "toolsruntime.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QStackedWidget>
#include <QWebEngineView>
#include <QTreeView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDir>
#include <QSvgRenderer>
#include <QTextStream>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QFrame>
#include <Windows.h>

// ---------------------------------------------------------------------------
// Helper: load & scale SVG/bitmap (copied from aboutdlg.cpp pattern)
// ---------------------------------------------------------------------------
static QPixmap ribbonLoadAndScaleImage(const QString &filePath, int width, int height,
                                       Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio)
{
    if (filePath.endsWith(".svg", Qt::CaseInsensitive) ||
        filePath.endsWith(".svgz", Qt::CaseInsensitive))
    {
        QSvgRenderer renderer(filePath);
        if (renderer.isValid())
        {
            QPixmap pixmap(width, height);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            if (aspectRatioMode == Qt::KeepAspectRatio)
            {
                QRectF viewBox = renderer.viewBoxF();
                qreal aspect = viewBox.width() / viewBox.height();
                int targetWidth, targetHeight;
                if (width / height > aspect)
                {
                    targetHeight = height;
                    targetWidth = static_cast<int>(height * aspect);
                }
                else
                {
                    targetWidth = width;
                    targetHeight = static_cast<int>(width / aspect);
                }
                int x = (width - targetWidth) / 2;
                int y = (height - targetHeight) / 2;
                renderer.render(&painter, QRectF(x, y, targetWidth, targetHeight));
            }
            else
            {
                renderer.render(&painter, QRectF(0, 0, width, height));
            }
            painter.end();
            return pixmap;
        }
    }

    QPixmap pixmap(filePath);
    if (!pixmap.isNull())
        return pixmap.scaled(width, height, aspectRatioMode, Qt::SmoothTransformation);
    return QPixmap();
}

// ---------------------------------------------------------------------------
// Private implementation
// ---------------------------------------------------------------------------
class RibbonAboutDlgPrivate
{
    Q_DECLARE_PUBLIC(RibbonAboutDlg)
public:
    RibbonAboutDlgPrivate(RibbonAboutDlg *obj, const QColor &accent)
        : q_ptr(obj)
        , m_accentColor(accent)
        , m_borderWidth(2)
        , m_WebView(nullptr)
        , m_componentsView(nullptr)
        , pComponentsModel(nullptr)
        , m_dragging(false)
    {
        m_currentLocale = QLocale::system();
    }

    // ---------- version ----------
    QString GetVersionNumberString()
    {
        QString versionNumberString;
        char moduleName[MAX_PATH + 1];
        GetModuleFileNameA(nullptr, moduleName, MAX_PATH);

        DWORD dummyZero;
        DWORD versionSize = GetFileVersionInfoSizeA(moduleName, &dummyZero);
        if (!versionSize)
            return QString();

        void *pVersion = malloc(versionSize);
        if (!pVersion)
            return QString();

        if (!GetFileVersionInfoA(moduleName, 0, versionSize, pVersion))
        {
            free(pVersion);
            return QString();
        }

        UINT length;
        VS_FIXEDFILEINFO *pFixInfo = nullptr;
        if (VerQueryValueA(pVersion, "\\", (LPVOID *)&pFixInfo, &length))
        {
            versionNumberString = QStringLiteral("%1.%2.%3.%4")
                .arg((pFixInfo->dwFileVersionMS >> 16) & 0xffff)
                .arg((pFixInfo->dwFileVersionMS >> 0) & 0xffff)
                .arg((pFixInfo->dwFileVersionLS >> 16) & 0xffff)
                .arg((pFixInfo->dwFileVersionLS >> 0) & 0xffff);
        }
        free(pVersion);
        return versionNumberString;
    }

    // ---------- components ----------
    void ReadComponents()
    {
        QString filename = toolFullFileNameFromDir("components.xml");
        if (!QFile::exists(filename))
            return;

        QDomDocument doc("components");
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
            return;
        if (!doc.setContent(&file))
        {
            file.close();
            return;
        }
        file.close();

        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();
        while (!n.isNull())
        {
            QDomElement e = n.toElement();
            if (!e.isNull() && e.tagName() == "Package")
                ReadComponentPackage(&e);
            n = n.nextSibling();
        }

        if (m_componentsView && pComponentsModel)
        {
            m_componentsView->expandAll();
            m_componentsView->resizeColumnToContents(0);
            m_componentsView->resizeColumnToContents(1);
        }
    }

    void ReadComponentPackage(QDomElement *Elem)
    {
        QMap<QString, QString> Package;
        QDomNode n = Elem->firstChild();
        while (!n.isNull())
        {
            QDomElement e = n.toElement();
            if (!e.isNull())
                Package.insert(e.tagName(), e.text());
            n = n.nextSibling();
        }

        QStandardItem *titleItem = new QStandardItem();
        titleItem->setText(Package["Title"]);

        QStandardItem *nameItem = new QStandardItem();
        nameItem->setText(Package["Name"]);

        QStandardItem *versionItem = new QStandardItem();
        versionItem->setText(Package["Version"]);

        qint64 size = Package["Size"].toLongLong();
        QStandardItem *sizeItem = new QStandardItem();
        sizeItem->setText(m_currentLocale.formattedDataSize(size, 2, QLocale::DataSizeTraditionalFormat));

        QDate installDate = QDate::fromString(Package["InstallDate"], Qt::ISODate);
        QString datesString = installDate.toString(Qt::SystemLocaleShortDate);
        if (!Package["LastUpdateDate"].isEmpty())
        {
            QDate lastUpdateDate = QDate::fromString(Package["LastUpdateDate"], Qt::ISODate);
            datesString += QStringLiteral("/%1").arg(lastUpdateDate.toString(Qt::SystemLocaleShortDate));
        }

        QStandardItem *datesItem = new QStandardItem();
        datesItem->setText(datesString);

        if (pComponentsModel)
            pComponentsModel->appendRow({ titleItem, nameItem, versionItem, sizeItem, datesItem });
    }

    // ---------- changelog ----------
    void ReadVersionsTitles()
    {
        QDir current = QDir::current();
        if (!current.cd("changelog"))
            return;

        for (const QFileInfo &fi : current.entryInfoList({"com.rs.*.xml"}))
        {
            QDomDocument doc;
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

        // Build projects navbar
        QString projects;
        QMapIterator<QString, QFileInfo> iter(m_Projects);
        while (iter.hasNext())
        {
            iter.next();
            if (project == iter.key())
                projects += QStringLiteral("<li><a class=\"active\" href=\"#%1\">%1</a></li>").arg(iter.key());
            else
                projects += QStringLiteral("<li><a href=\"#%1\">%1</a></li>").arg(iter.key());
        }

        // Load templates
        QString body_template = toolReadTextFileContent(":/about/ribbon_index_template.html");
        QString add_template = toolReadTextFileContent(":/about/add_template.html");
        QString fix_template = toolReadTextFileContent(":/about/fixed_template.html");

        // Compute derived colours from accent color
        QColor accent = m_accentColor;

        // Navbar: use accent directly (matches QHeaderView)
        QColor navbarColor = accent;
        // Navbar hover: slightly darker
        QColor navbarHoverColor(
            navbarColor.red() * 88 / 100,
            navbarColor.green() * 88 / 100,
            navbarColor.blue() * 88 / 100
        );
        // Active tab: darker than navbar to stand out
        QColor activeColor(
            navbarColor.red() * 72 / 100,
            navbarColor.green() * 72 / 100,
            navbarColor.blue() * 72 / 100
        );

        // "Fixed" badge: complementary hue (hue + 170°) to distinguish from "New"
        QColor fixedColor = accent.toHsv();
        int fixedHue = (fixedColor.hue() + 170) % 360;
        fixedColor.setHsv(fixedHue,
                          qMin(fixedColor.saturation(), 200),
                          fixedColor.value());

        // Replace CSS variables in template
        body_template.replace("{%navbar-bg%}", navbarColor.name());
        body_template.replace("{%navbar-hover-bg%}", navbarHoverColor.name());
        body_template.replace("{%active-bg%}", activeColor.name());
        body_template.replace("{%new-badge-bg%}", QString("rgba(%1,%2,%3,0.1)")
                                  .arg(accent.red()).arg(accent.green()).arg(accent.blue()));
        body_template.replace("{%new-badge-text%}", accent.name());
        body_template.replace("{%fixed-badge-bg%}", QString("rgba(%1,%2,%3,0.1)")
                                  .arg(fixedColor.red()).arg(fixedColor.green()).arg(fixedColor.blue()));
        body_template.replace("{%fixed-badge-text%}", fixedColor.name());
        body_template.replace("{%code-color%}", accent.name());

        body_template.replace("{%projects%}", projects);
        body_template.replace("{%Project%}", project);

        // Parse XML and build version entries
        QDomDocument doc;
        QString body = toolReadTextFileContent(m_Projects[project].absoluteFilePath());
        doc.setContent(body);

        QString version;
        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();
        while (!n.isNull())
        {
            QDomElement e = n.toElement();
            version += QStringLiteral("<h2 class=\"hs-docs-heading\"><code>v%1</code></h2>").arg(e.attribute("ver"));

            QDomNode fix = e.firstChild();
            while (!fix.isNull())
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

        QString content_divider = QStringLiteral("<div class=\"content-divider\">%1</div>").arg(version);
        body_template.replace("{%content_divider%}", content_divider);

        // Write to temp file and load
        QDir tmp = QDir::temp();
        QFile tempfile(tmp.absoluteFilePath(QStringLiteral("%1_ribbon.html").arg(project)));
        if (tempfile.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&tempfile);
            stream.setCodec("Utf-8");
            stream << body_template.toUtf8();
            tempfile.close();
            m_WebView->load(QUrl::fromLocalFile(tempfile.fileName()));
        }
    }

    // ---------- UI pointers ----------
    RibbonAboutDlg *q_ptr;

    QColor m_accentColor;
    int m_borderWidth;

    // Title bar widgets
    QWidget *m_titleBar;
    QLabel *m_logoLabel;
    QLabel *m_titleLabel;
    QLabel *m_versionLabel;
    QLabel *m_descriptionLabel;
    QToolButton *m_closeButton;
    QFrame *m_titleSeparator;

    // Tab bar
    QWidget *m_tabBar;
    QToolButton *m_changelogBtn;
    QToolButton *m_componentsBtn;
    QFrame *m_tabSeparator;

    // Content
    QStackedWidget *m_stackedWidget;
    QWebEngineView *m_WebView;
    QTreeView *m_componentsView;
    QStandardItemModel *pComponentsModel;

    // Bottom
    QLabel *m_bottomLabel;

    // Drag state
    bool m_dragging;
    QPoint m_dragStartPos;

    // Changelog data
    QMap<QString, QFileInfo> m_Projects;
    QLocale m_currentLocale;
};

// ---------------------------------------------------------------------------
// RibbonAboutDlg implementation
// ---------------------------------------------------------------------------
RibbonAboutDlg::RibbonAboutDlg(const QString &config,
                               const QColor &accentColor,
                               QWidget *parent)
    : QDialog(parent)
    , d_ptr(new RibbonAboutDlgPrivate(this, accentColor))
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground, false);
    setAttribute(Qt::WA_DeleteOnClose, false);
    setFixedSize(720, 540);
    setMouseTracking(true);

    Q_D(RibbonAboutDlg);

    // Build UI
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(d->m_borderWidth, d->m_borderWidth,
                                   d->m_borderWidth, d->m_borderWidth);
    mainLayout->setSpacing(0);

    // --- Title bar ---
    d->m_titleBar = new QWidget(this);
    d->m_titleBar->setFixedHeight(120);
    d->m_titleBar->installEventFilter(this);

    QHBoxLayout *titleLayout = new QHBoxLayout(d->m_titleBar);
    titleLayout->setContentsMargins(20, 12, 12, 12);
    titleLayout->setSpacing(16);

    d->m_logoLabel = new QLabel(d->m_titleBar);
    d->m_logoLabel->setFixedSize(96, 96);
    d->m_logoLabel->setAlignment(Qt::AlignCenter);
    titleLayout->addWidget(d->m_logoLabel);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(4);

    d->m_titleLabel = new QLabel(d->m_titleBar);
    d->m_titleLabel->setWordWrap(true);
    d->m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    infoLayout->addWidget(d->m_titleLabel);

    d->m_versionLabel = new QLabel(d->m_titleBar);
    infoLayout->addWidget(d->m_versionLabel);

    d->m_descriptionLabel = new QLabel(d->m_titleBar);
    d->m_descriptionLabel->setWordWrap(true);
    d->m_descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    infoLayout->addWidget(d->m_descriptionLabel);

    infoLayout->addStretch();
    titleLayout->addLayout(infoLayout, 1);

    // Close button
    d->m_closeButton = new QToolButton(d->m_titleBar);
    d->m_closeButton->setFixedSize(36, 36);
    d->m_closeButton->setFocusPolicy(Qt::NoFocus);
    d->m_closeButton->setAutoRaise(true);
    d->m_closeButton->setCursor(Qt::ArrowCursor);
    connect(d->m_closeButton, &QToolButton::clicked, this, &QDialog::close);
    titleLayout->addWidget(d->m_closeButton, 0, Qt::AlignTop | Qt::AlignRight);

    d->m_titleBar->setLayout(titleLayout);
    mainLayout->addWidget(d->m_titleBar);

    // --- Separator: title → tab bar ---
    d->m_titleSeparator = new QFrame(this);
    d->m_titleSeparator->setFrameShape(QFrame::HLine);
    d->m_titleSeparator->setFixedHeight(2);
    d->m_titleSeparator->setObjectName(QStringLiteral("titleSeparator"));
    mainLayout->addWidget(d->m_titleSeparator);

    // --- Tab bar ---
    d->m_tabBar = new QWidget(this);
    d->m_tabBar->setFixedHeight(36);
    QHBoxLayout *tabBarLayout = new QHBoxLayout(d->m_tabBar);
    tabBarLayout->setContentsMargins(16, 0, 16, 0);
    tabBarLayout->setSpacing(0);

    d->m_changelogBtn = new QToolButton(d->m_tabBar);
    d->m_changelogBtn->setText(tr("История изменений"));
    d->m_changelogBtn->setObjectName(QStringLiteral("tabChangelog"));
    d->m_changelogBtn->setCheckable(true);
    d->m_changelogBtn->setChecked(true);
    d->m_changelogBtn->setAutoRaise(false);
    d->m_changelogBtn->setFocusPolicy(Qt::NoFocus);
    d->m_changelogBtn->setCursor(Qt::PointingHandCursor);
    d->m_changelogBtn->setFixedHeight(32);

    d->m_componentsBtn = new QToolButton(d->m_tabBar);
    d->m_componentsBtn->setText(tr("Компоненты"));
    d->m_componentsBtn->setObjectName(QStringLiteral("tabComponents"));
    d->m_componentsBtn->setCheckable(true);
    d->m_componentsBtn->setAutoRaise(false);
    d->m_componentsBtn->setFocusPolicy(Qt::NoFocus);
    d->m_componentsBtn->setCursor(Qt::PointingHandCursor);
    d->m_componentsBtn->setFixedHeight(32);

    tabBarLayout->addWidget(d->m_changelogBtn);
    tabBarLayout->addWidget(d->m_componentsBtn);
    tabBarLayout->addStretch();
    d->m_tabBar->setLayout(tabBarLayout);
    mainLayout->addWidget(d->m_tabBar);

    // --- Separator: tab bar → content ---
    d->m_tabSeparator = new QFrame(this);
    d->m_tabSeparator->setFrameShape(QFrame::HLine);
    d->m_tabSeparator->setFixedHeight(2);
    d->m_tabSeparator->setObjectName(QStringLiteral("tabSeparator"));
    mainLayout->addWidget(d->m_tabSeparator);

    // --- Stacked content ---
    d->m_stackedWidget = new QStackedWidget(this);

    d->m_WebView = new QWebEngineView(d->m_stackedWidget);
    d->m_stackedWidget->addWidget(d->m_WebView);

    d->m_componentsView = new QTreeView(d->m_stackedWidget);
    d->m_componentsView->setRootIsDecorated(false);
    d->m_componentsView->setAlternatingRowColors(true);
    d->m_componentsView->setSelectionMode(QAbstractItemView::SingleSelection);
    d->m_componentsView->setFocusPolicy(Qt::NoFocus);
    d->m_componentsView->header()->setStretchLastSection(true);

    d->pComponentsModel = new QStandardItemModel(this);
    d->pComponentsModel->setHorizontalHeaderLabels({
        tr("Компонент"), tr("ID"), tr("Версия"),
        tr("Размер"), tr("Установлен/Обновлен")
    });
    d->m_componentsView->setModel(d->pComponentsModel);
    d->m_componentsView->header()->resizeSection(0, 180);
    d->m_componentsView->header()->resizeSection(1, 220);

    d->m_stackedWidget->addWidget(d->m_componentsView);
    d->m_stackedWidget->setCurrentIndex(0);
    mainLayout->addWidget(d->m_stackedWidget, 1);

    // --- Bottom bar ---
    d->m_bottomLabel = new QLabel(this);
    d->m_bottomLabel->setText(tr("<b>Версия Qt: </b>%1").arg(QT_VERSION_STR));
    d->m_bottomLabel->setFixedHeight(32);
    d->m_bottomLabel->setAlignment(Qt::AlignCenter);
    d->m_bottomLabel->setObjectName(QStringLiteral("bottomLabel"));
    mainLayout->addWidget(d->m_bottomLabel);

    setLayout(mainLayout);

    // --- Connect tab buttons ---
    connect(d->m_changelogBtn, &QToolButton::clicked, [d]() {
        d->m_changelogBtn->setChecked(true);
        d->m_componentsBtn->setChecked(false);
        d->m_stackedWidget->setCurrentIndex(0);
    });
    connect(d->m_componentsBtn, &QToolButton::clicked, [d]() {
        d->m_changelogBtn->setChecked(false);
        d->m_componentsBtn->setChecked(true);
        d->m_stackedWidget->setCurrentIndex(1);
    });

    // --- Load config JSON ---
    QFile file(config);
    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject obj = doc.object();

        QString debugSuffix;
#ifdef _DEBUG
        debugSuffix = QStringLiteral(" (DEBUG)");
#endif
        d->m_titleLabel->setText(QStringLiteral("<html><body><p style=\"font-size:16pt; font-weight:600;\">%1%2</p></body></html>")
                                     .arg(obj["application"].toString(), debugSuffix));

        QPixmap scaledLogo = ribbonLoadAndScaleImage(obj["logo"].toString(), 96, 96);
        d->m_logoLabel->setPixmap(scaledLogo);

        d->m_descriptionLabel->setText(obj["description"].toString());
        d->m_versionLabel->setText(QStringLiteral("<b>%1: </b>%2")
                                       .arg(tr("Версия"), d->GetVersionNumberString()));

        d->ReadVersionsTitles();
        d->RenderHtmlForProject(obj["initalproject"].toString());

        file.close();
    }

    // --- Components ---
    d->ReadComponents();

    connect(d->m_WebView, &QWebEngineView::urlChanged,
            this, &RibbonAboutDlg::urlChanged);

    // Apply all styles
    applyStyles();
}

RibbonAboutDlg::~RibbonAboutDlg()
{
    delete d_ptr;
}

void RibbonAboutDlg::setAccentColor(const QColor &color)
{
    Q_D(RibbonAboutDlg);
    d->m_accentColor = color;
    applyStyles();
    update();
}

// ---------------------------------------------------------------------------
// Styles
// ---------------------------------------------------------------------------
void RibbonAboutDlg::applyStyles()
{
    Q_D(RibbonAboutDlg);

    QString accentName = d->m_accentColor.name();
    QString accentR = QString::number(d->m_accentColor.red());
    QString accentG = QString::number(d->m_accentColor.green());
    QString accentB = QString::number(d->m_accentColor.blue());

    // ---------- Title bar: white ----------
    d->m_titleBar->setStyleSheet("background-color: white;");

    // Title bar labels: dark text (only direct QLabel, no borders)
    d->m_titleLabel->setStyleSheet("QLabel { color: #222222; background: transparent; border: none; }");
    d->m_versionLabel->setStyleSheet("QLabel { color: #333333; background: transparent; border: none; }");
    d->m_descriptionLabel->setStyleSheet("QLabel { color: #333333; background: transparent; border: none; }");

    // ---------- Accent separator: title → tab bar ----------
    d->m_titleSeparator->setStyleSheet(
        QStringLiteral("QFrame#titleSeparator { background-color: %1; border: none; }").arg(accentName));

    // ---------- Close button: dark icon, red hover ----------
    d->m_closeButton->setStyleSheet(
        "QToolButton {"
        "  background-color: transparent;"
        "  border: none;"
        "  icon-size: 18px;"
        "}"
        "QToolButton:hover {"
        "  background-color: #e81123;"
        "  border-radius: 2px;"
        "}"
        "QToolButton:pressed {"
        "  background-color: #f1707a;"
        "}"
    );
    {
        QPixmap closePix(18, 18);
        closePix.fill(Qt::transparent);
        QPainter p(&closePix);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(QPen(QColor("#666666"), 2));
        p.drawLine(3, 3, 15, 15);
        p.drawLine(15, 3, 3, 15);
        p.end();
        d->m_closeButton->setIcon(QIcon(closePix));
    }

    // ---------- Tab bar: light grey ----------
    d->m_tabBar->setStyleSheet("background-color: #F5F5F5;");

    // Tab buttons
    QString tabBtnStyle =
        QStringLiteral(
            "QToolButton {"
            "  background-color: transparent;"
            "  color: #555555;"
            "  border: none;"
            "  padding: 4px 16px;"
            "  font-size: 12px;"
            "  font-family: 'Segoe UI', sans-serif;"
            "}"
            "QToolButton:hover {"
            "  background-color: rgba(0,0,0,0.06);"
            "}"
            "QToolButton:checked {"
            "  background-color: transparent;"
            "  color: %1;"
            "  border-bottom: 2px solid %1;"
            "}"
        ).arg(accentName);
    d->m_changelogBtn->setStyleSheet(tabBtnStyle);
    d->m_componentsBtn->setStyleSheet(tabBtnStyle);

    // ---------- Accent separator: tab bar → content ----------
    d->m_tabSeparator->setStyleSheet(
        QStringLiteral("QFrame#tabSeparator { background-color: %1; border: none; }").arg(accentName));

    // ---------- Stacked widget ----------
    d->m_stackedWidget->setStyleSheet("background-color: #F0F0F0;");

    // ---------- Tree view ----------
    d->m_componentsView->setStyleSheet(
        QStringLiteral(
            "QTreeView {"
            "  background-color: #F0F0F0;"
            "  alternate-background-color: #FAFAFA;"
            "  border: none;"
            "  font-size: 12px;"
            "  color: #333333;"
            "}"
            "QTreeView::item {"
            "  padding: 4px 0px;"
            "}"
            "QTreeView::item:selected {"
            "  background-color: %1;"
            "  color: white;"
            "}"
            "QHeaderView::section {"
            "  background-color: %1;"
            "  color: white;"
            "  padding: 6px 8px;"
            "  border: none;"
            "  border-right: 1px solid rgba(255,255,255,0.2);"
            "  font-weight: bold;"
            "  font-size: 12px;"
            "}"
        ).arg(accentName)
    );

    // ---------- Bottom bar: light, thin accent top border ----------
    d->m_bottomLabel->setStyleSheet(
        QStringLiteral(
            "QLabel {"
            "  background-color: #FAFAFA;"
            "  color: #888888;"
            "  font-size: 11px;"
            "  padding: 4px;"
            "  border-top: 1px solid rgba(%1,%2,%3,0.3);"
            "}"
        ).arg(accentR, accentG, accentB)
    );
}

// ---------------------------------------------------------------------------
// Paint border
// ---------------------------------------------------------------------------
void RibbonAboutDlg::paintEvent(QPaintEvent *event)
{
    Q_D(RibbonAboutDlg);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw the accent-colored border around the whole window
    painter.setPen(QPen(d->m_accentColor, d->m_borderWidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5));

    QDialog::paintEvent(event);
}

// ---------------------------------------------------------------------------
// Event filter for title bar dragging
// ---------------------------------------------------------------------------
bool RibbonAboutDlg::eventFilter(QObject *obj, QEvent *event)
{
    Q_D(RibbonAboutDlg);

    if (obj == d->m_titleBar)
    {
        switch (event->type())
        {
        case QEvent::MouseButtonPress:
        {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            if (me->button() == Qt::LeftButton)
            {
                d->m_dragging = true;
                d->m_dragStartPos = me->globalPos() - frameGeometry().topLeft();
                event->accept();
                return true;
            }
            break;
        }
        case QEvent::MouseButtonRelease:
        {
            d->m_dragging = false;
            event->accept();
            return true;
        }
        case QEvent::MouseMove:
        {
            if (d->m_dragging)
            {
                QMouseEvent *me = static_cast<QMouseEvent *>(event);
                move(me->globalPos() - d->m_dragStartPos);
                event->accept();
                return true;
            }
            break;
        }
        default:
            break;
        }
    }

    return QDialog::eventFilter(obj, event);
}

// ---------------------------------------------------------------------------
// URL change -> re-render changelog for the clicked project
// ---------------------------------------------------------------------------
void RibbonAboutDlg::urlChanged(const QUrl &url)
{
    Q_D(RibbonAboutDlg);
    QString fragment = url.fragment();
    if (fragment.isEmpty())
        return;

    d->m_WebView->blockSignals(true);
    d->RenderHtmlForProject(fragment);
    d->m_WebView->blockSignals(false);
}