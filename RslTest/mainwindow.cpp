#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rslexecutor.h"
#include "rsscript/rslstaticmodule.h"
#include "rsscript/registerobjlist.hpp"
#include "toolsruntime.h"
#include <QVariant>
#include <QDebug>
#include <QDockWidget>
#include <QListView>
#include <QAction>
#include "codeeditor/codeeditor.h"
#include <QStyleFactory>

constexpr static const char name[] = "TestModule";

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
    qDebug() << QStyleFactory::keys();
    ui->setupUi(this);

    //ui->listView->setModel(&m_Errors);

    QAction *exec = ui->toolBar->addAction("Выполнить");

    QDockWidget *doc = new QDockWidget("Errors");

    CodeEditor *pEditor = new CodeEditor();
    pEditor->setReadOnly(true);
    pEditor->setPlainText(toolReadTextFileContent("Test.mac"));
    setCentralWidget(pEditor);

    QListView *listView = new QListView(this);
    listView->setModel(&m_Errors);
    doc->setWidget(listView);
    addDockWidget(Qt::BottomDockWidgetArea, doc, Qt::Horizontal);

    RegisterObjList::inst()->RegisterRslObject<TestObject>();
    RegisterObjList::inst()->RegisterRslObject<ChildObject>();
    RegisterObjList::inst()->addStaticModule<TestModule, name>(new TestModule());

    connect(exec, &QAction::triggered, this, &MainWindow::on_pushButton_clicked);
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
    //ui->l
    //qDebug() << exec.errors();
}

