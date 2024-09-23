// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QDebug>
#include <QStyle>

#include <objbase.h>

int main(int argc, char *argv[])
{
    CoInitialize(NULL);
    QApplication a(argc, argv);

    qDebug() << qApp->style();
    QStyle *style = QStyleFactory::create(DefaultStyleName);
    a.setStyle(style);

    MainWindow w;
    w.show();
    return a.exec();
}
