#include "iofileinfo.h"
#include <QFileInfo>
#include <QVariant>

IoFileInfo::IoFileInfo(const QString& path, QObject *parent)
    : QObject{parent}
{
    fileInfo.reset(new QFileInfo(path));
}

// Проверка существования файла или директории
bool IoFileInfo::exists() const
{
    return fileInfo->exists();
}

// Получение абсолютного пути
QString IoFileInfo::absoluteFilePath() const
{
    return fileInfo->absoluteFilePath();
}

// Получение имени файла
QString IoFileInfo::fileName() const
{
    return fileInfo->fileName();
}

// Получение базового имени файла (без расширения)
QString IoFileInfo::baseName() const
{
    return fileInfo->baseName();
}

// Получение расширения файла
QString IoFileInfo::suffix() const
{
    return fileInfo->suffix();
}

// Получение пути к директории
QString IoFileInfo::path() const
{
    return fileInfo->path();
}

// Получение абсолютного пути к директории
QString IoFileInfo::absolutePath() const
{
    return fileInfo->absolutePath();
}

// Проверка, является ли объект файлом
bool IoFileInfo::isFile() const
{
    return fileInfo->isFile();
}

// Проверка, является ли объект директорией
bool IoFileInfo::isDir() const
{
    return fileInfo->isDir();
}

// Получение размера файла
qint64 IoFileInfo::size() const
{
    return fileInfo->size();
}

// Получение времени последнего изменения
QDateTime IoFileInfo::lastModified() const
{
    return fileInfo->lastModified();
}

// Получение времени последнего чтения
QDateTime IoFileInfo::lastRead() const
{
    return fileInfo->lastRead();
}

// Получение времени создания
QDateTime IoFileInfo::created() const
{
    return fileInfo->birthTime();
}

// Проверка прав на чтение
bool IoFileInfo::isReadable() const
{
    return fileInfo->isReadable();
}

// Проверка прав на запись
bool IoFileInfo::isWritable() const
{
    return fileInfo->isWritable();
}

// Проверка прав на выполнение
bool IoFileInfo::isExecutable() const
{
    return fileInfo->isExecutable();
}
