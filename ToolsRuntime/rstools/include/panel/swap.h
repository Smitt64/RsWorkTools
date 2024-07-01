/***************************************************************************

   ���⥬� ᢮����� �ணࠬ�      27.02.1992
    swap.asm  - �᭮���� �����, ᮤ�ঠ騩:
      int   far  run_swap(char far *prog, char far *comstr, int swap_size);
            - ����᪠�� ������ �ணࠬ��  
      void  far runstat(char far *swap);
            - �����頥� ��� �訡��  
      void  far setswap(char far *swap);
            - ��⠭�������� ��� 䠩�� ᢮�����

***************************************************************************/

#ifndef __SWAP_H
#define __SWAP_H

#include "pexpdef.h"


#define  DSBLXMS   1
#define  ENBLXMS   0

#ifdef __cplusplus
extern "C" {
#endif

/*   ����᪠�� ������ �ணࠬ��, �����頥� ��� �����襭�� ��� -1    */
_RSPNLEXP int Run_prog(
                       char *program,   /*  ����᪠���� �ணࠬ��                       */
                       char *com,       /*  ��������� ��ப�                            */
                       char *initmes,   /*  ���⮢�� ᮮ�饭��  ���  NULL              */
                       char *pausemes,  /*  ������⥫쭮� ᮮ�饭�� � ��㧠 ��� NULL   */
                       int   swap_size  /*  ������ � KB ����室��� ��� �ணࠬ��       */
                      );

_RSPNLEXP int Run_progEx(
                         char *program,   /*  ����᪠���� �ணࠬ��                       */
                         char *com,       /*  ��������� ��ப�                            */
                         char *initmes,   /*  ���⮢�� ᮮ�饭��  ���  NULL              */
                         char *pausemes,  /*  ������⥫쭮� ᮮ�饭�� � ��㧠 ��� NULL   */
                         int   swap_size, /*  ������ � KB ����室��� ��� �ணࠬ��       */
                         int   catureHd
                        );

_RSPNLEXP int  Run_progEx2(
                           char *program,   /*  ����᪠���� �ணࠬ��                       */
                           char *com,       /*  ��������� ��ப�                            */
                           char *initmes,   /*  ���⮢�� ᮮ�饭��  ���  NULL              */
                           char *pausemes,  /*  ������⥫쭮� ᮮ�饭�� � ��㧠 ��� NULL   */
                           int   swap_size, /*  ������ � KB ����室��� ��� �ணࠬ��       */
                           int   catureHd,
                           FILE *capFile
                          );

#ifndef RSL_MD_FLAT
    int  far run_swap(char far *prog, char far *comstr, int swap_size);
    /*       �᫨ swap_size == 640 , � ���ᨬ���� ᢮����    */
    void far setswap(char far *swap);
    int  far runstat(void);

    int   XMSswap(int dsbl);
#endif // __FLAT__

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */