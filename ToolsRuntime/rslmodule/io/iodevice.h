#ifndef IODEVICE_H
#define IODEVICE_H

#include <QObject>

class QIODevice;
class IoDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isOpen READ isOpen CONSTANT)
    Q_PROPERTY(bool isWritable READ isWritable CONSTANT)
    Q_PROPERTY(bool isReadable READ isReadable CONSTANT)
    Q_PROPERTY(bool isSequential READ isSequential CONSTANT)
    Q_PROPERTY(bool atEnd READ atEnd CONSTANT)
    Q_PROPERTY(bool canReadLine READ canReadLine CONSTANT)
    Q_PROPERTY(qint64 pos READ pos CONSTANT)
    Q_PROPERTY(qint64 size READ size CONSTANT)
    Q_PROPERTY(qint64 bytesAvailable READ bytesAvailable CONSTANT)
    Q_PROPERTY(qint64 bytesToWrite READ bytesToWrite CONSTANT)
    Q_PROPERTY(qint32 openMode READ openMode CONSTANT)

    Q_PROPERTY(QString errorString READ errorString CONSTANT)
public:
    IoDevice(QObject *parent = nullptr);
    virtual ~IoDevice();

    bool isOpen() const;
    bool isWritable() const;
    bool isReadable() const;
    bool isSequential() const;
    bool atEnd() const;
    bool canReadLine() const;
    qint64 pos() const;
    qint64 size() const;
    qint64 bytesAvailable() const;
    qint64 bytesToWrite() const;
    qint32 openMode() const;
    QString errorString() const;

    Q_INVOKABLE bool open(int mode);
    Q_INVOKABLE void close();

    Q_INVOKABLE bool seek(qint64 pos);
    Q_INVOKABLE qint64 skip(qint64 maxlen);
    Q_INVOKABLE bool reset();
    Q_INVOKABLE void ungetChar(char c);
    Q_INVOKABLE bool putChar(char c);
    Q_INVOKABLE bool getChar(char *c);

    Q_INVOKABLE QByteArray peek(qint64 maxlen);
    Q_INVOKABLE QByteArray read(qint64 maxlen);
    Q_INVOKABLE QByteArray readAll();
    Q_INVOKABLE QByteArray readLine(qint64 maxlen = 0);
    Q_INVOKABLE qint64 write(const QByteArray &byteArray);
    Q_INVOKABLE qint64 write(const QString &str);

    QIODevice *device();
    QIODevice *device() const;

signals:
    void aboutToClose();
    void bytesWritten(qint64 bytes);
    void channelBytesWritten(int channel, qint64 bytes);
    void channelReadyRead(int channel);
    void readChannelFinished();
    void readyRead();

protected:
    virtual void setIoDevice(QIODevice *device);

private:
    QScopedPointer<QIODevice> ioDevice;
};

#endif // IODEVICE_H
