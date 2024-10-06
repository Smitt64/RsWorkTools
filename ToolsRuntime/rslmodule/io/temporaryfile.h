#ifndef TEMPORARYFILE_H
#define TEMPORARYFILE_H

#include "rslmodule/io/iofile.h"
#include <QObject>

class QIODevice;
class QTemporaryFile;
class TemporaryFile : public IoFile
{
    Q_OBJECT
    Q_PROPERTY(QString fileTemplate READ fileTemplate WRITE setFileTemplate)
    Q_PROPERTY(bool autoRemove READ autoRemove WRITE setAutoRemove)
public:
    Q_INVOKABLE TemporaryFile(QObject *parent = nullptr);
    Q_INVOKABLE TemporaryFile(const QString &templateName, QObject *parent = nullptr);
    virtual ~TemporaryFile();

    QString fileTemplate() const;
    void setFileTemplate(const QString &name);

    bool autoRemove() const;
    void setAutoRemove(bool b);

    Q_INVOKABLE bool open();

protected:
    QTemporaryFile *tmpfile() const;
    QTemporaryFile *tmpfile();
};

#endif // TEMPORARYFILE_H
