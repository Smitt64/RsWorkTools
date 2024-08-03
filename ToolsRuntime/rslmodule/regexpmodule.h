#ifndef REGEXPMODULE_H
#define REGEXPMODULE_H

#include "rsscript/rslstaticmodule.h"

class RegExpModule : public RslStaticModule
{
public:
    constexpr static const char name[] = "RegExp";

    RegExpModule();
    virtual ~RegExpModule() = default;

protected:
    void Init() Q_DECL_OVERRIDE;
    void Proc() Q_DECL_OVERRIDE;
};

#endif // REGEXPMODULE_H
