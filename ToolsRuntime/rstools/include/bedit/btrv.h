/*******************************************************************************
 FILE         :   BTRV.H

 COPYRIGHT    :   VECsoft, 1992
                  R-Style Software Lab, 1994,1998

 DESCRIPTION  :   BTRVFILE interface (Btieve & DBtriever/400)

 PROGRAMMED BY:   Emil Dovidovitch, Serg Kubrin, Yakov Markovitch

 CREATION DATE:   28 Oct 1992

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/bedit/h/btrv.h $
                  $Revision: 45 $
                  $Date: 7.09.00 14:18 $
                  $Author: Kormukhin $
                  $Modtime: 7.09.00 12:33 $
*******************************************************************************/

//              Borland's C low & BTRVFILE levels interface
//                   to the Btrieve Record Manager 5.00
//

/*
   September 22,1994
   ��ਭ �.  �����প� DPMI16
   September 27,1994
   ��ਭ �.  �����প� DPMI32

   27 December 1994
   �謠��� �. 1) ��������� 2 ���� ⨯� void * � �������� BTRVFILE
             2) ������� #define �������� �� enum
   January 6,1995   Serg Kubrin (K78)
             �뭥� ����䥩� � Btrieve � 䠩� nwbtrv.h
   November 2,1995   Serg Kubrin (K78)
             Add BLOB field support. Need Btrieve 6.x
   27 December 1995
     ��મ��� �. (JNS) ������� #ifdef __cplusplus,
           ��ࠢ�� ��।������ RecFltr � KeyFltr

   13/02/1996 Yakov E. Markovitch (JNS)
     Added bfSt operation and all corresponding structures
   09/01/1997 Yakov E. Markovitch (JNS)
    Multi-client support added.
*/

#ifndef __BTRV_H
#define __BTRV_H

#define BTRVH

#include <rstypes.h>
#include "nwbtrv.h"
#include "bexpdef.h"
#include "beditdef.h"
#include "mdb/mdb.h"
#include <rscdefs.h>

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


// -----------------------------------------------------------------------------
#ifdef RSL_PL_DOS
    // ���� �ணࠬ�� �� ������ �ᯮ�짮���� ��� �㭪��
    int BTRV(int OP, void *POS_BLK, void *DATA_BUF, int *DATA_LEN, void *KEY_BUF, char KEY_NUM);

    #define  BTRV(op, pos, data, dlen, key, num)  NWcall(op, pos, data, dlen, key, 255, num)
#endif

// -----------------------------------------------------------------------------
//  ���饭�� � ����� ���� � �������
#ifdef RSL_MD_FLAT
    #define  koff(s_name, m_name)  (offsetof(s_name, m_name) + 1)
    #define  klen(s_name, m_name)  sizeof(((s_name *)0)->m_name)
#else
    #define  koff(s_name, m_name)  (offsetof(s_name, m_name) + 1)
    #define  klen(s_name, m_name)  sizeof(((s_name _FAR *)0)->m_name)
#endif

//-@H------ BEGIN __ BLOB data_________________________________ --------@@-

#ifndef BLOB_DATA_DEFINED
#define BLOB_DATA_DEFINED
    #include <packpsh1.h>

    // -------------------------------------------
    // This structure is written at the begining of BLOB field.
    typedef struct TBlobHeader
          {
           unsigned char  state;        // TBlobState
           db_uint32      size;         // ��騩 ࠧ��� BLOB-����
           unsigned char  blobType;     // 1 - if RSL values saved in BLOB
           char           reserved[8];
          } TBlobHeader;

    #include <packpop.h>

    // -------------------------------------------
    // ����ﭨ� BLOB
    enum TBlobState
       {
        BLOB_OK = 1,
        BLOB_DIRTY
       };

    // -------------------------------------------
    // ������ BLOB'�
    typedef enum TBlobOpenMode
          {
           BLOB_OPEN_READ,      // �⥭��
           BLOB_OPEN_WRITE,     // ������
           BLOB_OPEN_RDWR,      // �⥭��/������
           BLOB_CLOSED          // BLOB ������
          } TBlobOpenMode;

    #include <packpshn.h>

    // -------------------------------------------
    // ���� �ࠢ����� BLOB'��
    typedef struct tagTBlobControlData
          {
           TBlobOpenMode  openMode;
           TBlobHeader    hd;
           unsigned int   bufSize;      // ������ ���� (�뫮 ���� unsigned)
           char          *bufStart;     // ��砫� ����
           char          *bufLast;      // ��᫥���� �ᯮ��㥬�� ������ � ���� (�� 䠪�)
           char          *bufPtr;       // �����⥫� �� ����
           unsigned long  offsInBlob;   // ���饭�� � Blob ��ࢮ�� ���� � ����
           bool           isDirty;      // True �᫨ Blob �� ������
           bool           isEmpty;      // True �᫨ ���� ����
           bool           newPos;       // True �᫨ � Blob �뫠 ��⠭������ ����� ������
          }  TBlobControlData;

    #include <packpop.h>
#endif // BLOB_DATA_DEFINED
//-@@------ END __ BLOB data___________________________________ --------@E-

/************************************************************************/

typedef struct btrvfile BTRVFILE;

// �㭪�� 䨫���樨 ����ᥩ
typedef int (*JBtrFltr)(BTRVFILE *Br);

// ���� ��� ������
typedef enum
      {
       DBT_BTRIEVE = 0,   // Btrieve
       DBT_MDB,           // �������� ����ᥩ � ����⨢��� �����
       DBT_SQL,           // SQL ����� Btrieve'�
       DBT_USER
      } DB_TYPE;

#include <packpshn.h>

// -----------------------------------------------------------------------------
typedef struct
      {
       MDB_CURSOR  *cursor;      // ����� ���
      } BMDB_DATA;

