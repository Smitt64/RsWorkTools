/*-----------------------------------------------------------------------*-
 File Name   : nwbtrv.h

 Sybsystem   : Btrieve
 Description : ����ন� ������ � Btrieve ��� DOS, DPMI16, DPMI32,
               Windows

               ��� DPMI16 � DPMI32 ����� �ᯮ�짮���� �������
               Btrieve Bstop !!!

 Source      : nwbtrv.c

 Library(s)  :

 Copyright (c) 1991 - 1994 by R-Style JSC
 All Rights Reserved.

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/bedit/h/nwbtrv.h $
                  $Revision: 34 $
                  $Date: 8.06.00 16:37 $
                  $Author: Kubrin $
                  $Modtime: 8.06.00 16:37 $

-*- History -------------------------------------------------------------*-
    January 6,1995   Sergei Kubrin (K78) - Create file
    February 13,1995   Serg Kubrin (K78)
      �����প� Windows NT

   09/01/1997 Yakov E. Markovitch (JNS)
    Multi-client support added.
   13/02/1997 Yakov E. Markovitch (JNS)
    btrvNuke & bfStop added.
   20/06/1997 Yakov E. Markovitch (JNS)
    Codes of extended operations added to Btrieve_Operations.

-*-----------------------------------------------------------------------*/

#include "platform.h"

#ifndef __NWBTRV_H
#define __NWBTRV_H

#include "bexpdef.h"

#include <bdate.h>
#include <rstypes.h>

// -----------------------------------------------------------------------------
#if defined(SQLBUILD) || (defined(__RSRTL_DLL) && defined (_MSC_VER) && !defined (NO_BKMK_CLASS))
    #ifdef __cplusplus
        #include "sql/bkmrk.h"

        typedef Bookmark  recpos_t;
    #else
        typedef long      recpos_t;
        #define  RECPOS_T_ZERO      (0L)
        #define  RECPOS_T_NOT_ZERO  (1L)
    #endif // __cplusplus
#else //!SQLBUILD
    #define  RECPOS_T_ZERO      (0L)
    #define  RECPOS_T_NOT_ZERO  (1L)
    #define  BKMHANDLE          long

    typedef  long  recpos_t;
#endif //SQLBUILD

