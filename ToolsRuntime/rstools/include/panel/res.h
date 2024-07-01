/*
         RES.H    Resource loading system
         April 13, 1993   Written by S. Kubrin

         Modified:
 April 28,1994  Serg Kubrin (K78)
          �����প� ᯨ᪠ ᢮������ ������
 February 1, 1996 by A.Dakhov
         Support of accelerators table, string table and rewritten menus
 August 5,1996   Serg Kubrin (K78)
         Add Version number and reserv in ResHeader
 January 13,1997   Serg Kubrin (K78)
         ��������� �࠭���⭠� ��ࠡ�⪠ ���������� 䠩��
         ��� �� �ᯮ�짮����� ����室��� �� ����⨨ 䠩��
         ������ 䫠� RO_TRN

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/panel/h/res.h $
                  $Revision: 27 $
                  $Date: 14.02.00 15:26 $
                  $Author: Lebedev $
                  $Modtime: 14.02.00 13:21 $
*/

#ifndef __RES_H
#define __RES_H

#ifndef _RESH
#define _RESH

#include "pexpdef.h"

#ifndef RSL_FTIME_DEFINED
#define RSL_FTIME_DEFINED

    #include <packpsh1.h>

    struct ftime
         {
          unsigned   ft_tsec  : 5;   /* Two second interval */
          unsigned   ft_min   : 6;   /* Minutes */
          unsigned   ft_hour  : 5;   /* Hours */
          unsigned   ft_day   : 5;   /* Days */
          unsigned   ft_month : 4;   /* Months */
          unsigned   ft_year  : 7;   /* Year */
         };

    #include <packpop.h>

#endif

#include <systypes.h>

#include "rslcvt.h"
#include "rstypes.h"
#include "listobj.h"

#ifdef __cplusplus
extern "C" {
#endif

//    ���� �訡��
#define  L_OPENLINK      1  // �� �������� ������ �ਫ�������� �����
#define  L_OPENLIB       2  // �� �������� ������ ������⥪� ����ᮢ
#define  L_LIBTYPE       3  // ����७ �ଠ� 䠩��
#define  L_DIRNOPEN      4  // �� �������� ������ ��⠫��, 䠩� �� �����
#define  L_DIRNOMEM      5  // �� �������� ������ ��⠫��, ��� �����
#define  L_DIRREAD       6  // �訡�� �⥭�� ��⠫���
#define  L_DIRWR         7  // �訡�� ����� ��⠫���
#define  L_DIREMPTY      8  // ��⠫�� ����
#define  L_DIRMISS       9  // ������� ��⠫��� ���������
#define  L_DUPOB        10  // �㡫�஢���� ����� ��⠫���
#define  L_POSLIB       11  // �� �������� ᯮ��樮��஢����� � ������⥪�
#define  L_ADDMEM       12  // �� �������� �������� ����� , ��� �����
#define  L_RESGET       13  // �� �������� ������ �����
#define  L_RESPUT       14  // �� �������� ������� �����
#define  L_WRSIZE       15  // ������ ����ᠭ���� ����� �� ࠢ�� ����襭����
#define  L_VERSION      16  // �������ন������ ����� �����
#define  L_TRNACTIV     17  // Transaction already activ
#define  L_TRN          18  // Error in transaction
#define  L_OPENFORWR    19  // Error open for write access denide
#define  L_STRMMODE     20  // Bad stream mode
#define  L_OLDFORMAT    21  // ���ॢ訩 �ଠ� 䠩��

#define  L_LASTER       21  // ����� ��᫥���� �訡��

//  Bit flags for ResOpen
#define  RO_USELINKED    1  // �������� �ᯮ�짮���� �ਫ��������� �����
#define  RO_READDIR      2  // �� ����⨨ ����� ��⮬���᪨ ���� ����������
#define  RO_MODIFY       4  // �������� ������஢��� ������ ���ࠧ㬥���� RO_READDIR
#define  RO_CREATE       8  // �᫨ �����-䠩� �� �������, ᮧ���� ��� ���ࠧ㬥���� RO_MODIFY
#define  RO_NEW         16  // �ᯮ������ ᮢ���⭮ � RO_CRETE. �᫨ �����-䠩� �������, � �� 㤠�����
#define  RO_FLUSH       32  // �ந������� ������ ���������� � ����� ���஢ ��᫥ ������� ���������
#define  RO_TRN         64  // Use Transaction


//  ����� ���ﭨ�
#define  DIR_READ        1  // ��⠫�� ���⠭
#define  DIR_MODIFIED    2  // ��⠫�� ������஢��
#define  FREE_READ       4  // ���᮪ ᢮������ ������ ���⠭
#define  NEED_FLUSH      8  // ����室��� �믮����� flush �� ����䨪����
#define  USE_TRN        16  // Update operations is transactional
#define  READ_MODE      32  // Read only mode

/*
   Resource file ��⮨� �� ��������� � ᫥����� �� ��� ��ꥪ⮢
   � ��������� ᮤ�ন��� ᬥ饭�� ��ꥪ�, ����� �ᯮ������ ���
   ��⠫�� �� �⮬� �� ᬥ饭�� �����뢠���� � ���� ��ꥪ�
*/

#include <packpsh1.h>

//  �� ������� ����ᠭ� � ��砫� 䠩��
typedef struct tagHeader
      {
       char       password[7]; /*  ��஫쭮� ᫮��  */
       db_int32   DataOffset;  /*  ���饭�� � ������⥪� ��� ����� ������ ����� */
       db_uint16  NumElem;     /*  ������⢮ ��ꥪ⮢ � ������⥪� */
       db_uint16  ElemSize;    /*  ������ ����� ��⠫���         */
       db_uint16  NumFree;     /*  ������⢮ ᢮������ ������      */
       uchar      isTrn;       /*  File in transaction              */
       char       reserv[7];
      } Header;

//  ������ ������� ����ᠭ� � ��砫� ������� �����
//  �㭪樨 ���騥 � ����騥 ������ ����� ������� ����� �
//  ⥪�饬� ResHeader �१ �㭪樨
//  ResGetCurHeader, ResSetCurHeader. � ⥪�饬� ������ ��⠫���
//  �१ �㭪�� ResGetCurPtr
typedef struct tagResHeaderOld
      {
       db_uint16      resSize;  /* ������ ��ꥪ�   */
       unsigned char  reserv;   /* Object reserved  */
       unsigned char  ver;      /* Object version   */
       struct ftime   ftime;    /* ��� � �६� ᮧ����� �� io.h */
      } ResHeaderOld;

typedef struct tagResHeader
      {
       db_uint16      resSizeLo;  /* ������ ��ꥪ�   */
       unsigned char  headVer;    /* Object header version  */
       unsigned char  ver;        /* Object version   */
       struct ftime   ftime;      /* ��� � �६� ᮧ����� �� io.h */
       db_uint16      resSizeHi;  /* ������ ��ꥪ� Hi bits in header version 1 */
      } ResHeader;

#define  MAKESIZE(rh)        ((size_t)( ((rh)->resSizeLo) | ((size_t)((rh)->resSizeHi) ) << 16))
#define  RHEADSIZE(rh)       ((rh)->headVer == 0 ? sizeof(ResHeaderOld) : sizeof (ResHeader))
#define  RHEADSIZEFOR(size)  ((size) < 0xffff ? sizeof(ResHeaderOld) : sizeof (ResHeader))

//  �� �⠭��ୠ� ���� ����� ����������.
//  �� ��⠫��� ���짮��⥫� ������ ���� 㭠᫥������ �� �⮩ ��������
//  ����� LibElem
typedef  struct tagResource
      {
       db_int32 offset;            /* ���饭�� ��ꥪ� � ������⥪� */
      } Resource;

#define P_RES(ptr)   ((Resource*)(ptr))

typedef struct
      {
       long  offset;   /* ���饭�� ᢮������� ����� � 䠩�� */
       long  size;
      } FreeBlock;

//  ��� �८�ࠧ������ � �㭪樨 �ࠢ����� ��⠫����
typedef int  (*RES_CMP)(Resource *rc1, Resource *rc2);
typedef void (*RES_CVT)(struct tagResFile *, HRSLCVT hcvt, Resource *);

#include <packpop.h>

#include <packpshn.h>

typedef struct tagResFile
      {
       Header      head;        /* ��������� 䠩��                          */
       int         hd;          /* ���ਯ�� 䠩��. -1 �᫨ 䠩� �� ����� */
       Resource   *Directory;   /* �������� ����������                */
       FreeBlock  *freeblock;   /* �������� �����                    */
       RES_CMP     ResCmp;
                                /* �㭪�� �ࠢ����� ����⮢ ��⠫���     */
       unsigned    flags;       /* ��⠫�� ���⠭,������஢�� ...       */
       long        base;        /* ���饭�� ��砫� 䠩�� ����ᮢ           */

       char       *fileName;

       HRSLCVT     hcvtRd;
       HRSLCVT     hcvtWr;
       RES_CVT     cvtDirProc;
      } ResFile;


typedef ResFile *HRSLRESFILE;

typedef int (*RES_PUT)(HRSLRESFILE, void *);
typedef int (*RES_GET)(HRSLRESFILE, void **);

// ���ᠭ�� ��� �㭪権 ᬮ�� � 䠩�� RES.DOC

_RSRESEXP char      *ResError        (int stat);
_RSRESEXP int        ResGetObject    (HRSLRESFILE rf, Resource *rc, void **data, RES_GET Get);
_RSRESEXP int        ResPutObject    (HRSLRESFILE rf, Resource *rc, void *data, RES_PUT Put, long size);
_RSRESEXP int        ResWritePrefix  (HRSLRESFILE rf, Resource *rc);
_RSRESEXP int        ResReadPrefix   (HRSLRESFILE rf, Resource *rc);
_RSRESEXP ResHeader *ResGetCurHeader (void);
_RSRESEXP Resource  *ResGetCurPtr    (void);
_RSRESEXP Resource  *ResSetCurPtr    (Resource *rc);
_RSRESEXP ResFile   *ResFileGetCurPtr(void);
_RSRESEXP ResFile   *ResFileSetCurPtr(ResFile *rf);
_RSRESEXP void       ResSetCurHeader (void);
// _RSRESEXP void         ResClear            (HRSLRESFILE rf);
_RSRESEXP int        SetResModify    (int Modify);
_RSRESEXP int        ResOpen         (ResFile *rt, const char *lib, const char *password, RES_CMP ResCmp,
                                      RES_CVT resCvt, int flags, ...);
_RSRESEXP int        ResClose        (HRSLRESFILE rt);
_RSRESEXP int        ResGetElem      (HRSLRESFILE rt, Resource *res);
_RSRESEXP int        ResAddElem      (HRSLRESFILE rf, Resource *rc);
_RSRESEXP int        ResPutDirectory (HRSLRESFILE rt);
_RSRESEXP int        ResGetDirectory (HRSLRESFILE rt);
_RSRESEXP int        ResFreeDirectory(HRSLRESFILE root);
_RSRESEXP int        ResNumObjects   (HRSLRESFILE rf);
_RSRESEXP int        ResDelObject    (HRSLRESFILE rf, Resource *res);

int ResReadPrefixEx (ResFile *rf, Resource *rc, ResHeader *rhead);


//  ��堭��� ���樨 �� ��⠫���
typedef struct tagResIter
      {
       unsigned   limit;
       unsigned   size;
       char      *ptr;
       unsigned   current;
      } ResIter;

_RSRESEXP void      ResSetIter(ResIter *iter, HRSLRESFILE rf);
_RSRESEXP Resource *ResGetIter(ResIter *iter);

typedef struct tagMemStream
      {
       CNTLIST  pages;
       size_t curPos;
      } TMemStream;

size_t MemStremWrite    (TMemStream *st, char *buff, size_t sz);
size_t MemStremRead     (TMemStream *st, char *buff, size_t sz);
size_t MemStremGetSize  (TMemStream *st);
int    MemStremSetPos   (TMemStream *st, size_t pos);
int    MemStremGetPos   (TMemStream *st, size_t *pos);
void   MemStremInit     (TMemStream *st);
void   MemStremDone     (TMemStream *st);
int    MemStremSaveToRes(TMemStream *st, int hd);


typedef struct tagResStream
      {
       ResFile    *rf;
       int         mode;
       int         stat;
       long        size;
       Resource   *rc;
       TMemStream  memStrm;
       size_t      strmCurSize;
       bool        useMewStrm;
    // April 19,2001
       ResHeader   rhead;
       long        resLibPos;  // Position in LBR for read/write operations
      } ResStream;

#define  RES_MODE_CLOSED   0
#define  RES_MODE_READ     1
#define  RES_MODE_WRITE    2

_RSRESEXP int ResOpenStream (ResStream *strm, ResFile *rf, Resource *rc, int mode, int ver, long size);
_RSRESEXP int ResCloseStream(ResStream *strm);
_RSRESEXP int ResWriteStream(ResStream *strm, void *buff, int size, int *);
_RSRESEXP int ResReadStream (ResStream *strm, void *buff, int size, int *);

int ResSetPosStream (ResStream *strm, size_t pos);
int ResGetSizeStream(ResStream *strm, size_t *size);
int ResGetPosStream (ResStream *strm, size_t *pos);


//  ***************************************************************
//           ResLib  part

_RSRESEXP char *MakeEXT(char *buff, char *name, char *ext);


// -----------------------------------------------------------------------------
#define  RESNAMELEN    9       // ����� ������������ ����� (� �ਪ������ ���� ��� 0-ᨬ����,
                               // � � ������⥪� ����ᮢ - � 0-ᨬ�����).
#define  RESFILELEN  255       // ����� ������������ ������⥪� ����ᮢ (����� ������� ⠪��
                               // �⭮�⥫�� ��� ��᮫��� ����). �� �������� � RESNAMELEN
                               // ���� ��� 0-ᨬ��� �㦭� �뤥���� �⤥�쭮.


// -----------------------------------------------------------------------------
typedef struct tagLibElem
      {
       Resource        rc;
       char            name[RESNAMELEN];
       unsigned  char  type;
      } LibElem;

typedef struct tagLibStream
      {
       ResStream  derived;
       LibElem    rc;
       int        oldflags;
       bool       inTrnMode;
      } LibStream;

// -----------------------------------------------------------------------------
// Types of resources
#define  RES_MENU         1    // ����
#define  RES_STAT         2    // ��ப� �����
#define  RES_DIALOG       3    // ���� ������
#define  RES_HIST         4    // ��ப� ���᪠���
#define  RES_REPORT       5    // ���� ��� ����
#define  RES_BFSTRUCT     6    // ������� Btrieve 䠩��
#define  RES_DBLINK       7    // ���� ����� �࠭��� �ମ� � 䠩��� ������

#define  RES_PANEL        8
#define  RES_SCROL        9
#define  RES_REP         10
#define  RES_BS          11
#define  RES_LS          12
#define  RES_ACCEL       13
#define  RES_STRTABLE    14

#define  RES_MENU2       15

#define  RES_LAST       RES_MENU2

#define  RES_RSFORM     100
#define  RES_RSMENU     101
#define  RES_RSREPORT   102
#define  RES_IMGLST     104
#define  RES_RSAXEXP    110
#define  RES_RSCODEGEN  111

// Types of objects in RES_MENU resource
#define  RES_MITEM        1
#define  RES_MPOPUP       2
#define  RES_DELIM        3

// Types of objects in RES_DIALOG resource
#define  RES_CLUST        1
#define  RES_HVIEW        2
#define  RES_LABEL        3
#define  RES_TEXT         4
#define  RES_BUTN         5
#define  RES_SBUTN        6
#define  RES_LIST         7
#define  RES_BLIST        8
#define  RES_EDIT         9
#define  RES_CHAIN       10



_RSRESEXP int  _OpenLib    (HRSLRESFILE rf, const char *fname, int flags);
_RSRESEXP int  PutResObject(HRSLRESFILE rf, const char *name, unsigned char type, void *data, RES_PUT Put, long size);
_RSRESEXP int  GetResObject(HRSLRESFILE rf, const char *name, unsigned char type, void **data, RES_GET Get);

_RSRESEXP int  FindResObject  (HRSLRESFILE rf, const char *name, unsigned char type);
_RSRESEXP int  DeleteResObject(HRSLRESFILE rf, const char *name, unsigned char type);
_RSRESEXP int  RenameResObject(HRSLRESFILE rf, const char *name, unsigned char type, const char *newName);

_RSRESEXP int  LibOpenStream (LibStream *strm, ResFile *rf, int mode, const char *name, unsigned char type,
                              int ver, long size);
_RSRESEXP int  LibCloseStream(LibStream *strm, bool abort);
_RSRESEXP int  LibWriteStream(LibStream *strm, void *buff, int size,int*);
_RSRESEXP int  LibReadStream (LibStream *strm, void *buff, int size,int*);

#define  LibSetPosStream(pStrm, pos)    ResSetPosStream(&(pStrm)->derived, pos)
#define  LibGetPosStream(pStrm, pos)    ResGetPosStream(&(pStrm)->derived, pos)
#define  LibGetSizeStream(pStrm, size)  ResGetSizeStream(&(pStrm)->derived, size)

//----------------------------------------------------------------------
//      ����஥��� ��⠫���
typedef enum
      {
       RS_NAME, RS_TYPE, RS_DATE, RS_COMM
      } DirSort;

typedef struct
      {
       char           name[RESNAMELEN];
       unsigned char  type;
       long           size;
       struct ftime   ftime;
       unsigned char  flags;  // ���짮��⥫�᪨� 䫠��
       unsigned char  ver;
      } RLibDirElem;

typedef struct
      {
       RLibDirElem  *Elem;
       int           NumElem;   // ������⢮ ����������� ����⮢ � ᯨ᪥
       int           alloced;   // ������⢮ ����⮢ ��� ���஥ ��।����� ������
       DirSort       sortby;    // ���冷� ���஢��
       int           cmtSize;
      } RLibDir;


#define  NEXT_DIR(elem, dir)   ((RLibDirElem *)((char *)((elem) + 1) + (dir)->cmtSize))
#define  CMT_PTR(elem)         ((char *)((elem) + 1))
#define  ELEM_PTR(dir, num)    ((RLibDirElem *)((char *)((dir)->Elem) + ((dir)->cmtSize + sizeof(RLibDirElem)) * (num)))


typedef int (*RLibDirElemFiltr)(RLibDirElem *);


_RSRESEXP void SortDirectory  (RLibDir *dir, DirSort sortby);
_RSRESEXP int  BuildDirectory (RLibDir *dir, HRSLRESFILE rf, RLibDirElemFiltr filt, int num, int cmtSize);
_RSRESEXP void FreeDirectory  (RLibDir *dir);
_RSRESEXP int  RemDirElem     (RLibDir *dir, int n);
_RSRESEXP int  RemDirElemName (RLibDir *dir, const char *name, unsigned char type);
_RSRESEXP int  AddDirElem     (RLibDir *dir, const char *name, unsigned char type, long size, struct ftime *ftime,
                               unsigned char ver, unsigned char flags, const char *cmt);
_RSRESEXP void SetResLibBaseAddr(long addr);

#include <packpop.h>

#ifdef __cplusplus
}
#endif

#endif  // _RESH
#endif  // __RES_H

/* <-- EOF --> */