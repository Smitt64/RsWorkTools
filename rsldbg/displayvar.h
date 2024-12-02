#ifndef DISPLAYVAR_H
#define DISPLAYVAR_H

#include <QObject>
#include "cdebugroot.h"

#define MAX_TYPENAME 100
#define MAX_VALUE    100
#define MAX_NAME     100

class DisplayVar
{
public:
    DisplayVar(CDebugRoot* pDebug);
    virtual ~DisplayVar();

    QString        str_name;
    QString        str_type;
    QString        str_value;
    QString        str_proc;
    unsigned char  depth;
    bool           is_object;
    bool           is_expanded;
    bool           is_lvalue;
    RSLVALUE       val;
    RSLEXPCTX      exp;
    RSLSTACK       st;
    RSLPROC        proc;
    RSLVINFO       info;
    CDebugRoot*    debug;
    RSLMODULE      mod;

    bool isFakeChildrensItem;

    void Free(void);
    bool IsError () const;
    bool IsStringLikeEditable () const;

private:
    QString str_error;
};

#endif // DISPLAYVAR_H
