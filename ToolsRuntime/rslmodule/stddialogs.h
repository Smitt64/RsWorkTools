#ifndef STDDIALOGS_H
#define STDDIALOGS_H

#include "rsscript/rslstaticmodule.h"

class StdDialogs : public RslStaticModule
{
public:
    constexpr static const char name[] = "StdDialogs";

    StdDialogs();
    virtual ~StdDialogs() = default;

protected:
    void Init() Q_DECL_OVERRIDE;
    void Proc() Q_DECL_OVERRIDE;
};

extern QWidget* GetTopLevelWidget();
extern void msgCritical();
extern void msgWarning();
extern void msgInformation();
extern void msgQuestion();

extern void msgGetButtonConstantName();
#endif // STDDIALOGS_H