#include <packpsh1.h>

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// ����樨 ��� bfOp, bfGet � �.�.
typedef enum Btrieve_Operations
      {
       Bopenposblk   = -2,  // ���⨢��� ������ ᯥ樠�쭮 ��� �ᯮ�짮����� �
                            // ���墠�稪� ����権 ��� ��࠭���� ॠ�쭮�� posblk
       Bnop          = -1,

       Bopen         = 0,   // ������ �������騩 䠩�
       Bclose        = 1,   // ������� 䠩�

       Binsert       = 2,   // ��⠢��� ������
       Bupdate       = 3,   // �������� ������
       Bdelete       = 4,   // ������� ������

       // ����樨 BgetE - BgetL �믮����� ����祭�� ����ᥩ � ��⮬ ���祢��� 䨫���
       BgetE         = 5,   // ������� ������ �� ����� (�筮� ᮮ⢥��⢨�)
       BgetN         = 6,   // ������� ᫥������ ������ �� �����
       BgetP         = 7,   // ������� �।����� ������ �� �����
       BgetGT        = 8,   // ������� ������ ��ண� ����� ���祭�� ����
       BgetGE        = 9,   // ������� ������ ����� ��� ࠢ�� ���祭�� ����
       BgetLT        = 10,  // ������� ������ ��ண� ����� ���祭�� ����
       BgetLE        = 11,  // ������� ������ ����� ��� ࠢ�� ���祭�� ����
       BgetF         = 12,  // ������� ����� ������ �� �����
       BgetL         = 13,  // ������� ��᫥���� ������ �� �����

       Bcreate       = 14,  // ������� 䠩�
       BcreateX      = 14,  // ������� 䠩�
       Bstat         = 15,  // ������� ���ଠ�� � 䠩��
       Bextend       = 16,  // ��������� 䠩� ������ �� ��� �����᪨� ��᪠ (१��)
       Bsetdir       = 17,  // ��⠭����� ⥪�騩 ��⠫��
       Bgetdir       = 18,  // ������� ⥪�騩 ��⠫�� ��� �ॡ㥬��� ��᪠

       // ������ � �࠭����ﬨ
       BBT           = 19,  // ����� �࠭�����
       BET           = 20,  // �������� �࠭�����
       BAT           = 21,  // ��ࢠ�� �࠭�����

       Bgetpos       = 22,  // ������� ������ ⥪�饩 �����
       BgetD         = 23,  // ������� ������ �� �� ����樨
       BstepN        = 24,  // ������� ᫥������ ������ � 䨧��᪮� ��᫥����⥫쭮��
       Bstop         = 25,  // ��⠭����� ࠡ��� ⥪�饣� ������ ����
       Bver          = 26,  // ������� ���ᨨ ��������� ����
       Bunlock       = 27,  // ��������஢��� ������(�)
       Breset        = 28,  // �᢮������ ������, ������ �����⮬
       Bsetown       = 29,  // ��⠭����� �������� ��� 䠩��
       Bclearown     = 30,  // ����� �������� � 䠩��
       BcreateSI     = 31,  // �������� ���� � �������騩 䠩�
       BdropSI       = 32,  // ������� ���� �� �������饣� 䠩��

       // ����� � �����ﬨ � 䨧��᪮� ��᫥����⥫쭮�� (��� ���祢��� 䨫���)
       BstepF        = 33,  // ������� ����� ������ � 䨧��᪮� ��᫥����⥫쭮��
       BstepL        = 34,  // ������� ��᫥���� ������ � 䨧��᪮� ��᫥����⥫쭮��
       BstepP        = 35,  // ������� �।����� ������ � 䨧��᪮� ��᫥����⥫쭮��

       // ����樨 � ����⭮� �����প��
       BgetNX        = 36,  // ������� ���� ��� ��᪮�쪮 ᫥����� ����ᥩ �� �����
       BgetPX        = 37,  // ������� ���� ��� ��᪮�쪮 �।���� ����ᥩ �� �����
       BstepNX       = 38,  // ������� ���� ��� ��᪮�쪮 ᫥����� ����ᥩ � 䨧��᪮� ��᫥����⥫쭮��
       BstepPX       = 39,  // ������� ���� ��� ��᪮�쪮 �।���� ����ᥩ � 䨧��᪮� ��᫥����⥫쭮��
       BinsertX      = 40,  // �������� ���� ��� ��᪮�쪮 ����ᥩ � 䠩�

       // ����樨 � ��業�� ����樮��஢����� (��� �஫������)
       BgetPcnt      = 44,  // ������� ������ �� ��������� ��業⭮�� ࠧ��饭�� � ��⮬ ����
       BfindPcnt     = 45,  // ������� ��業⭮� ࠧ��饭�� ⥪�饩 �����

       // BLOB � �.�.
       BupdateChunk  = 53,  // �������� ���� �����

       // ������� ����権 BgetE - BgetL, �� ࠡ���� ⮫쪮 � ���砬� (ᠬ� ������ �� ����㦠����).
       // � �拉 ��砥� �������� 㢥����� ᪮���� ࠡ��� � ����
       BgkE          = 55,  // ������� ���� ����� (�筮� ᮮ⢥��⢨�)
       BgkN          = 56,  // ������� ���� ᫥���饩 �����
       BgkP          = 57,  // ������� ���� �।��饩 �����
       BgkGT         = 58,  // ������� ���� ����� ��ண� ����� ��������� ���祭�� ����
       BgkGE         = 59,  // ������� ���� ����� ����� ��� ࠢ�� ��������� ���祭�� ����
       BgkLT         = 60,  // ������� ���� ����� ��ண� ����� ��������� ���祭�� ����
       BgkLE         = 61,  // ������� ���� ����� ����� ��� ࠢ�� ��������� ���祭�� ����
       BgkF          = 62,  // ������� ���� ��ࢮ� �����
       BgkL          = 63,  // ������� ���� ��᫥���� �����

       // ���� ����樨 ��������� ⮫쪮 ��। BmaxOp !!!
       BmaxOp
      } Btrieve_Operations;


// -----------------------------------------------------------------------------
// �����஢�� (� �� ⮫쪮)
enum Locks
   {
    BIAS          =  50,    // ��� ����祭�� ����権 55 - 63 �� ᮮ⢥������� 5 - 13
    SWL           = 100,    // �����஢�� �⥭�� ����� ����� � ���������
    SNL           = 200,    // �����஢�� �⥭�� ����� ����� ��� ��������
    MWL           = 300,    // �����஢�� �⥭�� ��᪮�쪨� ����ᥩ � ���������
    MNL           = 400,    // �����஢�� �⥭�� ��᪮�쪨� ����ᥩ ��� ��������
    NOWAITMODIFY  = 500,    // �����஢�� ����� ��࠭��� ��� �������� (��� �����७��� �࠭���権)
    TRNCONCURENT  = 1000,   // �����७�� �࠭���樨 (��� V.6 ?)
    TRNCONCURRENT = TRNCONCURENT
   };

