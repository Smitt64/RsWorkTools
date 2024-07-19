// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef TOOLSRUNTIMEMODULE_H
#define TOOLSRUNTIMEMODULE_H

#include "rsscript/rslstaticmodule.h"

class ToolsRuntimeModule : public RslStaticModule
{
public:
    constexpr static const char name[] = "ToolsRuntime";

    ToolsRuntimeModule();

    //static void rslGetPostgreSQLInstallLocation();

protected:
    void Init() Q_DECL_OVERRIDE;
    void Proc() Q_DECL_OVERRIDE;
};

#endif // TOOLSRUNTIMEMODULE_H
