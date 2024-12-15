#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class CDebug;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    CDebug *m_curdbg;

    void AddTraceMsg(const QString &str, const int &level);

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MainWindow *ui;

    Qt::HANDLE m_tracemsg;
};

#endif // MAINWINDOW_H
