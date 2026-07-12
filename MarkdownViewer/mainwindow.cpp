// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "mainwindow.h"
#include "markdownview.h"
#include "markdownoptionspage.h"
#include "ribbon/applicationwidgetbase.h"
#include "ribbon/appoptionscontentwidget.h"
#include "ribbon/ribbonaboutdlg.h"
#include "toolsruntime.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDockWidget>
#include <QDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QMimeData>
#include <QProcess>
#include <QSettings>
#include <QShowEvent>
#include <QTabBar>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : SARibbonMainWindow(parent)
    , m_mdiArea(nullptr)
    , m_appWidget(nullptr)
    , m_optionsPage(nullptr)
    , m_settings(nullptr)
    , m_statusBar(nullptr)
{
    setWindowTitle(tr("Просмотр Markdown"));
    setWindowIcon(QIcon(QStringLiteral(":/markdownviewer/app-icon.svg")));
    resize(1280, 900);
    setAcceptDrops(true);

    m_settings = new QSettings(QDir::current().filePath("MarkdownViewer.ini"), QSettings::IniFormat, this);

    initMdiArea();
    initOutlineDock();
    initRibbon();
    initWindowButtonBar();
    initApplicationWidget();
    initStatusBar();
    loadSettings();
}

MainWindow::~MainWindow() = default;

void MainWindow::initStatusBar()
{
    m_statusBar = statusBar();
    updateStatusBar();
    connect(m_mdiArea, &QMdiArea::subWindowActivated,
            this, &MainWindow::updateStatusBar);
}

void MainWindow::updateStatusBar()
{
    if (!m_statusBar)
        return;

    QMdiSubWindow *active = m_mdiArea->activeSubWindow();
    if (!active)
    {
        m_statusBar->showMessage(tr("Нет открытых документов"));
        return;
    }

    MarkdownView *view = qobject_cast<MarkdownView *>(active->widget());
    if (!view)
    {
        m_statusBar->showMessage(tr("Нет открытых документов"));
        return;
    }

    const QString filePath = view->filePath();
    QString docName;
    QString sizeText;

    if (filePath.isEmpty())
    {
        docName = tr("Новый документ");
        sizeText = tr("%1 символов").arg(view->markdownSource().length());
    }
    else
    {
        const QFileInfo fi(filePath);
        docName = QDir::toNativeSeparators(filePath);
        const qint64 size = fi.size();
        if (size < 1024)
            sizeText = tr("%1 байт").arg(size);
        else if (size < 1024 * 1024)
            sizeText = tr("%1 КБ").arg(size / 1024);
        else
            sizeText = tr("%1 МБ").arg(size / (1024 * 1024));
    }

    const QString rendererName = (m_markdownSettings.renderer == MarkdownSettings::LocalQt)
                                     ? tr("Local Qt")
                                     : tr("GitHub API");

    m_statusBar->showMessage(tr("%1 | Размер: %2 | Рендерер: %3")
                                 .arg(docName, sizeText, rendererName));
}

void MainWindow::initMdiArea()
{
    m_mdiArea = new QMdiArea(this);
    m_mdiArea->setViewMode(QMdiArea::TabbedView);
    m_mdiArea->setDocumentMode(true);
    m_mdiArea->setTabsMovable(true);
    m_mdiArea->setTabsClosable(true);
    m_mdiArea->setTabPosition(QTabWidget::North);
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setBackground(QColor("#EBEFF2"));
    setCentralWidget(m_mdiArea);

    // Контекстное меню на вкладках MDI
    if (QTabBar *tabBar = m_mdiArea->findChild<QTabBar *>())
    {
        tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(tabBar, &QTabBar::customContextMenuRequested,
                this, &MainWindow::onTabContextMenuRequested);
    }
}

