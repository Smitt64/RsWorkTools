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

QString toolReadTextFileContent(const QString &filename)
{
    QString content;
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        content = f.readAll();
        f.close();
    }
    return content;
}