// -----------------------------------------------------------------------------
// ��ࠢ���騩 ���� 䠩��
struct btrvfile
     {
      union
          {
           posblk_t   posblk;            // ����樮��� ����
           BMDB_DATA  mdb;

           #ifdef SQLBUILD
               void  *sqlf;
           #endif

           #if defined(SQLBUILD) || (defined (__cplusplus) && defined(__RSRTL_DLL) && defined (_MSC_VER) && !defined (NO_BKMK_CLASS))
               IBtUserDataSource *user;
           #endif
          } internal;

      BTRVFILE   *prev;         // �।��騩 ����� ᯨ᪠
      BTRVFILE   *next;         // ������騩 ����� ᯨ᪠

      void       *RecBuf;       // ��뫪� �� ���� ������
      void       *KeyBuf;       // ��뫪� �� ���� ����

      void       *KeyTopVal;    // ��뫪� �� �������쭮� ���祭�� ����
      void       *KeyBotVal;    // ��뫪� �� ���ᨬ��쭮� ���祭�� ����

      void       *bfilePtr;     // �ᯮ������ � Btrieve C++ wrapper
      void       *userData;     // ���� ����� ���짮��⥫�, �易��� � 䠩���

      char       *FileName;     // ��� 䠩��

      JBtrFltr    RecFltr;      // ������ �롮� ����ᥩ
      JBtrFltr    KeyFltr;      // ���祢�� 䨫��� �롮� ����ᥩ

      int         MaxRecLen;    // ���ᨬ��쭠� ����� �����
      int         KeyNum;       // ����� ����
      int         RecLen;       // ������ ����� �����
      recpos_t    RecPos;       // �����᪨� ���� �����
      int         MaxKeyLen;    // ���ᨬ��쭠� ����� ���� ����

   /*   November 2,1995 (K78)
       BLOB field support.
   */
      int         fixsize;      // Record fixed part lenght
      TBlobControlData  *blob;
   /*   January 9,1997 (JNS)
       Multi-client support.
   */
      bclient_h   client;       // BTrieve Client ID
      DB_TYPE     type;         // ��� ���� ������
      int         nCaches;      // ������⢮ ���-��襩 � ������� 䠩��

   // June 27,2000
      unsigned char openMode;

   // May 23, 2001
      bool        redir;        // ��७��ࠢ����� �맮��� � ����
      BTRVFILE   *cache;        // ��뫪� �� ���� ��� (��� ��७��ࠢ�����)

      // truncated - ࠧ�蠥� �����஢��� ��� BEldata �� �⥭�� ����ᥩ
      // � ��६����� �����. �� �⮬ fixsize ������ ���� ࠢ�� ࠧ���� ����ﭭ��
      // ���.
      bool        truncated;

      // reversOrder - ����砥� ������� ����権 ������樨 �� 䠩��
      // (BstepF, BstepN, BstepL, BstepP, BgetF, BgetN, BgetL, BgetP)
      // �� �஢�� �㭪樨 bfGet.
      bool        reversOrder;

      void       *bs;           // bs - 㪠��⥫� �� ⥪�騩 �஫����

      char        _reserved[5]; // Reserved for future use. DON'T USE VOLUNTARY!!!
     };

// -----------------------------------------------------------------------------
typedef  void (*FreeUserTLSProc)(void *userData);

// -----------------------------------------------------------------------------
struct RSTLS
     {
      int              version;   // ����� ������ ��������. ��� ⥪�饩 ���ᨨ 0.
      int              m_bfstat;
      int              m_bfop;
      int              m_TrnFlag;
      int              m_TrnLockFlags;
      BTRVFILE        *m_bfcb;
      bool             m_useDistrTrn;
      char             m_szLastName[_MAX_PATH];
      void*            userData;
      FreeUserTLSProc  cleanProc;
      int              m_TrnCount;  // #2972 ���稪 ����஢ �࠭���樨 (�� 0)

      #ifdef SQLBUILD
          char         m_szLastRsdError[1024];
          btrvem_h     m_curBtrvEm_notUsedNow;
      #endif
     };


#include <packpop.h>

// JNS, 13/02/1996
/*******************************************************************************
 Key structure for BTRIEVE STAT operation
*******************************************************************************/

#include <packpsh1.h>

// -----------------------------------------------------------------------------
typedef struct JBtKSpec
      {
       db_int16       keyPos;    // Key position (offset)
       db_uint16      keyLen;    // Key length
       db_uint16      keyFlags;  // Key flags
       db_int32       uniqNum;   // Number of unique values
       unsigned char  keyType;   // Extended type
       unsigned char  nullVal;
       short          reserved;
       char           keyNum;
       char           ACSnum;
      } JBtKSpec;

// -----------------------------------------------------------------------------
typedef struct JBtFSpec
      {
       db_uint16      recLen;
       db_uint16      pageSize;
       db_uint16      indNum;
       db_uint32      recNum;
       db_uint16      fFlags;
       unsigned short reserved;
       db_uint16      unusedPg;
      } JBtFSpec;

typedef  JBtFSpec JBtrFileSpec;
typedef  JBtKSpec JBtrKeySpec;

// -----------------------------------------------------------------------------
typedef struct JBFSpec
      {
       JBtrFileSpec fs;
       JBtrKeySpec  ks[1];
      } JBFSpec;

#include <packpop.h>

_BTEXP int bfSt(BTRVFILE *bf, JBFSpec *spec, unsigned nkeys);

// ***************** end of (JNS) ********************

bool ToolsInitTLS(void);
void ToolsDoneTLS(void);

_BTEXP void   ToolsClearTLS(void);
_BTEXP RSTLS *ToolsGetTLS(void);



_BTEXP int  *__bfstat(void);
_BTEXP int  *__bfop(void);
_BTEXP int  *__TrnFlag(void);
_BTEXP int  *__TrnCount(void);
_BTEXP bool *__useDistrTrn(void);
_BTEXP BTRVFILE **__bfcb(void);
_BTEXP char *__szLastName(void);

_BTEXP void setUseDistrTrn(bool bOn);
_BTEXP int  SetOptLock(int optLock);

_BTEXP btrvem_h OpenDb(const char *connStr, const char *name);
_BTEXP btrvem_h SetCurrentDb(btrvem_h db);
_BTEXP btrvem_h GetCurrentDb();
_BTEXP btrvem_h FindDb(const char *name);
_BTEXP void     CloseDb(btrvem_h db);

#ifdef SQLBUILD
    _BTEXP char     *__szLastRsdError(void);
    _BTEXP btrvem_h *__curBtrvEm(void);
#endif


