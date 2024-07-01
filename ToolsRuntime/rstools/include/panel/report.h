//***********************************************************************
//             ������࠭�� ⥪�⮢� ����䥩� ��� Borland C.
//          �����প� ���⮢.
//           VECsoft 31.08.92
//
// VERSION INFO :   $Archive: /RS-Tools/Old_tools/panel/h/report.h $
//                  $Revision: 9 $
//                  $Date: 18.02.00 13:35 $
//                  $Author: Lebedev $
//                  $Modtime: 17.02.00 15:14 $
//
//***********************************************************************

#ifndef __REPORT_H
#define __REPORT_H

#include <stdio.h>

#include "menu.h"
#include "rstypes.h"

typedef unsigned char  r_coord_rpt;

_RSPNLEXP extern FILE *prnfile;           /* ���� ��� ����       */

#define  setpfile(prf) prnfile = prf; /* ��⠭����� 䠩� ��� ����   */

#define  HEADON   1   /* ���ﭨ� ��ப� : ������ ���������  */
#define  HEADOFF  2   /* ���ﭨ� ��ப� : �� ������ ���������  */

#include <packpshn.h>

/* ����⥫� ���� ����        */
typedef struct
      {
       unsigned char  RVt;        /* ��� ���祭��              */
       void          *RValue;     /* ���祭�� � ����            */
       unsigned int   RVp;        /* ������ ��᫥ �窨,�ਧ��� wrap ��� ��ப   */
       coord          x, y, l, h; /* ���न���� ����           */
       char         **wlist;      /* ���᮪ ��� wrap           */
      } RFIELD;

/* ��ப� ����        */
typedef struct
      {
       RFIELD         *RFList;  /* ���ᨢ ����� ��ப� ����    */
       unsigned char   RFnum;   /* ������⢮ ����� ��ப� ����   */
       unsigned char   Lspace;  /* �ਧ��� �ய�᪠ ��ப�    */
       unsigned char   Lcentr;  /* �ਧ��� 業��஢����      */
       coord           h;       /* ������⢮ ����� � ��ப�     */
       int             stat;    /* ���ﭨ� �뢮����� ��ப�    */
       int             ext;     /* ����� �������⥫쭮� ����� ��ப�   */
      } RLINE;

#define  NR(r)  sizeof(r)/sizeof(RFIELD)

/* ����⥫� ����        */
typedef struct
      {
       int      wide;     /* ��ਭ� ����        */
       int      pagesize; /* ����� ��࠭���       */
       RLINE   *titul;    /* ���������            */
       int      Page;     /* ����� ��࠭���       */
       int      Line;     /* ����� ����� �� ��࠭��    */
       char    *buff;     /* ����          */
       char    *wsave;    /* ������� ��࠭���� �����    */
       FILE    *fr;       // ���� ����
       int      nl;       // ��᫮ ��ப ����.
       RLINE  **rl;       // ��ப� ����.
      } REPRT;

#include <packpop.h>

