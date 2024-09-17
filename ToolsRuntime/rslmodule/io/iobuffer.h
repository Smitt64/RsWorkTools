#ifndef IOBUFFER_H
#define IOBUFFER_H

#include "iodevice.h"

class IoBuffer : public IoDevice
{
    Q_OBJECT
    Q_PROPERTY(QByteArray bytes READ bytes CONSTANT)
public:
    Q_INVOKABLE IoBuffer(QObject *parent = nullptr);
    Q_INVOKABLE IoBuffer(const QByteArray &data, QObject *parent = nullptr);

    const QByteArray &bytes() const;

private:
    QByteArray _byteArray;
};

#endif // IOBUFFER_H
