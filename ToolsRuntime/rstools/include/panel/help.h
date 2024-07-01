/*
         HELP ��⥬�
         VECsoft 30.01.91
         Modified:
         April 15,1993  Support for resource file format

         VERSION INFO :   $Archive: /RS-Tools/Old_tools/panel/h/help.h $
                          $Revision: 7 $
                          $Date: 12.09.00 17:39 $
                          $Author: Lebedev $
                          $Modtime: 12.09.00 16:41 $
*/

#ifndef  _HELPH
#define  _HELPH
#define  __HELP_H

#include <res.h>

#include "pexpdef.h"

#define  REF_LIST   1
#define  DEF_LIST   2

#include <packpsh1.h>

/* ���ଠ�� �� ����� ��⨢��� ����      */
typedef struct
      {
       db_int16  beg;         /* ��砫� ��⨢���� ����          */
       db_int16  end;         /* ����� ��⨢���� ����        */
       db_int16  page;        /* ����� ��࠭��� ����� -1 �᫨ ��࠭�� �� ��⨢��      */
      } HPACT;

// ������� ���������� Help 䠩��
typedef struct tagHelpElem
      {
       Resource  rc;
       db_int16  npage;   /* ����� ��࠭��� (���) � 䠩��          */
      } HelpElem;

#include <packpop.h>

typedef struct
      {
       char *str;       /* ���� ��ப� � ���� ���᪠���      */
       int   numact;    /* ������⢮ ��⨢��� ����� � ��ப� (-1 ��� ����������) */
       int   base;      /* ���� � ���ᨢ� ��⨢��� ����� ��� ����� ��࠭��� ��� ����������       */
      } HELP;

/* ��࠭�� �����          */
typedef struct
      {
       HELP  *ptrH;     /* ���ᨢ ��ப �����        */
       int    numstr;   /* ��᫮ ��ப           */
       HPACT *ptrA;     /* ���ᨢ ��⨢��� �����       */
       int    numact;   /* ��᫮ ��⨢��� �����        */
       HPACT *cA;       /* ����饥 ��⨢��� ����       */
      } PAGE;

#ifdef __cplusplus
extern "C" {
#endif

_RSPNLEXP int _OpenHelp(ResFile *rf, const char *fname, int flags);

/* �᢮������ ��࠭��� �����       */
_RSPNLEXP void PageFree(
                        PAGE *page     /* ��������� ��࠭���            */
                       );

/* �⥭�� ��࠭��� �����        */
/* �� �ᯥ� �����頥� 0       */
_RSPNLEXP int PageLoad(
                       ResFile *rf,
                       int      npage,
                       PAGE    *page      /* ��������� ��࠭���            */
                      );


_RSPNLEXP bool RslAddHelpRange (int ind1, int ind2, const char *name);

ResFile *RslGetHelpFile  (int page, int *status);
ResFile *RslGetHelpFileEx(int page, int *status, const char **fname, int *realPage /* ���筮 NULL */);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */