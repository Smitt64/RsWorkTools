#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(dbg)

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool connectToHost(const QString &hostName, quint16 port);
    QString lastError();

public slots:
    void dbgConnected();
    void dbgDisconnected();

private:
    Ui::MainWindow *ui;
    QString m_LastError;
    QScopedPointer<QTcpSocket> m_pSocket;
};
#endif // MAINWINDOW_H
