
/*
**  ���஢���� ��� 䠩�� �� � ����� ����ᥩ �������� ����ᥩ �
**  ����⨢��� ����� (���)
**  �������� ����樨:
**  - �४��஢���� ��� 䠩�� �� � ���
**  - ᨭ�஭����� ����� ����ᥩ ��� � 䠩��� ��
*/

#ifndef __BMDB_H
#define __BMDB_H

#include "btrv.h"
#include "mdb/mdb.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// ����⠭��
enum
   {
    BMDB_TEMPL_CACHE_WATERLINE =   50,  // ���૨��� LRU-��� 蠡�����

    BMDB_SKIP_REC              =   -3,  // �ய����� ������ �� �४���./ᨭ��.
    BMDB_ABORT                 =   -4,  // ��ࢠ�� �४��஢����
    BMDB_UPDATE_REC            =   -5,  // ���� ����� �������, �ॡ���� ����������

    BMDBE_SYNC_FAILED          = -101,  // ����� ᨭ�஭���樨
    BMDBE_ALREADY_ASSIGNED     = -102,  // � ����� ����஬ ����ᥩ 㦥 ���樨஢�� ������� Btrieve-䠩�
    BMDBE_NO_FILE_ASSIGNED     = -103,  // � ����� ����஬ ����ᥩ �� ���樨஢�� Btrieve-䠩�
    BMDBE_PRECACHE_FAILED      = -104   // �४��஢���� ����������
   };

// -----------------------------------------------------------------------------
// ����७�� �訡�� ��� ᮮ�饭�� BMDB_SYNC_FAIL.
// ���������� �㭪樥� mdbGetState.
enum
   {
    BMDBE_SF_OK                =  0,    // �ᯥ�
    BMDBE_SF_NO_FILE_ASSIGNED,          // ������ BMDBE_NO_FILE_ASSIGNED
    BMDBE_SF_SYNC_BEGIN,                // ��� �訡�� BMDB_SYNC_BEGIN
    BMDBE_SF_MDB_CREATE_CURSOR,         // �訡�� �맮�� mdbCreateCursor
    BMDBE_SF_MDB_GETPOS,                // �訡�� �맮�� mdbGetPos
    BMDBE_SF_MDB_NEXT,                  // �訡�� �맮�� mdbNext
    BMDBE_SF_BF_GDP,                    // �訡�� �맮�� bfGDp
    BMDBE_SF_CONCURENT,                 // ������ �����७⭮ ��������
    BMDBE_SF_BEFORE_UPD,                // �訡�� �맮�� BMDB_SYNC_BEFORE_UPD
    BMDBE_SF_BF_UPDATE,                 // �訡�� �맮�� bfOp(Bupdate)
    BMDBE_SF_AFTER_UPD,                 // �訡�� �맮�� BMDB_SYNC_AFTER_UPD
    BMDBE_SF_BEFORE_DEL,                // �訡�� �맮�� BMDB_SYNC_BEFORE_DEL
    BMDBE_SF_BF_DELETE,                 // �訡�� �맮�� bfOp(Bdelete)
    BMDBE_SF_AFTER_DEL,                 // �訡�� �맮�� BMDB_SYNC_AFTER_DEL
    BMDBE_SF_NO_ACTION,                 // �訡�� �맮�� BMDB_SYNC_NO_ACTION
    BMDBE_SF_BEFORE_INS,                // �訡�� �맮�� BMDB_SYNC_BEFORE_INS
    BMDBE_SF_BF_INSERT,                 // �訡�� �맮�� bfOp(Binsert)
    BMDBE_SF_BF_GP,                     // �訡�� �맮�� bfGP
    BMDBE_SF_AFTER_INS,                 // �訡�� �맮�� BMDB_SYNC_AFTER_INS
    BMDBE_SF_MDB_UPDATE,                // �訡�� �맮�� mdbUpdate
    BMDBE_SF_BF_ERROR,                  // ���� �訡�� DBT
    BMDBE_SF_MDB_ERROR                  // ���� �訡�� MDB
   };

