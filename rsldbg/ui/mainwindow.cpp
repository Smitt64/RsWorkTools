#include "mainwindow.h"
#include "cdebug.h"
#include "ui_mainwindow.h"
#include <QDebug>

#define tracemsg (*((CTraceMsg*)m_tracemsg))

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_curdbg(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_tracemsg = new CTraceMsg();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit closed();
    QMainWindow::closeEvent(event);
}

void MainWindow::AddTraceMsg(const QString &str, const int &level)
{
    TTraceMsg* msg = new TTraceMsg;
    msg->msg = str;
    msg->level = (MSGLEVEL)level;

    elem_tracemsg msg_aptr (msg);
    tracemsg.push_back(msg_aptr);

    qDebug() << str;
}
