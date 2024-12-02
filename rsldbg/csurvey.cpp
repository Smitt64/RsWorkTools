#include "csurvey.h"

CSurvey::CSurvey(CDebug *pParent) :
    CWatchV(pParent)
{
    unknownStr = "unknown";
    errorStr = "error";
    cannotevalStr = "cannot evaluate";
}

CSurvey::~CSurvey()
{

}

void CSurvey::RefreshValue (SpVarPtrType value)
{
    bool executed     = false;
    bool parseExpired = false;
    if (value->exp)
    {
        executed = ExecuteExpression (value->exp);
        if (!executed)
            parseExpired = true;
    }

    if (!executed)
    {
        bool parsed = ParseExpression (value);

        if (parsed)
            executed = ExecuteExpression (value->exp);

        if (!executed)
        {
            isObject = 0;
            if (parseExpired)
            {
                CString errStr;
                GetLastErrorText (m_parent, &errStr, errorStr);

                strcpy (valueStr, errStr.GetBuffer ());
                strcpy (typeStr,  errorStr.GetBuffer ());
            }
            else
            {
                CString errStr;
                GetLastErrorText (m_parent, &errStr, errorStr);

                strcpy (valueStr, errStr.GetBuffer ());
                strcpy (typeStr,  errorStr.GetBuffer ());
            }
        }
    }

    if (executed)
    {
        value->proc     = m_parent->GetCurProc ();
        value->st       = m_parent->GetCurStack ();
        int index       = m_parent->FindStackIndex (value->st);
        value->str_proc = (*m_parent->GetStackInfo ())[index]->procname;
    }

    info = NULL;
    ApplyCurInfoToElem (value);
}
