/*-----------------------------------------------------------------------*-

 File Name   : threadex.hpp

 Sybsystem   :   
 Description : Thread with wait and exit code support
    �ᯮ�짮����� �⠭������ ��楤�� _beginthread � _beginthreadNT
    �� �������� ������� ��� �����襭�� ��⮪�, �� �������� த�⥫�᪮��
    ��⮪� ������� �����襭�� ���୥�� ��⮪�. ���� 㪠����� ��楤���
    � �������� ����� ��⮪�, �ᯮ�짮���� ��� �����, �.�. �� �����襭��
    ��⮪� ��� ����� ����뢠����. ���⮬� ᮧ���� �㡫���� �⮣� ������.
    ��� ᨭ�஭���樨 ��⮪�� �ᯮ��㥬 ᥬ���.       

    ���������:

       TThread (BOOL useWait = TRUE);

    �᫨ useWait ࠢ�� FLASE, �⪫�砥��� ����������� �������� �����襭��
    ���୥�� ��⮪� � ����祭�� ���� ������.

    ��� ᮧ����� ������ ��⮪� ����室��� ᮧ���� ���୨� ����� ��
    TThread � ��८�।����� ����㠫��� �㭪樨� threadProc.


 Source      : threadex.cpp

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 October 31,1995  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __THREADEX_H
#define __THREADEX_H

#include "tooldef.h"

// -----------------------------------------------------------------------------
#ifndef INFINITE
    #define  INFINITE  0xFFFFFFFF  // Infinite timeout
#endif

// -----------------------------------------------------------------------------
class _TOOLEXP TThread
{
 public:
   enum ThreadState
      {
       NotActive,
       Running,
       Stopped,
       Suspended
      };

   virtual ~TThread();

   int getExitCode();

   int  wait (unsigned timeOut = INFINITE);
   bool wait2(unsigned timeOut = INFINITE);

   bool start();
   void terminate();

   int  terminateAndWait(unsigned timeOut = INFINITE);

   bool shouldTerminate()
   {
    return terminationRequest;
   }

   bool suspend();
   bool resume();

   ThreadState getState()
   {
    return state;
   }

   unsigned getThreadID()
   {
    return threadID;
   }

 protected:
   TThread(bool useWait = true, bool autoDelete = false);

   virtual int exit(int status);

   virtual int threadProc() = 0;

   virtual void setUp();

#if defined (_MSC_VER)
   static unsigned __stdcall entryPoint(void *d);
#else
   static void               entryPoint(void *);
#endif

#ifndef _MSC_VER
   int            retval;   // Thread termination staus
#endif

   uintptr_t      hd;   // Thread handle duplicate

   uintptr_t      sem;   // Semaphore handle

   bool           useWait;   // Use exit code and wait support
   bool           terminationRequest;
   bool           autoDelete;

   ThreadState    state;

   unsigned       threadID;
};

#endif // __THREADEX_H

/* <-- EOF --> */