void MainWindow::initRibbon()
{
    SARibbonBar *ribbon = ribbonBar();
    ribbon->applicationButton()->setText(tr("Файл"));

    // Категория "Главная"
    SARibbonCategory *categoryMain = new SARibbonCategory(tr("Главная"), this);
    categoryMain->setObjectName("categoryMain");
    ribbon->addCategoryPage(categoryMain);

    SARibbonPannel *panelFile = new SARibbonPannel(tr("Файл"), this);
    panelFile->setObjectName("panelFile");
    categoryMain->addPannel(panelFile);

    QAction *actOpen = new QAction(QIcon::fromTheme("OpenFolder"), tr("Открыть"), this);
    actOpen->setObjectName("actOpen");
    actOpen->setShortcut(QKeySequence::Open);
    connect(actOpen, &QAction::triggered, this, &MainWindow::onOpenDocument);
    panelFile->addLargeAction(actOpen);
    addAction(actOpen);
    toolAddActionWithTooltip(actOpen, tr("Открыть Markdown-файл"), QKeySequence::Open);

    QAction *actSave = new QAction(QIcon::fromTheme("Save"), tr("Сохранить"), this);
    actSave->setObjectName("actSave");
    actSave->setShortcut(QKeySequence::Save);
    connect(actSave, &QAction::triggered, this, &MainWindow::onSaveDocument);
    panelFile->addLargeAction(actSave);
    addAction(actSave);
    toolAddActionWithTooltip(actSave, tr("Сохранить документ в HTML или PDF"), QKeySequence::Save);

    QAction *actFind = new QAction(QIcon::fromTheme("ListBoxSearch"), tr("Найти"), this);
    actFind->setObjectName("actFind");
    actFind->setShortcut(QKeySequence::Find);
    connect(actFind, &QAction::triggered, this, &MainWindow::onFindInPage);
    addAction(actFind);
    toolAddActionWithTooltip(actFind, tr("Найти текст на странице"), QKeySequence::Find);

    SARibbonPannel *panelSearch = new SARibbonPannel(tr("Поиск"), this);
    panelSearch->setObjectName("panelSearch");
    categoryMain->addPannel(panelSearch);
    panelSearch->addLargeAction(actFind);

    SARibbonPannel *panelWindow = new SARibbonPannel(tr("Окно"), this);
    panelWindow->setObjectName("panelWindow");
    categoryMain->addPannel(panelWindow);

    QAction *actClose = new QAction(QIcon::fromTheme("CloseDocument"), tr("Закрыть"), this);
    actClose->setObjectName("actClose");
    actClose->setShortcut(QKeySequence::Close);
    connect(actClose, &QAction::triggered, this, &MainWindow::onCloseActiveDocument);
    panelWindow->addLargeAction(actClose);
    addAction(actClose);
    toolAddActionWithTooltip(actClose, tr("Закрыть активную вкладку"), QKeySequence::Close);

    QAction *actCloseAll = new QAction(QIcon::fromTheme("CloseDocumentGroup"), tr("Закрыть все"), this);
    actCloseAll->setObjectName("actCloseAll");
    connect(actCloseAll, &QAction::triggered, this, &MainWindow::onCloseAllDocuments);
    panelWindow->addLargeAction(actCloseAll);
    toolAddActionWithTooltip(actCloseAll, tr("Закрыть все вкладки"));

    // Категория "Вид"
    SARibbonCategory *categoryView = new SARibbonCategory(tr("Вид"), this);
    categoryView->setObjectName("categoryView");
    ribbon->addCategoryPage(categoryView);

    SARibbonPannel *panelView = new SARibbonPannel(tr("Отображение"), this);
    panelView->setObjectName("panelView");
    categoryView->addPannel(panelView);

    QAction *actFullscreen = new QAction(QIcon::fromTheme("Run"), tr("Полный экран"), this);
    actFullscreen->setObjectName("actFullscreen");
    actFullscreen->setShortcut(Qt::Key_F11);
    connect(actFullscreen, &QAction::triggered, this, &MainWindow::onToggleFullscreen);
    panelView->addLargeAction(actFullscreen);
    addAction(actFullscreen);
    toolAddActionWithTooltip(actFullscreen, tr("Переключить полноэкранный режим"), Qt::Key_F11);

    QAction *actOutline = m_outlineDock->toggleViewAction();
    actOutline->setIcon(QIcon::fromTheme("NumericListBox"));
    actOutline->setText(tr("Структура"));
    actOutline->setShortcut(QKeySequence(tr("F7")));
    panelView->addLargeAction(actOutline);
    addAction(actOutline);
    toolAddActionWithTooltip(actOutline, tr("Показать или скрыть панель структуры документа"), QKeySequence(tr("F7")));
}

void MainWindow::initWindowButtonBar()
{
    SARibbonSystemButtonBar *wbar = windowButtonBar();
    if (!wbar)
        return;

    QAction *actAbout = new QAction(QIcon::fromTheme("HelpApplication"), tr("О программе"), this);
    actAbout->setObjectName("actAbout");
    connect(actAbout, &QAction::triggered, this, &MainWindow::onAbout);

    wbar->addAction(actAbout);
    wbar->addSeparator();
    toolAddActionWithTooltip(actAbout, tr("Показать информацию о программе"));
}

