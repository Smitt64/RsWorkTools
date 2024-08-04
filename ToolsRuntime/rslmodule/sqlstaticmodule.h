#ifndef SQLSTATICMODULE_H
#define SQLSTATICMODULE_H

#include "rsscript/rslstaticmodule.h"

class SqlStaticModule : public RslStaticModule
{
public:
    constexpr static const char name[] = "Sql";

    SqlStaticModule();
    virtual ~SqlStaticModule() = default;

protected:
    void Init() Q_DECL_OVERRIDE;
    void Proc() Q_DECL_OVERRIDE;
};

#endif // SQLSTATICMODULE_H
