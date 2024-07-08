#include "toolsruntimemodule.h"
#include "rsscript/registerobjlist.hpp"
#include "toolsruntinersl.h"
#include "rslexecutor.h"
#include "errordlg.h"
#include <errorsmodel.h>

Q_GLOBAL_STATIC(ToolsRuntime, pToolsRuntime)

ToolsRuntimeModule::ToolsRuntimeModule() :
    RslStaticModule()
{
    RegisterObjList::inst()->RegisterRslObject<ToolsRuntime>();
    RegisterObjList::inst()->RegisterRslObject<ErrorsModel>();
    RegisterObjList::inst()->RegisterRslObject<ErrorDlg>(GenInfoUseParentProps | GenInfoUseParentMeths);
}

void ToolsRuntimeModule::Init()
{

}

void ToolsRuntimeModule::Proc()
{
    addConstant("ErrorModeInformation", ErrorDlg::ModeInformation);
    addConstant("ErrorModeMessageBox", ErrorDlg::ModeMessageBox);
    addConstant("ErrorModeWidget", ErrorDlg::ModeWidget);
    addConstant("ToolsRuntime", QVariant::fromValue((QObject*)pToolsRuntime));

    RegisterObjList::inst()->AddObject<ErrorsModel>();
    RegisterObjList::inst()->AddObject<ErrorDlg>();
    RegisterObjList::inst()->AddObject<ToolsRuntime>(false);
}

/*void ToolsRuntimeModule::rslGetPostgreSQLInstallLocation()
{

}*/
