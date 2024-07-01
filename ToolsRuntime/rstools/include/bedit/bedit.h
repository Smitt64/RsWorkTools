/*******************************************************************************
 FILE         :   BEDIT.H

 COPYRIGHT    :   VECsoft, 1992
                  R-Style Software Lab, 1998

 DESCRIPTION  :   ���⥬� ࠡ��� � �-䠩����

 PROGRAMMED BY:

 CREATION DATE:   26.11.92

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/bedit/h/bedit.h $
                  $Revision: 31 $
                  $Date: 28.06.00 16:57 $
                  $Author: Kubrin $
                  $Modtime: 28.06.00 16:52 $

 January 20,1995   Serg Kubrin (K78)
    �����প� �࠭���権 RSL - RslTrn, RslAbortTrn
 January 25,1995   Serg Kubrin (K78)
           �������� TrnFlag
 January 3, 1996 Yakov E. Markovitch (JNS)
   ��������� #ifdef __cplusplus, ��᪮�쪮 typedef'��
 February 3, 1996 Yakov E. Markovitch (JNS)
   Added nonscrollable area support
 February 20,1997   Serg Kubrin (K78)
   Horizontal scrolling

*******************************************************************************/

#ifndef __BEDIT_H
#define __BEDIT_H

#include "btrv.h"
#include "desktop.h"
#include "ledit.h"
#include "btserver.h"
#include "beditdef.h"
#include "msel.h"

// ��ࠬ���� ࠡ��� Bedit
#define  NUMREST       3   // ������⢮ ����⮪ ����⠭����� �஫���
#define  TRANSRETRY    5   // ��᫮ ��⮯���஢ � �࠭���樨
#define  TRANSDELAY  220   // �६� ����প� �� ��⮯���� � �࠭���樨

/*******************************************************************************
 JNS
*******************************************************************************/

// Says YES if field scrollable
#define  IsScrollable(p, f)        IsInPanel((p), (f))
// Puts all scrollable fields with offset x, y
#define  PutScrollable(p, x, y)    PutSFields((p), ON, x, y)
// Puts all static fields with offset x, y
#define  PutStatic(p, x, y)        PutSFields((p), OFF, x, y)


/*******************************************************************************/

typedef struct tagBSCROL  BSCROL;


/*******************************************************************************
 Functional typedefs
*******************************************************************************/
typedef int (*JBScrollMaker)(BSCROL *);

// ���ᠭ�� ��楤��� ���樠����樨 �����
typedef int (*JBScrollRecInit)(BSCROL *);

// �ਭ����� 㪠��⥫� �� �஫��� � ���祭�� �����
typedef int (*JBScrollRecTest)(BSCROL *, int);

typedef JBScrollMaker    bscrol_makerec_t;
typedef JBScrollRecInit  bscrol_initrec_t;
typedef JBScrollRecTest  bscrol_testrec_t;

//
// Logging function
//
typedef int (*bedit_logfn_t)(
                             int       iOperation,      // ������
                             BTRVFILE *bfile,           // ����஫��㥬� 䠩�
                             void     *saveb,           // ���� ����� �� �믮������ ����樨
                             int       iSizeVarLenS     // ����� ���� �� �믮������
                            );

/*******************************************************************************/

typedef struct tagTCOLINFO
      {
       const char *colHeader;
       const char *fieldName;
       int          colWidth;
       int           fldType;
       int          decPoint;
      } TCOLINFO;

/*******************************************************************************/

// ����⥫� ᪮����� 䠩�� Btrieve
struct tagBSCROL
     {
      PANEL            Mp;       // ������ �����
      BTRVFILE        *bfile;    // ��뫪� �� Btrieve-䠩�
      int              Mn;       // ������⢮ ����ᥩ ������
      int              numrec;   // ������⢮ ����ᥩ
      int              currec;   // ����� ⥪�饩 ��⨢��� �����
      coord            x,y,l,h;  // ���न���� ������
      PANEL           *Mpe;      // ������ ।���஢���� �����
      PANEL           *Mpi;      // ������ ����� �����

      JBScrollRecInit  n;        // ��楤�� ���樠����樨 �����
      JBScrollRecTest  i;        // ��楤�� �஢�ન �� �����.�����
      JBScrollRecTest  c;        // ��楤�� �஢�ન �� ����䨪.�����
      JBScrollRecTest  d;        // ��楤�� �஢�ન �� 㤠����� �����

      JBScrollMaker    make;     // ��楤�� ��������� ���� �஫����

      unsigned char    mode;     // ०��� �஫����
      void            *saveb;    // ���� ��࠭���� �����
      recpos_t        *ListA;    // ᯨ᮪ 䨧��. ���ᮢ ����ᥩ ������
      PANEL           *Mps;      // ������ �஫����

     //February 20,1997 (K78)
      PANEL            wrkPan;
      char             isTrunc;  // True if last field in wrkPan is truncated
      int              startF;
      char            *headBuff;
      void            *horSaveb; // ���� ��࠭���� �����  (used while horiz scroll)
     // char      inHorScrol;
      unsigned char    mode2;    // ०��� �஫���� (����� �� �����⨫��� � mode)

     // MSEL
      MULTISELECT     *lpSelect; // ᯨ᮪ ���ᮢ��� �롮�
      panel_keyproc_t  proc ;    // ��ࠡ��稪 ���������� - ⥯��� �� �࠭���� ���!

      int              sizeb;    // ����� ����� � ���� ��࠭����
      void            *filtr;    // ��ࠬ���� 䨫���
      void            *allocBuf; // �������᪨ �뤥������ ������

      long             flags;    // 䫠�� ⨯� FiltChanged � �.�.

      char            *userAskOnDelete;  // ���짮��⥫�᪨� ����� �� 㤠����� ����ᥩ

      void            *vHeader;  // �����⥫� �� ��ꥪ� ⨯� std:vector
      coord            old_y;    // ��室��� ���न��� ��砫� ������ �஫�����
      int              old_Mn;   // ��室��� �᫮ ��ப � ������ �஫�����
     };

// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

_BEDITFUNC int BsGoTo(BSCROL *Bs, int disp, int dir);
_BEDITFUNC int BsGoToFld(BSCROL *Bs, int disp);
_BEDITFUNC int BsGoToPos(BSCROL *Bs, recpos_t RecPos);

// �஢�ઠ, ��室����� �� ���� � ������ �஫�����
_BEDITFUNC int MouseInBScrol(BSCROL *Bs);

// �஢��塞 ��� ������� �஫�����, � ⮬ �᫥ � ������, ��� ����ᥩ, � ������������ �� ���� �஫�����
_BEDITFUNC int MouseInBScrolTotal(BSCROL *Bs);

int  IsInPanel(const PANEL *p, const FIELD *f);
void PutSFields(PANEL *p, int scr, coord xoffs, coord yoffs);

/************************ �ਬ�⨢�� �㭪樨 ***************************/

// ����७��� ᮮ�饭�� �� �訡�� ��������
_BEDITFUNC int bexperr(
                       const char *mes,      // ����� ᮮ�饭��
                       int   stat            // ����� ��������
                      );

// ���墠�稪 ᮮ�饭�� �� �訡��� �� ࠡ�� � B-trieve
typedef int (*BErrUserFilterType)(int errStat, const char *msg);
_BEDITFUNC BErrUserFilterType ToolsSetBErrFilter(BErrUserFilterType newFilter);

   // C���饭�� �� �訡�� ��������
_BEDITFUNC int berr( const char *mes, const char *rsd_mes = NULL );

// ������� ᮮ�饭�� berr'� � ���� ⥪�� (������䥩᭠� �㭪��)
_BEDITFUNC void get_berr_text(const char *mes, char *outmes);

// ��⠭����� ���짮��⥫�᪨� ⥪�� ����� �� 㤥���� �����
_BEDITFUNC void BsSetUserAsqOnDelete(BSCROL *Bs, const char *msg);

// ���樠����஢��� B-������
_BEDITFUNC void bsinit(BSCROL *Bs);

// �ࠢ���� ���� �����
_BEDITFUNC int bscompb(BSCROL *Bs);

// ���࠭��� ���� �����
_BEDITFUNC void bssaveb(BSCROL *Bs);

// ����⠭����� ���� �����
_BEDITFUNC void bsrestb(BSCROL *Bs);

// ������ ���������� �஫����
typedef enum UPDTMODE
      {
       UPDTCUR = 1,    // �������� ⥪���� ������ �� �࠭�
       UPDTREC,        // �������� ⥪���� ������, ����㧨� �� 䠩��
       UPDTSCRL,       // �������� ����ࠦ���� �ᥩ ������ �஫����
       UPDTZOOM,       // �������� ��࠭��� � �஫��� ��稭�� � RecPos
       UPDTPAGE        // �������� ��࠭��� ����ᥩ � ������� �஫����

      } UPDTMODE;

#define  CheckUpdtMod(Bs)  ((Bs)->mode & 0x0f)
#define  SetUpdtNo(Bs)     (Bs)->mode &= 0xf0  // ����� ������ �� ���������� �஫����

// ��⠭����� ०�� ���������� �஫����
_BEDITFUNC void SetUpdtMod(
                           BSCROL  *Bs,    // ����⥫� �஫����
                           int      mod    // ०��
                          );

/************************************************************************/
/*         ����� ।���஢���� �஫����         */
#ifndef IsScrlMod
    #define  IsScrlMod(Bs)   ((Bs)->mode & 0x10)               // ��।����� ०�� ।���஢���� �஫�����
    #define  SetScrlNo(Bs)   (Bs)->mode &= 0xef                // �몫. ०�� ।���஢����
    #define  SetScrlMod(Bs)  (Bs)->mode |= (unsigned char)0x10 // ���. ०�� ।���஢����
#endif

#ifndef IsReadOnlyMod
    #define  IsReadOnlyMod(Bs)   ((Bs)->mode & 0x20)               // ��।����� read-only ०��
    #define  SetReadOnlyNo(Bs)   (Bs)->mode &= 0xdf                // �몫. read-only ०��
    #define  SetReadOnlyMod(Bs)  (Bs)->mode |= (unsigned char)0x20 // ���. read-only ०��
#endif

/************************************************************************/
/*         ����� ��⠢�� �����             */
#ifndef IsInsMod
    #define  IsInsMod(Bs)   ((Bs)->mode & 0x40)                    // ��।����� ०�� ��⠢��
    #define  SetInsNo(Bs)   (Bs)->mode &= 0xbf                     // �몫. ०�� ��⠢�� �����
    #define  SetInsMod(Bs)  (Bs)->mode |= (unsigned char)0x40      // ���. ०�� ��⠢�� �����
#endif

#define  IsFindMod(Bs)          ((Bs)->mode & 0x80)                // ��।����� ०�� ���᪠
#define  SetFindNo(Bs)          (Bs)->mode &= 0x7f                 // �몫. ०�� ���᪠
#define  SetFindMod(Bs)         (Bs)->mode |= (unsigned char)0x80  // ���. ०�� ���᪠

