#ifndef TYPES_H
#define TYPES_H

#include <QSharedPointer>
#include "bp_data.h"
#include "rsl/dbgintf.h"

#include <vector>
//#include <string>
#include <memory>
#include <algorithm>

typedef struct tagTStackInfo
{
    RSLSTACK	st;
    RSLPROC		proc;
    RSLSTMT		stmt;
    RSLMODULE	mod;
    char		procname[100];
    char		modname[100];
    int			offs,
        len;
} TStackInfo;

typedef struct tagTModuleInfo
{
    QString		name;
    int			type;
    RSLMODULE	hmod;
} TModuleInfo;

typedef	QSharedPointer<TBpData> elem_bpdata;
typedef QSharedPointer<TStackInfo> elem_stackinfo;
typedef QSharedPointer<TModuleInfo> elem_moduleinfo;
//typedef std::auto_ptr < TTraceMsg > elem_tracemsg;

typedef	QString elem_qwatch;

typedef	std::vector<elem_bpdata>    CBPData;
typedef	std::vector<elem_stackinfo> CStackInfo;
//typedef std::vector<elem_tracemsg> CTraceMsg;

//typedef CTraceMsg::iterator     iter_tracemsg;
typedef	CBPData::iterator        iter_bpdata;
typedef CStackInfo::iterator      iter_stackinfo;

typedef std::vector<elem_moduleinfo> CModuleInfo;
typedef CModuleInfo::iterator iter_moduleinfo;

#endif // TYPES_H
