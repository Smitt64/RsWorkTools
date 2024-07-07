#include "playrep.h"
#include "statvars.h"
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
#include "rsscript/registerobjlist.hpp"
#include "rsscript/rslstaticmodule.h"
#include <QUuid>
#include <QDebug>
#include <QTextCodec>
#include <QMessageBox>

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

        /*hCurrInst = RslGetCurrentInstEx(UserData.nameSpace);
        objInst.reset(new TRSLConObjInstIntf(hCurrInst));\

        statErr.newErrList = &m_ErrList;
        statErr.oldErrList = NULL;*/

        //objInst->RslSendMes(IM_CHANGE_ERRORS, &statErr);
        //oldInst = RslSetCurrentInst(hCurrInst);
        //m_MacroExecutor = new TRsbRSLConInst();
    }

    ~RslExecutorPrivate()
    {
        if (Output)
            delete[] Output;
    }

    HRSLINST RslGetCurrentInstEx(const char *nameSpace)
    {
        TRSLCacheData  data;
        TRSLMsgHandler  *host = RslGetMainHost();

        data.cache = NULL;
        host->msgProc(host, NULL, IM_GETCACHE, &data);

        if(data.cache != NULL)
            return HRSLCACHE_FUN(data.cache)->SearchInstance(data.cache, nameSpace);

        return NULL;
    }

    //HRSLINST hCurrInst;
    //HRSLINST oldInst;

    char *Output;

    TMacroUserData UserData;
    TRSLErrorsList m_ErrList;
    RslExecutor *q_ptr;
    //TRSLMsgHandler *pHandler;
    //QScopedPointer<TInstSwitch> m_pCurrentInst;
    //QScopedPointer<TRSLConObjInstIntf> objInst;
    //TRsbRSLConInst *m_MacroExecutor;
    RslExecutorProc PlayRepProc;
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

/*bool RslExecutor::init(const QString &output)
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
    return true;
}*/

QStringList RslExecutor::errors()
{
    Q_D(RslExecutor);
    QStringList list;
    while (d->m_ErrList.ErrorNext())
    {
        ERRLISTELEM *elem = d->m_ErrList.GetCurrentErr();
        list.append(QString("Error at line %1 (pos %2, %3): %4")
                    .arg(elem->info.line)
                    .arg(elem->info.pos)
                    .arg(elem->info.file)
                    .arg(elem->info.mes));
    }

    return list;
}

/*bool RslExecutor::push(const QString &filename)
{
    Q_D(RslExecutor);

    if(!d->objInst->RslSendMes(IM_INIT_INSTANCE_REC, NULL))
    {
        int k = 0;
        k = 1;
    }
    //d->m_pCurrentInst.reset(new TInstSwitch(d->m_MacroExecutor->GetHRD()));
    //return d->m_MacroExecutor->PushModule(filename.toLocal8Bit().data());
    return false;
            //d->m_MacroExecutor->PushModule(filename.toLocal8Bit().data(), false, false);
}*/

/*bool RslExecutor::pop()
{
    Q_D(RslExecutor);
    //return d->m_MacroExecutor->PopModule();
    return false;
}*/

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
void RslExecutor::setDebugMacroFlag(const bool &Eanble)
{
    if (Eanble)
        SetDebugMacroFlag(1);
    else
        SetDebugMacroFlag(0);
}

void RslExecutor::onInspectModuleSymbol(Qt::HANDLE sym)
{

}

void RslExecutor::onWriteOut(const QString &msg)
{
    emit WriteOut(msg);
}

void WriteOutCaller(const QString &msg, void *userData)
{
    TMacroUserData *UserData = (TMacroUserData*)userData;
    UserData->m_pExecutor->onWriteOut(msg);
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
                   SF_STRUC | SF_ARRAY | SF_ALLMODULE, modname.toLocal8Bit().data());
}

