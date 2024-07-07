#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rslexecutor.h"
#include "rsscript/rslstaticmodule.h"
#include "rsscript/registerobjlist.hpp"
#include <QVariant>
#include <QDebug>

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
    ui->setupUi(this);

    RegisterObjList::inst()->RegisterRslObject<TestObject>();
    RegisterObjList::inst()->RegisterRslObject<ChildObject>();
    RegisterObjList::inst()->addStaticModule<TestModule, name>(new TestModule());
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

    qDebug() << exec.errors();
}

