/*************************************************************************
   File Name      : btdict.h
   Programmer     : Alexey Lebedev

   Creation       : 29 Aug 2002
   Description    : ������ � ��������� �������

   Copyright(C) 2002 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __BTDICT_H
#define __BTDICT_H

typedef struct tagBTKeyInfo   BTKeyInfo;
typedef struct tagBTFileInfo  BTFileInfo;
typedef struct tagBTFieldInfo BTFieldInfo;

#include "btrv.h"

#include "packpshn.h"

// �������� ����� ���� ������ � ��������� �������
struct tagBTFileInfo
     {
      char          *FileName;
      char          *FileOwner;
      char          *FileDesc;
      unsigned short FileFlags;
      unsigned short OwnerType;
      unsigned short MaxVarLen;
      unsigned short RecordLen;
      unsigned char  BLOBFlag;
      unsigned char  Reserve;

      char          *Dictionary; // ��� ������� ��

      unsigned short nFields;    // ����� ����� (������ ������� Fields)
      unsigned short nKeys;      // ����� ��������� ������ (������ ������� Keys)

      BTFieldInfo   *Fields;
      BTKeyInfo     *Keys;
      unsigned short PageSize;
     };

// �������� ���� � ��������� �������
struct tagBTFieldInfo
     {
      char          *FldName;    // �������� ����
      char          *FldDesc;    // �������� ����
      unsigned short FldType;    // ��� ���� FT_ (enum TFVT)
      unsigned short FldSize;    // ������ ���� � ������
      unsigned short FldOffset;  // �������� ���� � ������ �� ������ ������
      unsigned short FldLength;  // ����� ���� ��� ������
      short          FldPoint;   // ������� ����� ��� ������ ����
      unsigned char  Hidden;     // �� ���������� ���� � �����������
      unsigned char  Reserve;
     };

// �������� ����� � ��������� �������
struct tagBTKeyInfo
     {
      unsigned short FldNum;     // ����� ���� (������ � ������� Fields)
      unsigned short KeyNum;     // ����� �����, ��� �������� � ����� �����
                                 // ���������� �����
      unsigned short KeyType;    // ��� ����� (nwbtrv.h - extendet key type)
      unsigned short KeySize;    // ������ ����� � ������ (�� ������ FldSize)
      unsigned short KeyFlags;   // ����� ����� (nwbtrv.h - enum Key_Flags)
      char           KeyNull;    // ������� ��������
      unsigned short Reserve;
     };

#include "packpop.h"

/************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

_BTEXP void InitBtDictionary(void);
_BTEXP void DoneBtDictionary(void);

// ������ ������ �������� ��
// ���������: szDicList - ������ ��������, ����������� ';'
_BTEXP void BtSetDictionary(const char *szDicList);

// ������ ������ ��������� ��� ������ ��������
_BTEXP void BtSetDatabasePath(const char *szDataPath);

// �������� ������ ��������, ����������� ��������� �;� � ���������� ����� �������������� ������.
// ���� �������� szDicList ����� NULL, �� ������� ������ ���������� ����� �������������� ������.
_BTEXP int BtGetDictionary(char *szDicList);

// ��������� ������� � ������. � ������ ������ ���������� 0.
// ���� ����� ������� ��� ������� � ������, ������� ���������� 1.
// � ������ ������ ������������ -1.
// � szDic ������ ���� ������ ������ ���� �������.
_BTEXP int BtAddDictionary(const char *szDic);

// ������� ������� �� ������. � ������ ������ ���������� 0.
// ���� ������� ����������� � ������, ������� ���������� 1. ���� ��������� ������� ������,
// ����� ��������� ��� ��������.
// � ������ ������ ������������ -1.
// � szDic ������ ���� ������ ������ ���� �������.
_BTEXP int BtRemoveDictionary(const char *szDic);

// ��������� ������� ������� � ������. ���� ������� ������������, ���������� 0. ����� 1.
// � ������ ������ ������������ -1.
// � szDic ������ ���� ������ ������ ���� �������.
_BTEXP int BtCheckDictionary(const char *szDic);


// ����� �������� �����
enum BtFileInfoFlags
   {
    OnlyFileInfo = 1,
   };

// �������� �������� �����
_BTEXP BTFileInfo *BtGetFileInfo(const char *szFileName, int Flags);
_BTEXP BTFileInfo *BtGetFileInfo2(BTRVFILE *bf);

// ���������� �������� �����
_BTEXP void BtFreeFileInfo (BTFileInfo**);
_BTEXP void BtFreeFileInfo2 (BTRVFILE* bf, BTFileInfo**);

// �������� ��� � ����������
_BTEXP void BtClearCache();

// �������� ����������� �������� �������
_BTEXP BTFileInfo *BtGetDictFileInfo(const void *rbuf, size_t rlen);

// ���������, �������� �� ���������� �������� �������� ��
_BTEXP int IsBtDictFileInfo(const BTFileInfo *fileInfo);

// �������� �������� �������������� ����
_BTEXP int BtGetDictFldVal(const char *dict, const void *rec, size_t offset, char *buf, size_t sz);

// ��������� ����� � ������������ � ��������� ����� � �������
_BTEXP int BtSetDataBuffer(BTFileInfo *fileInfo, int key, int mode, void *buf, size_t *len);

#ifdef __cplusplus
}
#endif

#endif // __BTDICT_H

/* <-- EOF --> */