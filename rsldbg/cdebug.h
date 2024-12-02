#ifndef CDEBUG_H
#define CDEBUG_H

#include "cdebugroot.h"
#include "cqsurvey.h"
#include "csurvey.h"
#include "lf/bp_data.h"
#include "lf/types.h"

enum SaverCommands
{
    CMD_MODULE_LOAD,
    CMD_MODULE_UNLOAD
};

class SaverBase
{
public:
    virtual ~SaverBase()
    {

    };
    virtual void Save() = 0;
    virtual void Restore() = 0;
    virtual void Command (SaverCommands command, void* params) = 0;
};

typedef struct
{
    int offs;
    int reallength;
    bool isEnabled;
}BPinfo;

bool operator== (const BPinfo& inf1, const BPinfo& inf2);
std::ostream& operator<< (std::ostream& oc, const BPinfo& BPi);
std::istream& operator>> (std::istream& ic, BPinfo& BPi);

// ------------------------------------------------------------------

typedef struct
{
    std::string expStr;
    std::string contextStr;
    std::string typeStr;
    unsigned int depth;
    bool isObject;
    bool isExpanded;
}ExpInfo;

bool operator== (const ExpInfo& inf1, const ExpInfo& inf2);
std::ostream& operator<< (std::ostream& oc, const ExpInfo& inf);
std::istream& operator>> (std::istream& ic, ExpInfo& inf);

// ------------------------------------------------------------------

class CDebug : public CDebugRoot
{
    Q_OBJECT
public:
    CDebug(HRD inst, TDbgIntf*  dbgftable, QObject *parent = nullptr);
    virtual ~CDebug();

    void GetRemoteVersion();

    int m_nVerHi;
    short int m_nVerLo;

    iter_bpdata FindBP (TBpData*);
    bool AddBp(TBpData*);
    bool DelBp(TBpData*);
    bool DisBp(TBpData*);
    CBPData* GetBP();

    RSLPROC GetCurProc(int index = -1);
    RSLSTACK GetCurStack (int index = -1);
    CStackInfo* GetStackInfo (void);
    CSurvey* GetSurvey(void);

private:
    CStackInfo m_stackinfo;
    CSurvey m_survey;
    CQSurvey m_qsurvey;
    int m_index;
    CBPData m_bpdata;
    unsigned long bpKey;
};

#endif // CDEBUG_H
