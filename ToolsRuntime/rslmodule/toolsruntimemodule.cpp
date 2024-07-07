#include "toolsruntimemodule.h"
#include "rsscript/registerobjlist.hpp"
#include "toolsruntinersl.h"
#include "rslexecutor.h"
#include <errorsmodel.h>

Q_GLOBAL_STATIC(ToolsRuntime, pToolsRuntime)

ToolsRuntimeModule::ToolsRuntimeModule() :
    RslStaticModule()
{
    RegisterObjList::inst()->RegisterRslObject<ToolsRuntime>();
    RegisterObjList::inst()->RegisterRslObject<ErrorsModel>();
}

void ToolsRuntimeModule::Init()
{

}

void ToolsRuntimeModule::Proc()
{
    addConstant("ToolsRuntime", QVariant::fromValue((QObject*)pToolsRuntime));
    //addConstant("TEST_Constant", "TestConstant");
    RegisterObjList::inst()->AddObject<ErrorsModel>();
    RegisterObjList::inst()->AddObject<ToolsRuntime>(false);

    //RslExecutor::globalSet()

    //AddFunctionToRsl("toolGetPostgreSQLInstallLocation", rslGetPostgreSQLInstallLocation);
}

/*void ToolsRuntimeModule::rslGetPostgreSQLInstallLocation()
{

}*/
