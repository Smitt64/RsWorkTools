#ifndef TEMPORARYFILE_H
#define TEMPORARYFILE_H

#include "rslmodule/io/iofile.h"
#include <QObject>

class QIODevice;
class QTemporaryFile;
class QTemporaryDir;
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

// -----------------------------------------------------------------

class TemporaryDir : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool autoRemove READ autoRemove WRITE setAutoRemove)
    Q_PROPERTY(bool isValid READ isValid CONSTANT)
    Q_PROPERTY(QString errorString READ errorString CONSTANT)
    Q_PROPERTY(QString path READ path CONSTANT)
public:
    Q_INVOKABLE TemporaryDir(const QString &templatePath, QObject *parent = nullptr);
    Q_INVOKABLE TemporaryDir(QObject *parent = nullptr);
    virtual ~TemporaryDir();

    bool autoRemove() const;
    void setAutoRemove(bool b);

    bool isValid() const;
    QString errorString() const;
    QString path() const;

    Q_INVOKABLE bool remove();
    Q_INVOKABLE QString filePath(const QString &fileName) const;

private:
    QScopedPointer<QTemporaryDir> dir;
};

#endif // TEMPORARYFILE_H
