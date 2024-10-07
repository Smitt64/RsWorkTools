#ifndef PROCESS_H
#define PROCESS_H

#include "iodevice.h"
#include "qprocess.h"

class QProcessEnvironment;
class ProcessEnvironment : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty CONSTANT)
    Q_PROPERTY(QStringList keys READ keys CONSTANT)
public:
    Q_INVOKABLE ProcessEnvironment(QObject *parent = nullptr);
    ProcessEnvironment(const QProcessEnvironment &other, QObject *parent = nullptr);
    virtual ~ProcessEnvironment();

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool contains(const QString &name) const;
    Q_INVOKABLE void insert(const QString &name, const QString &value);
    Q_INVOKABLE void insert(QObject *Environment);
    Q_INVOKABLE void remove(const QString &name);
    Q_INVOKABLE QStringList	toStringList() const;
    Q_INVOKABLE QString	value(const QString &name, const QString &defaultValue = QString()) const;

    bool isEmpty() const;
    QStringList	keys() const;

    QProcessEnvironment &environment();

private:
    QScopedPointer<QProcessEnvironment> pEnv;
};

class QProcess;
class Process : public IoDevice
{
    Q_OBJECT
    Q_PROPERTY(QStringList arguments READ arguments WRITE setArguments)
    Q_PROPERTY(QStringList nativeArguments READ nativeArguments WRITE setNativeArguments)
    Q_PROPERTY(int error READ error CONSTANT)
    Q_PROPERTY(int exitCode READ exitCode CONSTANT)
    Q_PROPERTY(int exitStatus READ exitStatus CONSTANT)
    Q_PROPERTY(int readChannelCount READ readChannelCount CONSTANT)
    Q_PROPERTY(int readChannel READ readChannel WRITE setReadChannel)
    Q_PROPERTY(qint64 processId READ processId CONSTANT)
    Q_PROPERTY(int inputChannelMode READ inputChannelMode WRITE setInputChannelMode)
    Q_PROPERTY(int processChannelMode READ processChannelMode WRITE setProcessChannelMode)
    Q_PROPERTY(QObject *processEnvironment READ processEnvironment WRITE setProcessEnvironment)
    Q_PROPERTY(QString program READ program WRITE setProgram)
    Q_PROPERTY(QString workingDirectory READ workingDirectory WRITE setWorkingDirectory)
public:
    Q_INVOKABLE Process(QObject *parent = nullptr);
    virtual ~Process();

    QStringList arguments() const;
    void setArguments(const QStringList &arg);

    QStringList nativeArguments() const;
    void setNativeArguments(const QStringList &arg);

    Q_INVOKABLE void closeReadChannel(const int &channel);
    Q_INVOKABLE void closeWriteChannel();

    Q_INVOKABLE QByteArray readAllStandardError();
    Q_INVOKABLE QByteArray readAllStandardOutput();

    Q_INVOKABLE void setStandardErrorFile(const QString &fileName, const int &mode = QIODevice::Truncate);
    Q_INVOKABLE void setStandardInputFile(const QString &fileName);
    Q_INVOKABLE void setStandardOutputFile(const QString &fileName, const int &mode = QIODevice::Truncate);

    int error() const;
    int exitCode() const;
    int exitStatus() const;
    int readChannelCount() const;

    qint64 processId() const;

    int inputChannelMode() const;
    void setInputChannelMode(const int &mode);

    int readChannel() const;
    void setReadChannel(const int &mode);

    int processChannelMode() const;
    void setProcessChannelMode(const int &mode);

    QObject *processEnvironment() const;
    void setProcessEnvironment(QObject *envobj);

    QString	program() const;
    void setProgram(const QString &program);

    QString	workingDirectory() const;
    void setWorkingDirectory(const QString &dir);

    Q_INVOKABLE void start(const QString &program, const QStringList &arguments = {}, const int &mode = QIODevice::ReadWrite);
    Q_INVOKABLE void start(const int &mode = QIODevice::ReadWrite);

    Q_INVOKABLE bool waitForFinished(int msecs = 30000);
    Q_INVOKABLE bool waitForStarted(int msecs = 30000);

    Q_INVOKABLE void kill();
    Q_INVOKABLE void terminate();

    QProcess *proc() const;
    QProcess *proc();

signals:
    void errorOccurred(int error);
    void finished(int exitCode, int);
    void readyReadStandardError();
    void readyReadStandardOutput();
    void started();
    void stateChanged(int);
};

#endif // PROCESS_H