#define  IsEmptyScrol(Bs)       (!(Bs)->numrec)                    // ��஫���� ���⮩? (ࠡ�⠥� ��� �஫������ � AllowEmptyMod'�)

#define  IsRecModified(Bs)      ((Bs)->mode2 & 0x04)               // ��।����� ०�� �஫����� ��� ����ᥩ
#define  SetRecNotModified(Bs)  (Bs)->mode2 &= 0xfb                // �몫. ०�� �஫����� ��� ����ᥩ
#define  SetRecModified(Bs)     (Bs)->mode2 |= (unsigned char)0x04 // ���. ०�� �஫����� ��� ����ᥩ

#define  IsAllowEmptyMod(Bs)    ((Bs)->mode2 & 0x02)               // ��।����� ०�� �஫����� ��� ����ᥩ
#define  SetAllowEmptyNo(Bs)    (Bs)->mode2 &= 0xfd                // �몫. ०�� �஫����� ��� ����ᥩ
#define  SetAllowEmptyMod(Bs)   (Bs)->mode2 |= (unsigned char)0x02 // ���. ०�� �஫����� ��� ����ᥩ

#define  IsHorScrolMod(Bs)      ((Bs)->mode2 & 0x01)               // ��।����� ०�� ��ਧ��⠫쭮�� �஫��஢����
#define  SetHorScrolNo(Bs)      (Bs)->mode2 &= 0xfe                // �몫. ०�� ��ਧ��⠫쭮�� �஫��஢����
#define  SetHorScrolMod(Bs)     (Bs)->mode2 |= (unsigned char)0x01 // ���. ०�� ��ਧ��⠫쭮�� �஫��஢����

#define  IsVertScrolMod(Bs)     ((Bs)->mode2 & 0x08)               // ��।����� ०�� ��ਧ��⠫쭮�� �஫��஢����
#define  SetVertScrolNo(Bs)     (Bs)->mode2 &= 0xf7                // �몫. ०�� ��ਧ��⠫쭮�� �஫��஢����
#define  SetVertScrolMod(Bs)    (Bs)->mode2 |= (unsigned char)0x08 // ���. ०�� ��ਧ��⠫쭮�� �஫��஢����

#define  Internal1  0x10 // ���� ��⠭���������� � mode2. �ᯮ������ ⮫쪮 � �����㬥�� ��� ��।�����
                         //  ������ � �஫���� �� ������ �����. ��⠭���������� �� ��室� �� ������ �����.
                         //  ��������� ��᫥ ��।�� � ��ࠡ��稪 �஫����� ENTFIELD

#define  Internal2  0x40 // ���� ��⠭���������� � mode2. �ᯮ������ ⮫쪮 � �����㬥�� ��� ��।�����
                         // ������ � �஫���� �� ������ ।���஢����

#define  FiltMod    0x20 // ���� ��⠭����������, �᫨ � BSCROL'� �ᯮ������ ���짮��⥫�᪨� 䨫���

#define  IsFiltMod(Bs)     ((Bs)->mode2 & FiltMod)
#define  SetFiltMod(Bs)    ((Bs)->flags |= FiltChanged, (Bs)->mode2 |= FiltMod)
#define  UnsetFiltMod(Bs)  (Bs)->mode2 &= (unsigned char)~FiltMod

#define  SkipMakeMod  0x80   // ���� ��⠭����������, �᫨ � make �� ���� ��뢠�� GetE

#ifdef SQLBUILD
    #define  IsSkipMakeMod(Bs)   ((Bs)->mode2 & SkipMakeMod)
    #define  SetSkipMakeNo(Bs)   (Bs)->mode2 &= (unsigned char)~SkipMakeMod
    #define  SetSkipMakeMod(Bs)  (Bs)->mode2 |= (unsigned char)SkipMakeMod
#else
    #define  IsSkipMakeMod(Bs)   ((Bs)->mode2 & 0)
    #define  SetSkipMakeNo(Bs)   (Bs)->mode2 &= (unsigned char)~SkipMakeMod
    #define  SetSkipMakeMod(Bs)  (Bs)->mode2 &= (unsigned char)~SkipMakeMod
#endif

// -----------------------------------------------------------------------------
#define  BegBfile        0x00000001
#define  EndBfile        0x00000002
#define  FiltChanged     0x00000004
#define  PartLine        0x00000008
#define  PartRec         0x00000010

#define  dBSF_RunMode    0x00010000   // ��஫���� � ०��� BUseScrol


#define  IsBsRunMode(Bs)     ((Bs)->flags & dBSF_RunMode)
#define  SetBsRunMode(Bs)    ((Bs)->flags |= dBSF_RunMode)
#define  UnsetBsRunMode(Bs)  ((Bs)->flags &= ~dBSF_RunMode)


#define  IsBegBfile(Bs)      ((Bs)->flags & BegBfile)
#define  SetBegBfile(Bs)     (Bs)->flags |= BegBfile
#define  UnsetBegBfile(Bs)   (Bs)->flags &= ~BegBfile

#define  IsEndBfile(Bs)      ((Bs)->flags & EndBfile)
#define  SetEndBfile(Bs)     (Bs)->flags |= EndBfile
#define  UnsetEndBfile(Bs)   (Bs)->flags &= ~EndBfile

