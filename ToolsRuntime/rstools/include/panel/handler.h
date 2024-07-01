/*           ��ࠡ�⪠ �訡�� ��� Turbo C++ 1.01         */
/*        VECsoft 23.04.91          */

#include "pexpdef.h"

enum DOSERAS
   {
    DER_IGNORE,  /* ������ �ࠢ����� �ணࠬ��        */
    DER_RETRY,   /* ������� ������            */
    DER_ABORT    /* �맢��� ��ࠡ��稪 Ctrl Break                */
   };

extern int _RSPNLEXP  HandDevErr;  /* �ਧ��� �����஢����� �訡�� ���ன�⢠ */
extern int _RSPNLEXP  HandErrLev;  /* �ਧ��� �����⨬��� ABORT         */

#define  HardHand(Level)  { harderr(handler); HandErrLev = Level; }

#ifdef __cplusplus
extern "C" {
#endif

/* ��ࠡ��稪 ����᪨� �訡��    */
int handler(
            int err,   /* ����� �訡��         */
            int ax,    /* �������� �ணࠬ��   */
            int bp,
            int si
           );

int crewin(char *msg);

#ifdef __cplusplus
}
#endif

/* <-- EOF --> */