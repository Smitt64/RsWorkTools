
/*
**  ���⥩訩 �������� ����ᥩ � ����⨢��� �����
**
**  �������� ����樨:
**  - ��⠢��/㤠�����/���������� �����
**  - ���᪠ ����� �� �����
**  - ��᫥����⥫쭮�� ��ॡ�� ����ᥩ �� �������
*/

#ifndef __MDB_H
#define __MDB_H

#include "listobj.h"
#include "nwbtrv.h"
#include "bexpdef.h"

#ifdef MDB_USE_INDEX_MAPS
  #include <map>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// ����⠭��
// =========
enum
   {
    // ���ᨬ���� ���祭��
    MAX_IDX            = 25,         // ���ᨬ��쭮� ������⢮ �����ᮢ � ����� ����ᥩ
    MAX_SEG            = 15,         // ���ᨬ��쭮� ������⢮ ᥣ���⮢ � ������

    // �।��।������ ���祭�� ��� ᬥ饭��
    MDBO_PUSHBACK      = -1,         // �������� ���� ��᫥ ��᫥����� �������饣�

    // �訡��
    MDBE_OK            = 0,          // �� ���
    MDBE_NO_REC        = BEnorec,    // ��� �����, 㤮���⢮���饩
                                     // ���祭�� ����
    MDBE_EOF           = BEeof,      // ���⨣��� ����� 䠩��
    MDBE_DUP_KEY       = BEdupkey,   // �㡫�஢���� ���祭�� 㭨���쭮�� ����
    MDBE_BAD_POS       = BEinpos,    // ����ୠ� ⥪��� ������
    MDBE_BAD_KEY       = BEinvkey,   // ������ ����� ����
    MDBE_BAD_REC_SIZE  = BEldata,    // �����४⭠� ����� �����
    MDBE_INDEX_CHANGED = BEdifkey,   // ��������� ����� ����
    MDBE_NO_MEMORY     = -1,         // ��墠⪠ �����
    MDBE_OVERFLOW      = -2,         // ��९������� �� ���������� �����, ᥣ���⮢ etc.
    MDBE_WRONGPARAM    = -3,         // ������ ��ࠬ���
    MDBE_MULTIPLEAI    = -4,         // ����� ������ ��⮨��६��⭮�� ����
    MDBE_BADAIFLD      = -5          // ����⪠ ᤥ���� ��⮨��६���� ���������饥
                                     // ���� ��� ���� �� 楫��� ⨯�
   };

// ����
// ====

// -----------------------------------------------
// ����� �����
typedef enum
      {
       MDBRF_NO_FLAGS     = 0,
       MDBRF_WAS_UPDATED  = 0x01,    // �뫠 ��������� � ������ ��⠢��
       MDBRF_DELETED      = 0x02,    // ��������� ������
       MDBRF_KEEP         = 0x04,    // �� 㤠����� ������� ������ ��� 㤠������, � �� 㤠���� 䨧��᪨
                                     // ( ��� �㦤 ᨭ�஭���樨 )
       MDBRF_SAVE_ORIG    = 0x08,    // ���࠭��� � ���� �rig ���祭�� ������ �� ������ ��⠢��
       MDBRF_KEEP_AUTOINC = 0x10,    // ���࠭��� ���祭�� (���㫥���) ��⮨��६��⭮�� ���� �� ᨭ�஭���樨
       MDBRF_SYNCHRONIZED = 0x20     // ������ ��諠 ᨭ�஭����� � ⠡��楩 ��
      } MDB_REC_FLAGS;

// -----------------------------------------------
// ����� ������
typedef enum
      {
       MDBIF_UNIQUE = 1         // �������� ( ���㡫��㥬� ) ����
      } MDB_IDX_FLAGS;

// -----------------------------------------------
// ����� ᥣ���� ����
typedef enum
      {
       MDBSF_DESCEND = 1        // ����� ���冷� ���஢��
      } MDB_SEG_FLAGS;

