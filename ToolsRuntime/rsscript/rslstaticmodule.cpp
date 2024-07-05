#include "rslstaticmodule.h"
#include "statvars.h"
#include "rsl/isymbol.h"
#include "rslexecutor.h"

class RslStaticModulePrivate
{
    Q_DECLARE_PUBLIC(RslStaticModule)
public:
    RslStaticModulePrivate(RslStaticModule *obj)
    {
        q_ptr = obj;
        _Caller = nullptr;
    }

    RslStaticModuleCallerBase *_Caller;
    RslStaticModule *q_ptr;
};

RslStaticModule::RslStaticModule() :
    d_ptr(new RslStaticModulePrivate(this))
{

}

void RslStaticModule::setCaller(RslStaticModuleCallerBase *caller)
{
    Q_D(RslStaticModule);
    d->_Caller = caller;
}

RslStaticModuleCallerBase *RslStaticModule::caller()
{
    Q_D(RslStaticModule);
    return d->_Caller;
}

void RslStaticModule::Init()
{

}

void RslStaticModule::Close()
{

}

void RslStaticModule::addConstant(const QString &name, const QVariant &value)
{
    auto SetterFunc = [=](int type, void *ptr) -> void
    {
        RslAddConst(type, name.toLocal8Bit().data(), ptr);
    };

    SetValueFromVariant(SetterFunc, value);
}

void RslStaticModule::addGlobal(const QString &name, const QVariant &value)
{
    auto SetterFunc = [=](int type, void *ptr) -> void
    {
        SYMGLOBAL *sym= (SYMGLOBAL*)AddSymGlobal(type, name.toLocal8Bit().data());
        SymGlobalSet(P_SYM(sym), type, ptr);
    };

    SetValueFromVariant(SetterFunc, value);
}
