#include "sqlstaticmodule.h"
#include "toolsruntime.h"
#include "playrep.h"
#include "rsl/isymbol.h"
#include "rsl/krnlintf.h"
#include "conintf.hpp"
#include "rslexecutor.h"
#include "sql/sqlquery.h"
#include "sql/sqldatabase.h"
#include "rsscript/registerobjlist.hpp"
#include <QRegExp>
#include <QVariant>

static void Rsl_toolExecuteQuery()
{
    enum
    {
        prm_query = 0,
        prm_err,
    };

    if (GetFuncParamType(prm_query) != QVariant::UserType)
        ThrowParamTypeError(prm_query);
    else
    {
        SqlQuery *query = GetFuncParam<SqlQuery*>(0);

        if (!query)
            ThrowParamTypeError(prm_query);

        QString err;
        bool stat = !toolExecuteQuery(query->query(), &err);
        SetFuncParam(prm_err, err);

        ReturnVal(V_BOOL, &stat);
    }
}

SqlStaticModule::SqlStaticModule() :
    RslStaticModule()
{
    RegisterObjList::inst()->RegisterRslObject<SqlQuery>();
    RegisterObjList::inst()->RegisterRslObject<SqlDatabase>();
}

void SqlStaticModule::Init()
{

}

void SqlStaticModule::Proc()
{
    RegisterObjList::inst()->AddObject<SqlQuery>();
    RegisterObjList::inst()->AddObject<SqlDatabase>();

    AddStdProc(V_BOOL, "toolExecuteQuery", Rsl_toolExecuteQuery, 0);
}