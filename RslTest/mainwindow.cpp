// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rslexecutor.h"
#include "rsscript/rslstaticmodule.h"
#include "rsscript/registerobjlist.hpp"
#include "toolsruntime.h"
#include "optionsdlg/optionsdlg.h"
#include "optionsdlg/jsonsettings.h"
#include "spelling/spellcheckerdlg.h"
#include "spelling/spellchecker.h"
#include "spelling/spellstringsdlg.h"
#include <optionsdlg/rsloptionspage.h>
#include "codeeditor/codehighlighter.h"
#include <QVariant>
#include <QDebug>
#include <QDockWidget>
#include <QListView>
#include <QAction>
#include "codeeditor/codeeditor.h"
#include <QStyleFactory>
#include <QDebug>
#include <QSettings>
#include <QTabWidget>
#include <QSignalTransition>
#include <QStateMachine>
#include "windowactionsregistry.h"
#include <QSignalSpy>

//QSettings *pSettings;
Q_GLOBAL_STATIC_WITH_ARGS(QSettings, pSettings, ("RslTest.ini", QSettings::IniFormat));

constexpr static const char name[] = "TestModule";
const char *DefaultStyleName = "WindowsModernStyleOlive";

class TestModule : public RslStaticModule
{
public:

    TestModule() :
        RslStaticModule()
    {
    }

protected:
    void Proc()
    {
        addConstant("TEST_Constant", "TestConstant");
        RegisterObjList::inst()->AddObject<TestObject>();
        RegisterObjList::inst()->AddObject<ChildObject>();
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QSettings::Format format = (QSettings::Format)registerJsonSettingsFormat();
    //pSettings = new QSettings(format, QSettings::UserScope, "RslTest");

    qApp->setStyle(pSettings->value("Style", "windowsvista").toString());
    //ui->listView->setModel(&m_Errors);

    QAction *exec = ui->toolBar->addAction("Выполнить");
    exec->setObjectName("actionElementExec");

    QDockWidget *doc = new QDockWidget("Errors");

    QString macro = rslFindMacroFile("Test.mac");

    pEditor = new CodeEditor();
    pOutput = new CodeEditor();

    pEditor->setReadOnly(true);
    pOutput->setReadOnly(true);
    pEditor->setPlainText(toolReadTextFileContent(macro, "IBM 866"));

    pContainer = new QTabWidget(this);
    pContainer->addTab(pEditor, tr("Макрос"));
    pContainer->addTab(pOutput, tr("Вывод"));
    pContainer->setDocumentMode(true);
    setCentralWidget(pContainer);
    ToolApplyHighlighter(pEditor, HighlighterRsl);

    QListView *listView = new QListView(this);
    listView->setModel(&m_Errors);
    doc->setWidget(listView);
    addDockWidget(Qt::BottomDockWidgetArea, doc, Qt::Horizontal);

    ui->spellCheckerAction->setIcon(QIcon(":/img/CheckSpellingHS.png"));

    RegisterObjList::inst()->RegisterRslObject<TestObject>();
    RegisterObjList::inst()->RegisterRslObject<ChildObject>();
    RegisterObjList::inst()->addStaticModule<TestModule, name>(new TestModule());

    connect(exec, &QAction::triggered, this, &MainWindow::on_pushButton_clicked);

    connect(ui->actionOptions, &QAction::triggered, [=]()
    {
        OptionsDlg dlg(pSettings, this);
        dlg.setDefaultStyle(DefaultStyleName);
        dlg.addStylePage(QString(), "style");
        dlg.addCodeEditorPage("CodeEditor", "theme");
        dlg.addRslPage();
        dlg.addCommandsPage();
        dlg.addLogPage("RslTest");
        dlg.exec();
    });

    connect(ui->spellCheckerAction, &QAction::triggered, [=]()
    {
        SpellChecker *checker = nullptr;
        spellGetCheckerForLanguage("ru", &checker);

        //SpellCheckerDlg dlg(checker, this);
        //dlg.checkWord("приивет");
        SpellStringsDlg dlg(checker, this);
        dlg.appendString("Привет миир");
        dlg.appendString("тесст ощибка");
        dlg.exec();
    });

    ui->menu->insertAction(nullptr, exec);

    RslExecutor *executor = new RslExecutor();
    windowActionsRegistry()->scanActions(ui->menu);
    windowActionsRegistry()->scanActions(ui->menu_2);
    windowActionsRegistry()->setRslExecutor(executor);

    QList<QToolBar*> toolBars = windowActionsRegistry()->makeToolBars(pSettings, "UserCommands", "ToolBars");
    for (QToolBar *toolbar : toolBars)
        addToolBar(toolbar);

#ifdef _DEBUG
    windowActionsRegistry()->printActions();
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

class CheckedTransition : public QSignalTransition
{
public:
    CheckedTransition(TestObject *check)
        : QSignalTransition(check, SIGNAL(testSignal(int))) {}
protected:
    bool eventTest(QEvent *e)
    {
        if (!QSignalTransition::eventTest(e))
            return false;

        QStateMachine::SignalEvent *se = static_cast<QStateMachine::SignalEvent*>(e);
        qDebug() << se->arguments().at(0).toInt();
        return (se->arguments().at(0).toInt() == Qt::Checked);
    }
};

void MainWindow::on_pushButton_clicked()
{
    RslExecutor exec;

    /*auto Proc = [&exec]()
    {
        //QVariant result = exec.call("TestFunc", {"User Message"});

        //qDebug() << result;
    };*/

    pContainer->setCurrentIndex(1);
    pOutput->clear();
    connect(&exec, &RslExecutor::WriteOut, [=](const QString &str)
    {
        QTextCursor cursor = pOutput->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.insertText(str);
    });

    exec.setDebugMacroFlag(true);
    exec.playRep("Test.mac", "1.txt");

    m_Errors.setStringList(exec.errors());

    //obj = new TestObject();
    //spy = new QSignalSpy(obj, SIGNAL(testSignal(int)));
    /*QState *s1 = new QState();
    QState *s2 = new QState();
    CheckedTransition *t = new CheckedTransition(obj);
    s1->addTransition(t);

    QStateMachine m;
    m.addState(s1);
    m.addState(s2);*/
    //connect(obj, SIGNAL(testSignal(int)), SLOT(testSlot()));
    //emit obj->testSignal(10);
}

void MainWindow::testSlot()
{
    qDebug() << spy->signal() << spy->takeFirst();
}
