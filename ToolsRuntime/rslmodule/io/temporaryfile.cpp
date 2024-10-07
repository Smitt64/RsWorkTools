#include "temporaryfile.h"
#include <QTemporaryFile>
#include <QTemporaryDir>

TemporaryFile::TemporaryFile(QObject *parent)
    : IoFile{new QTemporaryFile(), parent}
{

}

TemporaryFile::TemporaryFile(const QString &templateName, QObject *parent)
    : IoFile{new QTemporaryFile(templateName), parent}
{

}

TemporaryFile::~TemporaryFile()
{

}

QTemporaryFile *TemporaryFile::tmpfile() const
{
    return dynamic_cast<QTemporaryFile*>(device());
}

QTemporaryFile *TemporaryFile::tmpfile()
{
    return dynamic_cast<QTemporaryFile*>(device());
}

QString TemporaryFile::fileTemplate() const
{
    return tmpfile()->fileTemplate();
}

void TemporaryFile::setFileTemplate(const QString &name)
{
    tmpfile()->setFileTemplate(name);
}

bool TemporaryFile::autoRemove() const
{
    return tmpfile()->autoRemove();
}

void TemporaryFile::setAutoRemove(bool b)
{
    tmpfile()->setAutoRemove(b);
}

bool TemporaryFile::open()
{
    return tmpfile()->open();
}

// -----------------------------------------------------------------

TemporaryDir::TemporaryDir(const QString &templatePath, QObject *parent)
    : QObject{parent}
{
    dir.reset(new QTemporaryDir(templatePath));
}

TemporaryDir::TemporaryDir(QObject *parent)
    : QObject{parent}
{
    dir.reset(new QTemporaryDir());
}

TemporaryDir::~TemporaryDir()
{

}

bool TemporaryDir::autoRemove() const
{
    return dir->autoRemove();
}

void TemporaryDir::setAutoRemove(bool b)
{
    dir->setAutoRemove(b);
}

QString TemporaryDir::path() const
{
    return dir->path();
}

QString TemporaryDir::errorString() const
{
    return dir->errorString();
}

bool TemporaryDir::isValid() const
{
    return dir->isValid();
}

bool TemporaryDir::remove()
{
    return dir->remove();
}

QString TemporaryDir::filePath(const QString &fileName) const
{
    return dir->filePath(fileName);
}
