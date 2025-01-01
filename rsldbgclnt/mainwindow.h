#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLoggingCategory>
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
class LogDockWidget;
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
    Ui::MainWindow *ui;
    QString m_LastError;
    QScopedPointer<QTcpSocket> m_pSocket;
    QScopedPointer<LogEventModel> m_LogModel;
    QScopedPointer<LogDockWidget> m_LogDockWidget;
    CodeEditor *m_pCodeEditor;
    DbgEditorLineWidgetProvider *m_pCodeEditorProvider;
};
#endif // MAINWINDOW_H