#define  IsFiltCh(Bs)        ((Bs)->flags & FiltChanged)
#define  SetFiltCh(Bs)       (Bs)->flags |= FiltChanged
#define  UnsetFiltCh(Bs)     (Bs)->flags &= ~FiltChanged

#define  IsPartLine(Bs)      (((Bs)->flags & PartLine) ? 1 : 0)
#define  SetPartLine(Bs)     (Bs)->flags |= PartLine
#define  UnsetPartLine(Bs)   (Bs)->flags &= ~PartLine

#define  IsPartRec(Bs)       (((Bs)->flags & PartRec) ? 1 : 0)
#define  SetPartRec(Bs)      (Bs)->flags |= PartRec
#define  UnsetPartRec(Bs)    (Bs)->flags &= ~PartRec

// -----------------------------------------------------------------------------
// ����ਨ����㬥�⠫�� ������ IsFindMod
// �����頥� 1, �᫨ �஫���� ��室���� � ०��� ���᪠ ����� � 0 � ��⠫���
// �����.
int IsBsFindMode(void);

// -----------------------------------------------------------------------------
// ����ਨ����㬥�⠫�� ������ SetFindMod
void SetBsFindMode(void);

// -----------------------------------------------------------------------------
// ����ਨ����㬥�⠫�� ������ SetFindNo
void SetBsFindNo(void);

/************************************************************************/

// ��⠭����� ᮮ�饭�� �஫����
// ᮮ�饭��
_BEDITFUNC void setBMES(char *mes);


/************************* ࠡ�� � �஫���� ***************************/
// ��஫��� ����ᥩ
_BEDITFUNC int BUseScrol(BSCROL *Bs);

// �뢥�� �஫���
_BEDITFUNC int BPutScrol(
                         BSCROL  *Bs,    // ����⥫� �஫����
                         int  savef      // 䫠� ���ﭨ�
                        );

// ������� �஫���
_BEDITFUNC void BRemScrol(BSCROL *Bs);

// �ᯮ�짮���� �஫���
_BEDITFUNC int BLoopScrol(BSCROL *Bs);

// ������� ����ᥩ �஫����
_BEDITFUNC int BEditScrol(BSCROL *Bs);

// ������� ����ᥩ �஫����
_BEDITFUNC int BRunScrol(BSCROL *Bs);

/************************* ��ࠡ��稪� ������� ***************************/

// ��� ������ ��楤��-��ࠡ��稪�� ���짮��⥫�
#define  QUITSCRL  -600  // ���਩�� �������� �஫���
#define  NOREC     -601  // ��� ����ᥩ � 䠩��

// ��� ������ ��楤��-�஢�ப ���짮��⥫�
#define  ALREXE      -1  // ������ �믮����� ���짮��⥫��

// �⠭���⭠� ��楤�� ������ ����� �����
// ������ �� ������:
// ESC - ��� �� ������
// F9  - ����� ������ � ����� ������� �����
_BEDITFUNC int inrec(
                     PANEL *p,  // ����⥫� ������ �����
                     int    key // ��� ������
                    );

// �⠭���⭠� ��楤�� ।���஢���� �����
// ������ �� ������:
// ESC - ��� �� ������
// F8  - 㤠���� ������ � ��� �� ������
// F9  - �������� ������ � ��� �� ������
_BEDITFUNC int edrec(
                     PANEL *p,  // ����⥫� ������ ।���஢����
                     int    key // ��� ������
                    );

// �⠭���⭠� ��楤�� ���㦨����� ᯨ᪠ ���.
// ������ �� ������:
//    ESC   - ��� �� �஫����
//    UP, DOWN, PDUP, PGDOWM, +Ctrl - �������� ⥪���� ������ (�᫨
//       ����) � ��३� �� ����� ������
//    ENTER - �।��⠢��� ������ ।���஢���� ����� (�᫨ ������)
//    F4    - ���� ������
//    F8    - 㤠���� ������
//    F9    - ����� ����� ������ ��� �।��⠢��� ������ ����� (�᫨ ������)
//    NOREC - ����� ����� ������ (�᫨ Edit)
_BEDITFUNC int scrlrec(
                       PANEL *p,  // ����⥫� ������ �஫����
                       int    key // ��� ������
                      );

// �������� ⥪���� ������
// ������: = key - ����� �믮����
// ������: = 0   - ����� ���믮����
// ������: =-key - ࠧ��襭 �஫���
_BEDITFUNC int updtrec(
                       PANEL *p,  // ����⥫� ������
                       int    key // ��� ������
                      );

/******************************* ���� **********************************/

typedef int (*find_rec_proc)(void *buff, void *value);

// ���� ������ ��ॡ�஬
// ��� ������:
//    0 - �������
//  < 0 - �� �������
//  > 0 - �訡�� Btrieve �� ���᪥
_BEDITFUNC int find_rec(
                        BTRVFILE *bf,      // ����⥫� �����
                        void     *value,   // ���祭�� ���᪠
                        find_rec_proc ic   // �ணࠬ�� �஢�ન �����
                       );

// ���� ������ ��ॡ�஬
// ��� ������:
//    0 - �������
//  < 0 - �� �������
//  > 0 - �訡�� Btrieve �� ���᪥
_BEDITFUNC int find_rec2(
                         BSCROL   *bs,      // ����⥫� �����
                         void     *value,   // ���祭�� ���᪠
                         find_rec_proc ic   // �ணࠬ�� �஢�ન �����
                        );

