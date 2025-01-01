#ifndef DBGSERVERPROTO_H
#define DBGSERVERPROTO_H

#ifndef IGNORE_INCLUDE
#include <QObject>
#include <QEvent>
#endif

#define RSLDBG_MAGIC "RSLDBG"

#define DBG_REQUEST_HANDSHAKE 100
#define DBG_REQUEST_EVENT 101

#define	MSG_BREAKPOINT (QEvent::User + 1)
#define MSG_FINISH     (QEvent::User + 2)
#define MSG_TRACE      (QEvent::User + 3)
#define MSG_EDITWATCH  (QEvent::User + 4)
#define	MSG_LOG        (QEvent::User + 5)

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
    qint16 event;
}DBGEVENT;

typedef struct
{
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
    DBGEVENT event;
    char BpSetted;
    DBGBPDATA bp;
}DBGBPEVENT;

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