#ifndef __cplusplus
    #define  bfstat       (*__bfstat())
    #define  bfop         (*__bfop())
    #define  bfcb         (*__bfcb())
    #define  szLastName   (__szLastName())
    #define  TrnFlag      (*__TrnFlag())
    #define  TrnCount     (*__TrnCount())
    #define  useDistrTrn  (*__useDistrTrn())

    #ifdef SQLBUILD
        #define  szLastRsdError  (__szLastRsdError())
        #define  curBtrvEm       (*__curBtrvEm())
    #endif
#else
    // In C++ we MUST qualify function calls as global namespace
    #define  bfstat       (*(::__bfstat()))
    #define  bfop         (*(::__bfop()))
    #define  bfcb         (*(::__bfcb()))
    #define  szLastName   (::__szLastName())
    #define  TrnFlag      (*(::__TrnFlag()))
    #define  TrnCount     (*(::__TrnCount()))
    #define  useDistrTrn  (*(::__useDistrTrn()))

    #ifdef SQLBUILD
        #define  szLastRsdError  (::__szLastRsdError())
        #define  curBtrvEm       (*__curBtrvEm())
    #endif
#endif

//-@H------ BEGIN __ BLOB functions ___________________________ --------@@-
_BTEXP int bfOpenBlob         (BTRVFILE *bf, TBlobOpenMode openMode);
_BTEXP int bfCloseBlob        (BTRVFILE *bf);
_BTEXP int bfReadBlob         (BTRVFILE *bf, char *buff, int size);
_BTEXP int bfWriteBlob        (BTRVFILE *bf, char *buff, int size);
_BTEXP int bfSetBlobPos       (BTRVFILE *bf, unsigned long pos);
_BTEXP int bfGetBlobPos       (BTRVFILE *bf, unsigned long *pos);
_BTEXP int bfGetBlobSize      (BTRVFILE *bf, unsigned long *size);
_BTEXP int bfTruncateBlob     (BTRVFILE *bf);
_BTEXP int bfSetBlobType      (BTRVFILE *bf, unsigned char tp);
//-@@------ END __ BLOB functions _____________________________ --------@E-

// �����饭�� BTRVFILE-�����

_BTEXP BTRVFILE *alloc_bf(int size);          // ��������� ����
_BTEXP void       free_bf(BTRVFILE *curblk);  // �᢮������ ����
_BTEXP BTRVFILE *alloc_bf_topblk(void);

// -----------------------------------------------------------------------------
// Open BTRVFILE
_BTEXP int bfOpen(BTRVFILE **bf,          // BTRVFILE ptr - returnd
                  const char *Name,       // ��� 䠩��
                  int         Mode,       // ����� ������ 䠩��
                  int         MaxRecLen,  // ���ᨬ��쭠� ����� �����
                  int         MaxKeyLen,  // ���ᨬ��쭠� ����� ���� ����
                  int         KeyNum,     // ����� ����
                  JBtrFltr    RecFltr,    // ������ �롮� ����ᥩ
                  JBtrFltr    KeyFltr,    // ���祢�� 䨫��� �롮� ����ᥩ
                  const char *Own,        // ��������
                  void       *RecBuf      // ��뫪� �� ���� ������
                 );

// -----------------------------------------------------------------------------
// Open BTRVFILE
_BTEXP int bfOpen2(BTRVFILE   **bf,         // BTRVFILE ptr - returnd
                   const char  *Name,       // ��� 䠩��
                   int          Mode,       // ����� ������ 䠩��
                   int          MaxRecLen,  // ���ᨬ��쭠� ����� �����
                   int          MaxKeyLen,  // ���ᨬ��쭠� ����� ���� ����
                   int          KeyNum,     // ����� ����
                   JBtrFltr     RecFltr,    // ������ �롮� ����ᥩ
                   JBtrFltr     KeyFltr,    // ���祢�� 䨫��� �롮� ����ᥩ
                   const char  *Own,        // ��������
                   void        *RecBuf,     // ��뫪� �� ���� ������
                   int          cacheSize
                  );

// -----------------------------------------------------------------------------
// Open BTRVFILE
_BTEXP int bfOpenEx(BTRVFILE    **bf,          // BTRVFILE ptr - returnd
                    const char   *Name,        // ��� 䠩��
                    int           Mode,        // ����� ������ 䠩��
                    int           KeyNum,      // ����� ����
                    int           useBlob,     // If true use BLOB field in record. Need Btrieve 6.x
                    int           varLenSize,  // Size of varlen buffer,if == 0, then used 1024
                    JBtrFltr      RecFltr,     // ������ �롮� ����ᥩ
                    JBtrFltr      KeyFltr,     // ���祢�� 䨫��� �롮� ����ᥩ
                    const char   *Own          // ��������
                   );

// -----------------------------------------------------------------------------
// Open BTRVFILE
_BTEXP int bfOpenEx2(BTRVFILE    **bf,          // BTRVFILE ptr - returnd
                     const char   *Name,        // ��� 䠩��
                     int           Mode,        // ����� ������ 䠩��
                     int           KeyNum,      // ����� ����
                     int           useBlob,     // If true use BLOB field in record. Need Btrieve 6.x
                     int           varLenSize,  // Size of varlen buffer,if == 0, then used 1024
                     JBtrFltr      RecFltr,     // ������ �롮� ����ᥩ
                     JBtrFltr      KeyFltr,     // ���祢�� 䨫��� �롮� ����ᥩ
                     const char   *Own,         // ��������
                     int           cacheSize    // ������ ���
                    );

// -----------------------------------------------------------------------------
// Open BTRVFILE
_BTEXP int bfOpenRSD(BTRVFILE **bf,  // BTRVFILE ptr - returnd
                     void      *rs   // RSD recordset
                    );

// -----------------------------------------------------------------------------
#if defined(SQLBUILD) || (defined (__cplusplus) && defined(__RSRTL_DLL) && defined (_MSC_VER) && !defined (NO_BKMK_CLASS))
    _BTEXP int bfOpenUser(BTRVFILE          **bf,
                          const char         *name,
                          IBtUserDataSource  *user,
                          int                 MaxRecLen,     // ���ᨬ��쭠� ����� �����
                          int                 MaxKeyLen,     // ���ᨬ��쭠� ����� ���� ����
                          JBtrFltr            RecFltr,       // ������ �롮� ����ᥩ
                          JBtrFltr            KeyFltr,       // ���祢�� 䨫��� �롮� ����ᥩ
                          void               *RecBuf         // ��뫪� �� ���� ������
                          );