// ���� ������ � �஫����
// ��� ������:
//    0 - �������
//  < 0 - �� �������
//  > 0 - �訡�� Btrieve �� ���᪥
_BEDITFUNC int find_srec(
                         BSCROL *Bs,        // ����⥫� �஫����
                         void   *value,     // ���祭�� ���᪠
                         int    way,        // ���ᮡ ���᪠: 0-�� �����, !0-��ॡ�஬
                         find_rec_proc ic   // �ணࠬ�� �஢�ન �����
                        );

// ���� ������ � �஫���� �� ����
// ��� ������:
//    0 - �������
//  < 0 - �� �������
//  > 0 - �訡�� Btrieve �� ���᪥
_BEDITFUNC int pick_srec(
                         BSCROL *Bs,          // ����⥫� �஫����
                         FIELD  *f,           // ���� ���᪠
                         int    way,          // ���ᮡ ���᪠: =0-�� �����, !0-��ॡ�஬
                         const char *Invite,  // �ਣ��襭�� � ����� ���祭�� ���᪠
                         const char *Oops     // ����饭�� �� ��㤠筮� ���᪥
                        );

// ��⠭����� 䨫��� � �஫���� �� ����
_BEDITFUNC int filtr_srec(
                          BSCROL *Bs,          // ����⥫� �஫����
                          FIELD  *f,           // ���� ���᪠
                          const char *Invite,  // �ਣ��襭�� � ����� ���祭�� 䨫���
                          const char *Oops     // ����饭�� �� ��㤠筮� ���᪥
                         );

// ����� 䨫��� � �஫���� �� ����
_BEDITFUNC int cancel_filtr_srec(
                                 BSCROL *Bs    // ����⥫� �஫����
                                );

// �������� �� ���� �஫����
_BEDITFUNC int bs_filtr(
                        BSCROL *Bs,            // ����⥫� �஫����
                        const char *Invite,    // �ਣ��襭�� � ����� ���祭�� 䨫���
                        const char *Oops       // ����饭�� �� ��㤠筮� ���᪥
                       );

// ����஢�� �� ���� �஫����
// ����⥫� �஫����
_BEDITFUNC int bs_sorting(BSCROL *Bs);

// ����� ����� �஫�����
// ����⥫� �஫����
_BEDITFUNC int bs_print(BSCROL *Bs);

// ����� � �ଠ�XML
// ����⥫� �஫����
_BEDITFUNC int bs_printXML(BSCROL *Bs);

// -----------------------------------------------------------------------------
// ���짮��⥫�᪠� ��ࠡ�⪠ �뤥������ ����ᥩ � �஫�����
// ����⥫� �஫����
_BEDITFUNC int bs_userselected(BSCROL *Bs);

// -----------------------------------------------
// ���祭�� 䫠���, ��।������ � �㭪�� BsSetUserSelectedFlags
typedef enum TEUserSelectedFlag
      {
       eUSF_DisableHandler  = 1,  // �����஢��� ��ࠡ��稪 bs_userselected
       eUSF_DisableProgress = 2,  // �� �����뢠�� �������� �ண���
       eUSF_DisableReport   = 4,  // �����஢��� �뢮� ���� ��᫥ ��ࠡ�⪨ �����
       eUSF_SkipReturnValue = 8   // �� ��ࠡ��뢠�� �����頥��� ����ᮬ ���祭��
      } TEUserSelectedFlag;

// -----------------------------------------------
// �㭪�� ��⠭���� ०���� ࠡ��� ��ࠡ��稪� bs_userselected
// [in]  flags - ��������� (�� OR) 䫠��� TEUserSelectedFlag
// [ret]       - �����頥� �।����� ��������� 䫠���
_BEDITFUNC int BsSetUserSelectedFlags(int flags);

// -----------------------------------------------------------------------------

find_rec_proc GetBsFindProc(void);

/********************* ࠡ�� � ��६����� ����� ***********************/

_BEDITFUNC int BEdit_Var   // ������� ��ப ��६����� ��� �����
(
 LSCROL   *L,    // ����⥫� ।���� ��ப
 BSCROL   *Bs,      // ����⥫� �஫����
 int     LConst, // ����� ����ﭭ�� ��� �����
 int     *CurrNum   // ⥪�饥 ������⢮ ��ப � �����
);

/********************* ࠡ�� � ����ᠬ� ******************************/

// ������� BSCROL
_BEDITFUNC BSCROL *CreateBS(
                            unsigned int    St,    // �⨫� ����ࠦ���� ������
                            unsigned char   Pnumf, // ������⢮ ����� ������
                            unsigned char   Pnumt, // ������⢮ ⥪�⮢ � ᯨ᪥
                            coord           x1,    // ���न���� ������
                            coord           y1,    //
                            coord           x2,    //
                            coord           y2,    //
                            int             PHelp, // ���᪠��� � ������
                            unsigned char   Pff,   // ����� ��砫쭮�� ����
                            int             Mn,    // ������⢮ ����ᥩ � ������
                            coord           x,     // ���न���� ������
                            coord           y,
                            coord           l,
                            coord           h,
                            unsigned        adsize
                           );

_BEDITFUNC BSCROL *CreateColInfoBS(
                                   unsigned int    St,            // �⨫� ����ࠦ���� ������
                                   coord           x,             // ���न���� ������
                                   coord           y,             //
                                   coord           cx,            // ������� �஫�����
                                   coord           cy,            //
                                   panel_keyproc_t proc,          // ��ࠡ��稪 ����������
                                   void           *Parm,          // ��� ��।�� ��ࠬ��஢
                                   const char     *Pstatus,       // ���ଠ�� ��� ��ப� ���ﭨ�
                                   const char     *Phead,         // ���������
                                   BTRVFILE       *bfile,
                                   TCOLINFO       *colInfo,
                                   int             numCols,
                                   int             readOnly,
                                   const char     *uniqName,
                                   unsigned        adsize
                                  );

