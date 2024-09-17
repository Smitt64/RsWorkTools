#include "iotextstream.h"
#include "iodevice.h"
#include "rslexecutor.h"
#include <QTextStream>
#include <QTextCodec>

IoTextStream::IoTextStream(QObject *device)
    : QObject{}
{
    IoDevice *objDevice = qobject_cast<IoDevice*>(device);

    if (objDevice)
        stream.reset(new QTextStream(objDevice->device()));
    else
        ThrowParamTypeError<IoDevice>(1);
}

bool IoTextStream::atEnd() const
{
    return stream->atEnd();
}

bool IoTextStream::autoDetectUnicode() const
{
    return stream->autoDetectUnicode();
}

void IoTextStream::setAutoDetectUnicode(bool enabled)
{
    stream->setAutoDetectUnicode(enabled);
}

QString IoTextStream::codec()
{
    return stream->codec()->name();
}

void IoTextStream::setCodec(const QString &_codec)
{
    stream->setCodec(_codec.toLocal8Bit().data());
}

int IoTextStream::fieldAlignment() const
{
    return stream->fieldAlignment();
}

void IoTextStream::setFieldAlignment(int alignment)
{
    stream->setFieldAlignment((QTextStream::FieldAlignment)alignment);
}

int IoTextStream::fieldWidth() const
{
    return stream->fieldWidth();
}

void IoTextStream::setFieldWidth(int width)
{
    stream->setFieldWidth(width);
}

void IoTextStream::flush()
{
    stream->flush();
}

bool IoTextStream::generateByteOrderMark() const
{
    return stream->generateByteOrderMark();
}

void IoTextStream::setGenerateByteOrderMark(bool enabled)
{
    stream->setGenerateByteOrderMark(enabled);
}

int IoTextStream::integerBase() const
{
    return stream->integerBase();
}

void IoTextStream::setIntegerBase(int base)
{
    stream->setIntegerBase(base);
}

int IoTextStream::numberFlags() const
{
    return stream->numberFlags();
}

void IoTextStream::setNumberFlags(int flags)
{
    stream->setNumberFlags((QTextStream::NumberFlags)flags);
}

qint64 IoTextStream::pos() const
{
    return stream->pos();
}

QString IoTextStream::read(qint64 maxlen)
{
    return stream->read(maxlen);
}

QString IoTextStream::readAll()
{
    return stream->readAll();
}

QString IoTextStream::readLine(qint64 maxlen)
{
    return stream->readLine(maxlen);
}

int IoTextStream::realNumberNotation() const
{
    return stream->realNumberNotation();
}

void IoTextStream::setRealNumberNotation(int notation)
{
    stream->setRealNumberNotation((QTextStream::RealNumberNotation)notation);
}

int IoTextStream::realNumberPrecision() const
{
    return stream->realNumberPrecision();
}

void IoTextStream::setRealNumberPrecision(int precision)
{
    stream->setRealNumberPrecision(precision);
}

QString IoTextStream::padChar() const
{
    return stream->padChar();
}

void IoTextStream::setPadChar(const QString &_padChar)
{
    stream->setPadChar(_padChar.front().toLatin1());
}

void IoTextStream::reset()
{
    stream->reset();
}

void IoTextStream::write(const QString &text)
{
    (*stream) << text;
}

void IoTextStream::write(const int &value)
{
    (*stream) << value;
}

void IoTextStream::write(const float &value)
{
    (*stream) << value;
}

void IoTextStream::write(const double &value)
{
    (*stream) << value;
}

QString IoTextStream::readStr()
{
    QString str;
    (*stream) >> str;
    return str;
}

int IoTextStream::readInt()
{
    int value;
    (*stream) >> value;
    return value;
}

float IoTextStream::readFloat()
{
    float value;
    (*stream) >> value;
    return value;
}

double IoTextStream::readDouble()
{
    double value;
    (*stream) >> value;
    return value;
}