/*************************************************************************
   File Name      : vcs.h
   Programmer     : Alexey Lebedev

   Creation       : 05 Nov 2002
   Description    : 

   Copyright(C) 2002 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __VCS_H
#define __VCS_H

#include "unixio.h"


#define  VCS_DONTGET      0x000001   // �� ���������� ���� �� ��������
#define  VCS_REPLACE      0x000002   // ��������� ���� �� ��������� ��� �������

#define  VCS_FROMEM       0x000004   // ����� �� RSEM

#define  VCS_KEEPCHECKED  0x000100   // ��������� CheckOut
#define  VCS_REMOVE       0x000200   // ������� ���� �� ��������� ����� CheckIn

#define  VCS_LEAVE        0x010000   // �������� ���� �� ��������� ��� ���������
#define  VCS_DELETE       0x020000   // ������� ���� �� ��������� ����� UndoCheckOut


#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
******************************************************************************/
bool InitVCS(void);
void DoneVCS(void);


/******************************************************************************
   RslLockFile - ��������� �����
   RslUnlockFile - ����� ����������
******************************************************************************/
bool RslLockFile(const char *szFileName);
bool RslUnlockFile(const char *szFileName);


/******************************************************************************
   ����� ����� ��������� �����
******************************************************************************/
int RslFindHandle(const char *szFileName);


/******************************************************************************
   �������� ����� ����������� �����
******************************************************************************/
int RslSetFileTime(const char *szFileName, rstimebuf *timebuf);


/******************************************************************************
   ����� ���� �� ��������������
   ���������:
      szLocalFile - ��� ����� �� ��������� ��� NULL
      szTargetFile- ��� ����� �� ������� ��� NULL, ���� szLocalFile !NULL
      Flags       - VCS_DONTGET | VCS_REPLACE
   ���������� ������ ���� � ����� �� ��������� ��� NULL.
******************************************************************************/

const char *CheckOutFile(const char *szLocalFile, const char *szTargetFile, long Flags);


/******************************************************************************
   ��������� �������������� �����, ����� ������� �� ��������������
   ���������:
      szLocalFile - ��� ����� �� ���������, ���������� �� CheckOut.
      Flags       - VCS_KEEPCHECKED ^ VCS_REMOVE
******************************************************************************/
bool CheckInFile(const char *szLocalFile, long Flags);


/******************************************************************************
   �������� �������������� �����, ����� ������� �� ��������������
   ���������:
      szLocalFile - ��� ����� �� ���������, ���������� �� CheckOut.
      Flags       - VCS_LEAVE | VCS_DELETE
******************************************************************************/
bool UndoCheckOutFile(const char *szLocalFile, long Flags);


/******************************************************************************
   �������� ������ ���� � ����� �� �������
   ���������:
      szLocalFile - ��� ����� �� ���������, ���������� �� CheckOut.
      bStatus     - ������� ��������� �����; ����� true, ���� ���� ���� �� ��������������
   ���������� ������ ���� � ����� �� ������� ��� NULL.
******************************************************************************/
const char *GetTargetFile(const char *szLocalFile, bool *bStatus);


/******************************************************************************
   �������� ������ ���� � ����� �� ���������
   ���������:
      szLocalFile - ��� ����� �� ���������, ���������� �� CheckOut.
      bStatus     - ������� ��������� �����; ����� true, ���� ���� ���� �� ��������������
   ���������� ������ ���� � ����� �� ������� ��� NULL.
******************************************************************************/
const char *GetLocalFile(const char *szTargetFile, bool *bStatus);


/******************************************************************************
   ����������� ���� � ��������� �� ������
   ���������:
      szLocalFile - ��� ����� �� ���������
      szTargetFile- ��� ����� �� �������
   ���������� ������ ���� � ����� �� ������� ��� NULL.
******************************************************************************/
const char* CopyFileToServer(const char *szLocalFile, const char *szTargetFile);


/******************************************************************************
   ����������� ���� � ������� �� ��������
   ���������:
      szLocalFile - ��� ����� �� ���������
      szTargetFile- ��� ����� �� �������
   ���������� ������ ���� � ����� �� ��������� ��� NULL.
******************************************************************************/
const char *CopyFileFromServer(const char *szLocalFile, const char *szTargetFile);

#ifdef __cplusplus
};
#endif

#endif // __VCS_H

/* <-- EOF --> */