// -----------------------------------------------------------------------------
// ����饭�� ���짮��⥫�᪨� �㭪権 �४��஢���� � ᨭ�஭���樨
enum
   {
    BMDB_PRECACHE_BEGIN  = 1,   // ��砫� �४��஢����
    BMDB_PRECACHE_END,          // ����� �४��஢����
    BMDB_PRECACHE_FAIL,         // ����� �४��஢����
    BMDB_PRECACHE_BEFORE_INS,   // ��। ��⠢��� ����� �� �४��஢����
    BMDB_PRECACHE_AFTER_INS,    // ��᫥ ��⠢�� ����� �� �४��஢����

    BMDB_SYNC_BEGIN      = 1,   // ��砫� ᨭ�஭���樨
    BMDB_SYNC_END,              // ����� ᨭ�஭���樨
    BMDB_SYNC_FAIL,             // ����� ᨭ�஭���樨
    BMDB_SYNC_BEFORE_INS,       // ��। ��⠢��� ����� �� ᨭ�஭���樨
    BMDB_SYNC_AFTER_INS,        // ��᫥ ��⠢�� ����� �� ᨭ�஭���樨
    BMDB_SYNC_BEFORE_UPD,       // ��। ����������� ����� �� ᨭ�஭���樨
    BMDB_SYNC_AFTER_UPD,        // ��᫥ ���������� ����� �� ᨭ�஭���樨
    BMDB_SYNC_BEFORE_DEL,       // ��। 㤠������ ����� �� ᨭ�஭���樨
    BMDB_SYNC_AFTER_DEL,        // ��᫥ 㤠����� ����� �� ᨭ�஭���樨
    BMDB_SYNC_NO_ACTION         // � ������ ������� ����⢨� �� �ந���������
   };

// �������⥫�� ��⠤���� ����� ��� ᨭ�஭���樨 � 䠩��� ��
typedef struct
      {
       recpos_t  recPos;      // ���. ������ ᮮ⢥�����饩 ����� � Btrieve-䠩��
      } BMDB_REC_AUX;

// �������⥫�� ��⠤���� ����� ����ᥩ ��� ᨭ�஭���樨 � 䠩��� ��
typedef struct
      {
       BTRVFILE  *bFile;      // �����⥫� �� 䠩����� ��६����� 䠩�� ��
      } BMDB_RECORDSET_AUX;

// �������⥫�� ��⠤���� 蠡����, ����砭���� �� ᫮����
// ࠧ��� = sizeof ( BMDB_TEMPLATE_AUX ) + strlen( dicPathname );
typedef struct
      {
       char  name[30];        // ��� ��������
       char  dicPathname[1];  // ��� ᫮����
      } BMDB_TEMPLATE_AUX;

// -----------------------------------------------------------------------------
// ������� 㪠��⥫� �� Btrieve-䠩�, ���樨஢���� � ����� ����஬
#define  bmdbAssignedFile(recordset)  ((recordset)->aux ? ((BMDB_RECORDSET_AUX *)((recordset)->aux))->bFile : NULL)

// ���� ������ � ����� �� 䨧. ����樨 ��ࢨ筮� Btrieve-�����
MDB_REC *bmdbGetByPosRaw(MDB_RECORDSET *recordset, recpos_t recPos);

// �������� ������ ����ᥩ ���樨஢���� Btrieve-䠩�
int bmdbAssign(MDB_RECORDSET *recordset, BTRVFILE *bf);

// ��ᮥ������ ���樨஢���� Btrieve-䠩� �� ����� ����ᥩ
int bmdbUnassign(MDB_RECORDSET *recordset);

