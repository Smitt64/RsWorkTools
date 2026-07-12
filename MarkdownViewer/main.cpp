// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "mainwindow.h"
#include "iconthememanager.h"
#include "toolsruntime.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    IconThemeManager::initialize("vs_theme");

    // Добавляем пути к темам относительно рабочей копии репозитория.
    // При деплое тема должна находиться рядом с исполняемым файлом в <appDir>/icons.
    QString appDir = QCoreApplication::applicationDirPath();
    IconThemeManager::addCustomPath(appDir + "/icons");
    IconThemeManager::addCustomPath(QDir(appDir).absoluteFilePath("../../ToolsRuntime/icons"));
    IconThemeManager::addCustomPath(QDir(appDir).absoluteFilePath("../ToolsRuntime/icons"));

    // Загрузка перевода SARibbon из ресурсов ToolsRuntime
    toolLoadTranslations();

    MainWindow w;
    w.show();

    // Открываем файлы, переданные через аргументы командной строки
    const QStringList args = a.arguments();
    for (int i = 1; i < args.size(); ++i)
    {
        const QString filePath = args.at(i);
        if (QFileInfo::exists(filePath))
            QMetaObject::invokeMethod(&w, "openFile", Qt::QueuedConnection, Q_ARG(QString, filePath));
    }

    return a.exec();
}
