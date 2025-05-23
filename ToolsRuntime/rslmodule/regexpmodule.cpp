#include "regexpmodule.h"
#include "playrep.h"
#include "rsl/isymbol.h"
#include "rsl/krnlintf.h"
#include "conintf.hpp"
#include "rslexecutor.h"
#include "rsscript/registerobjlist.hpp"
#include "rslmodule/regexp/regexp.h"
#include <QRegExp>
#include <QVariant>

static void Rsl_RegExpValidate()
{
    enum
    {
        prm_pattern = 0,
        prm_str,
        prm_pos,
    };

    if (GetFuncParamType(prm_pattern) != QVariant::String)
        ThrowParamTypeError(prm_pattern);

    if (GetFuncParamType(prm_str) != QVariant::String)
        ThrowParamTypeError(prm_str);

    if (GetFuncParamType(prm_pos) != QVariant::Int)
        ThrowParamTypeError(prm_pos);

    QString pattern = GetFuncParam(0).toString();
    QString str = GetFuncParam(1).toString();
    int pos = GetFuncParam(2).toInt();

    bool result = RegExpValidate(pattern, str, pos);
    ReturnVal(V_BOOL, &result);
}

static void Rsl_RegExpReplace()
{
    enum
    {
        prm_pattern = 0,
        prm_str,
        prm_after,
    };

    if (GetFuncParamType(prm_pattern) != QVariant::String)
        ThrowParamTypeError(prm_pattern);

    if (GetFuncParamType(prm_str) != QVariant::String)
        ThrowParamTypeError(prm_str);

    if (GetFuncParamType(prm_after) != QVariant::String)
        ThrowParamTypeError(prm_after);

    QString pattern = GetFuncParam(prm_pattern).toString();
    QString str = GetFuncParam(prm_str).toString();
    QString after = GetFuncParam(prm_after).toString();
    QString result = str.replace(QRegExp(pattern), after);

    ReturnVal(V_STRING, &result);
}


RegExpModule::RegExpModule() :
    RslStaticModule()
{
    RegisterObjList::inst()->RegisterRslObject<RegExp>();
}

void RegExpModule::Init()
{

}

void RegExpModule::Proc()
{
    RegisterObjList::inst()->AddObject<RegExp>();
    AddStdProc(V_BOOL, "RegExpValidate", Rsl_RegExpValidate, 0);
    AddStdProc(V_BOOL, "RegExpReplace", Rsl_RegExpReplace, 0);
}
