#include "mainwindow.h"
#include "style/qwindowsxpstyle.h"
#include "style/windowsmodernstyle.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new WindowsModernStyle());
    MainWindow w;
    w.show();
    return a.exec();
}
