#ifndef UISTATICMODULE_H
#define UISTATICMODULE_H

#include "rsscript/rslstaticmodule.h"

class UiStaticModule : public RslStaticModule
{
public:
    constexpr static const char name[] = "Ui";

    UiStaticModule();
    virtual ~UiStaticModule() = default;

protected:
    void Init() Q_DECL_OVERRIDE;
    void Proc() Q_DECL_OVERRIDE;
};

#endif // UISTATICMODULE_H
