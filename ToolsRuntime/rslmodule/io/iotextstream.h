#ifndef IOTEXTSTREAM_H
#define IOTEXTSTREAM_H

#include <QObject>

class IoDevice;
class QTextStream;
class IoTextStream : public QObject
{
    Q_OBJECT 
    Q_PROPERTY(bool atEnd READ atEnd CONSTANT)
    Q_PROPERTY(qint64 pos READ pos CONSTANT)
    Q_PROPERTY(bool autoDetectUnicode READ autoDetectUnicode WRITE setAutoDetectUnicode)
    Q_PROPERTY(QString codec READ codec WRITE setCodec)
    Q_PROPERTY(int fieldAlignment READ fieldAlignment WRITE setFieldAlignment)
    Q_PROPERTY(int fieldWidth READ fieldWidth WRITE setFieldWidth)
    Q_PROPERTY(bool generateByteOrderMark READ generateByteOrderMark WRITE setGenerateByteOrderMark)
    Q_PROPERTY(int integerBase READ integerBase WRITE setIntegerBase)
    Q_PROPERTY(int numberFlags READ numberFlags WRITE setNumberFlags)
    Q_PROPERTY(QString padChar READ padChar WRITE setPadChar)
    Q_PROPERTY(int realNumberNotation READ realNumberNotation WRITE setRealNumberNotation)
    Q_PROPERTY(int realNumberPrecision READ realNumberPrecision WRITE setRealNumberPrecision)
public:
    Q_INVOKABLE IoTextStream(QObject *device);

    bool atEnd() const;
    bool autoDetectUnicode() const;
    void setAutoDetectUnicode(bool enabled);

    QString codec();
    void setCodec(const QString &codec);

    int fieldAlignment() const;
    void setFieldAlignment(int alignment);

    int fieldWidth() const;
    void setFieldWidth(int width);

    bool generateByteOrderMark() const;
    void setGenerateByteOrderMark(bool enabled);

    int integerBase() const;
    void setIntegerBase(int base);

    int numberFlags() const;
    void setNumberFlags(int flags);

    QString padChar() const;
    void setPadChar(const QString &padChar);

    int realNumberNotation() const;
    void setRealNumberNotation(int notation);

    int realNumberPrecision() const;
    void setRealNumberPrecision(int precision);

    qint64 pos() const;
    Q_INVOKABLE QString read(qint64 maxlen);
    Q_INVOKABLE QString readAll();
    Q_INVOKABLE QString readLine(qint64 maxlen = 0);

    Q_INVOKABLE void flush();
    Q_INVOKABLE void reset();

    Q_INVOKABLE void write(const QString &text);
    Q_INVOKABLE void writeln(const QString &text);
    Q_INVOKABLE void write(const int &value);
    Q_INVOKABLE void write(const float &value);
    Q_INVOKABLE void write(const double &value);

    Q_INVOKABLE QString readStr();
    Q_INVOKABLE int readInt();
    Q_INVOKABLE float readFloat();
    Q_INVOKABLE double readDouble();

signals:

private:
    QScopedPointer<QTextStream> stream;
};

#endif // IOTEXTSTREAM_H
