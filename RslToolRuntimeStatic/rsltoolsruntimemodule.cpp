#include "rsltoolsruntimemodule.h"
#include "toolsruntimemodule.h"
#include "rsscript/registerobjlist.hpp"

QStringList RslToolsRuntimeModule::staticModules() const
{
    return { "ToolsRuntime" };
}

void RslToolsRuntimeModule::registerStaticModules()
{
    addStaticModule<ToolsRuntimeModule>(new ToolsRuntimeModule());
}