// ��������� BSCROL.
_BEDITFUNC BSCROL * CrBs(
                         ResFile    *rt,     // ������ �����.
                         const char *member  // ��� ࠧ����.
                        );

// ��।������ BSCROL �� �����
_BEDITFUNC void  BSetFlds(
                          BSCROL          *m,
                          panel_keyproc_t  proc,    // ��ࠡ��稪 ����������
                          void            *Parm,    // ��� ��।�� ��ࠬ��஢
                          void            *Pstatus, // ���ଠ�� ��� ��ப� ���ﭨ�
                          const char      *Phead,   // ���������
                          BTRVFILE        *bfile,   // ��뫪� �� Btrieve-䠩�
                          PANEL           *Mpe,     // ������ ।���஢���� �����
                          PANEL           *Mpi,     // ������ ����� �����
                          bscrol_initrec_t n,       // ��楤�� ���樠����樨 �����
                          bscrol_testrec_t i,       // ��楤�� �஢�ન �� �����.�����
                          bscrol_testrec_t c,       // ��楤�� �஢�ન �� ����䨪.�����
                          bscrol_testrec_t d,       // ��楤�� �஢�ન �� 㤠����� �����
                          bscrol_makerec_t make,    // ��楤�� ��������� ���� �஫����
                          ...
                         );

_BEDITFUNC int ConfCr(char *nf, void *cfb, int l);

_BEDITFUNC void BeditApp(const char *Systema, const char *Desk, const char *Copr, const char *Res,
                         const char *Helpfile, const char *Bdir);


_BEDITFUNC int TrnEx(bedit_trnfn_t t, void *Parm, const char *warn, int concurrent);

// ������ TrnEx ��� ��������� ������
_BEDITFUNC int TrnExClnt(bedit_trnfn_t t, void *Parm, const char *warn, int concurrent, bclient_h client);

#define  Trn(a, b, c)         TrnEx(a, b, c, 1)
#define  TrnClnt(a, b, c, d)  TrnExClnt(a, b, c, 1, d)


_BEDITFUNC int TrnSilentMode(int mode);

/*   January 20,1995 (K78)
    �����প� �࠭���権 RSL
*/
void RslErrorTrn(void);
int  RslTrn(bedit_trnfn_t proc, void *Parm);

// ��⠭����� �㭪�� ����� � ��ୠ�
_BEDITFUNC bedit_logfn_t SetBLogProc(bedit_logfn_t logproc);

// ����� �㭪�� ����� � ��ୠ�
_BEDITFUNC void RemBLogProc(void);

// �맢��� �㭪�� ����� � ��ୠ�
_BEDITFUNC int  _BLogProc(
                          int       iOperation,        // ������
                          BTRVFILE *bfile,             // ����஫��㥬� 䠩�
                          void     *saveb,             // ���� ����� �� �믮������ ����樨
                          int       iSizeVarLenS       // ����� ���� �� �믮������
                         );

_BEDITFUNC int  RestCurRec(BSCROL *Bs);
_BEDITFUNC void DrawHeads(BSCROL *Bs);

_BEDITFUNC int  SRinit(BSCROL *Bs, int key);
_BEDITFUNC void CopyWorkFields(BSCROL *Bs, int startF);

// MSEL
_BEDITFUNC int BsStandardProc(PANEL *p, int key);
_BEDITFUNC int BsStandardMake(BSCROL *Bs);

// ����� ������ �� 䠩�� � ��⮬ ���஢�� � �஫�����
_BEDITFUNC int BsGet(BSCROL *Bs, int bop);
_BEDITFUNC int BsGD(BSCROL *Bs);
_BEDITFUNC int BsGDEx(BSCROL *Bs);
_BEDITFUNC int BsGDp(BSCROL *Bs, recpos_t pos);
_BEDITFUNC int BsGDpEx(BSCROL *Bs, recpos_t pos);

// �������� �஫����, �ᯮ���� ��⠭������� ०��
_BEDITFUNC int BUpdateScrol(BSCROL *Bs);

/************* ��⮬���᪮� ��।������ �ਭ� ������� *****************/

#define  BsSetAutoWidth(ind, w)  SetAutoWidth(ind, w)

/************************ ����� BSCROL ***********************************/

typedef int (*BScrollPrint)(BSCROL *, const char *);

_BEDITFUNC void SetPrintBs(BScrollPrint);      // ��⠭����� �����
_BEDITFUNC void SetPrintBsXML(BScrollPrint);

_BEDITFUNC void SetPrnDirBs(const char *dir);  // ��⠭����� ���.��� 䠩���
                                               // � �ᯥ�⠭�� �஫�����

/****************************************************************************/

// �६����� �㭪�� ��� �⪫�祭�� � ����祭�� �����஢�� POSTREC
_BEDITFUNC void SetBlkPostRec(int val);

// ����祭��/�⪫�祭�� ��७�� �������� ���� �� ����� ��ப�
_BEDITFUNC void BSetDblStrHead(int val);

// ����祭��/�⪫�祭�� ������ ०��� ।���஢����
_BEDITFUNC void BSetNewEditMode(int val);

