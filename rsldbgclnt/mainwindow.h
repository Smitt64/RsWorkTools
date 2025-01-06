#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLoggingCategory>
#include <QStack>
//#define IGNORE_INCLUDE
#include "dbgserverproto.h"

Q_DECLARE_LOGGING_CATEGORY(dbg)

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CodeEditor;
class LogEventModel;
class StdViewDockWidget;
class CallStackModel;
class DbgEditorLineWidgetProvider;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum DBG_EVENTS
    {
        //EventBreakPoint = MSG_BREAKPOINT,
    };
    Q_ENUM(DBG_EVENTS);

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool connectToHost(const QString &hostName, quint16 port);
    QString lastError();

public slots:
    void dbgConnected();
    void dbgDisconnected();
    void readyRead();

protected:
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    virtual bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    QTextCodec *oem866;

    void InitDebugToolBar();
    void exec_continue(int trace_log);
    void applyCurrentStatement(const int &offs, const int &len, const int &line);

    Ui::MainWindow *ui;
    QString m_LastError;
    QScopedPointer<QTcpSocket> m_pSocket;
    QScopedPointer<LogEventModel> m_LogModel;
    QScopedPointer<CallStackModel> m_CallStackModel;

    QScopedPointer<StdViewDockWidget> m_LogDockWidget;
    QScopedPointer<StdViewDockWidget> m_StackDockWidget;

    CodeEditor *m_pCodeEditor;
    DbgEditorLineWidgetProvider *m_pCodeEditorProvider;

    QStack<DBGHEADER> m_LastHeader;
};
#endif // MAINWINDOW_H
