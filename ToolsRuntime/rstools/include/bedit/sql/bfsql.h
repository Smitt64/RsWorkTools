#ifndef __BFSQL_H
#define __BFSQL_H

#include "btrv.h"
#include "na/naoplog.h"
#include <string>


// -----------------------------------------------------------------------------
#define  dRSDRECORDSET  "RSD Recordset"         // #3225 ��ப��� �����䨪���


// -----------------------------------------------
int SqlOpen(
            BTRVFILE  **bf,        // BTRVFILE ptr - returned
            const char *Name,      // ��� 䠩��
            int         Mode,      // ����� ������ 䠩��
            int         MaxRecLen, // ���ᨬ��쭠� ����� �����
            int         MaxKeyLen, // ���ᨬ��쭠� ����� ���� ����
            int         KeyNum,    // ����� ����
            JBtrFltr    RecFltr,   // ������ �롮� ����ᥩ
            JBtrFltr    KeyFltr,   // ���祢�� 䨫��� �롮� ����ᥩ
            void       *RecBuf     // ��뫪� �� ���� ������
           );

// -----------------------------------------------
void *SqlGetRecordsetFromRSL(void *rs);

// -----------------------------------------------
int SqlOpenRsd(
               BTRVFILE **bf,        // BTRVFILE ptr - returned
               void      *rs         // RSD Recordset
              );

// -----------------------------------------------
int SqlOpen2(
             BTRVFILE  **bf,         // BTRVFILE ptr - returned
             const char *Name,       // ��� 䠩��
             int         Mode,       // ����� ������ 䠩��
             int         MaxRecLen,  // ���ᨬ��쭠� ����� �����
             int         MaxKeyLen,  // ���ᨬ��쭠� ����� ���� ����
             int         KeyNum,     // ����� ����
             JBtrFltr    RecFltr,    // ������ �롮� ����ᥩ
             JBtrFltr    KeyFltr,    // ���祢�� 䨫��� �롮� ����ᥩ
             void       *RecBuf,     // ��뫪� �� ���� ������
             int         cacheSize
            );

// -----------------------------------------------
int SqlOpenEx(
              BTRVFILE  **bf,        // BTRVFILE ptr - returned
              const char *Name,      // ��� 䠩��
              int         Mode,      // ����� ������ 䠩��
              int         KeyNum,    // ����� ����
              JBtrFltr    RecFltr,   // ������ �롮� ����ᥩ
              JBtrFltr    KeyFltr    // ���祢�� 䨫��� �롮� ����ᥩ
             );

// -----------------------------------------------
int SqlOpenEx2(
               BTRVFILE  **bf,       // BTRVFILE ptr - returned
               const char *Name,     // ��� 䠩��
               int         Mode,     // ����� ������ 䠩��
               int         KeyNum,   // ����� ����
               JBtrFltr    RecFltr,  // ������ �롮� ����ᥩ
               JBtrFltr    KeyFltr,  // ���祢�� 䨫��� �롮� ����ᥩ
               int         cacheSize // ࠧ��� ���
              );

// -----------------------------------------------
int SqlClose(BTRVFILE *cb);

// -----------------------------------------------
int sfAddLink(
              BTRVFILE   *bf,
              const char *depFields,
              const char *depFiles,
              const char *joinCond,
              const char *whereCond,
              va_list     ap
             );

// -----------------------------------------------
int sfAddLink2(
               BTRVFILE *bf,
               const char *depFields,
               const char *depFiles,
               const char *joinCond,
               const char *whereCond,
               void      **array,
               int         arr_size
              );

// -----------------------------------------------
int sfAddLinkEx(
                BTRVFILE    *bf,
                const char  *depFields,
                const char  *depFiles,
                const char  *joinCond,
                const char  *whereCond,
                void       **array,
                int          arr_size,
                TExprInfo   *colinfo,
                int          colinfo_size
               );


void bfUseRealQuery (BTRVFILE *bf);
void bfUseSqlKeyFltr(BTRVFILE *bf, bool bUse);
bool bfUseOptCursor (BTRVFILE *bf, bool bUse);
bool bfUseSortCursor(BTRVFILE *bf, bool bUse);
void bfSetSqlFlags  (BTRVFILE *bf, long flags, bool bSet);
void bfResetSqlFlags(BTRVFILE *bf, long flags);
long bfGetSqlFlags  (BTRVFILE *bf);
void bfSetBlobBuffer(BTRVFILE *bf, void *buf);

int sfCreateFromDic(const char *fmtName, const char *dicList, const char *path, bool temp);

int sfDropLink(BTRVFILE *bf);

bool bfUSorted(BTRVFILE *bf);

bool NeedSwapCursor(int key);

void CancelSqlQuery();

void SetEmProperty(int property, int value);

void SetSfProperty(BTRVFILE *bf, int prop, int val);
void GetSfProperty(BTRVFILE *bf, int prop, int *val);

int bfOperlogInsertXML(BTRVFILE *oplog, TOperLog_type::opType optype, const char *filename, void *buf, int len, int xml_format);
bool bfOplogBufferFromXML(BTRVFILE *oplog, void *buffer, int xml_format);
char *bfOplogStringCRC32_FromXML(BTRVFILE* oplog, int xml_format);

bool IsBtrvEmFile(BTRVFILE *bf);

#endif __BFSQL_H

/* <-- EOF --> */