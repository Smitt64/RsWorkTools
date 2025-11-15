#ifndef RICHTEXTMODULE_H
#define RICHTEXTMODULE_H

#include "rsscript/rslstaticmodule.h"

class RichTextStaticModule : public RslStaticModule
{
public:
    constexpr static const char name[] = "RichText";

    RichTextStaticModule();
    virtual ~RichTextStaticModule() = default;

protected:
    void Init() Q_DECL_OVERRIDE;
    void Proc() Q_DECL_OVERRIDE;
};

#endif // RICHTEXTMODULE_H
