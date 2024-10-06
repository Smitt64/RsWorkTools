#ifndef IOFILE_H
#define IOFILE_H

#include "iodevice.h"

class QFile;
class IoFile : public IoDevice
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFileName)
    Q_PROPERTY(QString symLinkTarget READ symLinkTarget CONSTANT)
    Q_PROPERTY(bool exists READ exists CONSTANT)
public:
    Q_INVOKABLE IoFile(QObject *parent = nullptr);
    Q_INVOKABLE IoFile(const QString &filename, QObject *parent = nullptr);

    QString filename() const;
    void setFileName(const QString &filename);

    bool exists() const;
    QString symLinkTarget() const;

    Q_INVOKABLE bool copy(const QString &newName);
    Q_INVOKABLE bool link(const QString &linkName);
    Q_INVOKABLE bool moveToTrash();

    Q_INVOKABLE bool remove();
    Q_INVOKABLE bool rename(const QString &newName);

protected:
    IoFile(QFile *file, QObject *parent = nullptr);
    QFile *file() const;
    QFile *file();
};

#endif // IOFILE_H