// -----------------------------------------------------------------------------
// ������ ������ 䠩���

enum OpenModes
   {
    Accelerated  =  -1,     // �᪮७�� ०��
    ReadOnly     =  -2,     // ����� "⮫쪮 ��� �⥭��"
    Verify       =  -3,     // ���䨪��� ������ (�� �����ন������)
    Exclusive    =  -4,     // ��᪫��� ०��

#ifdef SQLBUILD
    Share        =  -5,     // ������塞� ०��
    ExclusiveEx  =  -6,     // ����७�� �᪫��� ०�� (��� SQL)
#endif

    Normal       =   0,     // ��ଠ��� ०�� (�⥭��/������)

    // ----- ������� ��� �ਭ㤨⥫쭮� �����쭮� ࠡ��� � ����
    NormalL      =   6,
    AcceleratedL =   7,
    ReadOnlyL    =   8,
    ExclusiveL   =  10,

    // ----- ������� ��� �ਭ㤨⥫쭮� 㤠������ ࠡ��� � ����
    NormalR      =  99,
    AcceleratedR = 100,
    ReadOnlyR    = 101,
    ExclusiveR   = 103
   };

#ifndef  SQLBUILD
    #define  Share        Normal
    #define  ExclusiveEx  Exclusive
#endif


// -----------------------------------------------------------------------------
// �����প� ����権 ᮧ����� 䠩��� � �.�.

// ���樠��� ����� ���祩 (��� ����樨 Bcreate)
#define  KNrename    -127           // ��२�������� 䠩�
#define  KNdelete    -128           // ������� 䠩�

// -----------------------------------------------
// ����� 䠩��
enum File_Flags
   {
    VarLen            =      1,     // ������ ᮤ�ন� ��६����� ����
    TruncB            =      2,     // ��祭�� ������� �஡���� � �����
    PreAll            =      4,     // �।�뤥����� ��࠭��
    DataC             =      8,     // ���⨥ ������
    KeyOnly           =     16,     // ���� ᮤ�ঠ騩 ⮫쪮 ����
    BalancedIndex     =     32, // ������஢�� �����ᮢ
    Free10            =     64, // ��१�ࢨ஢��� 10% ᢮������� ����
    Free20            =    128, // --//-- 20%
    Free30            =    192, // --//-- 30%
    IncludeSystemData =    512, // ��������� ��⥬�� �����
    SPECIFY_KEY_NUMS  =   1024,     // ������᪨� ����� ����
    VATS_SUPPORT      =   2048, //
    IUO_KeyOnly       = 0x4000, // ����७��� 䫠� ��ࢠ����
    NotSystemData     =   4608, // �� ��������� ��⥬�� �����
    IUO_PendingKey    = 0x8000, // ����७��� 䫠� ��ࢠ����
    PageC             =   8192  // ���⨥ ��࠭��
   };

// -----------------------------------------------
typedef enum File_Flags  File_Flas;

// -----------------------------------------------
// ����� ����
enum Key_Flags
   {
    Kdup  =   1,                    // �㡫��㥬�
    Kmod  =   2,                    // �������㥬�
    Kbin  =   4,                    // ������ (����)
    Knull =   8,                    // �㫥��� ����
    Ksegm =  16,                    // ��������㥬�
    Kalt  =  32,                    // ����ୠ⨢��
    Kdes  =  64,                    // ���⭠� ���஢��
    Ksup  = 128,                    // �������騩�� �㡫��㥬� ()
    Kext  = 256,                    // ���� ᮤ�ন� ����� ���७���� ⨯�
    Kman  = 512                     // ��筮�
   };

// -----------------------------------------------
// ���ᨬ��쭮� �᫮ ᥣ���⮢ ����
// ������ �� ���ᨨ 䠩��/ࠧ��� ��࠭��� � ����� ���� �� 420.
#define  MaxKeySegm  119

// -----------------------------------------------
// ����७�� ⨯� ���祩 (Extended_Key_Type)
#define  Estring    '\x00'          // STRING
#define  Einteger   '\x01'          // INTEGER
#define  Efloat     '\x02'          // FLOAT
#define  Edate      '\x03'          // DATE
#define  Etime      '\x04'          // TIME
#define  Edecimal   '\x05'          // DECIMAL
#define  Emoney     '\x06'          // MONEY
#define  Elogical   '\x07'          // LOGICAL
#define  Enumeric   '\x08'          // NUMERIC
#define  Ebfloat    '\x09'          // BFLOAT (Microsoft basic standard)
#define  Elstring   '\x0A'          // LSTRING (Pascal compatible string)
#define  Ezstring   '\x0B'          // ZSTRING (C compatible string)
#define  Eunbin     '\x0E'          // UNSIGNED BINARY
#define  Eautoinc   '\x0F'          // AUTOINC
#define  Ecurrency  '\x13'          // CURRENCY

