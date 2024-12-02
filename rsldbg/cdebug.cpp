#include "cdebug.h"

CDebug::CDebug(HRD inst, TDbgIntf *dbgftable, QObject *parent)
    : CDebugRoot{parent}
{

}

CDebug::~CDebug()
{

}

void CDebug::GetRemoteVersion()
{
    if(m_nVerHi == 0)
    {
        do_GetVersion(&m_nVerLo, &m_nVerHi);

        if(m_dbgintfMajorVer > m_nVerHi)
            m_dbgintfMajorVer = m_nVerHi;
    }
}

iter_bpdata CDebug::FindBP(TBpData *bpdata)
{
    iter_bpdata  i;

    for(i = m_bpdata.begin(); i != m_bpdata.end(); ++i)
    {
        if((bpdata->bp_type == (*i)->bp_type) && (bpdata->len == (*i)->len) && (bpdata->offs == (*i)->offs))
            return i;
    }

    return m_bpdata.end();
}

bool CDebug::AddBp(TBpData *bpdata)
{
    iter_bpdata i = FindBP(bpdata);

    if(m_bpdata.end() != i)
        return false;
    else
    {
        bpdata->key = ++bpKey;

        elem_bpdata  bpnew(bpdata);
        m_bpdata.push_back(bpnew);
    }

    return true;
}

bool CDebug::DelBp(TBpData *bpdata)
{
    iter_bpdata  i = FindBP(bpdata);

    if(m_bpdata.end() == i)
        return false;
    else
        m_bpdata.erase(i);

    return true;
}

bool CDebug::DisBp(TBpData *bpdata)
{
    iter_bpdata  i = FindBP(bpdata);

    if(m_bpdata.end() == i)
        return false;
    else
        (*i)->bp_type = BP_DISABLED;

    return true;
}

CBPData* CDebug::GetBP()
{
    return &m_bpdata;
}

RSLPROC CDebug::GetCurProc(int index)
{
    int _index = index;

    if(index == -1)
        _index = m_index;

    return m_stackinfo[_index]->proc;
}

RSLSTACK CDebug::GetCurStack (int index)
{
    int  _index = index;

    if(index == -1)
        _index = m_index;

    return m_stackinfo[_index]->st;
}

CStackInfo* CDebug::GetStackInfo (void)
{
    return &m_stackinfo;
}

CSurvey* CDebug::GetSurvey(void)
{
    return &m_survey;
}
