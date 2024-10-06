#include "temporaryfile.h"
#include <QTemporaryFile>

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
