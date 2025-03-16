#include "iodir.h"
#include "iofileinfo.h"
#include "rslexecutor.h"

IoDir::IoDir(const QString &path) :
    QObject()
{

}

// Проверка существования директории
bool IoDir::exists() const
{
    return dir->exists();
}

// Получение абсолютного пути
QString IoDir::absolutePath() const
{
    return dir->absolutePath();
}

// Получение канонического пути (без символических ссылок)
QString IoDir::canonicalPath() const
{
    return dir->canonicalPath();
}

// Получение имени директории
QString IoDir::dirName() const
{
    return dir->dirName();
}

// Получение списка файлов и директорий
QStringList IoDir::entryList(const QStringList& filters, QDir::Filters filterFlags, QDir::SortFlags sortFlags) const
{
    return dir->entryList(filters, filterFlags, sortFlags);
}

// Получение списка файлов
QStringList IoDir::files(const QStringList& filters, QDir::Filters filterFlags, QDir::SortFlags sortFlags) const
{
    return dir->entryList(filters, filterFlags | QDir::Files, sortFlags);
}

// Получение списка директорий
QStringList IoDir::directories(const QStringList& filters, QDir::Filters filterFlags, QDir::SortFlags sortFlags) const
{
    return dir->entryList(filters, filterFlags | QDir::Dirs, sortFlags);
}

// Получение списка символических ссылок
QStringList IoDir::symLinks(const QStringList& filters, QDir::SortFlags sortFlags) const
{
    return dir->entryList(filters, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllEntries | QDir::NoSymLinks, sortFlags);
}

// Создание директории
bool IoDir::mkdir(const QString& dirName) const
{
    return dir->mkdir(dirName);
}

// Создание всех родительских директорий
bool IoDir::mkpath(const QString& dirPath) const
{
    return dir->mkpath(dirPath);
}

// Удаление директории
bool IoDir::rmdir(const QString& dirName) const
{
    return dir->rmdir(dirName);
}

// Удаление директории и всех её содержимого
bool IoDir::removeRecursively() const
{
    return dir->removeRecursively();
}

// Переименование директории
bool IoDir::rename(const QString& oldName, const QString& newName)
{
    return dir->rename(oldName, newName);
}

// Переход в родительскую директорию
bool IoDir::cdUp()
{
    return dir->cdUp();
}

// Переход в указанную директорию
bool IoDir::cd(const QString& dirName)
{
    return dir->cd(dirName);
}

// Получение текущей директории
QString IoDir::currentPath() const
{
    return dir->currentPath();
}

// Очистка директории (удаление всех файлов и поддиректорий)
bool IoDir::clear()
{
    bool success = true;
    QStringList entries = dir->entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const QString& entry : entries)
    {
        QString entryPath = dir->filePath(entry);
        QFileInfo entryInfo(entryPath);
        if (entryInfo.isDir())
        {
            QDir subDir(entryPath);
            success &= subDir.removeRecursively();
        }
        else
        {
            success &= QFile::remove(entryPath);
        }
    }
    return success;
}

// Получение пути к файлу или директории
QString IoDir::filePath(const QString& fileName) const
{
    return dir->filePath(fileName);
}

// Получение абсолютного пути к файлу или директории
QString IoDir::absoluteFilePath(const QString& fileName) const
{
    return dir->absoluteFilePath(fileName);
}

// Проверка, является ли путь относительным
bool IoDir::isRelative() const
{
    return dir->isRelative();
}

// Преобразование относительного пути в абсолютный
QString IoDir::absoluteFilePathFromRelative(const QString& relativePath) const
{
    return dir->absoluteFilePath(relativePath);
}

// Получение корневой директории
QString IoDir::rootPath() const
{
    return dir->rootPath();
}

// Проверка, является ли путь корневым
bool IoDir::isRoot() const
{
    return dir->isRoot();
}

// Получение домашней директории пользователя
QString IoDir::homePath() const
{
    return QDir::homePath();
}

// Получение временной директории
QString IoDir::tempPath() const
{
    return QDir::tempPath();
}

// Получение количества файлов и директорий
int IoDir::count() const
{
    return dir->count();
}

// Получение информации о файле или директории
QObject *IoDir::fileInfo(const QString& fileName) const
{
    IoFileInfo *obj = new IoFileInfo(dir->filePath(fileName));
    SetObjectOwnerProp(obj, RegisterInfoBase::RslOwner);
    return obj;
}