// -----------------------------------------------
// ⨯� �����
typedef enum
      {
       MDBFT_UNKNOWN = -1,
       MDBFT_INT     = 1,    // �����
       MDBFT_FLOAT,          // ��᫮ � ������饩 �窮�

       #if !defined(NUMERIC_AS_MONEY)
           MDBFT_MONEY,      // ���죨
       #else
           MDBFT_DUMMY3,
       #endif

       MDBFT_CHAR,           // ������
       MDBFT_STRING,         // ��ப�
       MDBFT_DATE,           // ��� � �ଠ� bdate
       MDBFT_TIME,           // �६� � �ଠ� btime
       MDBFT_AUTOINC,        // ��⮨��६��� (楫��)

       #if defined(USE_NUMERIC)
           MDBFT_NUMERIC,    // ��᫮ ⨯� Numeric
       #endif

       #if defined(NUMERIC_AS_MONEY)
           MDBFT_MONEY = MDBFT_NUMERIC
       #endif
      } MDB_FLD_TYPE;


// -----------------------------------------------
struct tagMDB_TEMPLATE;

// -----------------------------------------------
// ���� ������
typedef struct
      {
       unsigned       flags;           // �����
       LOBJ           iLinks[MAX_IDX]; // iLinks[i] ��।���� ���������
                                      // ����� � i-⮩ �������� ��᫥�-��
       void          *data;            // ����⢥��� �����
       void          *orig;            // ���祭�� ������ �� ������
                                      // ��⠢�� ����� (��� �㦤 ᨭ��. )
       unsigned long  id;              // �������� � �।���� ����� �����䨪��� �����
       void          *aux;             // �������⥫�� ��⠤����
       unsigned       recSize;         // ����� �����
       unsigned       origRecSize;     // ����砫쭠� ����� �����
      } MDB_REC;

// -----------------------------------------------------------------------------
#ifdef MDB_USE_INDEX_MAPS
    // ������� ������
    typedef struct tagMDB_INDEX_ENTRY
          {
           unsigned                 indexNum;   // ����� ������
           void                    *data;       // ����� �����
           unsigned                 number;     // ����� �।� ����ᥩ � ��������� ���祭��� ����
           struct tagMDB_TEMPLATE  *templ;      // ������

           tagMDB_INDEX_ENTRY() : indexNum(0), data(0), number(0), templ(0) {}

           tagMDB_INDEX_ENTRY(struct tagMDB_TEMPLATE *t, int idxNum, void *d, int n = 0)
                             : indexNum(idxNum), data(d), number(n), templ(t) {}

           bool operator <  (const tagMDB_INDEX_ENTRY &entry) const;
           bool operator == (const tagMDB_INDEX_ENTRY &entry) const;
          } MDB_INDEX_ENTRY;

    // �����᭮� ��ॢ�

    typedef std::map<MDB_INDEX_ENTRY, MDB_REC *, std::less<MDB_INDEX_ENTRY>>  MDB_INDEX_MAP;
    typedef MDB_INDEX_MAP::iterator                                           MDB_INDEX_MAP_POS;
#else
    typedef int MDB_INDEX_MAP_POS;
#endif

// -----------------------------------------------
// ����
typedef struct
      {
       MDB_FLD_TYPE  type;         // ��� ����
       unsigned      offset;       // ���饭�� ���� �� ��砫� ������ ( ���� )
       unsigned      size;         // ����� ���� ( ���� )
      } MDB_FIELD;

// -----------------------------------------------
// ������� ������
typedef struct
      {
       unsigned      flags;        // �����
       unsigned      fldNo;        // ����� ����
      } MDB_IDX_SEG;

// -----------------------------------------------
// ������
typedef struct
      {
       unsigned      flags;        // �����
       unsigned      nSegs;        // ��᫮ ᥣ���⮢ � ������
       MDB_IDX_SEG   segments[MAX_SEG];  // ��������
      } MDB_INDEX;

// -----------------------------------------------
// ������ ����� ����ᥩ
// �।�⠢��� ᮡ�� "⨯" ����� - ��।���� ����� ����� � �������
typedef struct tagMDB_TEMPLATE
      {
       unsigned      recSize;          // ������ �����
       unsigned      nFields;          // ������⢮ �����
       MDB_FIELD    *fields;           // ����
       unsigned      nIndexes;         // ������⢮ �����ᮢ
       MDB_INDEX    *indexes[MAX_IDX]; // �������
       char         *fldNames;         // ����� �����, ࠧ������� ᨬ����� '\x00'
       void         *aux;              // �������⥫�� ��⠤����
       unsigned      maxRecSize;       // ���ᨬ��쭠� ����� �����
       long          autoIncFld;       // ����� ��⮨��६��⭮�� ���� (-1 - ��� ⠪��� ����)
       long          autoIncStep;      // ��� ��⮨��६���
       bool          useIndexMaps;     // �ᯮ�짮���� ��ॢ�� (��୥�, ���樠⨢�� ���ᨢ� std::map) ���
                                       // �������� ��᫥����⥫쭮�⥩
       unsigned      primaryKey;       // ���ଠ�� � ��ࢨ筮� ���� � ���
      } MDB_TEMPLATE;