#endif

// -----------------------------------------------------------------------------
// �-�� ࠡ��� � SQL ����஬ � ����ன�� �����
// ⨯ SQL �����
typedef enum tagTBtEmQueryType
      {
       BTEM_GET_QUERY       = 1,   // ����� , �믮����騩 ����樨 Get
       BTEM_NEXTPREV_QUERY  = 2,   // ������, �믮����騥 Prev  Next
       BTEM_FIRSTLAST_QUERY = 3,   // ������, �믮����騥 First last
       BTEM_ALL_QUERIES     = 4    // �� ������
      } TBtEmQueryType;

// -----------------------------------------------
// ०�� ��⨬���樨
typedef enum tagTBtEmOptmMode
      {
       BTEM_NO_OPTM         = 0,   // ��� ��⨬���樨
       BTEM_HINT_OPTM       = 3    // ��⠭���� ���᪠���
      } TBtEmOptmMode;


_BTEXP int bfAddLink(BTRVFILE *bf, const char *depFields, const char *depFiles, const char *joinCond,
                     const char *whereCond, ...);

_BTEXP int bfAddLink2(BTRVFILE *bf, const char *depFields, const char *depFiles, const char *joinCond,
                      const char *whereCond, void **array, int arr_size);

// -----------------------------------------------------------------------------
typedef struct tagExprInfo
      {
       long  type;
       int   size;
      } TExprInfo;

_BTEXP int bfAddLinkEx(BTRVFILE *bf, const char *depFields, const char *depFiles, const char *joinCond,
                       const char *whereCond, void **array, int arr_size, TExprInfo *exprinfo, int exprinfo_size);

_BTEXP int bfAddLinkParam(BTRVFILE *bf, long type, void *val, int size);

_BTEXP int bfDropLink(BTRVFILE *bf);

_BTEXP bool bfLinked(BTRVFILE *bf);
_BTEXP bool bfOptimized(BTRVFILE *bf);

// -----------------------------------------------------------------------------
typedef struct tagTDepField
      {
       char  *Alias;
       char  *TableName;
       char  *FieldName;
      } TDepField;

// -----------------------------------------------------------------------------
typedef struct tagTLinkInfo
      {
       TDepField  *depFields;
       void      **pData ;
       int         nLinks;
      } TLinkInfo;

// -----------------------------------------------------------------------------
typedef struct tagTSqlInfoEx
      {
       void      **data_array;
       int         length;
       TExprInfo  *column_info;
       char       *sql;
      } TSqlInfoEx;

_BTEXP void bfGetLinkInfo(BTRVFILE *bf, TLinkInfo **info);
_BTEXP void bfFreeLinkInfo(TLinkInfo **info);

// -----------------------------------------------------------------------------
typedef enum tagTBfOperType
      {
       BF_BFOPG_OPER  = 1, 
       BF_BFGET_OPER  = 2, 
       BF_BSGET_OPER  = 3,
       BF_GET_ORDERBY = 100  
      } TBfOperType;

_BTEXP char *bfGetSql(BTRVFILE *bf, char *sql, int operType);

_BTEXP void bfGetSqlInfo(BTRVFILE *bf, int operType, TSqlInfoEx **info);
_BTEXP void bfFreeSqlInfo(TSqlInfoEx **info);

// -----------------------------------------------------------------------------
enum EMPROPERTY
   {
    EM_RESETONSWITCH = 1,
    EM_MAXCACHESIZE  = 2,
    EM_RECORDLOCK    = 3,
    EM_SQLBREAK      = 4,
    EM_MAXBLOBSIZE   = 5,
    EM_USENULLS      = 6,
    EM_TRNFLAG       = 7
   };

void bfSetEmProp(int property, int value, btrvem_h hem);

// -----------------------------------------------------------------------------
typedef struct tagTSortField
      {
       char  *Alias;
       char  *TableName;
       char  *FieldName;
       bool   Descending;
       void  *pData;
      } TSortField;

// -----------------------------------------------------------------------------
typedef struct tagTSortInfo
      {
       TSortField *sortFields;    
       int         nFields;
       bool        bUserSort;
      } TSortInfo;

_BTEXP void bfGetSortInfo(BTRVFILE *bf, TSortInfo **info);
_BTEXP void bfFreeSortInfo(TSortInfo **info, DB_TYPE type);

_BTEXP void bfSetToolsWhere(BTRVFILE *bf,const char *toolsWhere);

// -----------------------------------------------------------------------------
typedef enum tagTBfCursorType
      {
       BF_MAIN_CURSOR       = 1,  // �᭮���� �����
       BF_OPTIMIZED_CURSOR  = 2,  // ��⨬���஢���� �����
       BF_USERSORT_CURSOR   = 3   // ����� � ��
      } TBfCursorType;

_BTEXP int  bfSetCursor    (BTRVFILE *bf, int type, bool bSync);
_BTEXP int  bfGetCursorType(BTRVFILE *bf);
_BTEXP bool bfTestCursor   (BTRVFILE *bf, int type);

_BTEXP int bfAddUserSort  (BTRVFILE *bf, const char *sortFlds);
_BTEXP int bfDropUserSort (BTRVFILE *bf);

_BTEXP int  bfSetCaching  (BTRVFILE *bf, bool val);
_BTEXP void bfSetCacheMode(BTRVFILE *bf, int fullShift);
_BTEXP void bfResetCache  (BTRVFILE *bf);
_BTEXP void bfSkipBmSearch(BTRVFILE *bf, bool bSkip);

_BTEXP void bfLoadInfo(char **nameArr, int arrSize);

_BTEXP int  bfSetOptmMode(BTRVFILE       *bf, 
                          TBtEmQueryType  qt,   // ⨯ ����ᮢ, ��� ������ ��⠭���������� ०��
                          TBtEmOptmMode   om,   // ०�� ��⨬���樨
                          const char     *hint  // ���᪠��� SQL 
                         );

_BTEXP const char *bfSetHint(BTRVFILE      *bf, 
                             int            crsType,// ⨯ �����, ��� ���ண� ��⠭���������� ०�� 
                             int            qt,     // ⨯ ����ᮢ, ��� ������ ��⠭���������� ०�� 
                             int            all,    // �ਬ����� �� �ᥬ ����ࠬ � �⥪�
                             const char    *hint,   // ���᪠��� SQL 
                             char          *oldHint
                            );