// -----------------------------------------------
#if defined (USE_FDECIMAL)
    #define  EMONEY   Einteger
#else
    #define  EMONEY   Efloat
#endif

// -----------------------------------------------
typedef struct posblk_t
      {
       char  posblk[128];
      } posblk_t;

// -----------------------------------------------
typedef posblk_t  *posblk_h;


// -----------------------------------------------
// ����䨪��� 䠩��
typedef struct FileSpecs
      {
       db_baseint16   reclen;       // ������ 䨪�஢����� ��� �����
       db_baseint16   pagesize;     // ������ ��࠭��� (��⥭ 512, �� ������ �� ���ᨨ ����)
       db_baseint16   nindex;       // ��᫮ ���祩 (�����ᮢ) - ����訩 ����, � ���襬 - ����� 䠩��
                                    // (��-㬮�砭�� 0�00)
       db_ubaseint32  nrecords;     // �� ᮧ����� 䠩�� �� १��, � ��� ����樨 Stat ����� �����頥���
                                    // ������⢮ ����ᥩ � 䠩��.
       db_baseint16   fileflag;     // ����� 䠩�� (File_Flags)
       char           reserved[2];  // �����
       db_baseint16   prealloc;     // ��᫮ �।���⥫쭮 �뤥�塞�� ��࠭�� (��� ᮧ����� 䠩��,
                                    // ��� ����樨 Stat ����� �����頥��� �᫮ ���ᯮ��㥬�� ��࠭��
      } FileSpecs;

// -----------------------------------------------
#define  BTRV_FILE_SPECS(reclen, pagesize, nindex, fileflag, prealloc) \
{                             \
   (db_int16)(reclen),        \
   (db_int16)(pagesize),      \
   (db_int16)(nindex),        \
   (db_uint32)0,              \
   (db_int16)(fileflag),      \
   { '\0', '\0' },            \
   (db_int16)(prealloc)       \
}

// -----------------------------------------------
// ����䨪��� ����
typedef struct KeySpecs
      {
       db_baseint16   keypos;       // ������ ���� (��ࢮ�� ����) ����� ����� (�㬥��� ���� � ����� � 1-��)
       db_baseint16   keylen;       // ����� ���� (����)
       db_baseint16   keyflag;      // ����� ���� (Key_Flags)
       db_ubaseint32  nkeys;        // ����� (��� ����樨 Stat ����� �����頥��� �᫮ 㭨������ ���祩)
       char           keytype;      // ����७�� ⨯ ���� (Extended_Key_Type)
       char           nullvalue;    // �㫥��� ���祭�� (Null Value)
       unsigned char  reserved[4];  // �����
      } KeySpecs;

// -----------------------------------------------
#define  BTRV_KEY_SPECS(keypos, keylen, keyflag, keytype, nullvalue) \
{                             \
   (db_int16)(keypos),        \
   (db_int16)(keylen),        \
   (db_int16)(keyflag),       \
   (db_uint32)0,              \
   (keytype),                 \
   (nullvalue),               \
   { '\0', '\0', '\0', '\0' } \
}

// -----------------------------------------------
#define  BTRV_KEY_SPECS2(keypos, keylen, keyflag, keytype, nullvalue, keyNum) \
{                             \
   (db_int16)(keypos),        \
   (db_int16)(keylen),        \
   (db_int16)(keyflag),       \
   (db_uint32)0,              \
   (keytype),                 \
   (nullvalue),               \
   { '\0', '\0', (keyNum), '\0' } \
}

// -----------------------------------------------
// ����ୠ⨢��� ��᫥����⥫쭮��� ��� ���஢��
#ifndef __BTREXP_H
   typedef struct AltCol
         {
          char  sign;       // �������� (0xAC)
          char  name[8];    // ������쭮� ��� ��᫥����⥫쭮��
          char  tab[256];   // ������ ��४���஢��
         } AltCol;
#endif


// -----------------------------------------------
// ����� ᮧ����� 䠩��
enum
   {
    NotOverFile = -1,       // �� ��१����뢠�� �������騩 䠩�
    OverFile    = 0         // ��१����뢠�� �������騩 䠩�
   };


/********************************* Extend operation support *************/
// -----------------------------------------------
// Set key number parametr
enum
   {
    UseImmed  = -1,  // Use extension immediately
    UseNormal =  0   // Use extension until drive is full
   };


