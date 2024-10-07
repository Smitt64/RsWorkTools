#include "process.h"
#include "rslexecutor.h"
#include <QProcess>
#include <QProcessEnvironment>
#include <QDebug>

ProcessEnvironment::ProcessEnvironment(QObject *parent) :
    QObject(parent)
{
    pEnv.reset(new QProcessEnvironment());
}

ProcessEnvironment::ProcessEnvironment(const QProcessEnvironment &other,
                                       QObject *parent)
{
    pEnv.reset(new QProcessEnvironment(other));
}

ProcessEnvironment::~ProcessEnvironment()
{

}

void ProcessEnvironment::clear()
{
    pEnv->clear();
}

bool ProcessEnvironment::contains(const QString &name) const
{
    return pEnv->contains(name);
}

void ProcessEnvironment::insert(const QString &name, const QString &value)
{
    pEnv->insert(name, value);
}

void ProcessEnvironment::insert(QObject *Environment)
{
    ProcessEnvironment *env = qobject_cast<ProcessEnvironment*>(Environment);

    if (!env)
        ThrowParamTypeError<ProcessEnvironment>(1);

    pEnv->insert(*env->pEnv);
}

void ProcessEnvironment::remove(const QString &name)
{
    pEnv->remove(name);
}

bool ProcessEnvironment::isEmpty() const
{
    return pEnv->isEmpty();
}

QStringList	ProcessEnvironment::keys() const
{
    return pEnv->keys();
}

QStringList	ProcessEnvironment::toStringList() const
{
    return pEnv->toStringList();
}

QString	ProcessEnvironment::value(const QString &name, const QString &defaultValue) const
{
    return pEnv->value(name, defaultValue);
}

QProcessEnvironment &ProcessEnvironment::environment()
{
    return *pEnv;
}

// -----------------------------------------------------------------

Process::Process(QObject *parent)
    : IoDevice{parent}
{
    setIoDevice(new QProcess());

    connect(proc(), &QProcess::errorOccurred, this, &Process::errorOccurred);
    connect(proc(), qOverload<int,QProcess::ExitStatus>(&QProcess::finished), this, &Process::finished);
    connect(proc(), &QProcess::readyReadStandardError, this, &Process::readyReadStandardError);
    connect(proc(), &QProcess::readyReadStandardOutput, this, &Process::readyReadStandardOutput);
    connect(proc(), &QProcess::started, this, &Process::started);
    connect(proc(), &QProcess::stateChanged, this, &Process::stateChanged);
}

Process::~Process()
{

}

QProcess *Process::proc() const
{
    return dynamic_cast<QProcess*>(device());
}

QProcess *Process::proc()
{
    return dynamic_cast<QProcess*>(device());
}

QStringList Process::arguments() const
{
    return proc()->arguments();
}

void Process::setArguments(const QStringList &arg)
{
    proc()->setArguments(arg);
}

QStringList Process::nativeArguments() const
{
    return proc()->arguments();
}

void Process::setNativeArguments(const QStringList &arg)
{
    proc()->setArguments(arg);
}

void Process::closeReadChannel(const int &channel)
{
    proc()->closeReadChannel((QProcess::ProcessChannel)channel);
}

void Process::closeWriteChannel()
{
    proc()->closeWriteChannel();
}

int Process::error() const
{
    return proc()->error();
}

int Process::exitCode() const
{
    return proc()->exitCode();
}

int Process::exitStatus() const
{
    return proc()->exitStatus();
}

int Process::inputChannelMode() const
{
    return proc()->inputChannelMode();
}

void Process::setInputChannelMode(const int &mode)
{
    proc()->setInputChannelMode((QProcess::InputChannelMode)mode);
}

int Process::processChannelMode() const
{
    return proc()->processChannelMode();
}

void Process::setProcessChannelMode(const int &mode)
{
    proc()->setProcessChannelMode((QProcess::ProcessChannelMode)mode);
}

QObject *Process::processEnvironment() const
{
    ProcessEnvironment *env = new ProcessEnvironment(proc()->processEnvironment());
    SetObjectOwnerProp(env, RegisterInfoBase::RslOwner);

    return env;
}

void Process::setProcessEnvironment(QObject *envobj)
{
    ProcessEnvironment *env = qobject_cast<ProcessEnvironment*>(envobj);

    if (!env)
        ThrowParamTypeError<ProcessEnvironment>(1);

    proc()->setProcessEnvironment(env->environment());
}

qint64 Process::processId() const
{
    return proc()->processId();
}

QString	Process::program() const
{
    return proc()->program();
}

void Process::setProgram(const QString &program)
{
    proc()->setProgram(program);
}

QByteArray Process::readAllStandardError()
{
    return proc()->readAllStandardError();
}

QByteArray Process::readAllStandardOutput()
{
    QByteArray data = proc()->readAllStandardOutput();
    qDebug() << "Process::readAllStandardOutput" << data;
    return data;
}

int Process::readChannelCount() const
{
    return proc()->readChannelCount();
}

int Process::readChannel() const
{
    return proc()->readChannel();
}

void Process::setReadChannel(const int &mode)
{
    proc()->setReadChannel((QProcess::ProcessChannel)mode);
}

void Process::setStandardErrorFile(const QString &fileName, const int &mode)
{
    proc()->setStandardErrorFile(fileName, (QIODevice::OpenMode)mode);
}

void Process::setStandardInputFile(const QString &fileName)
{
    proc()->setStandardInputFile(fileName);
}

void Process::setStandardOutputFile(const QString &fileName, const int &mode)
{
    proc()->setStandardOutputFile(fileName, (QIODevice::OpenMode)mode);
}

QString	Process::workingDirectory() const
{
    return proc()->workingDirectory();
}

void Process::setWorkingDirectory(const QString &dir)
{
    proc()->setWorkingDirectory(dir);
}

void Process::start(const QString &program, const QStringList &arguments, const int &mode)
{
    proc()->start(program, arguments, (QIODevice::OpenMode)mode);
}

void Process::start(const int &mode)
{
    proc()->start((QIODevice::OpenMode)mode);
}

bool Process::waitForFinished(int msecs)
{
    return proc()->waitForFinished(msecs);
}

bool Process::waitForStarted(int msecs)
{
    return proc()->waitForStarted(msecs);
}

void Process::kill()
{
    return proc()->kill();
}

void Process::terminate()
{
    return proc()->terminate();
}