_BTEXP int   bfGetPosByBuffer(BTRVFILE *bf, recpos_t *pos);

_BTEXP void  CancelSqlQuery();
_BTEXP void *GetDefaultConnection();    
_BTEXP void *GetConnectionIntf();    
_BTEXP void *GetConnectionHandle();    
_BTEXP void  BtemDone();

/*********************************************************/ 

// -----------------------------------------------------------------------------
// Open BTRVFILE using specified client
_BTEXP int bfOpenClnt(BTRVFILE   **bf,         // BTRVFILE ptr - returnd
                      bclient_h    client,     // BTRIEVE client's handle
                      const char  *Name,       // ��� 䠩��
                      int          Mode,       // ����� ������ 䠩��
                      int          MaxRecLen,  // ���ᨬ��쭠� ����� �����
                      int          MaxKeyLen,  // ���ᨬ��쭠� ����� ���� ����
                      int          KeyNum,     // ����� ����
                      JBtrFltr     RecFltr,    // ������ �롮� ����ᥩ
                      JBtrFltr     KeyFltr,    // ���祢�� 䨫��� �롮� ����ᥩ
                      const char  *Own,        // ��������
                      void        *RecBuf      // ��뫪� �� ���� ������
                     );

// -----------------------------------------------------------------------------
// Open BTRVFILE
_BTEXP int bfOpenClntEx(BTRVFILE   **bf,         // BTRVFILE ptr - returnd
                        bclient_h    client,     // BTRIEVE client's handle
                        const char  *Name,       // ��� 䠩��
                        int          Mode,       // ����� ������ 䠩��
                        int          KeyNum,     // ����� ����
                        int          useBlob,    // If true use BLOB field in record. Need Btrieve 6.x
                        int          varLenSize, // Size of varlen buffer
                        JBtrFltr     RecFltr,    // ������ �롮� ����ᥩ
                        JBtrFltr     KeyFltr,    // ���祢�� 䨫��� �롮� ����ᥩ
                        const char  *Own         // ��������
                       );

// -----------------------------------------------------------------------------
// Open BTRVFILE using specified client
_BTEXP int btrvOpenClnt(BTRVFILE   **bf,      // BTRVFILE ptr - returnd
                        bclient_h    client,   // BTRIEVE client's handle
                        const char  *Name,   // ��� 䠩��
                        int          Mode,    // ����� ������ 䠩��
                        int          MaxRecLen,    // ���ᨬ��쭠� ����� �����
                        int          MaxKeyLen,    // ���ᨬ��쭠� ����� ���� ����
                        int          KeyNum, // ����� ����
                        JBtrFltr     RecFltr, // ������ �롮� ����ᥩ
                        JBtrFltr     KeyFltr, // ���祢�� 䨫��� �롮� ����ᥩ
                        const char  *Own,        // ��������
                        void        *RecBuf  // ��뫪� �� ���� ������
                       );

// -----------------------------------------------------------------------------
// Open BTRVFILE
_BTEXP int btrvOpenClntEx(BTRVFILE   **bf,           // BTRVFILE ptr - returnd
                          bclient_h    client,       // BTRIEVE client's handle
                          const char  *Name,         // ��� 䠩��
                          int          Mode,         // ����� ������ 䠩��
                          int          KeyNum,       // ����� ����
                          int          useBlob,      // If true use BLOB field in record. Need Btrieve 6.x
                          int          varLenSize,   // Size of varlen buffer
                          JBtrFltr     RecFltr,      // ������ �롮� ����ᥩ
                          JBtrFltr     KeyFltr,      // ���祢�� 䨫��� �롮� ����ᥩ
                          const char  *Own           // ��������
                         );


// -----------------------------------------------------------------------------
// ������� 䠩�
// [in] cb - �����⥫� �� 䠩�
_BTEXP int bfClose(BTRVFILE **cb);

// -----------------------------------------------------------------------------
// ������� �� ������ dbt-䠩��
_BTEXP int bfCloseAll(void);

// -----------------------------------------------------------------------------
// ��⠭����� ०�� ������ 䠩�� (��� SQL-०���)
// [in] bf   - �����⥫� �� 䠩�
// [in] mode - ����� ������
// [ret]     - �����頥� �।��騩 ०�� ������
_BTEXP int bfSetOpenMode(BTRVFILE *bf, int mode);

// -----------------------------------------------------------------------------
// ������� �� ᮥ������� � ����
#define  bfStop()  (btrvNuke(), BEOK)


// -----------------------------------------------------------------------------
// ����� �࠭�����
// [in] lock - ��� �����஢��
_BTEXP int bfBT(int lock);

// ����� ��।������� �࠭�����
// [in] lock - ��� �����஢��
_BTEXP int bfBDT(int lock);

// �������� �࠭�����
_BTEXP int bfET(void);

// �������� ��।������� �࠭�����
_BTEXP int bfEDT(void);

// ��ࢠ�� �࠭�����
_BTEXP int bfAT(void);

// ��ࢠ�� ��।������� �࠭�����
_BTEXP int bfADT(void);

// ����� �࠭����� ��� ������� �����⮬
// [in] lock   - ����� �����஢��
// [in] client - ���ਯ�� ������
_BTEXP int bfBTEx(int lock, bclient_h client);

// �������� �࠭����� ��� ������� �����⮬
// [in] client - ���ਯ�� ������
_BTEXP int bfETEx(bclient_h client);

// ��ࢠ�� �࠭����� ��� ������� �����⮬
// [in] client - ���ਯ�� ������
_BTEXP int bfATEx(bclient_h client);

// -----------------------------------------------------------------------------
// ������� ������ ⥪�饩 ����� (� bf->RecPos)
// [in]  bf - �����⥫� �� 䠩�
_BTEXP int bfGP(BTRVFILE *bf);

// �믮����� ������ (���� Binsert, Bupdate, Bdelete, Bstep*, Bget*)
// ��� ��� 䨫��� (KeyFltr � RecFltr)
// [in] bf - �����⥫� �� 䠩�
// [in] op - ������ (����᪠���� �����஢��)
_BTEXP int bfOp(BTRVFILE *bf, int op);