// -----------------------------------------------
// ��� ��஫� ��� ��⠭���� ��������
enum
   {
    SetORW  = 0,  // ����� �������� ��� ��� ०���� ������ 䠩��, ����� �� ��������
    SetOW   = 1,  // ����� �������� ��� ��� ०���� ������ 䠩��, �஬� ReadOnly, ����� �� ��������
    SetORWE = 2,  // ����� �������� ��� ��� ०���� ������ 䠩��, ����� ��������
    SetOWE  = 3   // ����� �������� ��� ��� ०���� ������ 䠩��, �஬� ReadOnly, ����� ��������
   };

/********************************* Unlock operation support *************/
// -----------------------------------------------
// Set key number parametr
enum
   {
    UnlockSingle =  0,  // Unlock single record lock
    UnlockOne    = -1,  // Unlock one record multiply locked
    UnlockAll    = -2   // Unlock all records multiply locked
   };

/********************************* Version operation support ************/
// -----------------------------------------------
typedef struct
      {
       db_int16   version;    // Version number
       db_int16   revision;   // Revision number
       char       netware;    // 'N' for NetWare Btrive
      } VersBuf;


// -----------------------------------------------------------------------------
// ���� �訡�� Pervasive
// � ᯨ᮪ �� ��諨 ���ॢ訥 � ᮢᥬ � ����᪨�

