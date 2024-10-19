#include "ionamespace.h"
#include <QDir>
#include <QFileInfoList>

IoNamespace::IoNamespace(QObject *parent)
    : QObject{parent}
{

}

QString IoNamespace::currentPath()
{
     return QDir::currentPath();
}

QStringList IoNamespace::drives()
{
    QStringList result;
    QFileInfoList lst = QDir::drives();
    for (int i = 0; i < lst.size(); i++)
        result.append(lst[i].path());

    return result;
}

QString IoNamespace::cleanPath(const QString &path)
{
     return QDir::cleanPath(path);
}

QString IoNamespace::fromNativeSeparators(const QString &pathName)
{
     return QDir::fromNativeSeparators(pathName);
}

QString IoNamespace::home()
{
     return QDir::homePath();
}

bool IoNamespace::isAbsolutePath(const QString &path)
{
    return QDir::isAbsolutePath(path);
}

bool IoNamespace::isRelativePath(const QString &path)
{
    return QDir::isRelativePath(path);
}

QString IoNamespace::listSeparator()
{
    return QDir::listSeparator();
}

bool IoNamespace::match(const QString &filter, const QString &fileName)
{
    return QDir::match(filter, fileName);
}

bool IoNamespace::match(const QStringList &filters, const QString &fileName)
{
    return QDir::match(filters, fileName);
}

QString IoNamespace::root()
{
    return QDir::rootPath();
}

QString IoNamespace::separator()
{
     return QDir::separator();
}

QString IoNamespace::temp()
{
     return QDir::tempPath();
}

QString IoNamespace::toNativeSeparators(const QString &pathName)
{
    return QDir::toNativeSeparators(pathName);
}

bool IoNamespace::copyFile(const QString &fileName, const QString &newName)
{
     return QFile::copy(fileName, newName);
}

bool IoNamespace::existsFile(const QString &fileName)
{
     return QFile::exists(fileName);
}

bool IoNamespace::moveToTrash(const QString &fileName)
{
     return QFile::moveToTrash(fileName);
}

bool IoNamespace::removeFile(const QString &fileName)
{
     return QFile::remove(fileName);
}

bool IoNamespace::renameFile(const QString &oldName, const QString &newName)
{
     return QFile::rename(oldName, newName);
}

QStringList IoNamespace::entryList(const QString &path, 
    const QStringList &nameFilters, 
    int filters, int sort)
{
    QDir dir(path);
    return dir.entryList(nameFilters, (QDir::Filters)filters, (QDir::SortFlags)sort);
}

QStringList IoNamespace::entryList(const QString &path,
        int filters, int sort)
{
    QDir dir(path);
    return dir.entryList((QDir::Filters)filters, (QDir::SortFlags)sort);
}
