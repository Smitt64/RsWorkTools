#include "iodevice.h"
#include <QIODevice>

IoDevice::IoDevice(QObject *parent)
    : QObject{parent}
{
}

IoDevice::~IoDevice()
{

}

bool IoDevice::isOpen() const
{
    return ioDevice->isOpen();
}

bool IoDevice::isReadable() const
{
    return ioDevice->isReadable();
}

bool IoDevice::isWritable() const
{
    return ioDevice->isWritable();
}

bool IoDevice::isSequential() const
{
    return ioDevice->isSequential();
}

bool IoDevice::atEnd() const
{
    return ioDevice->atEnd();
}

qint64 IoDevice::pos() const
{
    return ioDevice->pos();
}

qint64 IoDevice::size() const
{
    return ioDevice->size();
}

qint64 IoDevice::bytesAvailable() const
{
    return ioDevice->bytesAvailable();
}

qint64 IoDevice::bytesToWrite() const
{
    return ioDevice->bytesToWrite();
}

bool IoDevice::canReadLine() const
{
    return ioDevice->canReadLine();
}

QString IoDevice::errorString() const
{
    return ioDevice->errorString();
}

qint32 IoDevice::openMode() const
{
    return ioDevice->openMode();
}

bool IoDevice::open(int mode)
{
    return ioDevice->open((QIODevice::OpenMode)mode);
}

void IoDevice::close()
{
    ioDevice->close();
}

bool IoDevice::seek(qint64 pos)
{
    return ioDevice->seek(pos);
}

bool IoDevice::reset()
{
    return ioDevice->reset();
}

qint64 IoDevice::skip(qint64 maxlen)
{
    return ioDevice->skip(maxlen);
}

void IoDevice::ungetChar(char c)
{
    ioDevice->ungetChar(c);
}

bool IoDevice::putChar(char c)
{
    return ioDevice->putChar(c);
}

bool IoDevice::getChar(char *c)
{
    return ioDevice->getChar(c);
}

QIODevice *IoDevice::device()
{
    return ioDevice.data();
}

QIODevice *IoDevice::device() const
{
    return ioDevice.data();
}

void IoDevice::setIoDevice(QIODevice *device)
{
    ioDevice.reset(device);
}

QByteArray IoDevice::peek(qint64 maxlen)
{
    return ioDevice->peek(maxlen);
}

QByteArray IoDevice::read(qint64 maxlen)
{
    return ioDevice->read(maxlen);
}

QByteArray IoDevice::readAll()
{
    return ioDevice->readAll();
}

QByteArray IoDevice::readLine(qint64 maxlen)
{
    return ioDevice->readLine(maxlen);
}

qint64 IoDevice::write(const QByteArray &byteArray)
{
    return ioDevice->write(byteArray);
}

qint64 IoDevice::write(const QString &str)
{
    return ioDevice->write(str.toLocal8Bit());
}