#ifdef __cplusplus
extern "C" {
#endif

// �뢥�� ���� � 䠩� ����.
_RSPNLEXP int PfB(
                  FILE *f,                      // ����.
                  char *buff                    // ����.
                 );

/* ������� ���� ��� ����     */
/* �����頥� 㪠��⥫� �� ����        */
_RSPNLEXP char *makeB(
                      int lenb                       /* ����� ��ப�            */
                     );

/* �뢮� ��ப� � ��祭��� � ����    */
_RSPNLEXP int toB(
                  char *buff,     /* ����               */
                  int   offs,     /* ���饭�� � ����            */
                  char *stro,     /* ��ப� ��� �뢮��          */
                  int   outlen    /* ����� ���� ��� �뢮��         */
                 );

/* �뢥�� ���祭�� ���� � ����     */
_RSPNLEXP void FtoB(
                    char *buff,       /* ����               */
                    int   offs,       /* ���饭�� � ����            */
                    void *val,        /* ���祭��             */
                    int   lf,         /* ��ਭ� ����                                */
                    unsigned int vp,  /* ������ ��᫥ �窨           */
                    unsigned int tp   /* ��� ���祭��              */
                   );

/* �����஢��� ����            */
_RSPNLEXP void centrB(
                      char *b     /* ����          */
                     );

/* ����� ����� ��࠭���         */
_RSPNLEXP int newpage(
                      REPRT *r    /* ����⥫� ����          */
                     );

/* �뢥�� ��ப� ����         */
_RSPNLEXP int Prline(
                     REPRT *r,    /* ����⥫� ����          */
                     RLINE *rl    /* ����⥫� ��ப�           */
                    );

/* ��⠭����� ����� �뢮�� ��ப�     */
_RSPNLEXP void SetPrnStat(
                          RLINE *rl,      /* ����⥫� ��ப�           */
                          int   stat      /* ����� ��ப�           */
                         );

/* ��।����� ����� �뢮�� ��ப�     */
_RSPNLEXP int IsPrnStat(
                        RLINE *rl    /* ����⥫� ��ப�           */
                       );

/* �뢥�� ��ப� ����         */
_RSPNLEXP int Prlines(
                      REPRT  *r,              /* ����⥫� ����          */
                      RLINE **ln,     /* ���᮪ ����⥫�� ��ப       */
                      int     Numln     /* ������⢮ ����⥫�� ��ப         */
                     );

/* �뢥�� ��ப� ���� ��⮪��    */
_RSPNLEXP int PrlineFlow(
                         REPRT *r,       /* ����⥫� ����          */
                         RLINE *ln       /* ���᮪ ����⥫�� ��ப       */
                        );

/* ����� ����            */
_RSPNLEXP int brep(REPRT *r,
                   FILE  *f,
                   int    wd,         /* ��ਭ� ����        */
                   int    ps,         /* ����� ��࠭���       */
                   RLINE *tt,         /* ���������            */
                   int    np       /* ������ ���� � ����஬ ��࠭���   */
                  );

/* ����� ����            */
_RSPNLEXP REPRT *begrep(
                        int    wd,      /* ��ਭ� ����        */
                        int    ps,      /* ����� ��࠭���       */
                        RLINE *tt,      /* ���������            */
                        int    np       /* ������ ���� � ����஬ ��࠭���   */
                       );

/* ������ ����        */
_RSPNLEXP void endrep(
                      REPRT *r        /* ����⥫� ����        */
                     );

#include <packpsh1.h>

typedef struct
      {
       uchar        RFnum, Lspace, Lcentr;
       r_coord_rpt  h;
       db_int16     len;
      } RlineR;

typedef struct
      {
       uchar        RVt;
       db_uint16    RVp;
       r_coord_rpt  x, y, l, h, vfl, lens;
      } RfieldR;

typedef struct
      {
       db_int16  wd, ps, np, nline;
      } ReprtR;

#include <packpop.h>

// ��������� RLINE.
_RSPNLEXP RLINE * CrRline(
                          uchar RFnum,   // ������⢮ ����� ��ப� ����.
                          uchar Lspace,  // �ਧ��� �ய�᪠ ��ப�.
                          uchar Lcentr,  // �ਧ��� 業��஢����.
                          coord h        // ������⢮ ����� � ��ப�.
                         );

// ��������� RFIELD.
_RSPNLEXP void InsRfield(
                         RLINE *rl,
                         uchar  RVt,     // ��� ���祭��.
                         unsigned int RVp,     // ������ ��᫥ �窨,�ਧ��� wrap ��� ��ப.
                         coord  x,       // �⭮�⥫�� ���न���� ����.
                         coord  y,
                         coord  l,
                         coord  h
                        );

// ����� ��������� RFIELD.
_RSPNLEXP void FInsRfield(
                          RLINE *rl,
                          uchar  RVt,        // ��� ���祭��.
                          void  *RValue,     // ���祭�� � ����.
                          unsigned int  RVp, // ������ ��᫥ �窨,�ਧ��� wrap ��� ��ப.
                          coord  x,          // �⭮�⥫�� ���न���� ����.
                          coord  y,
                          coord  l,
                          coord  h
                         );

// ���뢠��� RLINE.
_RSPNLEXP RLINE *RdRline(
                         ResFile *rt,    // ������ �����.
                         char    *member // ��� ࠧ����.
                        );

#define  DelRline(rl)  free(rl)  // �������� RLINE.

// ������ ���ᮢ ��६����� � ���� ����.
_RSPNLEXP void SetRFlds(RLINE *rl, ...);

// ����� ��������� RFIELD.
_RSPNLEXP void FInsRfield(
                          RLINE *rl,
                          uchar  RVt,        // ��� ���祭��.
                          void  *RValue,     // ���祭�� � ����.
                          unsigned int  RVp, // ������ ��᫥ �窨,�ਧ��� wrap ��� ��ப.
                          coord  x,          // �⭮�⥫�� ���न���� ����.
                          coord  y,
                          coord  l,
                          coord  h
                         );

// ��������� REPRT.
_RSPNLEXP REPRT *CrRep(
                       ResFile    *rt,       // ������ �����.
                       const char *member,   // ��� ࠧ����.
                       FILE       *fr        // ���� ����.
                      );

_RSPNLEXP void DelRep(REPRT *r);

// ��������� REPRT.
_RSPNLEXP REPRT *CrRepF(
                        ResFile   *rt,       // ������ �����.
                        const char *member,  // ��� ࠧ����.
                        const char *fn       // ���� ����.
                       );

_RSPNLEXP void DelRepF(REPRT *r);

#ifdef __cplusplus
}
#endif

#endif // __REPORT_H

/* <-- EOF --> */