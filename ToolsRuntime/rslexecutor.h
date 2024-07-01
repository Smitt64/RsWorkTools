#ifndef RSLEXECUTOR_H
#define RSLEXECUTOR_H

#include <QObject>
#include "ToolsRuntime_global.h"

class RslExecutorPrivate;
class TOOLSRUNTIME_EXPORT RslExecutor : public QObject
{
    Q_OBJECT
    friend int Executor_MsgProcCaller(int mes, void *ptr, void *userData);
    friend int ShowVarsCaller(Qt::HANDLE sym, Qt::HANDLE mod, void *userData);
public:
    RslExecutor(QObject *parent = nullptr);
    virtual ~RslExecutor();

    bool init(const QString &output = QString());
    bool push(const QString &filename);

    QString getSymbolName(Qt::HANDLE sym);
    void globalSet(Qt::HANDLE sym, const QVariant &value);

protected:
    virtual void onBeginExec(const QString &modname);
    virtual void onInspectModuleSymbol(Qt::HANDLE sym);

private:
    RslExecutorPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(RslExecutor);
};

int TOOLSRUNTIME_EXPORT SetValueFromVariant(std::function<void(int,void*)> Setter, const QVariant &value);

#endif // RSLEXECUTOR_H
