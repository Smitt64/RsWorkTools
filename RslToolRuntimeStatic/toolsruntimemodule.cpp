#include "toolsruntimemodule.h"
#include "rsscript/registerobjlist.hpp"
#include <errorsmodel.h>

ToolsRuntimeModule::ToolsRuntimeModule() :
    RslStaticModule()
{
     RegisterObjList::inst()->RegisterRslObject<ErrorsModel>();
}

void ToolsRuntimeModule::Init()
{

}

void ToolsRuntimeModule::Proc()
{
    //addConstant("TEST_Constant", "TestConstant");
    RegisterObjList::inst()->AddObject<ErrorsModel>();
    //RegisterObjList::inst()->AddObject<ChildObject>();
    //RegisterObjList::inst()->AddObject("TestObject");
}
