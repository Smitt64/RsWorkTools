#include "mainwindow.h"
#include "widgets/CircularMenu.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QContextMenuEvent>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Circular Menu Example");
    resize(400, 300);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout(central);

    QPushButton *btnShow = new QPushButton("Show Circular Menu (or right-click anywhere)", this);
    layout->addWidget(btnShow);

    // Кнопка тоже может показывать меню
    connect(btnShow, &QPushButton::clicked, [this]() {
        showCircularMenu(QCursor::pos());
    });

    // Создаем круговое меню
    m_circularMenu = new CircularMenu(this);

    // Добавляем действия с реальными иконками (замените на свои пути или используйте стандартные)
    QAction *action1 = new QAction(QIcon::fromTheme("document-new"), "New", this);
    QAction *action2 = new QAction(QIcon::fromTheme("document-open"), "Open", this);
    QAction *action3 = new QAction(QIcon::fromTheme("document-save"), "Save", this);
    QAction *action4 = new QAction(QIcon::fromTheme("edit-cut"), "Cut", this);
    QAction *action5 = new QAction(QIcon::fromTheme("edit-copy"), "Copy", this);
    QAction *action6 = new QAction(QIcon::fromTheme("edit-paste"), "Paste", this);
    QAction *action7 = new QAction(QIcon::fromTheme("edit-paste"), "Paste", this);

    // Если иконки из темы не работают, создадим стандартные
    if (action1->icon().isNull())
    {
        action1->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
        action2->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
        action3->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
        action4->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
        action5->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
        action6->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    }

    m_circularMenu->addAction(action1);
    m_circularMenu->addAction(action2);
    m_circularMenu->addAction(action3);
    m_circularMenu->addAction(action4);
    m_circularMenu->addAction(action5);
    m_circularMenu->addAction(action6);
    m_circularMenu->addAction(action7);

    // Подключаем сигналы
    connect(m_circularMenu, &CircularMenu::triggered, this, &MainWindow::onMenuTriggered);
    connect(m_circularMenu, &CircularMenu::aboutToHide, this, []() {
        qDebug() << "Menu hidden";
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    // Показываем круговое меню при правом клике
    showCircularMenu(event->globalPos());
    event->accept();
}

void MainWindow::showCircularMenu(const QPoint &pos)
{
    m_circularMenu->showAt(pos);
}

void MainWindow::onMenuTriggered(QAction *action)
{
    qDebug() << "===================";
    qDebug() << "Action triggered:" << action->text();
    qDebug() << "===================";
}