// -----------------------------------------------
// ����� ����ᥩ
typedef struct
      {
       MDB_TEMPLATE      *templ;           // ������
       CNTLIST            iLists[MAX_IDX]; // ������� ��᫥����⥫쭮�� (ᯨ᪨)
       unsigned long      maxRecId;        // ����. �����䨪��� �����
       void              *aux;             // �������⥫�� ��⠤����
       long               maxAutoIncVal;   // ���ᨬ��쭮� ( �� ���. ����稭� ) ���祭�� ��⮨��६��⭮�� ����
       bool               deleteTempl;     // ������� template �� �����⨨ mdb-䠩��
       void              *pMdbFile;        // MDB file handle

       #ifdef MDB_USE_INDEX_MAPS
           MDB_INDEX_MAP  iMaps[MAX_IDX];  // ������� ��᫥����⥫쭮�� (��ॢ��)
       #endif
      } MDB_RECORDSET;

// -----------------------------------------------
// �����
typedef struct
      {
       MDB_RECORDSET  *recordset;        // ����� ����ᥩ
       unsigned        curIdx;           // ����騩 ������
       MDB_REC        *curPos;           // ������ ������
       MDB_REC        *prevPos;          // �।���� ������. ������� ⮫쪮 ��᫥ 㤠����� ⥪�饩 �����!!!
       MDB_REC        *nextPos;          // �������� ������. ������� ⮫쪮 ��᫥ 㤠����� ⥪�饩 �����!!!
       MDB_REC        *prevDistinctPos;  // �।���� ������ � �⫨�� ���祭��� ����. ������� ⮫쪮 ��᫥
                                         // 㤠����� ⥪�饩 �����!!!
       MDB_REC        *nextDistinctPos;  // �������� ������ � �⫨�� ���祭��� ����. ������� ⮫쪮 ��᫥
                                         // 㤠����� ⥪�饩 �����!!!
       void           *recBuf;           // ���� �����
       unsigned        recSize;          // ����� �����
       bool            ignoreDelFlag;    // �����஢��� 䫠� 㤠�����

       #ifdef MDB_USE_INDEX_MAPS
           MDB_INDEX_MAP_POS  imCurPos;
           MDB_INDEX_MAP_POS  imPrevPos;          // �।���� ������. ������� ⮫쪮 ��᫥ 㤠����� ⥪�饩 �����!!!
           MDB_INDEX_MAP_POS  imNextPos;          // �������� ������. ������� ⮫쪮 ��᫥ 㤠����� ⥪�饩 �����!!!
           MDB_INDEX_MAP_POS  imPrevDistinctPos;  // �।���� ������ � �⫨�� ���祭��� ����. ������� ⮫쪮
                                                  // ��᫥ 㤠����� ⥪�饩 �����!!!
           MDB_INDEX_MAP_POS  imNextDistinctPos;  // �������� ������ � �⫨�� ���祭��� ����. ������� ⮫쪮
                                                  // ��᫥ 㤠����� ⥪�饩 �����!!!
       #endif
      } MDB_CURSOR;


// -----------------------------------------------------------------------------
typedef void (*MDB_DONE_AUX_CALLBACK)(void *);

// �㭪樨 ��� ����७��� �㦤
// ===========================

// ����祭�� ���祭�� ��⮨��६��⭮�� ����
_BTEXP long mdbGetAutoIncValue(MDB_TEMPLATE *templ, void *rec);

// ��⠭���� ���祭�� ��⮨��६��⭮�� ����
_BTEXP void mdbSetAutoIncValue(MDB_TEMPLATE *templ, void *rec, long val);

