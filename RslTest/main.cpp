#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStyle  *style = QStyleFactory::create("WindowsModernStyle");
    a.setStyle(style);

    MainWindow w;
    w.show();
    return a.exec();
}