int Executor_MsgProcCaller(int mes, void *ptr, void *userData)
{
    TMacroUserData *UserData = (TMacroUserData*)userData;

    switch(mes)
    {
    case IM_DYNAMIC_MODULE:
    {
        TRslDynModule *DynModule = (TRslDynModule *)ptr;
        if(!DynModule->proc)
        {
            RslStaticModule *mod = RegisterObjList::inst()->staticModule(DynModule->name);

            qDebug() << DynModule->name;
            if (mod)
            {
                DynModule->init = mod->caller()->Init;
                DynModule->proc = mod->caller()->Proc;
                DynModule->done = mod->caller()->Close;
                //func(DynModule);
                if(DynModule->proc)
                    return 1;
            }
        }
    }
        break;
    case IM_SET_STMODULE_ADD:
        AddSystemModule();
        RegisterStringList();
        AddStdProc(V_GENOBJ, "StringList", RslStringList, 0);
        break;
    case IM_ERROR:
    {
        ERRINFO *error = (ERRINFO*)ptr;
        QTextCodec *codec = QTextCodec::codecForName("IBM 866");
        QString mes = QString("Файл %5. Строка %1 [%2]: %3 %4")
                .arg(error->line)
                .arg(error->pos)
                .arg(codec->toUnicode(GetErrMesEx(error->code, 0)))
                .arg(error->mes)
                .arg(error->file);
        UserData->m_pExecutor->onError(error->code, {mes});
    }
        break;
    case IM_MSGBOX:
        break;
    case IM_BEGIN_EXEC:
        UserData->m_pExecutor->onBeginExec((char*)ptr);
        break;
    case IM_MODULE_OPEN:
        RslOnlyDeclMode();
        break;
    case IM_OUTPUT:
        qDebug() << "Msg: " << (char*)ptr;
        WriteOutCaller(QString((char*)ptr), userData);
        break;
    }
    return Std_MsgProc ( mes, ptr, UserData );
}

int Executor_MsgProc (IMESSAGE mes, void *ptr, void *userData)
{
    return Executor_MsgProcCaller((IMESSAGE)mes, ptr, userData);
}

void RslExecutor::PlayRepProc()
{
    Q_D(RslExecutor);

    if (d->PlayRepProc)
        d->PlayRepProc();
}

bool RslPlayRepActionProc(void *UserData)
{
    TMacroUserData *dt = (TMacroUserData*)RslGetDataPassedToPlayRep(UserData);
    dt->m_pExecutor->PlayRepProc();

    return true;
}

void RslExecutor::playRep(const QString &filename, const QString &output, RslExecutorProc proc)
{
    Q_D(RslExecutor);

    TMacroUserData prm;
    prm.m_pExecutor = this;

    d->PlayRepProc = proc;

    bool isErrors = true;
    RunRSLEx(filename.toLocal8Bit().data(),
             output.toLocal8Bit().data(),
             d->UserData.nameSpace,
             Executor_MsgProc,
             RslPlayRepActionProc,
             &prm,
             false,
             0,
             &isErrors,
             &d->m_ErrList,
             output.toLocal8Bit().data());
    /*PlayRep(filename.toLocal8Bit().data(),
            output.toLocal8Bit().data(),
            Executor_MsgProc,
            RslPlayRepActionProc,
            0, &prm);*/

    d->PlayRepProc = RslExecutorProc();
}

QVariant RslExecutor::call(const QString &name, const QVariantList &params)
{
    Q_D(RslExecutor);
    TRsbParmForRsl RslParm(params.size());
    ISYMBOL* pSym = RslGetInstSymbol(name.toLocal8Bit().data());

    if (!pSym)
        return QVariant();

    auto SetterFunc = [&RslParm](int id, int type, void *ptr) -> void
    {
        ValueSet(RslParm[id].getValue(), type, ptr);
    };

    for (int i = 0; i < params.size(); i++)
    {
        SetValueFromVariant(std::bind(SetterFunc, i,
                                      std::placeholders::_1,
                                      std::placeholders::_2),
                            params[i]);
    }

    RslCallInstSymbol(pSym, RSL_DISP_RUN,
                      params.size(),
                      RslParm.getParmArray(),
                      RslParm.retVal().getValue());

    VALUE *retval = RslParm.retVal().getValue();
    return SetFromRslValue(retval);
}

void RslExecutor::onError(const int &code, const QString &mes)
{
    emit ErrorMessage(code, mes);
}

void AddFunctionToRsl(const QString &name, ToolRslStdProc proc)
{
    AddStdProc(V_UNDEF, name.toLocal8Bit().data(), proc, 0);
}

QVariant GetFuncParam(const int &id)
{
    VALUE *val;
    GetParm(id, &val);

    return SetFromRslValue(val);
}
