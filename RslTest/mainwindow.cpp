// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rslexecutor.h"
#include "rsscript/rslstaticmodule.h"
#include "rsscript/registerobjlist.hpp"
#include "toolsruntime.h"
#include "optionsdlg/optionsdlg.h"
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
        //RegisterObjList::inst()->AddObject("TestObject");
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->setStyle(pSettings->value("Style", "windowsvista").toString());

    //ui->listView->setModel(&m_Errors);

    QAction *exec = ui->toolBar->addAction("Выполнить");

    QDockWidget *doc = new QDockWidget("Errors");

    CodeEditor *pEditor = new CodeEditor();
    pEditor->setReadOnly(true);
    pEditor->setPlainText(toolReadTextFileContent("Test.mac", "IBM 866"));
    setCentralWidget(pEditor);
    ToolApplyHighlighter(pEditor, HighlighterRsl);

    QListView *listView = new QListView(this);
    listView->setModel(&m_Errors);
    doc->setWidget(listView);
    addDockWidget(Qt::BottomDockWidgetArea, doc, Qt::Horizontal);

    RegisterObjList::inst()->RegisterRslObject<TestObject>();
    RegisterObjList::inst()->RegisterRslObject<ChildObject>();
    RegisterObjList::inst()->addStaticModule<TestModule, name>(new TestModule());

    connect(exec, &QAction::triggered, this, &MainWindow::on_pushButton_clicked);

    connect(ui->actionOptions, &QAction::triggered, [=]()
    {
        OptionsDlg dlg(pSettings, this);
        dlg.setDefaultStyle(DefaultStyleName);
        dlg.addStylePage(QString(), "style");
        dlg.addCodeEditorPage();
        dlg.exec();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    RslExecutor exec;

    auto Proc = [&exec]()
    {
        QVariant result = exec.call("TestFunc", {"User Message"});

        qDebug() << result;
    };

    exec.setDebugMacroFlag(true);
    exec.playRep("Test.mac", "1.txt", Proc);

    m_Errors.setStringList(exec.errors());
}

