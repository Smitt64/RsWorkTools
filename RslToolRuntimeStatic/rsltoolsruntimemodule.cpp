// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "rsltoolsruntimemodule.h"
#include "rslmodule/toolsruntimemodule.h"
#include "rsscript/registerobjlist.hpp"
#include "rslmodule/regexpmodule.h"

QStringList RslToolsRuntimeModule::staticModules() const
{
    return { "ToolsRuntime", "RegExp" };
}

void RslToolsRuntimeModule::registerStaticModules()
{
    addStaticModule<ToolsRuntimeModule>(new ToolsRuntimeModule());
    addStaticModule<RegExpModule>(new RegExpModule());
}