#ifndef __BTREXP_H
    enum Berors
       {
        BEOK          =   0,        // �ᯥ譮
        BEinoper      =   1,        // �����४⭠� ������
        BEioerr       =   2,        // �訡�� �����/�뢮�� (�⥭��/�����)
        BEnoopen      =   3,        // ���� �� �����
        BEnorec       =   4,        // ������ �� ������� (�� ��������� �����)
        BEdupkey      =   5,        // �㡫�஢���� ���� (������ � ⠪�� ���箬 㦥 �������)
        BEinvkey      =   6,        // �����४�� ����� ����
        BEdifkey      =   7,        // ��������� ���� (����� ��᫥����⥫�묨 GET-�����ﬨ)
        BEinpos       =   8,        // �����४⭠� ������ ����� (��। �����ﬨ Update/Delete)
        BEeof         =   9,        // ����� 䠩��
        BEmodkey      =  10,        // ���������� �������塞��� ����
        BEfilenm      =  11,        // �����४⭮� ��� 䠩��
        BEnofile      =  12,        // ���� �� ������
        BEexfile      =  13,        // �訡�� ������ ���७�� 䠩�� (���� .^)
        BEclose       =  17,        // �訡�� ������� 䠩�� (��� � ���㬥��樨 ⠪�� ���)
        BEfulld       =  18,        // ��� ���� �� ��᪥
        BEinactive    =  20,        // ��� ���� �� ��⨢��
        BEsmallkey    =  21,        // ��������� ࠧ��� ���� ����
        BEldata       =  22,        // ��������� ࠧ��� ���� ������
        BEpagesize    =  24,        // �����४�� ࠧ��� ��࠭��� ��� ���� ������
        BEcreate      =  25,        // �訡�� ᮧ����� 䠩��
        BEkeynum      =  26,        // �����४�� ����� ����
        BEkeypos      =  27,        // �����४⭠� ������ ����
        BEreclen      =  28,        // �����४⭠� ����� �����
        BEkeylen      =  29,        // �����४⭠� ����� ����
        BEnobfile     =  30,        // ���� �� ���� ���४�� 䠩��� ����
        BEnoexpand    =  32,        // ���� �� ����� ���� ���७
        BEdirerror    =  35,        // �訡�� ��⠫���
        BEduptrn      =  37,        // ��㣠� �࠭����� 㦥 ��⨢��
        BEnobegtrn    =  39,        // �࠭����� �� �뫠 ����
        BEbadoper     =  41,        // ������ �� ����� ���� �믮�����
        BEnumrec      =  43,        // �����४�� ���� �����
        BEkeypath     =  44,        // �����४�� ���� ����
        BEkeyflag     =  45,        // �����४�� 䫠� ����
        BEfileacc     =  46,        // ��� ����㯠 � 䠩��
        BEmaxop       =  47,        // �ॢ�襭 ����� �� �᫮ ������� 䠩��� (���५)
        BEbadasc      =  48,        // �����४⭮� ��।������ ����ୠ⨢��� ��᫥����⥫쭮�� ���஢��
        BEexkeytype   =  49,        // �����४�� ���७�� ⨯ ����
        BEowner       =  50,        // �������� ��� 䠩�� 㦥 ��⠭�����
        BEbadowner    =  51,        // �����४⭮� ��� ��������
        BEbadvarlen   =  54,        // ����� ��६����� ��� ����� ���०����
        BEautoinc     =  55,        // �����४�� ��ਡ�� ��⮨��६��⭮�� ����
        BEbadindex    =  56,        // ������ ���०���
        BEefile       =  59,        // ���� 㦥 �������
        BEbadrec      =  60,        // ������ �뫠 �믮����� �� ��� ��� ����ᥩ (��� ����権 36 - 39)
        BEworkspace   =  61,        // ��������� ࠧ��� ࠡ�祣� ����࠭�⢠ (��� ����権 36 - 39)
        BEdes         =  62,        // �����४�� ���ਯ��
        BEbatchbuf    =  63,        // �����४�� ���� ������ ��� ����樨 ����⭮� ��⠢�� ����ᥩ
        BEfiltrlimit  =  64,        // ���⨣��� �।�� 䨫��� (��� ����権 36 - 39)
        BEfldoffset   =  65,        // �����४⭮� ᬥ饭�� ����
        BEbadddf      =  67,        // �訡�� ������ ᫮���� SQL (DDF-䠩��)
        BEconcurent =  70,   // �� �����㬥�⠫쭠� �訡�� "������ �����७⭮ ��������"
        BEdeadLock    =  78,        // �����������஢��
        BEconflict    =  80,        // ���䫨�� �� �஢�� ����ᥩ (���� �த� �����७⭮�� ��������� ����ᥩ)
        BEunlocrec    =  81,        // �訡�� ࠧ�����஢�� �����
        BElostpos     =  82,        // ����� ����樨 �����
        BErecloc      =  84,        // ������ ��� ��࠭�� �������஢���
        BEfileloc     =  85,        // ���� �������஢��
        BEtableful    =  86,        // ��९������� ⠡���� 䠩��� (> 65535)
        BEincmode     =  88,        // ��ᮢ���⨬� ०��
        BEserver      =  91,        // �訡�� �ࢥ�
        BElocktype    =  93,        // ��ᮢ���⨬� ⨯� �����஢�� �����
        BEpermis      =  94,        // �訡�� �ࠢ ����㯠
        BEsession     =  95,        // ���� ��ᨨ
        BEenv         =  96,        // �訡�� ��ࠬ��஢ ���㦥���
        BEsmallbuff   =  97,        // ��������� ࠧ��� ���� ������
        BEsmallcache  = 100,        // ��������� ࠧ��� ���
        BEsmallram    = 101,        // �������筮 ����⨢��� �����
        BEsmallstack  = 102,        // ��������� ࠧ��� �⥪�
        BEchunkOffs   = 103,        // ���誮� ����讥 ᬥ饭�� ��� �����
        BElocale      = 104,        // �����४⭠� ������
        BEnextchunk   = 106,        // ���������� ������� ᫥������ ���� �����
        BEsemaphore   = 109,        // �訡�� ᮧ����� ᥬ��� ��� ����㯠 � ����
        BEbusy        = 112,        // ���� �ᯮ������ ��㣨� �����⮬
        BElimkeynum   = 120,        // �ॢ�襭 ����� ���祩 ��� ������� ࠧ��� ��࠭���
        BElimfilesize = 132,        // �ॢ�襭 ����� �� ࠧ��� 䠩��
        BEnoasc       = 136,        // �� ������� ����ୠ⨢��� ��᫥����⥫쭮��� ��� ���஢��
        BEkeynumtrn   = 139,        // �����४�� ����� ����
        BEdupsyskey   = 146,        // �㡫�஢���� ��⥬���� ����
        BEsmalloffset = 151,        // ���饭�� ��� ����� ᫨誮� ����
        BElimlic      = 161,        // �ॢ�襭�� ��業������� ��࠭�祭��

        BEmaxerr,

        // ----- ����७�� �訡��
        BEminexterr   = -18,
        BEconstraint_parent_notfound = BEminexterr, // �ࠡ�⠫ constraint - ORA-02291 �� ���������� \��������� �����, �� ������� த�⥫�᪠� ������
        BEconstraint_child_found     = -17,         // �ࠡ�⠫ constraint - ORA-02292 �� 㤠�����\��������� �����, ������� ���୨� �����
        BEnomoreclnts = -16,                  // There are no more free client slots
        BEillclntid   = -15,                  // Illegal client id
        BEillclnth    = -14,                  // Illegal client handle
        BEinitReq     = -13,                  // Can not init requester (standart or ours)
        BEnodesc      = -12,                  // Description not found
        BEintegrviol  = -11,                  // ��� ����ࠡ�⠭�� �訡�� Oracle
        BEusrbrk      = -10,                  // Break SQL query
        BEinvrlen     = -9,                   // Dictionary record length mismatch
        BEdone        = -8,         // ������ �믮����� �ਪ����� ��ࠡ��稪�� ᠬ����⥫쭮
        BEFilterAbort = -7,                   // User filter will abort the transaction

        BEmaxexterr
       };
