#ifndef CSURVEY_H
#define CSURVEY_H

#include "cwatchv.h"

class CSurvey: public CWatchV
{
public:
    CSurvey(CDebug* pParent);
    virtual ~CSurvey();

private:
    void RefreshValue(SpVarPtrType value);
    QString unknownStr;
    QString errorStr;
    QString cannotevalStr;

    int isObject;
    bool isExp;
    char nameStr [MAX_NAME];
    char typeStr [MAX_TYPENAME];
    char valueStr[MAX_VALUE];
    RSLVINFO info;
    RSLVALUE val;
    RSLEXPCTX exp;
};

#endif // CSURVEY_H
