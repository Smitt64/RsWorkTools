/********************************************************************
   Created: 2002/03/27
   Created: 27:3:2002   16:04
   Filename: D:\Work\RSFormatsC\RSFmtIntfC.h
   Project: RSFormat
   Author: Andrew Ushakov
*********************************************************************/

#ifndef __RSFMTINTFC_H
#define __RSFMTINTFC_H

#include "fieldtp.h"
#include "platform.h"

#include "packpshn.h"

//////////////////////////////////////////////////////
// ������������ ��� �������� ������
//////////////////////////////////////////////////////
typedef struct _fmt_value
      {
       // ��� ������������� ��������
       int   typeValue;

       // ��������� �� ������
       void *pValueBuff;

       // ������ ������� (��� ���������� ������������ ������ ��� typeValue==STRING_T,
       // (��� �������� ������� ������ � ������ ���������� �������), 
       // ��� ��������� ����� ������ ����� ������, ����������� � pValueBuff ������ 
       // ������������ � ��������������� typeValue.
       unsigned short iBuffSize;
      } fmt_value;

#define  SET_FMT_VALUE(val, type, pBuff, size)  {val.typeValue = (type); val.pValueBuff = (pBuff); val.iBuffSize = (size); }

#if defined(RSL_MD_SUN)
    #define  RSFMTAPI
#elif defined(RSL_PL_WIN32)
    #define  RSFMTAPI  __stdcall
#endif

// ��������, ������������ ��� Move � Select
#define  FT_HOME              0
#define  FT_LEFT              1
#define  FT_RIGHT             2
#define  FT_END               3
#define  FT_LEFTWORD          6
#define  FT_RIGHTWORD         7

#define  FT_MODE_INSERT       1
#define  FT_MODE_OVERWRITE    0

#define  FT_FMT_C             1
#define  FT_FMT_RSL           2

#define  FT_FMT_OEM           1
#define  FT_FMT_ANSI          2

#define  FT_ALIGN_RIGHT       0
#define  FT_ALIGN_LEFT        1
#define  FT_ALIGN_CENTER      2

#define  FT_GETVAL          255

typedef struct tagFormat
      {
       // ������� ������, ������ ���� ������� ����� ������������
       bool (RSFMTAPI *Create)(struct tagFormat *, const char *);
       bool (RSFMTAPI *CreateEx)(struct tagFormat *, const char *);

       // ���������� ��������
       bool (RSFMTAPI *ImportStr)(struct tagFormat *, const char *);
       bool (RSFMTAPI *ImportVal)(struct tagFormat *, const fmt_value *);

       // �������� ��������
       bool (RSFMTAPI *Export)(struct tagFormat *, fmt_value *);

       // �������� �������������� ������
       const char* (RSFMTAPI *Result)(struct tagFormat *);

       // ����� �������������� insert(1) / overwrite(0)
       // ���������� ���������� ��������
       unsigned char (RSFMTAPI *ModeSet)(struct tagFormat *, unsigned char);
       unsigned char (RSFMTAPI *ModeGet)(struct tagFormat *);

       // ������ �����
       bool (RSFMTAPI *SetLimit)(struct tagFormat *, unsigned long);

       // �������� �����
       unsigned long (RSFMTAPI *GetLimit)(struct tagFormat *);

         // �������
       bool (RSFMTAPI *Del)(struct tagFormat *);
       bool (RSFMTAPI *BSpace)(struct tagFormat *);

       // �������� � ������� �������
       bool (RSFMTAPI *Insert)(struct tagFormat *, char);

       // ����������� ������
       bool (RSFMTAPI *Move)(struct tagFormat *, int);
       bool (RSFMTAPI *MoveTo)(struct tagFormat *, unsigned long);

       // true, ���� ������ ���-������ ��������, ����� - false
       bool (RSFMTAPI *IsSelected)(struct tagFormat *);

       // ����� / ������ ������� �����������
       unsigned long (RSFMTAPI *LeftSelected)(struct tagFormat *);
       unsigned long (RSFMTAPI *RightSelected)(struct tagFormat *);

       // �������� �������, ��� ������� ������������� �� �������
       // �������, ����� ������� ��
       bool (RSFMTAPI *Select)(struct tagFormat *, int);
       bool (RSFMTAPI *SelectTo)(struct tagFormat *, long);

       // �������� ��������
       void (RSFMTAPI *Unselect)(struct tagFormat *);

       // ������� �������
       unsigned long (RSFMTAPI *Current)(struct tagFormat *);

       // ������ ������ ��� ������������� �������� � �������
       void (RSFMTAPI *SetSpaceSymbol)(struct tagFormat *, char);

       // ������ ���������� ��� ������������� � Tools
       void (RSFMTAPI *CursorAtLastChar)(struct tagFormat *, int);

       // ������ ������ � ������� �� ���������
       bool (RSFMTAPI *SetDefCursor)(struct tagFormat *);

       // ���������� ���������� ������
       void (RSFMTAPI *ResetString)(struct tagFormat *);

       // ��� ���� ��������(���), �������� ����� �������������� ���
       // ����� ����� ����������
       bool (RSFMTAPI *Activate)(struct tagFormat *);

       // �������� ��� ��������� ���������� (1 - �++, 2 - RSL)
       unsigned char (RSFMTAPI *GetFmtVersion)(struct tagFormat *);

       // �������� ����� ����������� ��������-������������
       void (RSFMTAPI *GetDelimSym)(struct tagFormat *, char *, unsigned char);

       // ���������� ��������� OEM ��� ANSI (1 - OEM, 2- ANSI)
       unsigned char (RSFMTAPI *SetCode)(struct tagFormat *, unsigned char);

       // �������� ����� ������ ����������
       unsigned long (RSFMTAPI *GetVersion)(struct tagFormat *);

       // �������� ��� ������������
       unsigned char (RSFMTAPI *SetAlign)(struct tagFormat *, unsigned char);

       // ������ �������� ��� ���� ���������� � �������� �������� ������ � ��� ������ ��� ������
       // (1 - ��� ����������, 2 - ��� �� ������ ��-�� ���������� � �����). ������������� �������� ������������
       void (RSFMTAPI *SetVarType)(struct tagFormat *, int, int);

       // �������� ��������, ������������� � SetVarType
       void (RSFMTAPI *GetVarType)(struct tagFormat *, int *, int *);

       // ������ ��� ���� ��������� �� ���������
       void (RSFMTAPI *SetDefSelection)(struct tagFormat *);

       // ��� �� �������������� ����� ������� ��������
       bool (RSFMTAPI *IsDirty)(struct tagFormat *);

       // �������� ������ ��� ������������
       void (RSFMTAPI *RawResult)(struct tagFormat *, char *, int);

       // �������� �� ��� ������
       bool (RSFMTAPI *IsAllSelected)(struct tagFormat *);

       // ���������� ������ ��������������
       void (RSFMTAPI *SetDirty)(struct tagFormat *, bool);

       // �������� ��� ������������
       unsigned char (RSFMTAPI *SetAlignEx)(struct tagFormat *, unsigned char, int *);

       // 0 - �� ������������ ������ ��� ��������������, 1 - ������������
       unsigned char (RSFMTAPI *SetEditMode)(struct tagFormat *, unsigned char mode);
      } FORMAT, *PFORMAT;

#include "packpop.h"

#endif // __RSFMTINTFC_H

/* <-- EOF --> */