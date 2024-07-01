/************************************************************************
         ���� ��⥬� ��� Turbo C 2.1
         VECsoft 12.04.91

   VERSION INFO :   $Archive: /RS-Tools/Old_tools/PANEL/H/wrap.h $
                    $Revision: 4 $
                    $Date: 27.11.98 20:20 $
                    $Author: Lebedev $
                    $Modtime: 27.11.98 20:04 $
************************************************************************/

#include "pexpdef.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* ����祭�� ��ப�             */
/* �����頥� ���ᨢ 㪠��⥫�� �� ��᪨ */
_RSPNLEXP char **wrap(
                      const char *s,     /* ��ப�               */
                      int         l,     /* ����� ��᪠             */
                      int         h,     /* �������쭮� �᫮ ��᪮�        */
                      int        *n      /* ��᫮ ��᪮�               */
                     );

/* �᢮�������� ���祭��� ��ப�     */
_RSPNLEXP void freewrap(
                        char **w,    /* ���ᨢ 㪠��⥫�� �� ��᪨       */
                        int    n     /* ��᫮ ��᪮�               */
                       );

/* ��ꥤ������ ���祭��� ��ப�      */
/* � �᢮�������� �����        */
void unwrap(
            char **w,    /* ���ᨢ 㪠��⥫�� �� ��᪨       */
            int    n,    /* ��᫮ ��᪮�               */
            char  *s,    /* ��ப�               */
            int    p     /* ������ ��� ��ப�          */
           );

#ifdef __cplusplus
 } ;
#endif

/* <-- EOF --> */