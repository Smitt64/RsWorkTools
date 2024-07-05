#ifndef RSLEXECUTOR_H
#define RSLEXECUTOR_H

#include <QObject>
#include <QVariant>
#include "rsscript/registerinfobase.h"
#include "ToolsRuntime_global.h"

typedef std::function<void(void)> RslExecutorProc;

class RslExecutorPrivate;
class TOOLSRUNTIME_EXPORT RslExecutor : public QObject
{
    Q_OBJECT
    friend int Executor_MsgProcCaller(int mes, void *ptr, void *userData);
    friend int ShowVarsCaller(Qt::HANDLE sym, Qt::HANDLE mod, void *userData);
    friend void WriteOutCaller(const QString &msg, void *userData);
    friend bool RslPlayRepActionProc(void *UserData);
public:
    RslExecutor(QObject *parent = nullptr);
    virtual ~RslExecutor();

    void playRep(const QString &filename, const QString &output = QString(), RslExecutorProc proc = RslExecutorProc());
    /*bool init(const QString &output = QString());
    bool push(const QString &filename);
    bool pop();*/

    QStringList errors();

    QVariant call(const QString &name, const QVariantList &params);

    static QString getSymbolName(Qt::HANDLE sym);
    static void globalSet(Qt::HANDLE sym, const QVariant &value);
    //SetDebugMacroFlag
    void setDebugMacroFlag(const bool &Eanble);

signals:
    void WriteOut(QString);

protected:
    virtual void PlayRepProc();
    virtual void onBeginExec(const QString &modname);
    virtual void onInspectModuleSymbol(Qt::HANDLE sym);
    virtual void onWriteOut(const QString &msg);

private:
    RslExecutorPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(RslExecutor);
};

bool TOOLSRUNTIME_EXPORT CompareTypes(const int &MetaType, void *val);
void TOOLSRUNTIME_EXPORT StdValueSetFunc(void *val, int type, void *ptr);
QVariant TOOLSRUNTIME_EXPORT SetFromRslValue(void *value, bool isStringListProp = false);
int TOOLSRUNTIME_EXPORT SetValueFromVariant(std::function<void(int,void*)> Setter, const QVariant &value);

TOOLSRUNTIME_EXPORT void* MakeStringList(QStringList *lst, RegisterInfoBase::QObjectRslOwner owner);

#endif // RSLEXECUTOR_H