// �����頥� �� 0, �᫨ ����祭 ���� ०�� ।���஢����
_BEDITFUNC int BGetNewEditMode(void);

_BEDITFUNC int  BeditInit(void);
_BEDITFUNC void BeditDone(void);


// ��ࠢ��� ᮮ�饭�� � ��室� � ���� �� ��室� �� �஫�����

_BEDITFUNC int SendBsFldMesOut(BSCROL *Bs);

// ��⠭����� 䫠� �㭪� ����, �ਪ�. � �஫�����
_BEDITFUNC int SetBsMenuFlag(
                             BSCROL     *bs,    // 㪠��⥫� �� ������, � ���ன �ਪ९���� ����
                             int         key,   // ��� ������, ᮮ⢥����饩 �㭪�� ���� (�.�.0)
                             const char *name,  // ��� �㭪� ���� (�.�.NULL, �� �᫮��� key!=0)
                             int         flag,  // 䫠���
                             int         active // ON / OFF
                            );


// �㭪�� �᢮�������� �����, �뤥������ ��� �஫�����.
//  ��易⥫쭮 ������ ��뢠����, �᫨ � �஫����� �ਪ९���� ����, ����㦥���� �� �����
_BEDITFUNC void FreeBScrol(BSCROL *bs);

_BEDITFUNC void SetBlockMSel(int val);

// ��������� ࠧ��� �஫����� (��᫥���� ��ࠬ��� 0)
int BSResize(PANEL *p, int numRows, int l, bool isPartLine);

_BEDITFUNC int ResizeBScrol(PANEL *p, coord x, coord y, int fSizeMoveLoop);

// ��ᯮ������ �஫���� � �������� ����樨 �࠭�
_BEDITFUNC void AdjustBScrol(
                             BSCROL *Bs,   // ������ �஫��� �롮�
                             coord x,      // ���न���� ������ � ������⢮ �����
                             coord y,
                             coord l
                            );


_BEDITFUNC bool SetBsUserSort(BSCROL *Bs, const char *sortCond);

void RemoveBsBorder(BSCROL *Bs);

void ClearFileMsg(void);

/**************** ���짮��⥫�᪨� 䨫���� � ���஢�� *********************/

bool InitBsFiltr(BSCROL *Bs);
void DoneBsFiltr(BSCROL *Bs);

void UseBsFiltr(BSCROL *Bs, int on);
void ApplyBsFiltr(BSCROL *Bs);
void RestoreBsFiltr(BSCROL *Bs);

bool AddBsFiltr(BSCROL *Bs, int num, find_rec_proc ic, void *fd);
void *DelBsFiltr(BSCROL *Bs, int num);
void *GetBsFiltrData(BSCROL *Bs, int num);

bool AddBsFldFiltr(BSCROL *Bs, FIELD *f, int num, void *v, int len);
void DelBsFldFiltr(BSCROL *Bs, int num);
void DelBsFldFiltr2(BSCROL *Bs, int beg, int end);

bool IsBsFldFiltr(BSCROL *Bs);
_BEDITFUNC bool IsBsFiltr(BSCROL *Bs);

void SetBsFldFiltr(BSCROL *Bs);
int  ShowBsFldFiltr(BSCROL *Bs, const char *Invite, const char *Oops);
int  ShowBsFldSorting(BSCROL *Bs);
bool CanSortBsField(BSCROL *Bs, int field);
int  SwitchBsFldSorting(BSCROL *Bs, int field, int sort);

void SetBsFiltrSaveParm(SaveRestParm pSaveRest);

// -----------------------------------------------------------------------------
// ��ࠢ����� ���������� ������� �஫����� (��뢠���� �� ���⥪��
// BUseScrol - �.�. �� ��ࠡ��稪� �஫�����)
// [in,out]  Bs   - 㪠��⥫� �� �஫����
// [in]      col  - ����� �������
// [in]      hide - ����� (true)/�������� (false) �������
// [ret]          - �� �ᯮ������
_BEDITFUNC int HideBsColumn(BSCROL *Bs, int col, bool hide);

// -----------------------------------------------------------------------------
// ������� ������� � �஫����� (��뢠���� �� BUseScrol)
// [in,out]  Bs   - 㪠��⥫� �� �஫����
// [in]      col  - ����� �������
// [ret]          - �� �ᯮ������
_BEDITFUNC int RemoveBsColumn(BSCROL *Bs, int col);

// -----------------------------------------------------------------------------
// �������/������� ������ ������� ����஫������ �� ���⥪�⭮�� ���� EasyWin
// [in,out]  Bs    - 㪠��⥫� �� �஫����
// [in]      col   - ����� �������
// [in]      block - ������� ��뢠�� ������� (true)/ࠧ���� ��뢠�� (false) �������
// [ret]           - �� �ᯮ������
_BEDITFUNC int BlockHideBsColumn(BSCROL *Bs, int col, bool block);

// -----------------------------------------------------------------------------
// ��������� ����������� ������� ����஫����� � �ᯫ뢠�饩 ���᪠��� � ����
// [in,out]  Bs    - 㪠��⥫� �� �஫����
// [in]      col   - ����� ������� (������� � ⥪�饬 �।�⠢�����)
// [in]      head  - ����� �������� �������
// [in]      tips  - ���� ⥪�� �ᯫ뢠�饩 ���᪠���
_BEDITFUNC int SetBsColumnText(BSCROL *Bs, int col, const char *head, const char *tips);

