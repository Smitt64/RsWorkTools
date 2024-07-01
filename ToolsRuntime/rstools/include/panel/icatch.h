/*-----------------------------------------------------------------------*-
 File Name   : icatch.h  

 Sybsystem   : Tools  
 Description : TRY/CATCH support

 Source      : icatch.c

 Library(s)  : 

 Copyright (c) 1991 - 1994 by R-Style JSC
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 March 1,1994  Sergei Kubrin (K78) - Create file
 January 20,1995   Serg Kubrin (K78)
            �����稫 ����������� TRY/CATCH �� 10
            ���� JmpErrors
-*-----------------------------------------------------------------------*/

#ifndef __ICATCH_H
#define __ICATCH_H


/*
    ��� �ᯮ�짮���� TRY/CATCH
   �ਬ��:

#define ERCODE 10
void fun3 (void)
{
   if (!myfun ())
      THROW (ERCODE);
}

void fun1 (void)
{
   TRY
      fun1 ();  // �� ������⢮ ������権 ...
      fun2 ();
      fun3 ();
      fun4 ();
   CATCH        // �� THROW ��।��� �ࠢ����� �
      {         // jmperror - ��� �訡�� (��ࠬ��� THROW)
      printf ("Catch in fun1 %d\n",jmperror);
      freefun ();
      }
}

TRY/CATCH ����� ����� ����������� �� MAXERR (5).
THROW ��।��� �ࠢ����� �� ������訩 CATCH. �᫨ THROW ��뢠����
�� �� TRY , � ��뢠���� exit(5).
*/

#include <stdlib.h>
#include <setjmp.h>

#include "pexpdef.h"

#ifdef __cplusplus
extern "C" {
#endif


// ���� ��� THROW

#define  JMPONERROR  setjmp (errjmp[++usejmp <= MAXJMP ? usejmp-1 : MAXJMP - 1])
#define  CLEARJMP    usejmp--
#define  AGAINJMP    usejmp++

#define  ERRORJMP(er)  if(usejmp)                                               \
                        { usejmp--; longjmp(errjmp[usejmp], jmperror = (er)); } \
                       else exit(5);

#define  TRY        if((jmperror = JMPONERROR) == 0) {
#define  CATCH      CLEARJMP; } else
#define  ENDCATCH   }

#define  THROW(er)  { ERRORJMP(er); } 

#define  MAXJMP   10  // ���ᨬ��쭠� ����������� longjmp

extern int     _RSPNLEXP  usejmp;   // ����� �������� �������� TRY
extern int     _RSPNLEXP  jmperror; // ��� ��᫥���� �訡�� ��।���� THROW
extern jmp_buf _RSPNLEXP  errjmp[MAXJMP];  // ���� ���⥪�⮢ �⥪�

#ifdef __cplusplus
}
#endif

#endif // icatch_h

/* <-- EOF --> */