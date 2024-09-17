#ifndef IOSTATICMODULE_H
#define IOSTATICMODULE_H

#include "rsscript/rslstaticmodule.h"

class IoStaticModule : public RslStaticModule
{
public:
    constexpr static const char name[] = "Io";

    IoStaticModule();
    virtual ~IoStaticModule() = default;

protected:
    void Init() Q_DECL_OVERRIDE;
    void Proc() Q_DECL_OVERRIDE;
};

#endif // IOSTATICMODULE_H
