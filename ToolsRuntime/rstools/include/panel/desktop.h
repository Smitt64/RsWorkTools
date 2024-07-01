/*******************************************************************************
  RS-Bank 4.31,5.0                                       R-Style Software Lab

  ������࠭�� ⥪�⮢� ����䥩� ��� Borland C. DESCTOP

FILENAME: DESKTOP.H
CREATED: 02.11.92
VERSION INFO :  $Archive: /RS-Tools/Old_tools/panel/h/desktop.h $
                $Revision: 16 $
                $Date: 18.02.00 13:35 $
                $Author: Lebedev $
                $Modtime: 17.02.00 15:10 $

MODIFICATIONS:
  26.12.96 Kubrin Sergey
    �������஢�� ����祭�� windows.h
    ��������� �㭪�� JDELAY (jdelay.c)
  02.03.99 ���.
    � ����⥫� ���� (MENU) �������� ��� �࠭���� ���祭�� ��ࠬ���
    �㭪� ����
    ��������� �㭪樨 GetSelectedMenuItem() � SetSelectedMenuItem()
*******************************************************************************/

#ifndef DESKTOP
#define DESKTOP
#define __DESKTOP_H

#include <stdlib.h>

#include "menu.h"
#include "rsmain.h"
#include "jdelay.h"

#include "traceinit.h"


// -----------------------------------------------------------------------------
extern  HTRACE  hPanelTrace;


#include <packpshn.h>

// -----------------------------------------------------------------------------
#define  NM(m)  (sizeof(m)/sizeof(MENU))

// ��� �������஢���� popup-����
#define  POPUP_TAG  UCHAR_MAX

// ��������஢��� id-�㭪� ����
#define  MAKEMENUITEM(lo, hi)  ((lo & 0xFFFF) | ((hi & 0xFFFF) << 16))

// ��������஢��� ������ ����㭪�
#define  MAKETRACKPOS(lo, hi)  ((lo & 0xFFFF) | ((hi & 0xFFFF) << 16))

// 㪠��⥫� �� �ணࠬ�� ��� �맮�� �� ����
typedef int (*FPTR)(PANEL *p, MENU *v);

#include <packpop.h>