// ������� � ����� ����� �� ⥪�饩 ������ �������� Btrieve-䠩��
// �᫨ 䫠� merge �������, �� ��⠢�� ����� �ந�������� �஢�ઠ
// �� �㡫�஢���� Btrieve-����樨
// proc - ��楤�� ���짮��⥫�, params - ����� ��� ���
typedef int (*BMDB_PRECACHE_CALLBACK)(int msg, void *data, void *params);

int bmdbPrecacheEx(MDB_RECORDSET *recordset, bool merge, BMDB_PRECACHE_CALLBACK proc, void *params);

// ������� � ����� ����� �� ⥪�饩 ������ �������� Btrieve-䠩��
// �᫨ 䫠� merge �������, �� ��⠢�� ����� �ந�������� �஢�ઠ
// �� �㡫�஢���� Btrieve-����樨
int bmdbPrecache(MDB_RECORDSET *recordset, bool merge);

// ����஭���஢��� ����� ����ᥩ � ⥪�饩 �������� �������� Btrieve-䠩��
// �᫨ noExternalInsertionCheck �������, �� �믮����� �஢��� (I)
// proc - ��楤�� ���짮��⥫�, params - ����� ��� ���
typedef int (*BMDB_SYNC_CALLBACK)(int msg, void *data, void *params);

int bmdbSyncEx(MDB_RECORDSET *recordset, bool noExternalInsertionCheck, BMDB_SYNC_CALLBACK proc, void *params);

int bmdbSyncEx2(MDB_RECORDSET *recordset, bool noExternalInsertionCheck, BMDB_SYNC_CALLBACK proc, void *params, bool UseFilters);

// ����஭���஢��� ����� ����ᥩ � ⥪�饩 �������� �������� Btrieve-䠩��
// �᫨ noExternalInsertionCheck �������, �� �믮����� �஢��� (I)
int bmdbSync(MDB_RECORDSET *recordset, bool noExternalInsertionCheck);

// ���⢥न�� �ᯥ譮� �����襭�� �࠭���樨,
// � ࠬ��� ���ன �஢������� ᨭ�஭�����
int bmdbConfirmSync(MDB_RECORDSET *recordset);
int bmdbConfirmSyncEx(MDB_RECORDSET *recordset, bool UseFilters);

// ������� 蠡��� �� ������� �� ᫮����
_BTEXP MDB_TEMPLATE *bmdbCreateTemplateFromDic(char *dicPathname, char *name);

// ������� 蠡���, ����砭�� �� ᫮����
_BTEXP void bmdbDeleteTemplate(MDB_TEMPLATE *templ);

// ���⪠ ����� ����ᥩ
int bmdbClearRecordset(MDB_RECORDSET *recordset);

// �������� ����� ����ᥩ
void bmdbDeleteRecordset(MDB_RECORDSET *recordset);

// ���樠����஢��� ��� 蠡�����
int  bmdbInitTemplateCache(void);

// �����樠����஢��� ��� 蠡�����
void bmdbDoneTemplateCache(void);

// �ࠢ���� ��� ���祭��
int mdbCompare(const void *a, const void *b, MDB_FLD_TYPE type, unsigned size);

// ���� ��ࢨ筮�� ���� �����
_BTEXP void mdbTraceDumpPrimaryKey(const MDB_TEMPLATE *templ, const void *recData);

// ���� �����
_BTEXP void mdbTraceDumpRecord(const MDB_TEMPLATE *templ, const void *recData, unsigned recLen,
                               const void *oldRecData, unsigned oldRecLen, bool changedOnly);

// -----------------------------------------------------------------------------
// ������� ���७�� ����� �訡�� ��� ᮮ�饭�� BMDBE_SYNC_FAILED
// [out] stat - �����頥� ����� ����樨 � mdb � dbt-䠩����. ����易⥫��
//              ��ࠬ���.
// [ret]      - �����頥� ���७�� ��� �訡�� �� BMDBE_SF_.
_BTEXP int mdbGetState(int *stat);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */