/*
**  ������ Btrieve ��� �������� ����ᥩ � ����⨢��� ����� (���
**  
**  �।��⠢��� �।�⢠ ��� ���樨 �㭪権 bfOp() � bfGP()
**  
*/

#ifndef __MDBE_H
#define __MDBE_H

#include "mdb/mdb.h"
#include "nwbtrv.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// ����⠭��
enum
   {
    MDBEE_CANTOPEN = -201    // �訡�� "������" �� ����� bfOpenMdb()
   };

// -----------------------------------------------------------------------------
// �� ����樨 �����ন����� ���
#define  mdbeOpIsSupported(btrvOp)                                    \
                          ((btrvOp) == BgetF  || (btrvOp) == BgetL    \
                        || (btrvOp) == BgetN  || (btrvOp) == BgetP    \
                        || (btrvOp) == BgetGT || (btrvOp) == BgetGE   \
                        || (btrvOp) == BgetLT || (btrvOp) == BgetLE   \
                        || (btrvOp) == BgetE                          \
                        || (btrvOp) == BgkF    || (btrvOp) == BgkL    \
                        || (btrvOp) == BgkN    || (btrvOp) == BgkP    \
                        || (btrvOp) == BgkGT   || (btrvOp) == BgkGE   \
                        || (btrvOp) == BgkLT   || (btrvOp) == BgkLE   \
                        || (btrvOp) == BgkE                           \
                        || (btrvOp) == Bgetpos || (btrvOp) == BgetD   \
                        || (btrvOp) == Binsert || (btrvOp) == Bupdate \
                        || (btrvOp) == Bdelete)

// -----------------------------------------------------------------------------
// �㭪�� ��� �����প� bfOp ��� ���
int mdbeOp(int btrvOp, MDB_CURSOR *c, void *keyBuf, unsigned keyNum, int *recSize);

// �㭪�� ��� �����প� bfGP() ��� ���
int mdbeGetPos(MDB_CURSOR *c, recpos_t *pos);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */