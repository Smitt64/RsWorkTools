/*******************************************************************************
 FILE         :   MIX.H

 COPYRIGHT    :   R-Style Software Lab, 1992,1998

 DESCRIPTION  :   Multi-Panel

 PROGRAMMED BY:   Emil Dovidovitch

 CREATION DATE:   26 Sep 1992

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/PANEL/H/mix.h $
                  $Revision: 3 $
                  $Date: 13.10.98 23:35 $
                  $Author: Markovitch $
                  $Modtime: 13.10.98 23:29 $
*******************************************************************************/

#ifndef __MIX_H
#define __MIX_H

#include "pexpdef.h"

#include <packpshn.h>

typedef struct Mix
      {
       void  *bp;              // ��ꥪ�
       int    (*constr)();     // ���������
       void   (*destr)();      // ��������
       int    (*play)();       // �ந��뢠⥫�
       coord  kt, ke;          // �������� �� TAB � ENTER
       coord  x1, y1, x2, y2;  // �������� ���न����
       void  *Parm;            // ��� ��।�� ��ࠬ��஢

       // 18.02.1997 (K78)
       coord  kst;             // �������� �� Shift-Tab

       // Azartsov Valery 06.05.2010
       bool   IsBSCROL;        // �ਧ��� ��ꥪ� � ⨯�� BSCROL
       void  *AddParm;         // �������⥫�� ���짮��⥫�᪨� ��ࠬ���� (��� AddScroll)
      } Mix;

// �����ଠ
typedef struct Platform
      {
       PANEL *mp;           // ������
       int    (*constr)();  // ���������
       void   (*destr)();   // ��������
       Mix   *MixList;      // ���᮪ ��ꥪ⮢
       int    NMixList;     // ������⢮ ��ꥪ⮢
       Mix   *CurMix;       // ����騩 ��ꥪ�
       int    (*pr)();      // ��ࠡ��稪
       void  *Parm;         // ��� ��।�� ��ࠬ��஢
       int    SwitchKey;    // ��� ������ ��४��祭�� ����� ������� � �������묨 �஫�������
      } Platform;

#include <packpop.h>

#ifdef __cplusplus
extern "C" {
#endif

// �뢥�� �������
_RSPNLEXP int PutPlatform(
                          Platform *pf           // ����⥫� �������
                         );

// ����� �������
_RSPNLEXP void RemPlatform(
                           Platform *pf           // ����⥫� �������
                          );

_RSPNLEXP int UsePlatform(Platform *pf, int mff);

// ��ࠡ���� � �����ମ�.
// �����頥� -key ��� 0 �� ��墠⪥ �����.
_RSPNLEXP int LoopPlatform(
                           Platform *pf,      // ����⥫� �������.
                           int       mff
                          );

_RSPNLEXP int MouseInPanel(PANEL *p);

#ifdef __cplusplus
} ;
#endif

#endif // __MIX_H

/* <-- EOF --> */