void MainWindow::initApplicationWidget()
{
    m_appWidget = new ApplicationWidgetBase(this);

    m_optionsPage = new MarkdownOptionsPage();
    m_optionsPage->setSettings(m_settings);
    m_optionsPage->setMarkdownSettings(m_markdownSettings);
    connect(m_optionsPage, &MarkdownOptionsPage::settingsChanged,
            this, &MainWindow::onSettingsChanged);

    AppOptionsContentWidget *optionsWidget = m_appWidget->optionsWidget();
    optionsWidget->setSettings(m_settings);
    optionsWidget->addPage(tr("Markdown"), QIcon::fromTheme("MarkdownFile"), m_optionsPage);

    // Добавляем виджет настроек как вкладку в ApplicationWidgetBase
    m_appWidget->addTab(tr("Настройки"), optionsWidget);
    m_appWidget->hide();

    connect(ribbonBar()->applicationButton(), &QAbstractButton::clicked, [this]() {
        m_appWidget->exec();
    });
}

void MainWindow::initOutlineDock()
{
    m_outlineDock = new QDockWidget(tr("Структура документа"), this);
    m_outlineDock->setObjectName("outlineDock");
    m_outlineDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_outlineTree = new QTreeWidget(m_outlineDock);
    m_outlineTree->setObjectName("outlineTree");
    m_outlineTree->setHeaderHidden(true);
    m_outlineTree->setRootIsDecorated(true);
    m_outlineDock->setWidget(m_outlineTree);

    addDockWidget(Qt::LeftDockWidgetArea, m_outlineDock);

    connect(m_mdiArea, &QMdiArea::subWindowActivated,
            this, &MainWindow::updateOutline);
    connect(m_outlineTree, &QTreeWidget::itemClicked,
            this, &MainWindow::onOutlineItemClicked);
}

void MainWindow::updateOutline()
{
    m_outlineTree->clear();

    QMdiSubWindow *active = m_mdiArea->activeSubWindow();
    if (!active)
        return;

    MarkdownView *view = qobject_cast<MarkdownView *>(active->widget());
    if (!view)
        return;

    const QString text = view->markdownSource();
    const QStringList lines = text.split('\n');

    // Иерархия: parents[level] = последний элемент данного уровня (1..6)
    QTreeWidgetItem *parents[7] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

    for (int i = 0; i < lines.size(); ++i)
    {
        const QString &line = lines.at(i);
        int level = 0;
        while (level < line.length() && level < 6 && line.at(level) == '#')
            ++level;

        if (level == 0 || level > 6)
            continue;

        QString title = line.mid(level).trimmed();
        if (title.isEmpty())
            continue;

        // Убираем завершающие '#', если они есть (закрывающие заголовок)
        while (!title.isEmpty() && title.endsWith('#'))
        {
            title.chop(1);
            title = title.trimmed();
        }

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, title);
        item->setIcon(0, QIcon::fromTheme("MarkdownFile"));
        item->setToolTip(0, title);
        item->setData(0, Qt::UserRole, i);

        QTreeWidgetItem *parent = (level > 1) ? parents[level - 1] : nullptr;
        if (parent)
            parent->addChild(item);
        else
            m_outlineTree->addTopLevelItem(item);

        parents[level] = item;
        // Сбрасываем более глубокие уровни: предыдущие вложенные элементы больше не актуальны
        for (int j = level + 1; j <= 6; ++j)
            parents[j] = nullptr;
    }

    m_outlineTree->expandAll();
}

void MainWindow::onOutlineItemClicked(QTreeWidgetItem *item, int /*column*/)
{
    if (!item)
        return;

    const int lineNumber = item->data(0, Qt::UserRole).toInt();

    QMdiSubWindow *active = m_mdiArea->activeSubWindow();
    if (!active)
        return;

    MarkdownView *view = qobject_cast<MarkdownView *>(active->widget());
    if (!view)
        return;

    view->scrollToLine(lineNumber);
}

void MainWindow::loadSettings()
{
    if (!m_settings)
        return;

    m_settings->beginGroup("Markdown");
    m_markdownSettings.renderer = m_settings->value("renderer", static_cast<int>(MarkdownSettings::GitHubApi)).toInt()
                                  == static_cast<int>(MarkdownSettings::LocalQt)
                                  ? MarkdownSettings::LocalQt
                                  : MarkdownSettings::GitHubApi;
    m_markdownSettings.cssUrl = m_settings->value("cssUrl", m_markdownSettings.cssUrl).toString();
    m_markdownSettings.githubApiVersion = m_settings->value("githubApiVersion", m_markdownSettings.githubApiVersion).toString();
    m_markdownSettings.githubMode = m_settings->value("githubMode", m_markdownSettings.githubMode).toString();
    m_settings->endGroup();

    if (m_optionsPage)
        m_optionsPage->setMarkdownSettings(m_markdownSettings);
}

