/*******************************************************************************
 FILE         :   RSMAIN.H

 COPYRIGHT    :   R-Style Software Lab, 1999

 DESCRIPTION  :   RsMain description

 PROGRAMMED BY:   Yakov E. Markovitch

 CREATION DATE:   6 May 1999

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/PANEL/H/rsmain.h $
                  $Revision: 3 $
                  $Date: 6.05.99 18:57 $
                  $Author: Markovitch $
                  $Modtime: 6.05.99 18:57 $
*******************************************************************************/

#ifndef __RSMAIN_H
#define __RSMAIN_H

#include <stdlib.h>

#include "fs.h"
#include "rsatexit.h"
#include "pexpdef.h"


// -----------------------------------------------------------------------------
#include <packpshn.h>

typedef struct CheckArgsParm
      {
       int     argc;    // ��㬥��� ��������� ��ப�
       char  **argv;
       int     remote;  // 1 - ��姢����, 0 - ���姢����
       int     isGUI;   // 1 - ����᪮� �ਫ������ (����� �ᯮ�짮���� printf - � ����
                        //     ��� ���᮫� - ⮫쪮 MessageBox), 0 - ���᮫쭮�
      } CheckArgsParm;

#include <packpop.h>

// -----------------------------------------------------------------------------
#ifdef __cplusplus

    int RsFmMain(int argc, char **argv);  // This function is only used in C++!

extern "C" {
#endif


_RSPNLEXP extern int   isDebug;
_RSPNLEXP extern int   isRemote;
_RSPNLEXP extern int   isConsole;
_RSPNLEXP extern int   NormalTerminateFlag;
_RSPNLEXP extern bool  isSql;
          
_RSPNLEXP extern bool  isRceMode;

typedef int (CheckArgs_t)(const CheckArgsParm *);

_RSPNLEXP void DisableEasyWin(void);
_RSPNLEXP void SetCheckArgs(CheckArgs_t *);

// �㭪�� ��뢠���� �� fs_startApp. � �ਪ������ ���� ��� �㭪�� ����� ��८�।�����.
// �᫨ �㭪�� �����頥� 0, �ணࠬ�� �����蠥���.
// ��� ������: 1 - ࠧ���� ࠡ��� �ணࠬ��, 0 - �������.
int CheckArgs(const CheckArgsParm *);


// -----------------------------------------------------------------------------
#define  RSL_NO_EASYWIN    int CheckArgs(const CheckArgsParm *parm) { DisableEasyWin(); return 1; }
#define  RSFM_APPLICATION  int CheckArgs(const CheckArgsParm *parm) { DisableEasyWin(); SetRsFmMode (1); return 1; }


_RSPNLEXP int RslStartApp(int argc, char **argv, fsapp_mainproc_t rsMain, int noFS);
_RSPNLEXP int RslWinMain(char *szCmdLine, fsapp_mainproc_t rsMain, int noFS);

// �������� ࠡ��� �ணࠬ��.
// ��ࠬ��� cexit �������� ������ ०�� �����襭�� cexit:
//    1 - �맢��� �� �㭪樨 ��ॣ����஢��� �१ RslAddExitProc, �������� ࠡ��� �ணࠬ�� �㭪樥� exit();
//    0 - �������� ࠡ��� �ணࠬ�� �㭪樥� exit();
//   -1 - ���਩�� �������� �ணࠬ�� �맢�� abort().    
// exitCode - ��� ������ �ਫ������
_RSPNLEXP void RslStopApp(int cexit, int exitCode);


// -----------------------------------------------------------------------------
// ��⠭����� ��� ������ �� �ਫ������
// �����頥��� ���祭�� �� �ᯮ������
_RSPNLEXP int RslSetExitCode(int code);

// ������ ��㬥�⮢ ��������� ��ப�. ���뢠� ��㬥���, �����祭�� �
// ������ ����窨 - "large argument". ��� ⠪�� ��㬥�⮢ �������⥫�
// �����ন������ esc-ᨬ����: \xHH \oOOO \DDD � �⠭����� \a\b\f\n\r\t.

_RSPNLEXP size_t cmds2argl(char *cmds, char **argv, size_t size);


// -----------------------------------------------------------------------------
int RsMain(int argc, char **argv);

_RSPNLEXP void SetRsFmMode (int mod);
_RSPNLEXP int  IsRsFmMode  (void);
_RSPNLEXP void DoneRsForms (void);
          bool UseLocalRsFm();


_RSPNLEXP int RsFmAppInit  (int argc, char **argv);
_RSPNLEXP int RsFmAppInitEx(int argc, char **argv, const char *appName);

void SyncWithTerminal(const char *appName);


#ifdef __cplusplus
} ;
#endif


// -----------------------------------------------------------------------------
#define  RSMAIN(argc, argv)  int RsMain(int argc, char **argv)

#endif // __RSMAIN_H

/* <-- EOF --> */