// ������ bfOp, �� � ����ᨬ��� �� ���祭�� bf->truncated � ����⢥ ����� �����
// �㤥� �ᯮ�짮������ ���� ����� 䨪�஢����� ��� (bf->fixsize), ���� ���ᨬ��쭠�
// ����� ����� (bf->MaxRecLen).
// bf->truncated ࠢ�� true ��� ����ᥩ, ᮤ�ঠ�� ��६����� ���� ���� BLOB.
_BTEXP int bfOpG   (BTRVFILE *bf, int op);

// ----------------------------------------------------------------------------
// ��饥: �㭪樨 ���� bfGD*2 �⫨����� �� bfGD* �������� ⥬, �� ���� � SQL-०���
//        ࠡ��� �ᯮ����� SQL-������.

// ������� ������ �� 䠩�� �� �� ����樨 (bf->RecPos)
// [in] bf - �����⥫� �� 䠩�
_BTEXP int bfGD    (BTRVFILE *bf);
_BTEXP int bfGD2   (BTRVFILE *bf);

// ������� ������ �� 䠩�� �� �� ����樨 (bf->RecPos) � ��⮬ 䨫��� (KeyFltr � RecFltr)
_BTEXP int bfGDEx  (BTRVFILE *bf);
_BTEXP int bfGDEx2 (BTRVFILE *bf);

// ������� ������ �� 䠩�� �� �� ����樨 pos
_BTEXP int bfGDp   (BTRVFILE *bf, recpos_t pos);
_BTEXP int bfGDp2  (BTRVFILE *bf, recpos_t pos);

// ������� ������ �� 䠩�� �� �� ����樨 pos � ��⮬ 䨫��� (KeyFltr � RecFltr)
_BTEXP int bfGDpEx (BTRVFILE *bf, recpos_t pos);
_BTEXP int bfGDpEx2(BTRVFILE *bf, recpos_t pos);

// ������� ������ �� 䠩�� �� �� ����樨 (bf->RecPos) � �����஢��� loc
_BTEXP int bfGDl   (BTRVFILE *bf, int loc);
_BTEXP int bfGDl2  (BTRVFILE *bf, int loc);

// ������� ������ �� 䠩�� �� �� ����樨 pos � �����஢��� loc
_BTEXP int bfGDpl  (BTRVFILE *bf, recpos_t pos, int loc);
_BTEXP int bfGDpl2 (BTRVFILE *bf, recpos_t pos, int loc);

// �믮����� ������ (���� Bstep*, Bget* � Bgk*) � ��⮬ 䨫��� (KeyFltr � RecFltr)
// [in] bf    - �����⥫� �� 䠩�
// [in] GetOp - ������ (����᪠���� �����஢��)
_BTEXP int bfGet   (BTRVFILE *Br, int GetOp);

// -----------------------------------------------------------------------------
// ������ bfGet � ������������ ���뢠��� ���⥫��� ����権
typedef bool (*TBreakProc) (void);
_BTEXP int bfGetEx (BTRVFILE *Br, int GetOp, TBreakProc ptr);

// -----------------------------------------------------------------------------
// ������ �᫮ ����ᥩ � 䠩�� �� ⥪�饬� ����� � 䨫����.
// ��� SQL-⠡����, �᫨ � ����஥筮� 䠩�� rsreq.ini ��।���� ��ࠬ��� GETRECCOUNT,
// ������ ����ᥩ �㤥� �ந�������� �� ����� -2.
// [in]  bf - �����⥫� �� 䠩�
// [out] nr - ��᫮ ����ᥩ
_BTEXP int bfNRec(BTRVFILE  *bf, unsigned long *nr);

// -----------------------------------------------------------------------------
// �����頥� �᫮ ����ᥩ �� ⥪�饬� ����� � 䨫����.
// bfNRec � ����ᨬ��� �� ����஥� ����� ������ -1
// [in]  bf - �����⥫� �� 䠩�
// [out] nr - ��᫮ ����ᥩ
_BTEXP int bfGetNRec(BTRVFILE *bf, unsigned long *nr);

// -----------------------------------------------------------------------------
// ��⠭����� ��������
// [in] pb   - �����⥫� �� 䠩�
// [in] OWN  - ��஫�
// [in] type - ��� ��஫� (SetORW � �.�.)
_BTEXP int bfSetOWN(BTRVFILE *pb, const char *OWN, int type);

// -----------------------------------------------------------------------------
// ������� ���� 䠩��
// [in] Nfile   - �������� 䠩��
// [in] Nclone  - �������� �����
// [in] OWNfile - ��஫� 䠩�� (�������� 䠩��)
_BTEXP int bfClone(const char *Nfile, const char *Nclone, const char *OWNfile);

// -----------------------------------------------------------------------------
// ������� 䠩�
// [in] fname - �������� 䠩��
// [in] data  - ���ᠭ�� �������� 䠩��
// [in] len   - ����� ���ᠭ��
// [in] mode  - �����
//                 OverFile    - ��१����뢠�� �������騩 䠩�
//                 NotOverFile - �� ��१����뢠�� �������騩 䠩�
_BTEXP int bfCreate(const char *fname, void *data, int *len, int mode);

// -----------------------------------------------------------------------------
// ������� 䠩� �� ���ᠭ�� � ���७��� ���ଠ樥� (�㡫� bfCreate)
// [in] fname - �������� 䠩��
// [in] data  - ���ᠭ�� �������� 䠩��
// [in] len   - ����� ���ᠭ��
// [in] mode  - �����
//                 OverFile    - ��१����뢠�� �������騩 䠩�
//                 NotOverFile - �� ��१����뢠�� �������騩 䠩�
_BTEXP int bfCreateEx(const char *fname, void *data, int *len, int mode);

// -----------------------------------------------------------------------------
// ������� 䠩� �� ���ᠭ��  ᫮���
// [in] fmtName - ��� �������� � ᫮���
// [in] dicList - ���᮪ ᫮��३ �१ ';'
// [in] path    - ���� � ᮧ�������� ⠡��� (����� � ������ ⠡����)
_BTEXP int bfCreateFromDic(const char *fmtName, const char *dicList, const char *path);

// -----------------------------------------------------------------------------
typedef struct BtrvSpec
      {
       FileSpecs   *fs;                // ���ᠭ�� 䠩��
       KeySpecs    *ks;                // ���ᠭ�� ���祩
       AltCol      *acs;               // ⠡���� ����ୠ⨢��� ���஢��
      } BtrvSpec;