void MainWindow::saveSettings()
{
    if (!m_settings)
        return;

    m_settings->beginGroup("Markdown");
    m_settings->setValue("renderer", static_cast<int>(m_markdownSettings.renderer));
    m_settings->setValue("cssUrl", m_markdownSettings.cssUrl);
    m_settings->setValue("githubApiVersion", m_markdownSettings.githubApiVersion);
    m_settings->setValue("githubMode", m_markdownSettings.githubMode);
    m_settings->endGroup();
    m_settings->sync();
}

void MainWindow::applySettingsToAllViews()
{
    const QList<QMdiSubWindow *> windows = m_mdiArea->subWindowList();
    for (QMdiSubWindow *subWindow : windows)
    {
        MarkdownView *view = qobject_cast<MarkdownView *>(subWindow->widget());
        if (view)
        {
            view->setSettings(m_markdownSettings);
            view->rerender();
        }
    }
}

void MainWindow::onSettingsChanged()
{
    if (!m_optionsPage)
        return;

    const MarkdownSettings newSettings = m_optionsPage->markdownSettings();
    if (m_markdownSettings == newSettings)
        return;

    m_markdownSettings = newSettings;
    saveSettings();
    applySettingsToAllViews();
    updateStatusBar();
}

void MainWindow::onTabContextMenuRequested(const QPoint &pos)
{
    QTabBar *tabBar = qobject_cast<QTabBar *>(sender());
    if (!tabBar)
        return;

    const int index = tabBar->tabAt(pos);
    if (index < 0)
        return;

    // Переключаемся на выбранную вкладку, чтобы корректно получить её документ
    tabBar->setCurrentIndex(index);

    QMdiSubWindow *subWindow = m_mdiArea->activeSubWindow();
    if (!subWindow)
        return;

    MarkdownView *view = qobject_cast<MarkdownView *>(subWindow->widget());
    if (!view)
        return;

    const QString filePath = view->filePath();
    if (filePath.isEmpty())
        return;

    QMenu menu(this);
    QAction *actShowInFolder = menu.addAction(tr("Показать в каталоге"));
    menu.addSeparator();
    QAction *actCopyFullPath = menu.addAction(tr("Копировать полный путь"));
    QAction *actCopyFileName = menu.addAction(tr("Копировать имя файла"));
    QAction *actCopyDir = menu.addAction(tr("Копировать путь к папке"));

    QAction *chosen = menu.exec(tabBar->mapToGlobal(pos));
    if (!chosen)
        return;

    if (chosen == actShowInFolder)
    {
        const QString nativePath = QDir::toNativeSeparators(filePath);
        QProcess::startDetached("explorer", QStringList() << QStringLiteral("/select,") << nativePath);
        return;
    }

    QClipboard *clipboard = QApplication::clipboard();
    if (chosen == actCopyFullPath)
        clipboard->setText(filePath);
    else if (chosen == actCopyFileName)
        clipboard->setText(QFileInfo(filePath).fileName());
    else if (chosen == actCopyDir)
        clipboard->setText(QFileInfo(filePath).absolutePath());
}

MarkdownView *MainWindow::createMarkdownView(const QString &filePath)
{
    MarkdownView *view = new MarkdownView();
    view->setSettings(m_markdownSettings);
    connect(view, &MarkdownView::fileDropped, this, &MainWindow::openFile);

    if (!filePath.isEmpty() && !view->loadFromFile(filePath))
    {
        qWarning() << "Failed to load markdown file:" << filePath;
        delete view;
        return nullptr;
    }

    QMdiSubWindow *subWindow = m_mdiArea->addSubWindow(view);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    QString title = view->documentTitle();
    if (title.isEmpty())
        title = filePath.isEmpty() ? tr("Новый документ") : QFileInfo(filePath).fileName();
    subWindow->setWindowTitle(title);
    subWindow->setWindowIcon(QIcon::fromTheme("MarkdownFile"));
    subWindow->showMaximized();

    return view;
}

QMdiSubWindow *MainWindow::findSubWindow(const QString &filePath) const
{
    if (filePath.isEmpty())
        return nullptr;

    const QList<QMdiSubWindow *> windows = m_mdiArea->subWindowList();
    for (QMdiSubWindow *subWindow : windows)
    {
        MarkdownView *view = qobject_cast<MarkdownView *>(subWindow->widget());
        if (view && view->filePath() == filePath)
            return subWindow;
    }
    return nullptr;
}

void MainWindow::onOpenDocument()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Открыть Markdown"),
        QString(),
        tr("Markdown файлы (*.md *.markdown);;Все файлы (*.*)"));

    if (filePath.isEmpty())
        return;

    openFile(filePath);
}

