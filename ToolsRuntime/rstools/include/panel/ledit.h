//-*--------------------------------------------------------------------------*-
//
// File Name   : ledit.h
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// December 29, 1992
//-*--------------------------------------------------------------------------*-

// �������� �����

#ifndef __LEDIT_H
#define __LEDIT_H

#define LEDIT

#include "desktop.h"
#include "marklist.h"

/************************************************************************/
/*         ����� �������������� ���������         */
#ifndef IsScrlMod
    #define  IsScrlMod(mn)   ((mn)->mode & 0x10) /* ����������� ����� ���.*/
    #define  SetScrlNo(mn)   (mn)->mode &= 0xef
    #define  SetScrlMod(mn)  (mn)->mode |= (unsigned char)0x10
#endif

/************************************************************************/
/*         ����� ������� ������             */
#ifndef IsInsMod
    #define  IsInsMod(mn)   ((mn)->mode & 0x40)
    #define  SetInsNo(mn)   (mn)->mode &= 0xbf
    #define  SetInsMod(mn)  (mn)->mode |= (unsigned char)0x40
#endif


// ������ �� bedit.h
#define  LIsAllowEmptyMod(L)    ((L)->mode2 & 0x02)               // ���������� ����� ���������� ��� �������
#define  LSetAllowEmptyNo(L)    (L)->mode2 &= 0xfd                // ����. ����� ���������� ��� �������
#define  LSetAllowEmptyMod(L)   (L)->mode2 |= (unsigned char)0x02 // ���. ����� ���������� ��� �������


#define  Internal_1  0x20 // ���� ��������������� � mode. ������������ ������ �
                          // ����������� ��� ���������� �������� � ��������� ��
                          // ������ �����. ��������������� ���
                          // ������ �� ������ �����. ���������
                          // ����� �������� � ���������� ���������� ENTFIELD

#define  Internal_2  0x80 // ���� ��������������� � mode. ������������ ������ �
                          // ����������� ��� ���������� �������� � ��������� ��
                          // ������ ��������������

#ifdef __cplusplus
extern "C" {
#endif

// ���������� ��������� ���������
_RSPNLEXP extern void setLMES(
                              const char *mes    // ���������
                             );

/* ���� �������� �� ��������� �������. ������   */
enum LRETINP
   {
    INPNO = 0,   /* ��������� ���� ������         */
    INPYES,      /* ��������� ���� ������         */
    INPTOP,      /* ��������� ���� ������ � ������ ���������  */
    INPBOT       /* ��������� ���� ������ � ����� ���������   */
   };

struct LSCROL;

#ifdef __cplusplus
    typedef int (*lscrol_makerec_t)(LSCROL *);

    // ��������� ��������� �� �������� � �������� �������
    typedef int (*lscrol_testrec_t)(LSCROL *);
#else
    typedef int (*lscrol_makerec_t)();

    // ��������� ��������� �� �������� � �������� �������
    typedef int (*lscrol_testrec_t)();
#endif

typedef lscrol_makerec_t  lscrol_initrec_t;

// ��������� ��������� ����� ���������
typedef struct LSCROL
      {
       SCROL             m;          // ��������� ���������
       int               LLine;      // ����� ������ ������
       int               MaxL;       // ������������ ���������� �����
       PANEL*            Mpi;        // ������ ����� ������
       PANEL*            Mpe;        // ������ �������������� ������
       lscrol_initrec_t  n;          // ��������� ������������� ������
       lscrol_testrec_t  i;          // ��������� �������� ��� ����� ������
       lscrol_testrec_t  c;          // ��������� �������� ��� ��������� ������
       lscrol_testrec_t  d;          // ��������� �������� ��� �������� ������
       unsigned char     mode;       // ������ �������������� ���������
       void*             saveL;      // ������ �� ����� ���������� ������
       unsigned char     mode2;      // ������ ��������� (������� �� ����������� � mode)
       MARKLIST         *MList;      // ������ ���������� �������
      } LSCROL;

/************************ ����������� ������� ***************************/

/* ��������� ������� ������ � ������ ���������� */
_RSPNLEXP void lsaveb(
                      LSCROL *L    /* ��������� ���������           */
                     );

/* ������������ ������� ������ �� ������ ������.*/
_RSPNLEXP void lrestb(
                      LSCROL *L    /* ��������� ���������           */
                     );

/* �������� ������� ������ � ������� ���������� */
_RSPNLEXP int lcmpb(
                    LSCROL *L    /* ��������� ���������           */
                   );

/*********************** �������������� ������ ***************************/
/* ����������� ��������� ����� ������     */
_RSPNLEXP int inLine(
                     PANEL  *p,     /* ��������� ������ �����        */
                     int     key    /* ��� �������               */
                    );

/* ����������� ��������� �������������� ������  */
_RSPNLEXP int edline(
                     PANEL  *p,     /* ��������� ������ ��������������     */
                     int     key    /* ��� �������               */
                    );

/* ����������� ��������� ��������� �����  */
_RSPNLEXP int scrline(
                      PANEL *p,     /* ��������� ������ ���������       */
                      int    key    /* ��� �������               */
                     );

// ����������� ��������� ��������� ����� ��� SCROL
// ��������� ������
_RSPNLEXP int scrolline(
                        PANEL *p,     /* ��������� ������ ���������       */
                        int    key    /* ��� �������               */
                       );

/* ������� ������� ��������� �����     */
_RSPNLEXP int LPutScrol(
                        LSCROL  *L      /* ��������� ��������� �����        */
                       );

/* ������� ������� ��������� �����     */
_RSPNLEXP void LRemScrol(
                         LSCROL  *L      /* ��������� ��������� �����        */
                        );

/* ������������ �������� ��� ��������������  */
_RSPNLEXP int LLoopScrol(
                         LSCROL *L    /* ��������� ��������� �����        */
                        );

/* �������� �����             */
_RSPNLEXP int LEditScrol(
                         LSCROL *L    /* ��������� ��������� �����        */
                        );

// �������� ������� ���������
_RSPNLEXP int LRunScrol(LSCROL *L);

/****************************** ����� ***********************************/
/* ����� ������ ��������� �����        */
/*    0 - �������          */
/* != 0 - �� �������          */
_RSPNLEXP int find_line(
                        LSCROL *L,      /* ��������� ���������           */
                        void   *value,  /* �������� ������            */
                        int     (*ic)(void *buff, void *value) /* ��������� �������� ������   */
                       );

/* ����� ������ � ���������         */
/*    0 - �������          */
/* != 0 - �� �������          */
_RSPNLEXP int find_sline(
                         LSCROL *L,      /* ��������� ���������           */
                         void   *value,  /* �������� ������            */
                         int     (*ic)(void *buff, void *value) /* ��������� �������� ������   */
                        );

/* ����� ������ � ���������            */
/*    0 - �������                      */
/* != 0 - �� �������                   */
_RSPNLEXP int pick_sline(
                         LSCROL *L,                /* ��������� ���������                 */
                         FIELD  *f,                /* ���� ������                         */
                         const char *Invite,       /* ����������� � ����� �������� ������ */
                         const char *Oops          /* ��������� ��� ��������� ������      */
                        );

// ����� ������ � SCROL
int pick_scrline
                (
                 SCROL  *M,             /* ��������� ���������                 */
                 FIELD  *f,             /* ���� ������                         */
                 const char *Invite,    /* ����������� � ����� �������� ������ */
                 const char *Oops       /* ��������� ��� ��������� ������      */
                );

/************************ �������������� ������� ************************/
/* �������� ������ ������ � ������     */
/* �������� ��������� �����         */
_RSPNLEXP int arr_to_list(
                          LSCROL  *L,        /* ��������� ��������� �����        */
                          void    *Arr,      /* ������ ������           */
                          int     *CurrNum   /* ������� ���������� �����         */
                         );

/* �������� ������ �������� ��������� ����� �   */
/* ������ ������           */
_RSPNLEXP int list_to_arr(
                          LSCROL  *L,        /* ��������� ��������� �����        */
                          void    *Arr,      /* ������ ������           */
                          int     *CurrNum   /* ������� ���������� �����         */
                         );

/* ���������� ������,������� ��� ������ ��������*/
/* ��������� �����            */
_RSPNLEXP void free_line(
                         LSCROL  *L      /* ��������� ��������� �����        */
                        );

/* �������� ������� ������       */
_RSPNLEXP int LEdit_Arr(
                        LSCROL  *L,        /* ��������� ��������� �����        */
                        void    *Arr,      /* ������ ������           */
                        int     *CurrNum   /* ������� ���������� ������ �������      */
                       );

// ����������� LSCROL.
_RSPNLEXP LSCROL *CrLs(
                       ResFile    *rt,     // �������� ������.
                       const char *member  // ��� �������.
                      );

/************************** ����� ����� *********************************/

_RSPNLEXP void LSetFlds(
                        LSCROL   *l,
                        panel_keyproc_t  proc,  // ���������� ����������
                        void     *Parm,        // ��� �������� ����������
                        void     *Pstatus,     // ���������� ��� ������ ���������
                        char     *Phead,       // ���������
                        panel_recproc_t  rec,  // ��������� ������������� �������
                        void     *Ms,          // ������ �������
                        int       Mnumfl,      // ���������� �������
                        int       LLine,       // ����� ������ ������
                        int       MaxL,        // ������������ ���������� �����
                        PANEL    *Mpi,         // ������ ����� ������
                        PANEL    *Mpe,         // ������ �������������� ������
                        lscrol_initrec_t  n,   // ��������� ������������� ������
                        lscrol_testrec_t  i,   // ��������� �������� ��� ����� ������
                        lscrol_testrec_t  c,   // ��������� �������� ��� ��������� ������
                        lscrol_testrec_t  d    // ��������� �������� ��� �������� ������
                       );


/* ������� ������������ ������, ���������� ��� ���������� */

_RSPNLEXP void  FreeLScrol      (LSCROL *ls);
_RSPNLEXP void  LsSetMultiSelect(LSCROL *Ls);
_RSPNLEXP void *LsFirstSelected (LSCROL *Ls);
_RSPNLEXP void *LsNextSelected  (LSCROL *Ls, void *item);
_RSPNLEXP bool  LsIsRecSelected (LSCROL *Ls, int n);
_RSPNLEXP int   LsGetNumSelected(LSCROL *Ls);

#ifdef __cplusplus
}
#endif

#endif // __LEDIT_H

/* <-- EOF --> */