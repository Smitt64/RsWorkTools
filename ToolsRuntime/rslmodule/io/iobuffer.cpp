#include "iobuffer.h"
#include <QBuffer>

IoBuffer::IoBuffer(QObject *parent)
    : IoDevice{parent}
{
    setIoDevice(new QBuffer(&_byteArray));
}

IoBuffer::IoBuffer(const QByteArray &data, QObject *parent)
    : IoDevice{parent}
{
    _byteArray = data;
    setIoDevice(new QBuffer(&_byteArray));
}

const QByteArray &IoBuffer::bytes() const
{
    return _byteArray;
}
