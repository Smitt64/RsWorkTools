#include <Windows.h>
#include "mainwindow.h"
#include "logdockwidget.h"
#include "logevent.h"
#include "models/logeventmodel.h"
#include "ui_mainwindow.h"
#include "dbgeditorlinewidgetprovider.h"
#include <dbgserverproto.h>
#include <codeeditor/codeeditor.h>
#include <codeeditor/cppcodehighlighter.h>
//#include <QIODevice>

Q_LOGGING_CATEGORY(dbg, "rsldbg")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    m_pSocket(new QTcpSocket())
{
    ui->setupUi(this);
    m_LogModel.reset(new LogEventModel());
    m_LogDockWidget.reset(new LogDockWidget());
    m_LogDockWidget->setModel(m_LogModel.data());

    m_pCodeEditor = new CodeEditor(this);
    m_pCodeEditor->setReadOnly(true);
    m_pCodeEditor->setAutoHighlightCurrentLine(false);
    setCentralWidget(m_pCodeEditor);

    m_pCodeEditorProvider = new DbgEditorLineWidgetProvider();
    m_pCodeEditor->setCodeEditorLineWidgetProvider(m_pCodeEditorProvider);

    ToolApplyHighlighter(m_pCodeEditor, HighlighterRsl);

    addDockWidget(Qt::BottomDockWidgetArea, m_LogDockWidget.data());

    connect(m_pSocket.data(), &QTcpSocket::connected, this, &MainWindow::dbgConnected);
    connect(m_pSocket.data(), &QTcpSocket::disconnected, this, &MainWindow::dbgDisconnected);
}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug(dbg()) << "~MainWindow()";
}

QString MainWindow::lastError()
{
    return m_LastError;
}

void MainWindow::dbgConnected()
{
    qInfo(dbg()) << "Connected...";
}

void MainWindow::dbgDisconnected()
{
    qInfo(dbg()) << "Disconnected...";
}

bool MainWindow::connectToHost(const QString &hostName, quint16 port)
{
    bool hr = true;
    m_pSocket->connectToHost(hostName, port);

    qInfo(dbg()) << "Try connect to" << hostName << ":" << port;
    m_pSocket->waitForConnected();
    qInfo(dbg()) << "State" << m_pSocket->state();

    if (m_pSocket->state() != QAbstractSocket::ConnectedState)
    {
        m_LastError = QString("Can't connecto to debug host %1:%2")
                        .arg(hostName)
                        .arg(port);
        qCritical(dbg()) << m_LastError;
        return false;
    }

    m_pSocket->waitForReadyRead();
    QByteArray headerdata = m_pSocket->read(sizeof(DBGHEADER));

    if (headerdata.size() != sizeof(DBGHEADER))
    {
        m_LastError = QString("Invalid header size: expected %1, received %2")
            .arg(sizeof(headerdata))
            .arg(headerdata.size());

        qCritical(dbg()) << m_LastError << headerdata;

        m_pSocket->disconnectFromHost();
        return false;
    }

    DBGHEADER *hdr = (DBGHEADER*)headerdata.data();
    if (!DbgCheckHeader(hdr))
    {
        m_LastError = "Invalid header magic";
        qCritical(dbg()) << m_LastError;

        m_pSocket->disconnectFromHost();
        return false;
    }

    if (hdr->action != DBG_REQUEST_HANDSHAKE)
    {
        m_LastError = "Invalid header action";
        qCritical(dbg()) << m_LastError;

        m_pSocket->disconnectFromHost();
        return false;
    }

    qInfo(dbg()) << "connectToHost" << hr;
    connect(m_pSocket.data(), &QTcpSocket::readyRead, this, &MainWindow::readyRead);

    if (m_pSocket->bytesAvailable())
        readyRead();

    return hr;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_pSocket->disconnectFromHost();
    QMainWindow::closeEvent(event);
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == MSG_LOG)
    {
        LogEvent *log = (LogEvent*)event;
        m_LogModel->append(log);
        //event->accept();
    }

    return QMainWindow::event(event);
}

void MainWindow::readyRead()
{
    QByteArray data = m_pSocket->readAll();
    QDataStream stream(&data, QIODevice::ReadOnly);

    DBGHEADER header;
    stream.readRawData((char*)&header, sizeof(DBGHEADER));

    if (!DbgCheckHeader(&header))
    {
        m_LastError = "Invalid header magic";
        qCritical(dbg()) << m_LastError;
        return;
    }

    if (header.action == DBG_REQUEST_EVENT)
    {
        DBGEVENT *event = (DBGEVENT*)(((char*)data.data()) + sizeof(DBGHEADER));

        if (event->event == MSG_BREAKPOINT)
        {
            DBGBPEVENT *bp = (DBGBPEVENT*)event;
            qDebug(dbg()) << "Handle breakpoint event. Is setted:" << bp->BpSetted;
        }
        else
            qDebug(dbg()) << "Handle unknown event:" << event->event;
    }
    //qDebug(dbg()) << data;
}
