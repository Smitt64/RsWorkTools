// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "mainwindow.h"
#include "ribbon/applicationwidgetbase.h"
#include "ribbon/ribboncommandscontentwidget.h"
#include "ribbon/ribboncommandstorage.h"
#include "rslexecutor.h"
#include "windowactionsregistry.h"

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QXmlStreamReader>
#include <QDebug>

#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : SARibbonMainWindow(parent)
    , m_appWidget(nullptr)
    , m_actionsManager(nullptr)
    , m_executor(nullptr)
    , m_settings(nullptr)
    , m_hasApplyCustomizeXml(false)
{
    setWindowTitle(tr("Ribbon Macro Test"));
    resize(1200, 800);

    m_settings = new QSettings(QDir::current().filePath("RibbonMacroTest.ini"), QSettings::IniFormat, this);
    m_executor = new RslExecutor(this);
    windowActionsRegistry()->setRslExecutor(m_executor);

    initRibbon();
    loadMacrosAndApplyCustomization();
    initApplicationWidget();
}

MainWindow::~MainWindow()
{
    if (m_appWidget && m_appWidget->macroActionsContentWidget())
        m_appWidget->macroActionsContentWidget()->save(m_settings);
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

    QAction *actOpen = new QAction(style()->standardIcon(QStyle::SP_DialogOpenButton), tr("Открыть"), this);
    actOpen->setObjectName("actOpen");
    connect(actOpen, &QAction::triggered, this, []() {
        qDebug() << "Open triggered";
    });
    panelFile->addLargeAction(actOpen);

    QAction *actSave = new QAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("Сохранить"), this);
    actSave->setObjectName("actSave");
    connect(actSave, &QAction::triggered, this, []() {
        qDebug() << "Save triggered";
    });
    panelFile->addLargeAction(actSave);

    // Общий обработчик для макрокоманд на Ribbon
    connect(ribbonBar(), &SARibbonBar::actionTriggered, this, [this](QAction *action) {
        QString macrofile = action->data().toString();
        if (!macrofile.isEmpty())
            m_executor->playRep(macrofile);
    });

    // Категория "Вид"
    SARibbonCategory *categoryView = new SARibbonCategory(tr("Вид"), this);
    categoryView->setObjectName("categoryView");
    ribbon->addCategoryPage(categoryView);

    SARibbonPannel *panelView = new SARibbonPannel(tr("Отображение"), this);
    panelView->setObjectName("panelView");
    categoryView->addPannel(panelView);

    QAction *actFullscreen = new QAction(style()->standardIcon(QStyle::SP_TitleBarMaxButton), tr("Полный экран"), this);
    actFullscreen->setObjectName("actFullscreen");
    connect(actFullscreen, &QAction::triggered, this, [this]() {
        isFullScreen() ? showNormal() : showFullScreen();
    });
    panelView->addLargeAction(actFullscreen);

    // Менеджер действий создаётся после построения Ribbon
    m_actionsManager = new SARibbonActionsManager(ribbon);
    m_actionsManager->setParent(this);
}

void MainWindow::loadMacrosAndApplyCustomization()
{
    if (!m_actionsManager)
        return;

    // Загрузка списка макрокоманд
    m_settings->beginGroup("RibbonCommands");
    QByteArray commandsData = QByteArray::fromBase64(m_settings->value("CommandList").toByteArray());
    QJsonArray commandsJson = QJsonDocument::fromJson(commandsData).array();
    m_settings->endGroup();

    for (const QJsonValue &value : commandsJson)
    {
        QJsonObject obj = value.toObject();
        QString id = obj.value("id").toString();
        QString macrofile = obj.value("macrofile").toString();
        QString title = obj.value("title").toString();
        QString icon = obj.value("icon").toString();

        if (id.isEmpty() || macrofile.isEmpty())
            continue;

        QAction *act = new QAction(QIcon(icon), title, m_actionsManager);
        act->setObjectName(id);
        act->setData(macrofile);

        m_actionsManager->registeAction(act, SARibbonActionsManager::UserDefineActionTag + 1, id);
    }

    m_actionsManager->setTagName(SARibbonActionsManager::UserDefineActionTag + 1, tr("Макрокоманды"));

    // Применение сохранённой XML-настройки Ribbon (только один раз)
    if (!m_hasApplyCustomizeXml)
    {
        m_settings->beginGroup("RibbonCommands");
        QByteArray ribbonXml = QByteArray::fromBase64(m_settings->value("RibbonCustomization").toByteArray());
        m_settings->endGroup();

        if (!ribbonXml.isEmpty())
        {
            QString xmlPath = QDir::temp().filePath("ribbon_macro_test_customize.xml");
            QFile f(xmlPath);
            if (f.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                f.write(ribbonXml);
                f.close();
            }
            m_hasApplyCustomizeXml = sa_apply_customize_from_xml_file(xmlPath, ribbonBar(), m_actionsManager);
        }
        else
        {
            m_hasApplyCustomizeXml = true;
        }
    }
}

void MainWindow::initApplicationWidget()
{
    m_appWidget = new ApplicationWidgetBase(this);
    m_appWidget->addMacroActionsContentWidget(m_actionsManager);

    RibbonCommandsContentWidget *macroWidget = m_appWidget->macroActionsContentWidget();
    if (macroWidget)
        macroWidget->load(m_settings);

    connect(ribbonBar()->applicationButton(), &QAbstractButton::clicked, [this]() {
        m_appWidget->show();
    });
}
