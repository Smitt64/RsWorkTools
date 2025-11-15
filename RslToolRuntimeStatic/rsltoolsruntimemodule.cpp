// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "rsltoolsruntimemodule.h"
#include "rslmodule/toolsruntimemodule.h"
#include "rsscript/registerobjlist.hpp"
#include "rslmodule/regexpmodule.h"
#include "rslmodule/sqlstaticmodule.h"
#include "rslmodule/stddialogs.h"
#include "rslmodule/uistaticmodule.h"
#include "rslmodule/iostaticmodule.h"
#include "rslmodule/iostaticmodule.h"
#include "rslmodule/rslstr/strlib.h"
#include "rslmodule/richtextmodule.h"

QStringList RslToolsRuntimeModule::staticModules() const
{
    return { "ToolsRuntime", "RegExp", "Sql", "StdDialogs", "Ui", "Io", "StrLib", "RichText" };
}

void RslToolsRuntimeModule::registerStaticModules()
{
    addStaticModule<ToolsRuntimeModule>(new ToolsRuntimeModule());
    addStaticModule<RegExpModule>(new RegExpModule());
    addStaticModule<SqlStaticModule>(new SqlStaticModule());
    addStaticModule<StdDialogs>(new StdDialogs());
    addStaticModule<UiStaticModule>(new UiStaticModule());
    addStaticModule<IoStaticModule>(new IoStaticModule());
    addStaticModule<StrStaticModule>(new StrStaticModule());
    addStaticModule<RichTextStaticModule>(new RichTextStaticModule());
}