void MainWindow::openFile(const QString &filePath)
{
    if (filePath.isEmpty())
        return;

    if (QMdiSubWindow *existing = findSubWindow(filePath))
    {
        m_mdiArea->setActiveSubWindow(existing);
        return;
    }

    MarkdownView *view = createMarkdownView(filePath);
    if (view)
        view->setFocus();
}

void MainWindow::onSaveDocument()
{
    QMdiSubWindow *active = m_mdiArea->activeSubWindow();
    if (!active)
        return;

    MarkdownView *view = qobject_cast<MarkdownView *>(active->widget());
    if (!view)
        return;

    // Имя по умолчанию берём из исходного Markdown-файла или используем "document"
    QString suggestedName;
    const QString mdPath = view->filePath();
    if (!mdPath.isEmpty())
    {
        const QFileInfo fi(mdPath);
        suggestedName = fi.absolutePath() + QStringLiteral("/") + fi.baseName();
    }

    QString selectedFilter;
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Сохранить как"),
        suggestedName,
        tr("HTML файлы (*.html *.htm);;PDF файлы (*.pdf)"),
        &selectedFilter);

    if (filePath.isEmpty())
        return;

    // Если пользователь не указал расширение, подставляем по выбранному фильтру
    QString suffix = QFileInfo(filePath).suffix().toLower();
    if (suffix.isEmpty())
    {
        if (selectedFilter.contains(QStringLiteral("pdf"), Qt::CaseInsensitive))
        {
            filePath += QStringLiteral(".pdf");
            suffix = QStringLiteral("pdf");
        }
        else
        {
            filePath += QStringLiteral(".html");
            suffix = QStringLiteral("html");
        }
    }

    bool ok = false;
    if (suffix == QStringLiteral("html") || suffix == QStringLiteral("htm"))
        ok = view->saveAsHtml(filePath);
    else if (suffix == QStringLiteral("pdf"))
        ok = view->saveAsPdf(filePath);
    else
        QMessageBox::warning(this, tr("Ошибка"), tr("Неизвестный формат файла:\n%1").arg(filePath));

    if (!ok)
        QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось сохранить файл:\n%1").arg(filePath));
}

void MainWindow::onCloseActiveDocument()
{
    QMdiSubWindow *active = m_mdiArea->activeSubWindow();
    if (active)
        active->close();
}

void MainWindow::onCloseAllDocuments()
{
    m_mdiArea->closeAllSubWindows();
}

void MainWindow::onToggleFullscreen()
{
    isFullScreen() ? showNormal() : showFullScreen();
}

void MainWindow::onAbout()
{
    RibbonAboutDlg dlg(QStringLiteral(":/markdownviewer/about.json"),
                       QColor(91, 95, 199),
                       this);
    dlg.exec();
}

void MainWindow::onFindInPage()
{
    QMdiSubWindow *active = m_mdiArea->activeSubWindow();
    if (!active)
        return;

    MarkdownView *view = qobject_cast<MarkdownView *>(active->widget());
    if (!view)
        return;

    view->showSearchWidget();
}

void MainWindow::showEvent(QShowEvent *event)
{
    SARibbonMainWindow::showEvent(event);

    // Акцентный цвет приложения — индиго
    const QColor accentColor(91, 95, 199);

    //setRibbonTheme(SARibbonTheme::RibbonThemeOffice2013);

    QFile file(QStringLiteral(":/markdownviewer/theme-office2013-indigo.qss"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString qss = QString::fromUtf8(file.readAll());
        setStyleSheet(qss);
    }

    SARibbonBar *ribbon = ribbonBar();
    if (ribbon)
    {
        ribbon->setTabBarBaseLineColor(accentColor);
        ribbon->setWindowTitleTextColor(accentColor);
    }

    if (m_appWidget)
        m_appWidget->setMenuPanelColor(accentColor);

    setContentsMargins(2, 2, 2, 2);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
    else
        SARibbonMainWindow::dragEnterEvent(event);
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (!mimeData->hasUrls())
    {
        SARibbonMainWindow::dropEvent(event);
        return;
    }

    event->acceptProposedAction();

    const QList<QUrl> urls = mimeData->urls();
    for (const QUrl &url : urls)
    {
        if (!url.isLocalFile())
            continue;

        const QString filePath = url.toLocalFile();
        if (filePath.isEmpty())
            continue;

        // Открываем файл в новой вкладке
        QMetaObject::invokeMethod(this, "openFile", Qt::QueuedConnection, Q_ARG(QString, filePath));
    }
}