// �ࠢ����� ���祭�� ���� � ���� ����� ����ᥩ
int mdbCompareKeys(const void *rec1, const void *rec2, MDB_TEMPLATE *templ, MDB_INDEX *index);

// ��ࠡ���� �� ��᢮���� ��⮨��६���
_BTEXP void mdbHandleExplicitAutoIncAssignment(MDB_RECORDSET *recordset, long val);

// �����᪮� 㤠����� ����� �� ��।������ 㪠��⥫�
void mdbDeleteRaw(MDB_RECORDSET *recordset, MDB_REC *rec, MDB_DONE_AUX_CALLBACK doneRecAux);

// �㭪樨 ��� ��饣� ���짮�����
// ==============================

// ��⠢��� ������ ( c 䫠���� � ��⠤���묨 )
_BTEXP int mdbInsertEx(MDB_CURSOR *c, unsigned flags, void *aux);

// ��⠢��� ������
_BTEXP int mdbInsert(MDB_CURSOR *c);

// ������� ������ � ��⠤���묨
_BTEXP int mdbDeleteEx(MDB_CURSOR *c, MDB_DONE_AUX_CALLBACK doneRecAux);

// ������� ������
_BTEXP int mdbDelete(MDB_CURSOR *c);

// �������� ������
_BTEXP int mdbUpdate(MDB_CURSOR *c);

// ����� �� ����� ������ �� ⥪�饬� �����
_BTEXP int mdbFirst(MDB_CURSOR *c);

// ����� �� ��᫥���� ������ �� ⥪�饬� �����
_BTEXP int mdbLast(MDB_CURSOR *c);

// ������� ⥪���� ������
_BTEXP void *mdbGetPos(MDB_CURSOR *c);

// ��⠭����� ⥪���� ������ ( pos ������ ���� �।���⥫쭮 ����祭�
// �� ����� �맮�� mdbGetPos()
_BTEXP void mdbSetPos(MDB_CURSOR *c, void *pos);

// ����� ᫥������ ������ �� ����� ( ���७��� )
_BTEXP int mdbNextEx(MDB_CURSOR *c, bool distinctKeyVal);

// ����� ᫥������ ������ �� �����
_BTEXP int mdbNext(MDB_CURSOR *c);

// ����� �।����� ������ �� ����� ( ���७��� )
_BTEXP int mdbPrevEx(MDB_CURSOR *c, bool distinctKeyVal);

// ����� �।����� ������ �� �����
_BTEXP int mdbPrev(MDB_CURSOR *c);

// ����� ������, ������� �� �����, 祬 ⥪�騩 ���� �����
_BTEXP int mdbGetGT(MDB_CURSOR *c);

// ����� ������, ������� ��� ࠢ��� �� �����, 祬 ⥪�騩 ���� �����
_BTEXP int mdbGetGE(MDB_CURSOR *c);

// ����� ������, ������� �� �����, 祬 ⥪�騩 ���� �����
_BTEXP int mdbGetLT(MDB_CURSOR *c);

// ����� ������, ������� ��� ࠢ��� �� �����, 祬 ⥪�騩 ���� �����
_BTEXP int mdbGetLE(MDB_CURSOR *c);

// ������� ������, ࠢ��� �� ����� ⥪�饬� �����
_BTEXP int mdbGetEQ(MDB_CURSOR *c);

// ������� �����䨪��� �����
_BTEXP unsigned long mdbGetId(MDB_CURSOR *c);

// ���� ������ �� �����䨪����
_BTEXP int mdbGetById(MDB_CURSOR *c, unsigned long id);

// ������� ⥪�騩 ����� ����
_BTEXP unsigned mdbGetKeyNum(MDB_CURSOR *c);

// ��⠭����� ⥪�騩 ����� ����
_BTEXP int mdbSetKeyNum(MDB_CURSOR *c, unsigned keyNum);

// ������� 蠡���
_BTEXP MDB_TEMPLATE *mdbCreateTemplate(unsigned recSize);

// ������� 蠡��� � �����ﬨ ��६����� �����
_BTEXP MDB_TEMPLATE *mdbCreateTemplateVar(unsigned recSize, unsigned maxRecSize);

// ������� 蠡��� ( � ���. ��⠤���묨 )
_BTEXP MDB_TEMPLATE *mdbCreateTemplateEx(unsigned recSize, unsigned maxRecSize, void *aux);