// -----------------------------------------------------------------------------
// �뤥����� ���� ������� �⨫��
// ��뢠���� �� �㭪樨 Make.
// [in,out] bs     - 㪠��⥫� �� �஫����
// [in]     n      - ����� ���� (�᫨ < 0 - ��� ��ப�)
// [in]     style  - �⨫� ���� (-1 - ���㫥��� �ᥣ� St)
//                   ��� �⨫� ������ �।�⠢���� ᮡ�� १���� ࠡ��� �����
//                   ReS. ���ਬ��, ��� ��⠭���� �⨫� SMES ��ࠬ���� style �㦭� ��।���
//                   ���祭�� ���� ReS(SMES, 0).
_BEDITFUNC void SetBsRowStyle(BSCROL *bs, int n, int style);

// -----------------------------------------------------------------------------
// �뤥����� ���� ������� �⨫��
// � �⫨稥 �� SetBsRowStyle ��ࠬ���� style ����� �����।�⢥��� �������� ���祭�� �⨫�
// (SMES, SRMS � �.�.).
// ��뢠���� �� �㭪樨 Make.
// [in,out] bs     - 㪠��⥫� �� �஫����
// [in]     n      - ����� ���� (�᫨ < 0 - ��� ��ப�)
// [in]     style  - �⨫� ���� (-1 - ���㫥��� �ᥣ� St)
_BEDITFUNC void SetBsRowStyle2(BSCROL *bs, int n, int style);

// -----------------------------------------------------------------------------
// �뤥����� ���� ������� �⨫��
// � �⫨稥 �� SetBsRowStyle ��ࠬ���� style ����� �����।�⢥��� �������� ���祭�� �⨫�
// (SMES, SRMS � �.�.).
// ��뢠���� �� �㭪樨 Make.
// [in,out] bs     - 㪠��⥫� �� �஫����
// [in]     n      - ����� ���� (�᫨ < 0 - ��� ��ப�)
// [in]     style  - �⨫� ���� (-1 - ���㫥��� �ᥣ� St)
// [in]     color  - 梥� 䮭� ���� (dBACK_STYLE_*) (�ᯮ������, �᫨ � ����⢥ style
//                   ��।��� ���祭�� SUSR)
_BEDITFUNC void SetBsRowStyleEx2(BSCROL *bs, int n, int style, int color);

// -----------------------------------------------------------------------------
// �뤥����� ⥪�� ���� ������묨 梥⮬ � �ଠ⮬
// ��뢠���� �� �㭪樨 Make.
// [in,out] bs     - 㪠��⥫� �� �஫����
// [in]     n      - ����� ���� (�᫨ < 0 - ��� ��ப�)
// [in]     color  - 梥� ⥪�� ���� (dTEXT_STYLE_*) (�᫨ < 0 - ��������� �� ��⠭����������)
// [in]     format - �ଠ� ⥪�� ���� (dTEXT_FORMAT_*) (�᫨ < 0 - ��������� �� ��⠭����������)
_BEDITFUNC void SetBsRowColor(BSCROL *bs, int n, int color, int format);

// -----------------------------------------------------------------------------
// �뤥����� ⥪�� ���� ������묨 梥⮬, �ଠ⮬ � �⨫��
// ��뢠���� �� �㭪樨 Make.
// [in,out] bs     - 㪠��⥫� �� �஫����
// [in]     n      - ����� ���� (�᫨ < 0 - ��� ��ப�)
// [in]     color  - 梥� ⥪�� ���� (dTEXT_STYLE_*) (�᫨ < 0 - ��������� �� ��⠭����������)
// [in]     format - �ଠ� ⥪�� ���� (dTEXT_FORMAT_*) (�᫨ < 0 - ��������� �� ��⠭����������)
// [in]     style  - 梥� 䮭� ���� (dBACK_STYLE_*) (�᫨ < 0 - ��������� �� ��⠭����������)
_BEDITFUNC void SetBsRowColorEx(BSCROL *bs, int n, int color, int format, int style);

// -----------------------------------------------------------------------------
// ������ ������ ��� ���� �஫�����
// ��뢠���� �� �㭪樨 Make.
// [in,out] bs     - 㪠��⥫� �� �஫����
// [in]     n      - ����� ���� (�᫨ < 0 - ��� ��ப�)
// [in]     iconID - �����䨪��� ������
_BEDITFUNC void SetBsFieldIcon(BSCROL *bs, int n, int iconID);

// -----------------------------------------------------------------------------
// ������ ������ ��� ���� �஫�����
// ��뢠���� �� �㭪樨 Make.
// [in,out] bs     - 㪠��⥫� �� �஫����
// [in]     n      - ����� ���� (�᫨ < 0 - ��� ��ப�)
// [in]     iconID - �����䨪��� ������
// [in]     flags  - 䫠�� �⮡ࠦ���� ������
_BEDITFUNC void SetBsFieldIconF(BSCROL *bs, int n, int iconID, int flags);

// -----------------------------------------------------------------------------
// ����� ०���� ��� SetBsSizeMode
#define  dSBSM_No          0    //
#define  dSBSM_Maximized   1    // ������ �஫���� � �࠭��� ������᪮� ������ ��������
                                // ���� �ਫ������

// -----------------------------------------------------------------------------
// ������ ०�� ��������� ࠧ��� ����஫�����
// [in,out] bs   - 㪠��⥫� �� �஫����
// [in]     mode - ०�� (�. ����⠭�� ���� dSBSM_*)
_BEDITFUNC void SetBsSizeMode(BSCROL *bs, int mode);


/****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // __BEDIT_H

/* <-- EOF --> */