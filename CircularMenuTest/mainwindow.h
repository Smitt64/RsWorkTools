#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CircularMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;  // Добавляем обработчик контекстного меню

private slots:
    void showCircularMenu(const QPoint &pos);
    void onMenuTriggered(QAction *action);

private:
    void onShowBlockingMenu();
    void customizeMenuStyle();
    CircularMenu *m_circularMenu;
};

#endif // MAINWINDOW_H