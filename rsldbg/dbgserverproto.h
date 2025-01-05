#ifndef DBGSERVERPROTO_H
#define DBGSERVERPROTO_H

#ifndef IGNORE_INCLUDE
#include <QObject>
#include <QEvent>
#endif

#define RSLDBG_MAGIC "RSLDBG"

#define DBG_REQUEST_HANDSHAKE 100
#define DBG_REQUEST_EVENT 101
#define DBG_REQUEST_UPDATETEXT 102
#define DBG_REQUEST_EXECCONTNUE 103
#define DBG_REQUEST_UPDATEBP 104
#define DBG_REQUEST_UPDATESTACK 105

#define	MSG_BREAKPOINT (QEvent::User + 1)
#define MSG_FINISH     (QEvent::User + 2)
#define MSG_TRACE      (QEvent::User + 3)
#define MSG_EDITWATCH  (QEvent::User + 4)
#define	MSG_LOG        (QEvent::User + 5)

#define  EXECCONTNUE_STEP_OVER           1
#define  EXECCONTNUE_STEP_INTO           2
#define  EXECCONTNUE_DBG_RUN             3
#define  EXECCONTNUE_DBG_ABORT           4
#define  EXECCONTNUE_STEP_OUT            5
#define  EXECCONTNUE_RUN_NEXTMOD         6
#define  EXECCONTNUE_RUN_THISMOD         7
#define  EXECCONTNUE_DBG_ABORT_DEFAULT   8
#define  EXECCONTNUE_EXEC_INTEREXP       9

enum DBGBP_TYPE
{
    DBGBP_ENABLED,
    DBGBP_DISABLED,
    DBGBP_INVISIBLE,
    DBGBP_DELETED
};

#include <packpsh1.h>
typedef struct
{
    char magic[6]; // RSLDBG
    qint32 size;
    qint16 action;
}DBGHEADER;

typedef struct
{
    char BpSetted;
    qint16 bp_type;
    qint32 offs;
    qint32 len;
    char modname[_MAX_PATH];
    qint32 line;
    quint32 key;

    qint64 mod, stmt;
}DBGBPDATA;

typedef struct
{
    qint16 event;
    qint32 flags;

    union
    {
        DBGBPDATA bp_event;
    } data;
}DBGEVENT;

typedef struct
{
    char filename[_MAX_PATH];
    qint32 size;

    qint32 offs;
    qint32 len;
    qint32 line;
}DBG_UPDATETEXT;

typedef struct
{
    qint32 trace_log;
}DBG_EXECCONTNUE;

typedef struct
{
    qint32 count;
}DBG_UPDATEBP;

typedef struct
{
    char func[_MAX_PATH], fullfilename[_MAX_PATH];
    int offs;
    int len;
    int line;
}DBG_UPDATSTACK;
#include <packpop.h>

inline bool DbgCheckHeader(DBGHEADER *hdr)
{
    return !memcmp(hdr->magic, RSLDBG_MAGIC, sizeof(hdr->magic));
}

inline void DbgMakeHeader(DBGHEADER *hdr, const int &datasize,  const qint16 &action)
{
    memset(hdr, 0, sizeof(DBGHEADER));
    memcpy(hdr->magic, RSLDBG_MAGIC, sizeof(hdr->magic));

    hdr->size = datasize;
    hdr->action = action;
}

#endif // DBGSERVERPROTO_H