#ifdef __cplusplus
extern "C" {
#endif

_RSPNLEXP extern ResFile AppRes;

_RSPNLEXP PANEL *CreateVM(MENU *v, int n, int x, int y, int fl);

_RSPNLEXP PANEL *CreateVMEx(MENU *v, int n, int x, int y, int fl, MenuVar *VarMenu);

// ������� ������ ��ਧ��⠫쭮�� ����.
// �����頥� 㪠��⥫� �� ��।������� ������.
_RSPNLEXP PANEL *CreateHM(MENU  *horm,           // ���᮪ ��ਧ��⠫쭮�� ����.
                          int    n,              // ��� ࠧ���.
                          int    y               // ����� ��ப� ��� ����.
                         );

// ������� ������ ��ਧ��⠫쭮�� ����.
// �����頥� 㪠��⥫� �� ��।������� ������.
_RSPNLEXP PANEL *CreateHMEx(MENU     *horm,           // ���᮪ ��ਧ��⠫쭮�� ����.
                            int       n,              // ��� ࠧ���.
                            int       y,              // ����� ��ப� ��� ����.
                            MenuVar  *VarMenu         // �����⥫� �� ��������, ᮤ�ঠ��� ���� "������".
                           );

// �뢥�� � ��ࠡ��뢠�� ����.
// �����頥� ��� ������� ����.
_RSPNLEXP int LoopM(PANEL *p,              // ������ ����.
                    uchar  clf             // YES ࠧ�蠥� ����뢠���� �� ESC.
                   );

// ���� ��ਧ��⠫쭮�� ����.
// �����頥� ��� ������� ���� ��� 0.
_RSPNLEXP int TestHM(PANEL *p,             // ������ ����.
                     uchar  ffl
                    );

// �������� ��뫪� �� ��࠭�� �㭪� ���� (��᫥ ࠡ��� �㭪樨 TestHM
_RSPNLEXP MENU *GetSelectedMenuItem(void);

// ��⠭����� ����� ��뫪� �� ⥪�騩 �㭪� ���� (����� ���� 0)
// �����頥� �।��饥 ���祭��
_RSPNLEXP MENU *SetSelectedMenuItem(MENU *);

// �������� �����䨪��� ⥪�饣� �㭪� ���� (��� UseMenuPanel ���������!)
int GetCurrentMenuItemId(void);

// ��⠭����� �����䨪��� ⥪�饣� �㭪� ���� (��� UseMenuPanel ���������!)
void SetCurrentMenuItemId(int id);

// ���� ��ਧ��⠫쭮�� ����. �����頥� ��� ������� ���� ��� event.
_RSPNLEXP int TestMenu(PANEL *p,              // ������ ����.
                       int    event
                      );

// ������� ������ ��ਧ��⠫쭮�� ����.
// �����頥� 㪠��⥫� �� ��।������� ������.
_RSPNLEXP PANEL *CrHM(MENU  *horm,           // ���᮪ ��ਧ��⠫쭮�� ����.
                      int    n,              // ��� ࠧ���.
                      int    y               // ����� ��ப� ��� ����.
                     );

// ������� ������ ��ਧ��⠫쭮�� ����.
// �����頥� 㪠��⥫� �� ��।������� ������.
_RSPNLEXP PANEL *CrHMEx(MENU     *horm,            // ���᮪ ��ਧ��⠫쭮�� ����.
                        int       n,               // ��� ࠧ���.
                        int       y,               // ����� ��ப� ��� ����.
                        MenuVar  *VarMenu
                       );

// -----------------------------------------------------------------------------
// �஢����, �� ��室���� �� �� � ����
_RSPNLEXP bool InMenuProgress(void);

// ������ ��ப� �����.
_RSPNLEXP void PutStatLine(void);

// �������� �����.
_RSPNLEXP void UseStatLine(const char *s);

// �ਭ㤨⥫쭮 �������� �����.
_RSPNLEXP void UseStatLine2(const char *s);

_RSPNLEXP int  SizeStatLine(void);
_RSPNLEXP void SaveStatLine(void *psave);
_RSPNLEXP void RestStatLine(void *psave);

_RSPNLEXP void appdesk(const char *Systema, const char *Desk, const char *Copr, const char *Resurs, const char *Helpfile);

_RSPNLEXP void SetNoDesk(int value);


int RSNotifyInit(void);
void RSNotifyDone(void);

// ��ࠡ��稪 ������ ����᭮� ��ப�
int HandlerStatusCmd(int secID, int cmd, int val);

// ��⠭����� ���짮��⥫�᪨� ᯨ᮪ �����ন������ ���祩
// ����� �ப� s �� ����� �ॢ���� 1000 ᨬ����� (�\� ����-ᨬ����)
_RSPNLEXP void SetAppCmdKey(const char *s);

// -----------------
enum
   {
    eAS_NORMAL = 0,      // ��ଠ��� ࠧ���
    eAS_MAX,             // ��ᯠ���� ���� �� ���� �࠭
    eAS_MIN,             // ��������஢��� ����
    eAS_RESTORE,         // ����⠭����� �।��饥 ���ﭨ� ����
    eAS_ACTIVATE = 100,  // ��⨢��� ���� (�������� ������ � �᪡��)
    eAS_ACTIVATE2        // ��⨢��� ���� (�� ���������� �������)
   };

// ���ᨬ���஢���/��������஢��� ���� �ਫ������.
// ����⠥� ⮫쪮 � EasyWin-०���.
// [in]  cmdSize - ������� ����樨 (�. eAS_)
// [ret]         - �� �ᯮ������
_RSPNLEXP int SetAppSize(int cmdSize);

void PanelKbdIndic(int flag);

_RSPNLEXP extern int NoteBook;  // �ணࠬ�� ����饭� �� note book
_RSPNLEXP extern int MonoMode;
_RSPNLEXP extern int RslDefVideoMode;

_RSPNLEXP void SetAppTitle(const char *title);
_RSPNLEXP int  GetAppTitle(char *buf, int size);

_RSPNLEXP int  PanelInit(void);
_RSPNLEXP void PanelDone(void);

_RSPNLEXP int  SetAutoSend(int tick);
_RSPNLEXP void SendToTerminal(void);
_RSPNLEXP void ForceSendToTerminal(void);


// ����� ���譨� �ணࠬ�� � 2-� � 3-� ������.
//
//    StartProgram - ����᪠�� �� �믮������ ������ �ணࠬ��.
//       �����頥� �����䨪��� ����� ��� ��� ���, � �᫨
//       �ந��諠 �訡��, � -1.
//       �᫨ ����� ��� �������� ���������� (DOS, Win16, Dpmi32),
//       ��뢠�� Run_prog �, �᫨ ��� �訡��, �����頥� 0.
//       �᫨ fDetached ࠢ�� false, �����頥� ��� �����,
//       ����� ������ ���� ������ � ������� StopProgram ��� WaitProgram.
//       �᫨ fDetached ࠢ�� tru�, �����頥� �����䨪��� �����.
//
//    StopProgram - ���뢠�� �믮������ ����饭��� �ணࠬ��. ��ࠬ���
//       exitCode ������ ��� �����襭�� ����� (��� Win32) � ᯮᮡ
//       �����襭��. �᫨ �� ࠢ�� -1, ����� �㤥� �����襭 ���਩��.
//       �����頥� 0 ��� -1 � ��砥 �訡��.
//
//    WaitProgram - ������� �����襭�� ����饭��� �ணࠬ�� � �祭��
//       ��������� �६��� ���ࢠ�� �६��� (� �����ᥪ㭤��). �᫨
//       ��ࠬ��� timeOut ࠢ�� -1, �������� �믮������ ����࠭�祭��
//       �����. �����頥� 0 ��� -1 � ��砥 �訡��.
//
//    ExecuteProgram - ����᪠�� ������ �ணࠬ�� � ���� �� �����襭��
//       �����頥�:
//          0, �᫨ �ணࠬ�� �믮����� ��� �騡��
//         -1, �᫨ �ந��諠 �訡��
//         -2, �᫨ �믮������ �ணࠬ�� �뫮 ��ࢠ�� ���짮��⥫��
//

#define  RSL_STILL_ACTIVE  0x0103

_RSPNLEXP intptr_t StartProgram(
                                const char *ProgName,   // ��� �ணࠬ��
                                const char *CmdArgs,    // ��ப� � ᯨ᪮� ��㬥�⮢
                                bool        fRemote,    // � 3� ������ ���᪠�� �� �ନ����
                                bool        fDetached   // ������� ��� �� ����饭�� �����
                               );

intptr_t StartProgramEx(
                        const char *ProgName,   // ��� �ணࠬ��
                        const char *CmdArgs,    // ��ப� � ᯨ᪮� ��㬥�⮢
                        bool        fRemote,    // � 3� ������ ����᪠�� �� �ନ����
                        bool        fDetached,  // ������� ��� �� ����饭�� �����

                        intptr_t    inHandle,   // �⠭����� ����
                        intptr_t    outHandle,  // �⠭�����
                                                // �᫨ ������, ����� ��᫥��� 奭���

                        int         console     // 0 - detached, 1 - current console, 2 - new console
                       );

_RSPNLEXP int StopProgram(
                          intptr_t    hProcess,   // ��� �����, ����祭�� �� StartProgram
                          int         exitCode,   // ��� �����襭�� �����
                          bool        fRemote     // � 3� ������ ���᪠�� �� �ନ����
                         );

_RSPNLEXP int WaitProgram(
                          intptr_t    hProcess,   // ��� �����, ����祭�� �� StartProgram
                          int         timeOut,    // �६� �������� � ����ᥪ㭤��
                          bool        fRemote     // � 3� ������ ���᪠�� �� �ନ����
                         );

_RSPNLEXP int ExecuteProgram(
                             const char *ProgName,   // ��� �ணࠬ��
                             const char *CmdArgs,    // ��ப� � ᯨ᪮� ��㬥�⮢
                             bool        fRemote     // � 3� ������ ���᪠�� �� �ନ����
                            );

bool ActivateProcessWindow (uint32 ptocId, const char *header, bool fRemote);
void OpenDesignerResource (uint32 pId, const char *head, const char *resId, bool remote);


// �뢥�� �� �࠭ ������ � ᮮ�饭���
_RSPNLEXP PANEL * PutMessage(const char *MsgStr, const char *Status);

// ����� ������ � ᮮ�饭��� � �࠭�
_RSPNLEXP void RemMessage(PANEL *pnl);

bool IsKbdFiltered(void);
void SetKbdFiltered();

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */