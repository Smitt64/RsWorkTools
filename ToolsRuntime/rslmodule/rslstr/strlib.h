#ifndef STRLIB_H
#define STRLIB_H

#include "rsscript/rslstaticmodule.h"

class StrStaticModule : public RslStaticModule
{
public:
    constexpr static const char name[] = "StrLib";

    StrStaticModule();
    virtual ~StrStaticModule() = default;

protected:
    void Init() Q_DECL_OVERRIDE;
    void Proc() Q_DECL_OVERRIDE;
};

#endif // STRLIB_H