// ��������� ���ଠ�� �� �������� BtrvSpec � ����� �����.
// �����頥� �ॡ㥬� ࠧ��� ����� �����, ��� ࠧ��� �ᯮ�짮������� �����.
_BTEXP size_t PlaceDataFromBtrvSpec(const BtrvSpec *spec, void *data, size_t len);

// �஢���� ����稥 ���祩 �� �����ப�. ���뢠� fld_size, ��⠢����
// �������⥫�� ����. �����頥� ����室���� � �����筮� ������⢮ �����.
_BTEXP int InsSubFieldInBtrvSpec(BtrvSpec *spec, int fld_size);


// Set BTrieve record/key buffer
// Parameters:
//    bf       -  file object
//    buffer   -  data or key buffer to set. If NULL bf->RecBuf or bf->KeyBuf used (depends on key parameter)
//    len      -  buffer length.
//    key      -  -1 for data buffer, key number for key buffer
//    mode     -  mode == 0 - set fields to zero values; mode > 0  - set buffer to max values ; mode < 0  - set buffer to min values ;
_BTEXP int btrvSetBuffer(BTRVFILE *bf, void *buffer, size_t *len, int key, int mode) ;


// Convert 'Panel field_type' to 'Btrieve Extendet_Key_Type'
// Parametrs:
//    FVt   - ���祭�� ⨯� 'Panel field_type'
//    ks    - 㪠��⥫� �� �������� KeySpecs ��� ����� १���� �८�ࠧ������
_BTEXP void fvtype2btrv(int FVt, KeySpecs *ks);


_BTEXP int BfConcurrentProhib(int tp);

// defaults:
// repCount  - 5
// delayTime - 500
// randomDelay - 0
// isConcurent - FALSE

_BTEXP int BfUnlock(BTRVFILE *bf, int mode, recpos_t addr);


_BTEXP int BtChkRecGD (BTRVFILE *bf, void *savebuf);
_BTEXP int BtChkRecGDl(BTRVFILE *bf, void *savebuf, int loc);

#define  GD(f, b)  BtChkRecGD((f), (b))

#include <packpshn.h>

// -----------------------------------------------------------------------------
typedef struct tagTBfErrorInfo
      {
       int      _bfstat;
       int      _bfop;
       BTRVFILE *_bfcb;
       char      _bfName[_MAX_PATH];
      } TBfErrorInfo;

#include <packpop.h>

_BTEXP void BfSaveError     (TBfErrorInfo *info);
_BTEXP void BfRestoreError  (TBfErrorInfo *info);

_BTEXP void BfSaveErrorEx   (TBfErrorInfo *info);
_BTEXP void BfRestoreErrorEx(TBfErrorInfo *info);

#define  BfSaveError     BfSaveErrorEx
#define  BfRestoreError  BfRestoreErrorEx


#define  NCC_OPERATION   -10

// ����� � �ᥢ��-BTRVFILE, �।�⠢���騬 ���-�����

// ������ �ᥢ��-BTRVFILE
_BEDITEXP int bfOpenMdb(BTRVFILE **bFile,        // ��室
                        char      *name,         // ��� ��������
                        char      *dicPathname); // ��� ᫮����

// ������ �ᥢ��-BTRVFILE
_BEDITEXP int bfOpenMdbEx(BTRVFILE **bFile,          // ��室
                          char      *name,           // ��� ��������
                          char      *dicPathname,    // ��� ᫮����
                          bool       useIndexMaps);  // �ᯮ�짮���� �ॢ������ ������� �� ���� std::map

// ������ �ᥢ��-BTRVFILE (�� ��⮢�� �������, � ������)
_BEDITEXP int bfOpenMdbTemplEx(BTRVFILE     **bFile,     // ��室
                               MDB_TEMPLATE  *templ, 
                               char          *name);

// ������ �ᥢ��-BTRVFILE (�� ��⮢�� �������)
_BEDITEXP int bfOpenMdbTempl(BTRVFILE     **bFile,     // ��室
                             MDB_TEMPLATE  *templ);

// ��ᮥ������� � Btrieve-䠩�� � ����⢥ ���
_BEDITEXP int bfAttach(BTRVFILE *bFile, BTRVFILE *target);

// ��ᮥ������� �� Btrieve-䠩��
_BEDITEXP int bfDetach(BTRVFILE *bFile);

// ������� ॠ��� Btrieve-䠩� ��� ���

_BEDITEXP BTRVFILE *bfBtrvForCache(BTRVFILE *Cache );

// �४��஢����
_BEDITEXP int bfPrecache(BTRVFILE *bFile, bool merge);

typedef int bfprecache_proc(int msg, void *data, void *params);

_BEDITEXP int bfPrecacheEx(BTRVFILE *bFile, bool merge, bfprecache_proc proc, void *params);

// ����஭�����
_BEDITEXP int bfSync(BTRVFILE *bFile, bool noExternalInsertionCheck);

// ����஭����� (� ��楤�ன ��ࠡ�⪨ ᮡ�⨩)
typedef int bfsync_proc(int msg, void *data, void *params);

_BEDITEXP int bfSyncEx (BTRVFILE *bFile, bool noExternalInsertionCheck, bfsync_proc proc, void *params);
_BEDITEXP int bfSyncEx2(BTRVFILE *bFile, bool noExternalInsertionCheck, bfsync_proc proc, void *params, bool UseFilters);

// ���⢥न�� �ᯥ譮� �����襭�� �࠭���樨,
// � ࠬ��� ���ன �ந��������� ᨭ�஭�����
_BEDITEXP int bfConfirmSync  (BTRVFILE *bFile);
_BEDITEXP int bfConfirmSyncEx(BTRVFILE *bFile, bool UseFilters);

// ���⪠
_BEDITEXP int bfClear(BTRVFILE *bFile);

// ���⪠ ⥪�饩 ������ ��������
_BEDITEXP int bfClearView(BTRVFILE *bFile);

// ��७��ࠢ����� �맮��� � ����
_BEDITEXP void bfSetRedirect(BTRVFILE *bFile, bool redir);

_BTEXP int BfGetLockFlags(BTRVFILE *bf, int op);

