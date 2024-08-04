#include "sqlstaticmodule.h"
#include "sql/sqlquery.h"
#include "sql/sqldatabase.h"
#include "rsscript/registerobjlist.hpp"
#include <QRegExp>
#include <QVariant>

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
}