// ������� 蠡���
_BTEXP void mdbDeleteTemplate(MDB_TEMPLATE *templ);

// ������� 蠡��� ( � ���. ��⠤���묨 )
_BTEXP void mdbDeleteTemplateEx(MDB_TEMPLATE *templ, MDB_DONE_AUX_CALLBACK doneAux);

// �������� ���� ( �᫨ offset == MDBO_PUSHBACK, �������� ���� ��᫥ ��᫥����� �����. )
_BTEXP int mdbAddField(MDB_TEMPLATE *templ, char *name, MDB_FLD_TYPE type, unsigned size, unsigned offset);

// ��⠭����� ��⮨��६��⭮� ���� (��� �����樨 �������� �� ᫮����)
_BTEXP int mdbSetAutoIncField(MDB_TEMPLATE *templ, unsigned fldNo);

// ��⠭����� 蠣 ��⮨��६���
_BTEXP void mdbSetAutoIncStep(MDB_TEMPLATE *templ, long step);

// ��⠭����� 䫠��� �ᯮ�짮����� �ॢ������� �����ᮢ
_BTEXP void mdbUseIndexMaps(MDB_TEMPLATE *templ, bool b);

// �������� ������
_BTEXP MDB_INDEX *mdbAddIndex(MDB_TEMPLATE *templ, unsigned flags);

// �������� ᥣ���� ������
_BTEXP int mdbAddSegment(MDB_INDEX *idx, unsigned fldNo, unsigned flags);

// ������� ����� ����ᥩ ( � �������⥫�묨 ��⠤���묨 � ��楤�ன ��ࠡ�⪨ ᮮ�饭�� )
_BTEXP MDB_RECORDSET *mdbCreateRecordsetEx(MDB_TEMPLATE *templ, void *aux);

// ������� ����� ����ᥩ
_BTEXP MDB_RECORDSET *mdbCreateRecordset(MDB_TEMPLATE *templ);

// ������ ����� ����ᥩ ( � 㤠������ ���. ��⠤����� )
_BTEXP int mdbClearRecordsetEx(MDB_RECORDSET *recordset, MDB_DONE_AUX_CALLBACK doneRecAux);

// ������ ����� ����ᥩ
_BTEXP int mdbClearRecordset(MDB_RECORDSET *recordset);

// ������� ����� ����ᥩ ( � 㤠������ ���. ��⠤����� )
_BTEXP void mdbDeleteRecordsetEx(MDB_RECORDSET *recordset, MDB_DONE_AUX_CALLBACK doneRecAux,
                                 MDB_DONE_AUX_CALLBACK doneRecordsetAux);

// ������� ����� ����ᥩ
_BTEXP void mdbDeleteRecordset(MDB_RECORDSET *recordset);

// ������� �����
_BTEXP MDB_CURSOR *mdbCreateCursor(MDB_RECORDSET *recordset);

// ������� �����
_BTEXP void mdbDeleteCursor(MDB_CURSOR *c);

// ������� ( 㪠��⥫� �� ) ���祭��, ⨯ � ࠧ��� ���� �� ��� ������
void *mdbFieldByNum(MDB_CURSOR *c, unsigned num, MDB_FLD_TYPE *type, unsigned *size);

// ������� ( 㪠��⥫� �� ) ���祭��, ⨯ � ࠧ��� ���� �� ��� �����
void *mdbFieldByName(MDB_CURSOR *c, char *name, MDB_FLD_TYPE *type, unsigned *size);

// ��⠭����� ���祭�� 䫠�� �����஢���� 䫠�� 㤠�����
_BTEXP bool mdbSetIgnoreDelFlag(MDB_CURSOR *c, bool ignoreDelFlag);

// ������� ����� ⥪�饩 �����
_BTEXP unsigned mdbGetRecordSize(MDB_CURSOR *c);

// ��⠭����� ����� ⥪�饩 �����
_BTEXP int mdbSetRecordSize(MDB_CURSOR *c, unsigned size);

// ������� ����� ����� �� 㬮�砭��
_BTEXP unsigned mdbGetDefaultRecSize(MDB_CURSOR *c);

// ��⠭����� ����� ����� �� 㬮�砭��
_BTEXP void mdbSetDefaultRecSize(MDB_CURSOR *c);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */