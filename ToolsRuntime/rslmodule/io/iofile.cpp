#include "iofile.h"
#include <QFile>

IoFile::IoFile(QObject *parent)
    : IoDevice{parent}
{
    setIoDevice(new QFile());
}

IoFile::IoFile(const QString &filename, QObject *parent)
    : IoDevice{parent}
{
    setIoDevice(new QFile(filename));
}

IoFile::IoFile(QFile *file, QObject *parent)
    : IoDevice{parent}
{
    setIoDevice(file);
}

QFile *IoFile::file() const
{
    return dynamic_cast<QFile*>(device());
}

QFile *IoFile::file()
{
    return dynamic_cast<QFile*>(device());
}

QString IoFile::filename() const
{
    return file()->fileName();
}

void IoFile::setFileName(const QString &filename)
{
    file()->setFileName(filename);
}

bool IoFile::exists() const
{
    return file()->exists();
}

QString IoFile::symLinkTarget() const
{
    return file()->symLinkTarget();
}

bool IoFile::copy(const QString &newName)
{
    return file()->copy(newName);
}

bool IoFile::link(const QString &linkName)
{
    return file()->link(linkName);
}

bool IoFile::moveToTrash()
{
    return file()->moveToTrash();
}

bool IoFile::remove()
{
    return file()->remove();
}

bool IoFile::rename(const QString &newName)
{
    return file()->rename(newName);
}
