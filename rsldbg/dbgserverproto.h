#ifndef DBGSERVERPROTO_H
#define DBGSERVERPROTO_H

#include <QObject>

#define RSLDBG_MAGIC "RSLDBG"

#define DBG_REQUEST_HANDSHAKE 100

#include <packpsh1.h>
typedef struct
{
    char magic[6]; // RSLDBG
    qint32 size;
    qint16 action;
}DBGHEADER;

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