// Opening database dictionaryes list.
// Parameters:
//    ProjList - list of names of database dictionaryes separated by semicolon;
//    dbfilePath - path to database;

_BTEXP BTRVFILE **OpenProjFiles(const char *ProjList, const char *dbfilePath);

// Closing database dictionaryes list.
//
_BTEXP void CloseProjFiles(BTRVFILE **ProjFiles);

// Searching Btrv-file description in database dictionaryes.
// Parameters:
//    Name - Btrvfile name;
//    ProjFiles - list of open database dictionaryes;
// Returns:
//    Pointer on database dictionary, in which found the description
//    of Btrv-file.
//
_BTEXP BTRVFILE *FindProjFiles(const char *Name, BTRVFILE **ProjFiles);

_BTEXP void InitBtrFileList(void);
_BTEXP void DoneBtrFileList(void);

// �㭪�� 㢥�������� � �࠭������
typedef void (trnNotifyProc)(int bop, bclient_h clnt, void *procParm);

// ��������/������� �㭪�� 㢥�������� � �࠭������
_BTEXP void bfAddTrnNotify(trnNotifyProc trnNotify, bclient_h clnt, void *procParm);
_BTEXP void bfDelTrnNotify(trnNotifyProc trnNotify, bclient_h clnt, void *procParm);

// ��ࠢ��� 㢥�������� � �࠭���樨
_BTEXP void bfSendTrnNotify(int bop, bclient_h clnt);

// ������� ��� 䠩�� ��� ��᫥���� ����樨
_BTEXP const char *bfGetLastName(void);

_BTEXP char *GetRsdTblName(char *rsdName, const char *btrName);
_BTEXP char *GetRsdFldName(char *rsdName, const char *btrName);

// callback ��� ��⠭���� ᢮��� ᮥ�������
typedef void (*ConnectionCallback)(void *conn);
_BTEXP void SetConnectionCallBack(ConnectionCallback cbConn);
ConnectionCallback GetConnectionCallback();

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#define  FILL_FILE_SPECS(reclen, pagesize, nindex, fileflag)  BTRV_FILE_SPECS(reclen, pagesize, nindex, fileflag, 0)
#define  FILL_KEY_SPECS(keypos, keylen, keyflag, keytype)     BTRV_KEY_SPECS(keypos, keylen, keyflag, keytype, '\0')


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// ����䥩� ��� ��࠭����/����⠭������� ���ଠ樨 � ���ﭨ� 䠩��

// -----------------------------------------------------------------------------
// ������� ����� �吝��� ᯨ᪠ ��� ४��ᨢ���� �맮��
typedef struct TMkfPtr
      {
       struct TMkfPtr  *prev;        // �����⥫� �� �।��騩 ����� � ᯨ᪥
       void            *saveTV;      // �����⥫� �� ���� ��࠭���� ���孥�� ���祭�� ����
       void            *saveBV;      // �����⥫� �� ���� ��࠭���� ������� ���祭�� ����
       size_t           size;        // ������ ���஢ ��࠭���� ���祭�� ����
       int              saveKey;     // ���祭�� ����� ����
       JBtrFltr         saveKF;      // �����⥫� �� �㭪�� 䨫���樨 ����ᥩ - ���祢�� 䨫���
       JBtrFltr         saveRF;      // �����⥫� �� �㭪�� 䨫���樨 ����ᥩ - 䨫��� �� ������
       bool             flag;        // ��ࠢ���騩 䫠�
       void            *saveRB;      // �����⥫� �� ���� ��࠭���� �����
       recpos_t         saveRP;      // ������ � 䠩��
      } TMkfPtr;

// �����뢠�� ���ﭨ� 䠩��
_BTEXP int BtrvFileMKF(TMkfPtr **curr, BTRVFILE *bfile);

// ����⠭�������� ���ﭨ� 䠩��
_BTEXP void BtrvFileRKF(TMkfPtr **curr, BTRVFILE *bfile);

// �����뢠�� ���ﭨ� 䠩��
_BTEXP int BtrvFileMKF2(TMkfPtr **curr, BTRVFILE *bfile, bool flag, bool NeedRecPos = false);

// ����⠭�������� ���ﭨ� 䠩��
_BTEXP void BtrvFileRKF2(TMkfPtr **curr, BTRVFILE *bfile, bool flag);

// -----------------------------------------------------------------------------
// ����� ��� ࠡ��� � BLOB
// -----------------------------------------------------------------------------
class _BTEXP CRSBlob
    {
     private:
       BTRVFILE       *m_pv_bf;

       TBlobOpenMode   m_pv_BlobOpenMode;        // ����� ������ BLOB
       bool            m_pv_OpenBlob;            // �ਧ��� ������ BLOB

       bool            m_pv_MustClose;           // BLOB �㦭� �㤥� �������

       // ----------------------------------------
     protected:
       void _Init(void);

     public:
       // ----------------------------------------
       CRSBlob(BTRVFILE *bf = NULL);
       CRSBlob(BTRVFILE *bf, TBlobOpenMode openMode);
      ~CRSBlob();

       // ----------------------------------------
       BTRVFILE *Attach(BTRVFILE *bf);

       // ----------------------------------------
       // ������� ���ﭨ� BLOB
       TBlobOpenMode State(void);

       // ----------------------------------------
       // ������ BLOB
       int Open(TBlobOpenMode openMode = BLOB_OPEN_RDWR);

       // ----------------------------------------
       // ������� BLOB
       int Close(void);

       // ----------------------------------------
       // ������ ����� �� BLOB
       int Read(char *buff, int size);

       // ----------------------------------------
       // ������� ����� � BLOB
       int Write(char *buff, int size);

       // ----------------------------------------
       // ��⠭����� ������
       int SetPos(unsigned long pos);

       // ----------------------------------------
       // ������� ������
       int GetPos(unsigned long *pos);

       // ----------------------------------------
       // ������� ࠧ��� BLOB'�
       int GetSize(unsigned long *size);

       // ----------------------------------------
       // ��१��� BLOB
       int Truncate(void);

       // ----------------------------------------
       // ������ ⨯
       int SetType(unsigned char tp);

       // ----------------------------------------
       // ��६���� �� ��砫�
       int Rewind(void);
    };

/************************* End Of File **********************************/

#endif // __BTRV_H

/* <-- EOF --> */