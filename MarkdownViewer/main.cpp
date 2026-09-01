// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "mainwindow.h"
#include "iconthememanager.h"
#include "toolsruntime.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QLocalSocket>

// Пытаемся передать пути к файлам уже запущенному экземпляру.
// Возвращает true, если экземпляр найден и файлы переданы ему.
static bool tryForwardToRunningInstance(const QStringList &filePaths)
{
    QLocalSocket socket;
    socket.connectToServer(kSingleInstanceKey);
    if (!socket.waitForConnected(500))
        return false;

    QByteArray data;
    for (const QString &path : filePaths)
        data += path.toUtf8() + '\n';

    socket.write(data);
    socket.flush();
    socket.waitForBytesWritten(1000);
    socket.disconnectFromServer();
    return true;
}

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

    // Файлы, переданные через аргументы командной строки
    QStringList filePaths;
    const QStringList args = a.arguments();
    for (int i = 1; i < args.size(); ++i)
    {
        if (QFileInfo::exists(args.at(i)))
            filePaths.append(args.at(i));
    }

    // Single-instance: если приложение уже запущено (например, открыли .md
    // из проводника по ассоциации) - передаём файлы ему и завершаемся,
    // файлы откроются вкладками в существующем окне
    if (tryForwardToRunningInstance(filePaths))
        return 0;

    MainWindow w;
    w.show();

    for (const QString &filePath : filePaths)
        QMetaObject::invokeMethod(&w, "openFile", Qt::QueuedConnection, Q_ARG(QString, filePath));

    return a.exec();
}
