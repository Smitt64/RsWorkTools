#include "toolsruntime.h"
#include <QDir>
#include <QApplication>

ToolsRuntime::ToolsRuntime()
{

}

QString toolFullFileNameFromDir(const QString &file)
{
    QDir dir = QDir::current();
    QString fullfilename = dir.absoluteFilePath(file);

    if (QFile::exists(fullfilename))
        return fullfilename;

    dir = QDir(QApplication::applicationDirPath());
    fullfilename = dir.absoluteFilePath(file);

    if (QFile::exists(fullfilename))
        return fullfilename;

    return QString();
}
