// -------------------------------------------------------------------------------------------------

#ifndef __BTRVEM_H
#define __BTRVEM_H

#include "rsfmt.h"

// -----------------------------------------------
class CRsdRecordset;
class CRsdAccessor;
class AccCollection;
class bm_manager;

struct tagRSDRSNAMEBINDING;

// -----------------------------------------------
enum crttabtype
   {
    tempbyfs   = 1,  // ��������� �� FileStat
    tempbyfmt  = 2,  // ��������� �� RSDDICT_FMT
    tabbyname  = 3,  // ���������� �� ����� (RSDDICT_FMT ������ �� �����)
    tabbynfmt  = 4   // ���������� �� ����� � RSDDICT_FMT        
   };

// -----------------------------------------------
typedef tagRSDRSNAMEBINDING   *RSDFIELDSDEF;
typedef tagRSDRSNAMEBINDING  **RSDKEYSDEF;

// -----------------------------------------------
enum SFFLAGS
   {
    sfflocked  =  0x01,  // lock ����
    sffinvcrec =  0x02,  // ������ ������� � ������� ������� ���������
    sfftmptab  =  0x04,  // ������� ���������
    sffsingrec =  0x08,  // ��� ������ �������� ������ ���� ������.
    sffunused1 =  0x10,  // 
    sffreset   =  0x20,  // �������� ��� ��� �������� ��������    
    sffrealqry =  0x40,  // �� ������������ ���
    sffunused2 =  0x80,  // 
    sffusekf   =  0x100, // ������������ �������� ������
    sffuseoptc =  0x200, // ������������ ������� addlink (���.������ � �����������)
    sffusesrtc =  0x400, // ������������ ���������������� ���������� 
    sffnupdacc =  0x800, // �� ��������� ����� ������
    sffrsdtab  = 0x1000  // SQLFILE �� ������ ���������� RSD
   };

// -----------------------------------------------
enum SFPROPERTY
   {
    SF_AUTOUPDATE_RECBUF = 1,
    SF_OPEN_MODE         = 2
   };

// -----------------------------------------------
typedef struct tagSQLFILE
      {
       char           *fileName;       // ��� �������

       tagRSDRSNAMEBINDING  *primaryKey;
       tagRSDRSNAMEBINDING **KeyDef;
       tagRSDRSNAMEBINDING  *fileDef;        

       CRsdRecordset  *pMulti;         // ��������� �� Multi

       CRsdAccessor   *curAcc;         // ��������, ��������������� �������� �����
       CRsdAccessor   *updAcc;         // ��������, ����� ������� �������� ���������� � ������� 
                                           // �� ����� ���� ��� �������� ��� ������ ������
       CRsdAccessor   *pkAcc;          // ��������, ������� ����� ����������� ����� ����������� �����
       
       void           *recBuf;         // ��������� �� ����� ������
       void           *keyBuf;         // ��������� �� ����� �����
       void           *pkBuf;          // ����� ����������� �����
       void           *topVal;         // ������� �������� �����
       void           *botVal;         // ������  �������� �����

       void           *hints;          // ��������� ��� �����������

       AccCollection  *accessors;      // ��������� ��� �������� �����������.
       bm_manager     *bmk_manager;    // ���������� bookmark'���

       void           *status;         // �������
       
       RSDICT_FMT     *fmt;            // ��������� �� ������, ������ ������������ ��� �������� ������� � ������ ����
       
       void           *mainSf;         // ��� �������� ��������� �������
       void           *optmSf;         // ��� �������� ����������������� �������
       void           *sortSf;         // ��� �������� ��-�������
       void           *nextSf;         // ��� ����� ��������

       int             crsType;        // ��� �������

       long            tid;            // ������������� �������

       long            maxvarlen;
       int             recSize;
       int             keySize;
       int             pkSize;

       int             nkeys;
       int             nfields;
       int             curKey;         // ������� ����

       int             mode;           // ����� ��������
       int             nAuto;          // ���������������� ����, ���� 0, �� ������� ��� ��������������
       long            flags;          // SFFLAGS
       void           *sortInfo;
       void           *lastpSf;        // ������ � ��������� ���������
       void           *sort_columns;
       void           *varBufPtr;      // ���������� ����� ������ �������� �� recBuf    

       char            reserve[8];    // ������
      } SQLFILE;

#include "emintf.h"

#endif __BTRVEM_H

/* <-- EOF --> */