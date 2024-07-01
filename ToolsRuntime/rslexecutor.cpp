#include "statvars.h"
#include "playrep.h"
#include "rsl/isymbol.h"
#include "rsl/objintf.hpp"
#include "rsl/krnlintf.h"
#include "conintf.hpp"

#include "statvars.h"
#include "rsscript/typeinfo_p.h"
#include "rsscript/registerinfobase.h"
#include "rsscript/TRsbRSLInstTmpl.hpp"
#include "rslexecutor.h"
#include "erlist.hpp"
#include <QUuid>

int Executor_MsgProc (IMESSAGE mes, void *ptr,void *UserData);

typedef TRsbRSLInstTmpl<TRSLConObjInstIntf> TRsbRSLConInstIntf;
typedef TConInstPtr<TRsbRSLConInstIntf, TRSLConObjMsgHandler> TRsbRSLConInst;

class TMacroUserData : public STD_USERDATA
{
public:
  TMacroUserData() :
      STD_USERDATA()
  {
  }

  RslExecutor *m_pExecutor;
  RslExecutorPrivate *m_pEcecPrivate;
};

class RslExecutorPrivate
{
    Q_DECLARE_PUBLIC(RslExecutor)
public:
    RslExecutorPrivate(RslExecutor *obj) :
        q_ptr(obj)
    {
        Output = nullptr;

        qstrcpy(UserData.nameSpace, QUuid::createUuid().toString(QUuid::WithoutBraces).toLocal8Bit().data());
        UserData.m_pExecutor = obj;
        UserData.m_pEcecPrivate = this;
    }

    ~RslExecutorPrivate()
    {
        if (Output)
            delete[] Output;
    }

    char *Output;

    TMacroUserData UserData;
    TRSLErrorsList m_ErrList;
    RslExecutor *q_ptr;
    TRSLMsgHandler *pHandler;

    TRsbRSLConInst *m_MacroExecutor;
};

RslExecutor::RslExecutor(QObject *parent) :
    QObject(parent),
    d_ptr(new RslExecutorPrivate(this))
{

}

RslExecutor::~RslExecutor()
{
    delete d_ptr;
}

bool RslExecutor::init(const QString &output)
{
    Q_D(RslExecutor);

    if (!output.isEmpty())
    {
        d->Output = new char[256 * 4];
        qstrcpy(d->Output, output.toLocal8Bit().data());

        d->UserData.output = d->UserData.originalOutput = d->Output;
    }

    d->m_ErrList.ClearErrors();

    return d->m_MacroExecutor->Init(NULL, d->Output, d->UserData.nameSpace, NULL,
                                    Executor_MsgProc, &d->UserData,
                                    NULL, &d->m_ErrList);
}

bool RslExecutor::push(const QString &filename)
{
    Q_D(RslExecutor);

    return d->m_MacroExecutor->PushModule(filename.toLocal8Bit().data(), false, true);
}

QString RslExecutor::getSymbolName(Qt::HANDLE sym)
{
    ISYMBOL *symbol = (ISYMBOL*)sym;
    return QString(symbol->name);
}

void RslExecutor::globalSet(Qt::HANDLE sym, const QVariant &value)
{
    ISYMBOL *symbol = (ISYMBOL*)sym;

    auto SetterFunc = [=](int type, void *ptr) -> void
    {
        SymGlobalSet (symbol, type, ptr);
    };

    SetValueFromVariant(SetterFunc, value);
}

void RslExecutor::onInspectModuleSymbol(Qt::HANDLE sym)
{

}

int ShowVarsCaller(Qt::HANDLE sym, Qt::HANDLE mod, void *userData)
{
    TMacroUserData *UserData = (TMacroUserData*)userData;
    UserData->m_pExecutor->onInspectModuleSymbol(sym);
    return 0;
}

static int ShowVars (ISYMBOL *sym, SYMMODULE *mod, void *userData)
{
    return ShowVarsCaller(sym, mod, userData);
}

void RslExecutor::onBeginExec(const QString &modname)
{
    InspectModule (ShowVars, SF_SPECIAL | SF_VAR | SF_MACRO | SF_FILE |
                   SF_STRUC | SF_ARRAY | SF_ALLMODULE, (char *)ptr);
}

int Executor_MsgProcCaller(int mes, void *ptr, void *userData)
{
    TMacroUserData *UserData = (TMacroUserData*)userData;

    switch(mes)
    {
    case IM_BEGIN_EXEC:
        UserData->m_pExecutor->onBeginExec((char*)ptr);
        break;
    }
    return Std_MsgProc ( mes, ptr, UserData );
}

int Executor_MsgProc (IMESSAGE mes, void *ptr, void *userData)
{
    return Executor_MsgProcCaller((IMESSAGE)mes, ptr, userData);
}