#endif

// -----------------------------------------------
typedef struct TBtID
      {
       char           reserv[12];
       char           id1[2];
       unsigned short id2;
      } BTClientID;

// -----------------------------------------------
typedef short  btrvem_h;

// -----------------------------------------------------------------------------
#define  BTRVEM_DEF_HANDLE  ((btrvem_h)0)
#define  BTRVEM_BAD_HANDLE  ((btrvem_h)-1)
#define  BTRVEM_NULL_HANDLE ((btrvem_h)-2)

// -----------------------------------------------------------------------------
typedef unsigned long   bclient_t; // ����� ������ ��� ࠡ��� � ����
typedef short           bclient_h; // ����� ����७���� ������ ��� ࠡ��� � ����

// -----------------------------------------------
#define  BCLIENT_BAD_HANDLE  ((bclient_h)-1)
#define  BCLIENT_DEF_HANDLE  ((bclient_h)0)
#define  BCLIENT_EXT_HANDLE  ((bclient_h)-2)
#define  BCLIENT_BAD_CLIENT  ((bclient_t)0xFFFFFFFEUL)

_BTEXP bclient_h bGetClient(const char *name, int *stat);
_BTEXP int       bReleaseClient(bclient_h client);

_BTEXP bclient_h bSetDefClient(bclient_h client, int *stat);
_BTEXP bclient_h btrvGetClient(const char *name);
_BTEXP bclient_h btrvSetDefClient(bclient_h client);
_BTEXP bclient_t btrvGetDefClient(void);
_BTEXP bclient_t btrvHandle2Id(bclient_h handle);
_BTEXP bclient_h btrvId2Handle(bclient_t id);
_BTEXP int       btrvReleaseClient(bclient_h client);

// Close ALL CLIENTS IMMEDIATELY!
_BTEXP void btrvNuke(void);

_BTEXP void InitBtrClients(void);
_BTEXP void DoneBtrClients(void);

// -----------------------------------------------
#define  NWcall(OP, POS_BLK, DATA_BUF, DATA_LEN, KEY_BUF, KEY_LEN, KEY_NUM) \
               NWcallEx((OP), (POS_BLK), (DATA_BUF), (DATA_LEN), (KEY_BUF), (KEY_LEN), (KEY_NUM), 0)

// -----------------------------------------------------------------------------
// ����饭��� �㭪�� ��� ࠡ��� � ���� Pervasive (�࠭���樮��� ०��)
// [in]     OP        - ����� ����樨
// [in]     POS_BLK   - ���� ���ᠭ�� dbt-䠩��
// [in]     DATA_BUF  - ���� ������
// [in,out] DATA_LEN  - ������ ���� ������
// [in]     KEY_BUF   - ���� ����
// [in]     KEY_LEN   - ������ ���� ����
// [in]     KEY_NUM   - ����� ����
// [in]     CLIENT_ID - ���ਯ�� ����७���� ������
// [ret]    �����頥� ����� ����樨 (0 - �ᯥ�, � ��⠫��� ����� ��� �訡��)
_BTEXP int NWcallEx(int OP, void *POS_BLK, void *DATA_BUF, int *DATA_LEN, void *KEY_BUF, int KEY_LEN,
                    signed char KEY_NUM, bclient_h CLIENT_ID);

// -----------------------------------------------------------------------------
// ������ NWcallEx, �� � ������������ ࠡ��� ��� ������� ���譨� �����⮬.
// ��� �⮣� � ����⢥ CLIENT_ID �㦭� ��।��� ���祭�� BCLIENT_EXT_HANDLE.
// � ��⨢��� ��砥 ��ࠬ��� BtrID �㤥� �ந����஢��.
_BTEXP int NWcallExClntID(int OP, void *POS_BLK, void *DATA_BUF, int *DATA_LEN, void *KEY_BUF,
                          int KEY_LEN, signed char KEY_NUM, bclient_h CLIENT_ID, BTClientID *BtrID);

// -----------------------------------------------------------------------------
// �����⥫� �� �ਪ������ �㭪��-��ࠡ��稪 ����権 � ����.
// ��ࠬ���� �㭪樨 ��������� ��������� NWcallExClntID.
// ��ࠡ��稪 ��뢠���� �����।�⢥��� ��। ���饭��� � ����.
// ������ ��ࠡ�⪨ ���祭��, �����頥��� ��ࠡ��稪��:
//      BEOK   - �믮����� �ॡ㥬�� ������ � ����
//      BEdone - ������ �믮����� ��ࠡ��稪�� ᠬ����⥫쭮, ���饭�� � ���� �� �믮������
//      ��⠫�� �����頥�� ���祭�� �ࠪ������ ��� �訡��, ���饭�� � ���� �� �믮������.
typedef int (*BtrUserFilterType)(int OP, void *POS_BLK, void *DATA_BUF, int *DATA_LEN, void *KEY_BUF,
                                 int KEY_LEN, signed char KEY_NUM, BTClientID *clntId);

// -----------------------------------------------------------------------------
// �����⥫� �� �ਪ������ �㭪��-��ࠡ��稪 ����ᮢ ����権 � ����.
// ��ࠬ���� �㭪樨 ��������� ��������� NWcallExClntID �� �᪫�祭��� stat, �����
// ᮤ�ন� ����� ����樨 � ����.
// ��ࠡ��稪 ��뢠���� �����।�⢥��� ��᫥ ���饭�� � ����.
// ���祭��, �����頥�� ��ࠡ��稪��, �� �ᯮ�������.
// �ਬ�砭��: �᫨ �� ��⠭����� ��ࠡ��稪 BtrUserFilterType � �� ���� �����,
//             �⫨�� �� BEOK, ��ࠡ��稪 ����ᮢ ����樨 �맢�� �� �㤥�.
typedef int (*BtrUserFilterStat)(int stat, int OP, void *POS_BLK, void *DATA_BUF, int *DATA_LEN, void *KEY_BUF,
                                 int KEY_LEN, signed char KEY_NUM, BTClientID *clntId);


// -----------------------------------------------------------------------------
// ��⠭����� ���짮��⥫�᪨� ��ࠡ��稪 ����権 � ����.
// �����頥� 㪠��⥫� �� �।�騩 ��ࠡ��稪.
BtrUserFilterType BtrSetUserCallBack(BtrUserFilterType newFilter);

// -----------------------------------------------------------------------------
// ��⠭����� ���짮��⥫�᪨� ��ࠡ��稪 ����ᮢ ����権 � ����.
// �����頥� 㪠��⥫� �� �।�騩 ��ࠡ��稪.
BtrUserFilterStat BtrSetUserCallBackStat(BtrUserFilterStat newFilter);

// Btrieve error message encription
_BTEXP char *BErrorText(int ErrorCode);
_BTEXP char *BErrorText2(int errorCode, int sql);


// -----------------------------------------------------------------------------
// �����প� ����権 � ���ﬨ ����� (��६���� ���, BLOB)

// -----------------------------------------------
struct tagTChunkDes
     {
      unsigned long   ChunkOffset;
      unsigned long   ChunkLen;
      void RSL_FAR   *UserData;
     };

// -----------------------------------------------
typedef struct tagTChunkDes  TChunkDes;

// -----------------------------------------------
typedef struct
      {
       unsigned long  Subfunction;
       unsigned long  NumChunks;
      } TChunkHeader;

// -----------------------------------------------
typedef struct
      {
       TChunkHeader   head;
       TChunkDes      des[1];
      } TChunk;

// -----------------------------------------------
typedef struct
      {
       unsigned long  addr;
       TChunkHeader   head;
       TChunkDes      des[1];
      } TReadChunk;

// -----------------------------------------------
typedef struct
      {
       TChunkHeader   head;
       TChunkDes      des[1];
      } TWriteChunk;

// -----------------------------------------------
typedef struct
      {
       unsigned long  Subfunction;
       unsigned long  ChunkOffset;
      } TTruncateChunk;


// -----------------------------------------------------------------------------
#define  BT_DIRECT_RANDOM    0x80000000L
#define  BT_INDIRECT_RANDOM  0x80000001L

#define  BT_DIRECT_RECT      0x80000002L
#define  BT_INDIRECT_RECT    0x80000003L

#define  BT_TRUNCATE         0x80000004L

#define  BT_NEXT_BIAS        0x40000000L
#define  BT_APPEND_BIAS      0x20000000L

#define  BT_INDIRECT_BIT     0x00000001L

// -----------------------------------------------------------------------------

int getFullClientID(BTClientID *id, bclient_h client);

#ifdef __cplusplus
}
#endif

#include <packpop.h>

// -----------------------------------------------
#define  ErrorText(ErrorCode)  BErrorText(ErrorCode)

#endif  /* __NWBTRV_H */

/* <-- EOF --> */