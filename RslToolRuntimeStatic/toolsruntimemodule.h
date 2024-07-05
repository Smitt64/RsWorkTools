#ifndef TOOLSRUNTIMEMODULE_H
#define TOOLSRUNTIMEMODULE_H

#include "rsscript/rslstaticmodule.h"

class ToolsRuntimeModule : public RslStaticModule
{
public:
    constexpr static const char name[] = "ToolsRuntime";

    ToolsRuntimeModule();

protected:
    void Init() Q_DECL_OVERRIDE;
    void Proc() Q_DECL_OVERRIDE;
};

#endif // TOOLSRUNTIMEMODULE_H
