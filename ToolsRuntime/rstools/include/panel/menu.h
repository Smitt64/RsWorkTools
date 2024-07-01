//-*--------------------------------------------------------------------------*-
//
// File Name   : menu.h
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
//   November 18, 1992 Emil Dovidovich  - Create file
//-*--------------------------------------------------------------------------*-

//
// ������������� ��������� ��������� ��� Borland C.
//

#ifndef __MENU_H
#define __MENU_H

#define  MENUH

#include <stdarg.h>

#include "fs.h"
#include "res.h"
#include "pexpdef.h"
#include "rstring.h"
#include "bdate.h"

#ifdef RSL_PL_FLAT
    #define  MNFAR
#else
    #define  MNFAR  _far
#endif

#include "fieldtp.h"
#include "platform.h"
#include "rstypes.h"
#include "rslmenu.h"

#include <packpshn.h>


#ifndef USE_HANDLE32
    #if defined(RSL_PL_WIN64) || defined(__EWEXTT_BUILD)
        #define  USE_HANDLE32
    #endif
#endif


#define  ON           1
#define  OFF          0
#define  YES          1
#define  NO           0

#define  RslLeftTop   0
#define  RslLeftBot   1
#define  RslRightTop  2
#define  RslRightBot  3

#define  NBAR          0x0000U          // ��� �����
#define  SBAR          0x0200U          // ��������� �����
#define  DBAR          0x0100U          // ������� �����
#define  TBARTC        0x7E00U          // ��� TabControl'�
#define  TBARTCP       0x8E00U          // �������� TabControl'�
#define  TBAR          0xFE00U
#define  HBAR          0xFF00U

#define  PNL_BAR_TAG   0xFF

/*******************************************************************************
            Structure initializer macros

 DO NOT MAKE STATIC INITIALIZATION OF THAT STRUCTURES DIRECTLY!
 Use these macros instead
*******************************************************************************/
#define  PNL_TEXT_SINIT(style, value, x, y) { (style), (value), (x), (y) }

/**************************************************************************/

// -----------------------------------------------
// ��������� ��� FIELD::FFlags
#define  RF_REQUIRED     0x00000004
#define  RF_NEGATIVE     0x00000008

#define  RF_ELIPSBTN     0x00001000  // ���� � ������� "..." (����� �����������)
#define  RF_DOWNBTN      0x00002000  // ���� � ������� "" (����� �� ������)
#define  RF_EMPTY        0x00004000
#define  RF_TRIMZEROS    0x00008000

#define  RF_3STATE       0x00000100  // ������� CheckBox'� � ����� ����������� (��������� � RF_CHECKBOX)

#define  RF_GROUP        0x00010000
#define  RF_CHECKBOX     0x00020000
#define  RF_RADIO        0x00040000
#define  RF_PASSWORD     0x00080000

#define  RF_PROGRESS     0x00100000
#define  RF_TREEITEM     0x00200000
#define  RF_DYNLEN       0x00400000
#define  RF_INDIC        0x00800000

#define  RF_ASTEXT       0x01000000
#define  RF_NOEDGE       0x02000000
#define  RF_BLOCKHIDE    0x04000000  // ��������� �������� ������� � ���������������
#define  RF_FLAT         0x08000000

#define  RF_GROUPING     0x10000000
#define  RF_NOTABSTOP    0x20000000
#define  RF_NOCHECK      0x40000000
#define  RF_ANSI_STRING  0x80000000

// -----------------------------------------------
// ��������� ��� FIELD::Ftype
#define  TYPEF            0x0F
#define  DUMMF            0x10
#define  HIDEF            0x20   // ���� �������� �������������

#define  DISABLE          0x01   // ����� ���� ����������
#define  HIDDEN           0x02   // ����� ���� �������
#define  DISINEMPTYSCROL  0x04   // ����� ���� ���������� � ������ ����������

#define  DISABLE_INT      0x08   // ����� ���� ���������� (������������ ������ � �����������)
#define  BIGCMDNUMBER1    0x10   // ���� ���������� ���� ��� ������� ���� ����� MAX_SHORT.  x = Cmd - MAX_SHORT
#define  BIGCMDNUMBER2    0x20   // ���� ���������� ���� ��� ������� ���� ����� MAX_USHORT. x = Cmd - MAX_USHORT

#define  HIDEINEMPTYSCROL 0x40   // ����� ���� ������� � ������ ����������
#define  HIDE_INT         0x80   // ����� ���� ������� (������������ ������ � �����������)


#define  isActF(t)         (!( (t) & DUMMF))
#define  setDF(t)          ((t) | DUMMF)
#define  typeF(t)          ((t) & TYPEF)

#define  isHideF(t)        ((t) & HIDEF)
#define  setHideF(t)       ((t) |= HIDEF)
#define  unsetHideF(t)     ((t) &= ~HIDEF)

#define  isDisableF(t)     ((t) & DUMMF)
#define  setDisableF(t)    ((t) |= DUMMF)
#define  unsetDisableF(t)  ((t) &= ~DUMMF)


#define  flagsF(t)         ((t) & ADDFLGF) // Added by (JNS), 12/02/1996

#define  FET   0x01      /* ������������� ����                           */
#define  FBT   0x02      /* ������                                       */
#define  FBS   0x03      /* ������ � �����                               */
#define  FWR   0x04      /* ������������� ���� �������������             */
#define  FVT   0x05      /* ��������������� ����                         */
#define  FSP   0x06      /* Subpanel                                     */
#define  FCL   0x07      /* Cluster (Check box or Radio button           */
#define  FVW   0x08      /* ������������� ����������������               */

#define  FHP   0x0F      /* ������ ������                                */
#define  FDM   0x11

#define  D_MENUPANELNAME    "$MENU$"     // ������������� ������, ���������� ������� ����

#define  D_MENUBTRLBRNAME   "itemsyst.dbt"   // ������������� ��� ���������� �������� ��� ��������
#define  D_MENUBTRRESNAME   "OPERMENU"       // ������������� ��� ������� ��������


#define  D_HELPCONTEXTLEN      255       // ����� ������ ��������� ������ (� ������ 0-�������)
#define  D_HELPCONTEXT      "_CONTEXT_"  // ��������
#define  D_HELPCONTEXTFILE  "\\file:\\"  // ����� ��� ������� ����� ������

#include "strtobd.h"

typedef struct tagTEXT      TEXT;
typedef struct tagBAR       BAR;
typedef struct tagFIELD     FIELD;
typedef struct tagFIELD_EX  FIELD_EX;
typedef struct tagPANEL     PANEL;
typedef struct tagPSAVER    PSAVER;
typedef struct tagFDATA     FDATA;
typedef struct tagSCROL     SCROL;

typedef  int  (*JKeyProc)(PANEL *p, int key);  // ���������� ���������� ������
typedef  void (*JRecProc)(SCROL *mn);          // ���������� ������ � ���������

typedef JKeyProc  panel_keyproc_t;
typedef JRecProc  panel_recproc_t;


// -----------------------------------------------------------------------------
// ���������� ����� �������� � ����� ���������� ���� (TEXT::St)
// ������ ���� St ���������� 4 �����, 2 ������� ��������� �� ������������ �����������.
// ������ ������� 2 ���� ����������� ����:
//
// ======= ������� ���� �������� �����
// 31
// 30  ����� ����� (dBACK_COLOR_) - ��� EW-������
// 29
// 28
// ----
// 27
// 26  ����� ����� (dTEXT_COLOR_) - ��� EW-������
// 25
// 24
// ======= ������� ���� �������� �����
// 23
// 22  ������
// ----
// 21
// 20
// 19  �������������� (dTEXT_FORMAT_)
// 18
// ----
// 17  ������������ ���������� ���� (dTEXT_ALIGN_)
// 16
// ======= ������� ���� �������� �����
// 15  ������������ ���������� ����� � ��� �����
// ...
//  0


// -----------------------------------------------------------------------------
// ��������� ������������ ��� TEXT. ��������� � ������� ����� �������� ����� ���� St
#define  dTEXT_ALIGN_LEFT          0x00
#define  dTEXT_ALIGN_RIGHT         0x01
#define  dTEXT_ALIGN_CENTER        0x02

// ��������� �������������� ��� TEXT. ��������� � ������� ����� �������� ����� ���� St
#define  dTEXT_FORMAT_NORMAL       0x00
#define  dTEXT_FORMAT_BOLD         0x01
#define  dTEXT_FORMAT_ITALIC       0x02
#define  dTEXT_FORMAT_UNDERLINE    0x04
#define  dTEXT_FORMAT_STRIKEOUT    0x08

// ��������� ����� ��� TEXT (������� CGA/WEB)
#define  dTEXT_STYLE_BLACK            0       //
#define  dTEXT_STYLE_BLUE             1       //
#define  dTEXT_STYLE_GREEN            2       //
#define  dTEXT_STYLE_CYAN             3       //
#define  dTEXT_STYLE_RED              4       //
#define  dTEXT_STYLE_MAGENTA          5       //
#define  dTEXT_STYLE_BROWN            6       //
#define  dTEXT_STYLE_LTGRAY           7       //
#define  dTEXT_STYLE_DKGRAY           8       //
#define  dTEXT_STYLE_LTBLUE           9       //
#define  dTEXT_STYLE_LTGREEN         10       //
#define  dTEXT_STYLE_LTCYAN          11       //
#define  dTEXT_STYLE_LTRED           12       //
#define  dTEXT_STYLE_LTMAGENTA       13       //
#define  dTEXT_STYLE_YELLOW          14       //
#define  dTEXT_STYLE_WHITE           15       //

#define  dTEXT_STYLE_DEFAULT         99       // ���� ��-��������� (�� �� ���� ������� ��������������)

// ������������ ����� ������ � �������
#define  dTEXT_MAX_COLOR             16

// ��������� ����� ��� BACK
#define  dBACK_STYLE_WHITE            0       //
#define  dBACK_STYLE_BLUE             1       //
#define  dBACK_STYLE_GREEN            2       //
#define  dBACK_STYLE_CYAN             3       //
#define  dBACK_STYLE_RED              4       //
#define  dBACK_STYLE_MAGENTA          5       //
#define  dBACK_STYLE_BROWN            6       //
#define  dBACK_STYLE_LTGRAY           7       //
#define  dBACK_STYLE_DKGRAY           8       //
#define  dBACK_STYLE_LTBLUE           9       //
#define  dBACK_STYLE_LTGREEN         10       //
#define  dBACK_STYLE_LTCYAN          11       //
#define  dBACK_STYLE_LTRED           12       //
#define  dBACK_STYLE_LTMAGENTA       13       //
#define  dBACK_STYLE_YELLOW          14       //
#define  dBACK_STYLE_BLACK           15       //

#define  dBACK_STYLE_DEFAULT         99       // ��� ��-��������� (�� �� ���� ������� ��������������)

// ������������ ����� ������ � �������
#define  dBACK_MAX_COLOR             16

// -----------------------------------------------------------------------------
// ������� ��� ����������������������� �������������
#define  dTEXT_ALIGN_MASK    0x00030000         // ����� ��� ��������� ������������ �� �����
#define  dTEXT_FORMAT_MASK   0x003C0000         // ����� ��� ��������� ������� �� �����
#define  dTEXT_COLOR_MASK    0x0F000000         // ����� ��� ��������� ����� �� �����
#define  dBACK_COLOR_MASK    0xF0000000         // ����� ��� ��������� ���� �� �����

#define  dTEXT_STYLE_MASK    0x0000FFFF         // ����� ��� ��������� �����


// -----------------------------------------------------------------------------
// ���������� �������

// �������� ������������ �� �����
#define  GetAlignSt(St)             (((St) & dTEXT_ALIGN_MASK) >> 16)
#define  GetTextAlignSt(St)         GetAlignSt((St))

// �������� ������ �� �����
#define  GetTextFormatSt(St)        (((St) & dTEXT_FORMAT_MASK) >> 18)

// �������� ���� �� �����
#define  GetTextColorSt(St)         (((St) & dTEXT_COLOR_MASK) >> 24)

// �������� ��� �� �����
#define  GetBackColorSt(St)         (((St) & dBACK_COLOR_MASK) >> 28)

// �������� ������������ � �����
#define  SetAlignSt(St, fl)         (((fl) << 16) | (St))
#define  SetTextAlignSt(St)         SetAlignSt((St))

// �������� ������ � �����
#define  SetTextFormatSt(St, fl)    (((fl) << 18) | (St))

// �������� ���� � �����
#define  SetTextColorSt(St, fl)     (((fl) << 24) | (St))

// �������� ��� � �����
#define  SetBackColorSt(St, fl)     (((fl) << 28) | (St))


// -----------------------------------------------------------------------------
// ��������� ���������� ���� ��������� ������ � ��������
// [in,out] p      - ��������� �� ������
// [in]     n      - ����� ���������� ����
// [in]     color  - ���� ������ (dTEXT_STYLE_*) (���� < 0 - ��������� �� ���������������)
// [in]     format - ������ ������ (dTEXT_FORMAT_*) (���� < 0 - ��������� �� ���������������)
_RSPNLEXP void SetPnTextColor(PANEL *p, int n, int color, int format);

// -----------------------------------------------------------------------------
// ��������� ���� ��������� ������ � ��������
// [in,out] p      - ��������� �� ������
// [in]     n      - ����� ����
// [in]     color  - ���� ������ (dTEXT_STYLE_*) (���� < 0 - ��������� �� ���������������)
// [in]     format - ������ ������ (dTEXT_FORMAT_*) (���� < 0 - ��������� �� ���������������)
_RSPNLEXP void SetPnFieldColor(PANEL *p, int n, int color, int format);

// -----------------------------------------------------------------------------
// ��������� ���� �������� ������, �������� � ������ ����
// [in,out] p      - ��������� �� ������
// [in]     n      - ����� ���� (���� < 0 - ��� ����)
// [in]     color  - ���� ������ (dTEXT_STYLE_*) (���� < 0 - ��������� �� ���������������)
// [in]     format - ������ ������ (dTEXT_FORMAT_*) (���� < 0 - ��������� �� ���������������)
// [in]     style  - ���� ���� ���� (dBACK_STYLE_*) (���� < 0 - ��������� �� ���������������)
_RSPNLEXP void SetPnFieldColorEx(PANEL *p, int n, int color, int format, int style);

// -----------------------------------------------------------------------------
// ��������� ���� �������� ������
// [in,out] p      - ��������� �� ������
// [in]     n      - ����� ���� (���� < 0 - ��� ����)
// [in]     style  - ����� ���� (-1 - ��������� ����� St)
//                   ��� ����� ������ ������������ ����� ��������� ������ �������
//                   ReS. ��������, ��� ��������� ����� SMES ��������� style ����� ��������
//                   �������� ���� ReS(SMES, 0).
_RSPNLEXP void SetPnFieldStyle(PANEL *p, int n, int style);

// -----------------------------------------------------------------------------
// ��������� ���� �������� ������
// � ������� �� SetPnFieldStyle ��������� style ����� ��������������� �������� �������� �����
// (SMES, SRMS � �.�.).
// [in,out] p      - ��������� �� ������
// [in]     n      - ����� ���� (���� < 0 - ��� ����)
// [in]     style  - ����� ���� (-1 - ��������� ����� St)
_RSPNLEXP void SetPnFieldStyle2(PANEL *p, int n, int style);

// -----------------------------------------------------------------------------
// ��������� ���� �������� ������
// [in,out] p      - ��������� �� ������
// [in]     n      - ����� ���� (���� < 0 - ��� ����)
// [in]     style  - ����� ���� (-1 - ��������� ����� St)
// [in]     color  - ���� ���� ���� (dBACK_STYLE_*) (������������, ���� � �������� style
//                   �������� �������� SUSR)
_RSPNLEXP void SetPnFieldStyleEx2(PANEL *p, int n, int style, int color);


// -----------------------------------------------------------------------------
// ���������� ����� �������� � ����� ���� (FIELD::St2)
// ������ ���� St2 ���������� 4 �����.
//
// ======= ������� ���� �������� �����
// 31
// ... ������
// 24
// ======= ������� ���� �������� �����
// 23
// ... ������
// 16
// ======= ������� ���� �������� �����
// 15  ������������ ���������� ����� � ��� �����
// ... ������
//  8
// ======= ������� ���� �������� �����
//  7
// ... ������
//  0  ������� ������� ������ � ���������� (������ ����������� �����)


// -----------------------------------------------------------------------------
#define  dFIELD_MARK_MASK    0x00000001         // ����� ��������� ���� � ����������

// ����������� � ��������� ��������� ���� � ����������
#define  SetMarkField(St)           ((St) | dFIELD_MARK_MASK)
#define  UnSetMarkField(St)         ((St) & ~dFIELD_MARK_MASK)
#define  IsMarkField(St)            ((St) & dFIELD_MARK_MASK)

#define  SetFieldSt2(St, fl)        ((St) | ((fl) & dFIELD_MARK_MASK))


// -----------------------------------------------------------------------------
// ���������� ����� �������� � ���� (FIELD::iconFlag)
// ������ ���� iconFlag ���������� 4 �����.
// ������ �������� ����� ����������� ����:
//
// === ������� ���� �������� �����
// ----
//  1  ������������ ���������� ���� (dICON_ALIGN_)
//  0


// -----------------------------------------------------------------------------
// ��������� ������������ ��� icon. ��������� � ������� ����� �������� ����� ���� iconFlag
#define  dICON_ALIGN_LEFT          0x00
#define  dICON_ALIGN_RIGHT         0x01
#define  dICON_ALIGN_CENTER        0x02

// -----------------------------------------------------------------------------
// ������� ��� ����������������������� �������������
#define  dICON_ALIGN_MASK    0x00000003         // ����� ��� ��������� ������������


// -----------------------------------------------------------------------------
// ���������� �������

// �������� ������������ �� �����
#define  GetIconAlignFlag(fl)         ((fl) & dICON_ALIGN_MASK)

// �������� ������������ �� ����
#define  SetIconAlignFlag(fl, align)  ((fl) | (align))


// -----------------------------------------------------------------------------
// ��������� ������
struct tagTEXT
     {
      unsigned int  St;       // ����� ����������� ���� (������� ����� ������������ ��� ������������)
      char         *TValue;   // �������� � ����
      coord         x, y;     // ������������� ���������� ����
     };

// ��������� ��������������
struct tagBAR
     {
      unsigned int  St;       // ����� ����������� ��������������
      uint8         x, y;
      uint8         l, h;     // ������������� ���������� ��������������
     };

struct tagFIELD_EX
     {
      void        *fmt;            // ������
      const char  *fmtName;        // ������ �� ������� ������ ������
      void        *view;           // �������� �������������� ����
      char        *tree;           // ��������� ������
      FDATA       *find;           // ��������� ������
      void        *edit;           // ������� �������� �������������� ����

    // October 08, 2009
      char        *name2;          // ��������� �� ������� #2753

                                   // ��������� � ������ ������� #3709 ��� FIELD::name � FIELD::tooltipStr
                                   // � ����� ����������� �����������
      char        *name,           // ��� ���� (������ ������������ ������ !!!)
                  *tips;           // ����������� ��������� � ���� (������ ������������ ������ !!!)
     };

// ��������� ����
struct tagFIELD
     {
      unsigned char  Ftype;          // ��� ���� (FET, FBT � �.�.)
      unsigned int   St;             // ����� ����������� ����
      unsigned char  FVt;            // ��� ��������
      void          *FValue;         // �������� � ����
      int            FVp;            // ������ ����� �����, ����. ����� ������
      coord          x, y, l, h;     // ������������� ���������� ����
      coord          kl, kr, ku, kd; // ������� ����� ��� ���������
      int            FHelp;          // ��������� � ����

      unsigned long  FFlags;         // Flags for object field (JNS)
      char          *name;           // (K78) The name of this field. May the same as FValue
      char          *formatStr;      // (K78)
      short int      group;          // (K78)

      // Feb. 25, 1997
      unsigned int   Xpos;
      FIELD_EX      *Xdata;
      char          *tooltipStr;     // ����� ����������� ���������

      int            iconID;         // ������������� ������, ��������� � ����
      int            iconFlag;       // ����� � ������ ������ (dICON_ALIGN_)

      unsigned int   St2;            // ���������� ����������� St
     };

// -----------------------------------------------------------------------------
// [MENU]
// -----------------------------------------------------------------------------

#define  dMENU_EDITOR_EXTENSION   22334  // ������������� �.�. ��������� "������� ����������"

// -----------------------------------------------
// ������� ����� ��� ������ ����
#define  dFMENU_TOOLBAR   0x00000001     // ����� ���� �������� �� ToolBar (��)
#define  dFMENU_CONTEXT   0x00000002     // ����� ���� �������� � ����������� ���� (��)

// -----------------------------------------------
// ����������� ������
#define  dFMENU_TB        dFMENU_TOOLBAR
#define  dFMENU_CM        dFMENU_CONTEXT

// -----------------------------------------------
// ���������� ������

// ����� ���� �������� � �� ToolBar, � � ����������� ����
#define  dFMENU_TBCM      (dFMENU_TOOLBAR | dFMENU_CONTEXT)

// -----------------------------------------------
// ��������������� ������� ��� �������� ���� �� ����������� ����

// �������� ����������
#define  RSCREATEMENU00(name)                                 \
                     { (name), 0 }

// �������� �������� ������������� ���� ��� ����������
#define  RSCREATEMENU30(name, x, par)                         \
                     { (name), 0, 0, (x), (char *)(par), 0 }

// �� �� �����, �� � ��������� �������������� �������� ��� ToolBar'�
#define  RSCREATEMENU31(name, x, par, iconID)                 \
                     { (name), 0, 0, (x), (char *)(par), 0, 0, 0, 0, 0, (iconID), 0 }

// �� �� �����, �� � ��������� ������
#define  RSCREATEMENU32(name, x, par, iconID, flags)          \
                     { (name), 0, 0, (x), (char *)(par), 0, 0, 0, 0, 0, (iconID), (flags) }

// �� �� �����, �� � ���������� �� �� � � ��
#define  RSCREATEMENU33(name, x, par, iconID)                 \
                     RSCREATEMENU32(name, x, par, iconID, dFMENU_TBCM)

// �������� �������� ������������� ���� ��� ���������� � ��������� ������ (DISABLE/HIDE)
#define  RSCREATEMENU34(name, x, par, flags)                  \
                     { (name), 0, 0, (x), (char *)(par), 0, (flags), 0 }

// �������� �������� ������������� ���� ��� ����������
#define  RSCREATEMENU40(name, x, par, help)                   \
                     { (name), 0, 0, (x), (char *)(par), (help) }

// �� �� �����, �� � ��������� �������������� �������� ��� ToolBar'�
#define  RSCREATEMENU41(name, x, par, help, iconID)           \
                     { (name), 0, 0, (x), (char *)(par), (help), 0, 0, 0, 0, (iconID), 0 }

// �� �� �����, �� � ��������� ������
#define  RSCREATEMENU42(name, x, par, help, iconID, flags)    \
                     { (name), 0, 0, (x), (char *)(par), (help), 0, 0, 0, 0, (iconID), (flags) }

// �� �� �����, �� � ���������� �� �� � � ��
#define  RSCREATEMENU43(name, x, par, help, iconID)    \
                     { (name), 0, 0, (x), (char *)(par), (help), 0, 0, 0, 0, (iconID), dFMENU_TBCM }

// �������� �������� �������������/��������������� ���� � �����������
#define  RSCREATEMENU50(name, v, n, x, par)                   \
                     { (name), (v), (n), (x), (char *)(par), 0 }

// �� �� �����, �� � ��������� �������������� �������� ��� ToolBar'�
#define  RSCREATEMENU51(name, v, n, x, par, iconID)           \
                     { (name), (v), (n), (x), (char *)(par), 0, 0, 0, 0, 0, (iconID), 0 }

// �� �� �����, �� � ��������� ������
#define  RSCREATEMENU52(name, v, n, x, par, iconID, flags)    \
                     { (name), (v), (n), (x), (char *)(par), 0, 0, 0, 0, 0, (iconID), (flags) }

// �� �� �����, �� � ���������� �� �� � � ��
#define  RSCREATEMENU53(name, v, n, x, par, iconID)    \
                     { (name), (v), (n), (x), (char *)(par), 0, 0, 0, 0, 0, (iconID), dFMENU_TBCM }

// -----------------------------------------------
// ��������� ����
typedef struct tagMENU
      {
       const char    *name;     // �������� ������.
       void          *v;        // ������� ��� ���������. ����� NULL.
       unsigned char  n;        // ���������� ������� ������� ��� 0 ��� ��������� ������.
       int            x;        // ��� ��������������� ���� ��������� ������.
                                // ��� ������������� - ��� ������, ��� ������� v == NULL.
       void          *par;      // ������ ������.
       int            Help;     // ����� �������� ������.
       short          fl;       // ���� (0/DISABLE/HIDDEN/DISINEMPTSCRL)
       uchar          ChPos;    // ������� ������������� �������
       uchar          Reserv;
       void          *parm;     // ��������� ������������

       uint32         iconID;   // ������������� ������ ��� ToolBar'�
       uint32         flags;    // ����� ������� ������ dFMENU_

       uint32         cmdEx;    // ���������� ������� (����� ������ ���� Alt, Shift, Ctrl ��� x).

       void          *ewdata;   // ������ ��� EasyWin-������ (��. EWMenuData). ������������ ������ � ���.

       #ifdef USE_HANDLE32
            void     *res0;
            uint32    key;

            uint32    res1;
            uint32    res2;
            uint32    res3;
            uint32    res4;
            uint32    res5;
       #endif
      } MENU;

// -----------------------------------------------
typedef struct
      {
       PANEL          *p;
       unsigned short  n;
       char           *com;
       const char     *name;    // ������������ �������
       const char     *lbr;     // ������������ ���������� ��������
       MENU            mn[1];
      } TMenu;

// -----------------------------------------------
typedef struct
      {
       MENU *freeMn;
       char *freeStr;
      } TLoadPtrs;

// -----------------------------------------------
// ��������� ���� "������"
typedef struct MenuVar
      {
       MENU         *VarM;        // ��������� �� ����� "������" � �����. ����
       MENU         *HookM;       // ������ ������� ���� �����. hook'��
       uchar         HookN;       // ���-�� ������� ����, �����. hook'��
      } MenuVar;

// -----------------------------------------------
// ��������� ��������������� ����, ��������.� ������
typedef struct PanHorMenu
      {
       MENU         *m;           // ������ ������� ���. ����
       int           n;           // ���-�� ������� ���. ����
       HRSLMENU      rm;          // ����� ��������, ���� ���� ������� �� �������
       MenuVar       VarMenu;     // ��������� ���� "������"
      } PanHorMenu;

// -----------------------------------------------
// ���������, ������������ ������ ���� ����� *Parm
typedef struct MenuParm
      {
       MENU          *m;         // ������ ������� ����
       unsigned char  n;         // ���������� ������� ����
       MenuVar        VarMenu;   // ��������� ���� "������"

       #ifdef RSL_EASYWIN
           int        popup;
           int        x;
           int        y;
       #endif

       int            ret;       // id ���������� ������ ����
       uchar          clf;       // YES ��������� ��������� ���� �� ESC
      } MenuParm;

// -----------------------------------------------
// ��������� ������
typedef struct tagSECTPAR
      {
       LOBJ        list_elem;
       int         x;         // �������. ����������; ���. �������� - �� ������ �������
       int         cx;        // ������
       bool        visible;   // ������� ��������� ���������
       int         align;     // ������������ 0 - �����, 1 - ������
       char       *str;
      } SECTPAR;

// -----------------------------------------------
// ��������� ������
typedef struct tagSECTIONS
      {
       CNTLIST  SectList;               // ������ ������
      } SECTIONS;

// -----------------------------------------------
// �������������� ����
typedef struct tagADDMENUPAR
      {
       LOBJ        list_elem;
       MENU       *m;          // ������ ������� ���. ����
       int         n;          // ���-�� ������� ���. ����
       HRSLMENU    rm;         // ����� ��������, ���� ���� ������� �� �������

       int         key;        // ��� �������, �������������� ������ ���� (�.�.0)
       char       *name;       // ��� ������ ���� (�.�.NULL, ��� ������� key!=0)
      } ADDMENUPAR;

// -----------------------------------------------
// ��������� �������������� ����
typedef struct tagADDMENU
      {
       CNTLIST  AddMenuList;           // ������ �������������� ����
      } ADDMENU;


// -----------------------------------------------------------------------------
// [PANEL]
// -----------------------------------------------------------------------------
typedef struct tagPANEL_EX
      {
       void           *ew_Ext;         // ���������� ������ ��� EasyWin
       SECTIONS       *Sect;           // ��������� ������
       void           *pblock;         // ������� ������������ ��� ���������� ������
       void           *fieldX;
       bool            freeneed;
       ADDMENU        *AdditionMenu;   // ��������� �������������� ����
       bool            BsEditMode;
       char            PHelpContext[D_HELPCONTEXTLEN]; // �������� ������
       PANEL          *ownerTab;       // ������-�������� ��� TabControl'� (�� #3107)
      } PANEL_EX;

// -----------------------------------------------
// ��������� ������
struct tagPANEL
     {
      unsigned int    St;            // ����� ����������� ������
      FIELD          *PFList;        // ������ ����� ������
      unsigned char   Pnumf;         // ���������� ����� ������
      TEXT           *PTList;        // ������ �������
      unsigned char   Pnumt;         // ���������� ������� � ������
      coord           x1,y1,x2,y2;   // ���������� ������ (����������)
      panel_keyproc_t proc ;         // ���������� ����������
      int             PHelp;         // ��������� � ������
      void           *Parm;          // ��� �������� ����������
      unsigned char   Pff;           // ����� ���������� ����
      char           *Pstatus;       // ���������� ��� ������ ���������
      char           *Phead;         // ���������
      FIELD          *Pcurfld;       // ������� �������� ����
      void           *PSave;         // ������� ����������

      void           *objPtr;
      unsigned long   flags;         // (K78)
      char           *statusRD;      // (K78) Status line for read only mode

      uchar           NeedGoToFldNum;
      uchar           firstf;
      uchar           lastf;
      uchar           reserv1;

      PanHorMenu      HorMenu;       // ��������� �����.����,��������.� ������
      PANEL          *HorMenuPan;    // ��������� �� ������ ���.����,�����.� ������
      char           *ResName;       // ��� ������� ������

      PANEL_EX       *Xdata;

      char           *ResFileName;   // ������������ lbr-�����. �� ����������� ���� �� ������� !!!

      unsigned long   flags2;        // ����������� flags
     };


#define  RFP_REJREQ        0x00000004
#define  RFP_UPDREQ        0x00000008
#define  RFP_DELREQ        0x00000010

#define  RFP_CHNGF         0x00000020   // ������ ���� ���������� ���� ��������
                                        // ����������� � EasyWin

#define  RFP_NOCLOSE       0x00000040   // ��� EasyWin

#define  RFP_RIGHTTEXT     0x00000080   // ��� ������ � ������ ����� ������ ������������
#define  RFP_TRANSPARENT   0x00000100   // �� ������������ (��. RFP_CENTERED)
#define  RFP_ANIMATED      0x00000200   // �� ������������
#define  RFP_FLOATING      0x00000400
#define  RFP_PINX          0x00000800
#define  RFP_PINY          0x00001000
#define  RFP_NOAUTODIR     0x00002000
#define  RFP_NOAUTONUM     0x00004000
#define  RFP_NOSHADOW      0x00008000

#define  RFP_CENTERED      0x00000100   // ������ RFP_TRANSPARENT

// This flags is used by BSCROL
#define  RFP_AUTOFILL      0x00010000   // ��������� ������ � ���. ��������� �� ������
#define  RFP_AUTOFIELDS    0x00020000   // ������������� ������������� ��������� ����� � ���������
#define  RFP_AUTOHEAD      0x00040000   // ������������ ��������� ������� � ���������
#define  RFP_REVERSORDER   0x00080000   // �������� ������� �������
#define  RFP_DENYSORT      0x01000000   // ��������� ���������� ���������

#define  P_NAME2ALLOCED    0x00100000   // ����� ���� name ������� � ����� ������ ������. ���� ��������������� � redit
                                        // ��� ���������� �������

// -----------------------------------------------
// ��� ����� ��������� � PANEL::flags2
#define  RFP_MAXIMIZED     0x00200000   // ���� ������� ���������, ����������� ��������� �� ���� �����
#define  RFP_MULTIHEAD     0x00400000   // ������������� ���������
#define  P_IMAGEEDITOR     0x00800000   // �������� (�������������) ����������� ������
// -----------------------------------------------


#define  P_OUT             0x00100000   // ����� �� ������ (����������)
#define  P_PROC            0x00200000   // ������� ���������� �� �����. �����������
#define  P_DELREC          0x00400000   // �������� ������ ����������
#define  P_FLDFOCUS        0x00800000   // ������������ ������ � �����������,
                                        // ��������, ��� � ������� ���� ����������
                                        // �����, �.�., � ���������� ������� ENTFIELD

#define  P_LOOPPANEL       0x00010000   // ������ �������� ����� LoopPanel (������������ ������ � �����������)

#define  P_MENU            0x01000000   // ������ ������������ ��� ����������� ����
#define  P_SCROL           0x02000000   // �������� Mp ������� SCROL'�
#define  P_LSCROL          0x04000000   // �������� Mp ������� LSCROL'�
#define  P_BSCROL          0x08000000   // �������� Mp ������� BSCROL'�

#define  P_PANEL_MASK      0x0f000000   // ����� ��� ����������� ������

#define  P_DISABLE         0x10000000
#define  P_TEXTEDITOR      0x20000000
#define  P_HELPER          0x40000000
#define  P_TABCTRL         0x80000000


#define  IsCenteredFl(p)      ((p)->flags & RFP_CENTERED)
#define  SetCenteredFl(p)     ((p)->flags |= RFP_CENTERED)
#define  UnsetCenteredFl(p)   ((p)->flags &= ~RFP_CENTERED)

#define  IsRightTextFl(p)      ((p)->flags & RFP_RIGHTTEXT)
#define  SetRightTextFl(p)     ((p)->flags |= RFP_RIGHTTEXT)
#define  UnsetRightTextFl(p)   ((p)->flags &= ~RFP_RIGHTTEXT)

#define  IsMaximizedFl(p)     ((p)->flags2 & RFP_MAXIMIZED)
#define  SetMaximizedFl(p)    ((p)->flags2 |= RFP_MAXIMIZED)
#define  UnsetMaximizedFl(p)  ((p)->flags2 &= ~RFP_MAXIMIZED)

#define  IsMultiheadFl(p)     ((p)->flags2 & RFP_MULTIHEAD)
#define  SetMultiheadFl(p)    ((p)->flags2 |= RFP_MULTIHEAD)
#define  UnsetMultiheadFl(p)  ((p)->flags2 &= ~RFP_MULTIHEAD)

#define  IsAutoFillFl(p)      ((p)->flags & RFP_AUTOFILL)
#define  SetAutoFillFl(p)     ((p)->flags |= RFP_AUTOFILL)
#define  UnsetAutoFillFl(p)   ((p)->flags &= ~RFP_AUTOFILL)

#define  IsAutoFieldsFl(p)    ((p)->flags & RFP_AUTOFIELDS)
#define  SetAutoFieldsFl(p)   ((p)->flags |= RFP_AUTOFIELDS)
#define  UnsetAutoFieldsFl(p) ((p)->flags &= ~RFP_AUTOFIELDS)

#define  IsAutoHeadFl(p)      ((p)->flags & RFP_AUTOHEAD)
#define  SetAutoHeadFl(p)     ((p)->flags |= RFP_AUTOHEAD)
#define  UnsetAutoHeadFl(p)   ((p)->flags &= ~RFP_AUTOHEAD)


#define  IsOutFl(p)           ((p)->flags & P_OUT)
#define  SetOutFl(p)          ((p)->flags |= P_OUT)
#define  UnsetOutFl(p)        ((p)->flags &= ~P_OUT)

#define  IsProcFl(p)          ((p)->flags & P_PROC)
#define  SetProcFl(p)         ((p)->flags |= P_PROC)
#define  UnsetProcFl(p)       ((p)->flags &= ~P_PROC)

#define  IsBsOutFl(bs)        ((bs)->Mp.flags & P_OUT)
#define  SetBsOutFl(bs)       ((bs)->Mp.flags |= P_OUT)
#define  UnsetBsOutFl(bs)     ((bs)->Mp.flags &= ~P_OUT)

#define  IsBsDenySort(bs)     ((bs)->flags & RFP_DENYSORT)
#define  SetBsDenySort(bs)    ((bs)->flags |= RFP_DENYSORT)
#define  UnsetBsDenySort(bs)  ((bs)->flags &= ~RFP_DENYSORT)

#define  IsBsProcFl(bs)       ((bs)->Mp.flags & P_PROC)
#define  SetBsProcFl(bs)      ((bs)->Mp.flags |= P_PROC)
#define  UnsetBsProcFl(bs)    ((bs)->Mp.flags &= ~P_PROC)

#define  IsDelRecFl(bs)       ((bs)->Mp.flags & P_DELREC)
#define  SetDelRecFl(bs)      ((bs)->Mp.flags |= P_DELREC)
#define  UnsetDelRecFl(bs)    ((bs)->Mp.flags &= ~P_DELREC)

#define  IsMenuFl(p)          ((p)->flags & P_MENU)
#define  SetMenuFl(p)         ((p)->flags |= P_MENU)

#define  IsFldFocus(p)        ((p)->flags & P_FLDFOCUS)
#define  SetFldFocus(p)       ((p)->flags |= P_FLDFOCUS)
#define  UnsetFldFocus(p)     ((p)->flags &= ~P_FLDFOCUS)

#ifndef SQLBUILD
    /* ���������� ����� ������������� ��� ����� ������ */
    #define  DisablePanFields(p) ((p)->flags |= P_DISABLE)

    /* �������� ����� ������������� ��� ����� ������ */
    #define  EnablePanFields(p)  ((p)->flags &= ~P_DISABLE)

    /* ���������, ��� ������ �������� */
    #define  IsEnablePanFlds(p)  (!((p)->flags & P_DISABLE))
#else
    #define  DisablePanFields(p)   BlockPanFields((p), BLOCK_READONLY_FIELDS)
    #define  EnablePanFields(p)    UnblockPanFields((p))
    #define  IsEnablePanFlds(p)    !IsBlockedPanFields((p))
#endif

#define  IsBScrlFl(p)     ((p)->flags & P_BSCROL)
#define  IsLScrlFl(p)     ((p)->flags & P_LSCROL)
#define  IsScrlFl(p)      ((p)->flags & P_SCROL)
#define  IsPanelFl(p)     (!((p)->flags & P_PANEL_MASK))

#define  IsChF(p)         ((p)->flags & RFP_CHNGF)
#define  SetChF(p)        ((p)->flags |= RFP_CHNGF)
#define  UnsetChF(p)      ((p)->flags &= ~RFP_CHNGF)

#define  IsNoClose(p)     ((p)->flags & RFP_NOCLOSE)
#define  SetNoClose(p)    ((p)->flags |= RFP_NOCLOSE)
#define  UnsetNoClose(p)  ((p)->flags &= ~RFP_NOCLOSE)


#define  IsChangeFld(p)   ((p)->NeedGoToFldNum != 255 && (p)->NeedGoToFldNum != (p)->Pcurfld - (p)->PFList)

#define  isTabStopFld(f)  (!((f)->FFlags & RF_NOTABSTOP))
#define  isAsTextFld(f)   (((f)->FFlags & RF_ASTEXT))
#define  isActiveFld(f)   (isActF((f)->Ftype) && !isHideF((f)->Ftype) && !isAsTextFld(f))

#define  isEmptyF(f)      (((f)->FFlags & RF_EMPTY))
#define  setEmptyF(f)     (((f)->FFlags |= RF_EMPTY))
#define  unsetEmptyF(f)   (((f)->FFlags &= ~RF_EMPTY))

#define  isBlockHideF(f)      (((f)->FFlags & RF_BLOCKHIDE))
#define  setBlockHideF(f)     (((f)->FFlags |= RF_BLOCKHIDE))
#define  unsetBlockHideF(f)   (((f)->FFlags &= ~RF_BLOCKHIDE))


// ������� ����������
struct tagPSAVER
     {
      int    PSa, PCs;       // ��������
      coord  PCx, PCy;       // �������
     };

/* ������ ��� ������              */
struct tagFDATA
     {
      PANEL  *Mp;            /* ������ ����������                   */
      int     nfield;        /* ����� ���� � ����������             */
      void   *vfield;        /* �������� ������                     */
      void   *value;         /* �������� ������                     */
      int     len;           /* ����� ��������                      */
      int     Vt;            /* ��� ��������                        */
      int     cond;          /* ��������                            */
      int     maker;         /* ����� ��������� ������� make()      */
     };

enum FindCondition
   {
    EQ = 0,
    NEQ,
    LT,
    GT,
    LE,
    GE,
    SUBSTR = 0,
    REGEXP,
    EQUALSTR,
    NOTEQUALSTR
   };

#define  NF(f)  sizeof(f)/sizeof(FIELD)
#define  NT(t)  sizeof(t)/sizeof(TEXT)

#define  SPartLine    0x00000001

#define  IsSPartLine(s)     (((s)->allocBuf && ((int *)(s)->allocBuf)[0] & SPartLine) ? 1 : 0)
#define  SetSPartLine(s)    ((s)->allocBuf ? (((int *)(s)->allocBuf)[0] |= SPartLine, 1) : 0)
#define  UnsetSPartLine(s)  ((s)->allocBuf ? (((int *)(s)->allocBuf)[0] &= ~SPartLine, 1) : 0)


#define  SFiltMod     0x0001   // ���� ���������������, ���� � SCROL'� ������������ ���������������� ������
#define  SNotQSearch  0x0002   // ���� ���������������, ���� � SCROL'� ����� ��������� ������� �����

#define  IsSFiltMod(s)     ((s)->flags & SFiltMod)
#define  SetSFiltMod(s)    ((s)->flags |= SFiltMod)
#define  UnsetSFiltMod(s)  ((s)->flags &= ~SFiltMod)

#define  IsSNotQSearch(s)     ((s)->flags & SNotQSearch)
#define  SetSNotQSearch(s)    ((s)->flags |= SNotQSearch)
#define  UnsetSNotQSearch(s)  ((s)->flags &= ~SNotQSearch)

/* ��������� ������������ �������               */
struct tagSCROL
     {
      PANEL      Mp;             // ������ ������
      int        Mn;             // ���������� ������� � �������
      coord      x, y, l, h;     // ���������� �������
      panel_recproc_t  rec;      // ��������� ������������� �������
      void      *Ms;             // ������ �������
      int        Mnumfl;         // ���������� �������
      int        currfl;         // ����� ������� �������� ������
      int        beginfl;        // ����� ������ ������ � �������
      int        currbar;        // ����� �������� ������ � �������
      coord      cursx;          // ���������� �������
      uchar    updtmod;          // ������� ���������� ���������
      PANEL   *Mps;              // ������ ���������
      panel_keyproc_t  proc;     // ���������� ����������

      // 28 Mar 2002 A.Lebedev
      PANEL    wrkPan;
      char     isTrunc;          // True if last field in wrkPan is truncated
      uchar    startF;
      short    flags;
      uchar    reserve;
      char    *headBuff;
      void    *allocBuf;         // ��������� �� ����������� ���������� ������
     };

// 1996 (JNS)
enum NavigationDir
   {
    Left = -2,
    Up,
    None,
    Down,
    Right
   };

// �-��� ��� ������������� ������ � ������.
// ����������: 0 - ������ ������������� �����,
//            !0 - � ��������� ������.
//
typedef int (* MaskMatchingProc)(const char *mask, const char *str);

// �-��� ��� �������������� ����� � SQL-���������.
// ���������� ��������� �� buf ��� ����������� ���������� ����� (���� �������
// ����������� ������ ������������), � ������� ������������ SQL-���������,
// ����������� ������, ��� NULL, ���� ��������� ������.
//
// ����������� ���������� ����� ������ ���� ����������� � ������� free()
// � ���������� �-���.
//
typedef char * (*ConvertMaskToSqlProc)(const char *mask, const char *fldname, char *buf, size_t sz);


#ifdef __cplusplus
extern "C" {
#endif

typedef int (*JChStrChange)(const char *olds, const char *s, int c);
_RSPNLEXP extern JChStrChange  edstring_callback;

// Lebedeva
typedef int (*SNRChange)(const char *olds,   // ������ �������� ����
                         char       *s,      // ����� �������� ����
                         int         size,   // ������ ���������� ������
                         int         c,      // ��� ������� �������
                         PANEL      *p);

_RSPNLEXP SNRChange SetSNRCallback(SNRChange);

// Lebedeva

/************************************************************************/
/*                                                                      */
/*                       ������ � �����������                           */
/*                                                                      */
/************************************************************************/

typedef int (* evfunction) (void);

_RSPNLEXP extern evfunction  getkb;
_RSPNLEXP extern evfunction  testkb;

/* ������� ���� ���������� � �����      */
_RSPNLEXP void ungkb(int key);

// ���������������������� ������ ungkb (�� ������ �������� scan � kbflags).
// ���������� ���������� �������� kbung. �������� kb ����� ���� NULL;
int ungetkb(int *key);

// ������ ungkb(), ������� ������� �� ����� ����������
#define  clrkb()  (ungkb(0))

_RSPNLEXP int _getkb(void);              /* �������� � ������ ���� � ����������  */
_RSPNLEXP int _testkb(void);             /* ������ ���� � ���������� ��� �������� */

_RSPNLEXP extern unsigned  dblklick; /* �������� � ����� ( = 10) ��� ����������� DBLKLICK */

int getEvent(int waitTime); /* ������ ���� � ���������� ��� ���� � ��������� ��� ��� */

// Added by Yakov E. Markovitch (JNS)
_RSPNLEXP void putCommand(int key);
_RSPNLEXP int  testCommand();
_RSPNLEXP int  testKey(int key);

_RSPNLEXP void playKString(const int *str);
_RSPNLEXP void playCString(const unsigned char *str);

/************************************************************************/
/*                                                                      */
/*                              ����                                    */
/*                                                                      */
/************************************************************************/
/* ����                                 */
_RSPNLEXP void beep(
                    int rep  /* ����� �������� */
                   );

bool RslSetBeep(bool on);

/************************************************************************/
/*                                                                      */
/*                          ������� ��� ������                          */
/*                                                                      */
/************************************************************************/

/* ������� ���������                            */
_RSPNLEXP void BordBar(
                       int x1,                // ����������
                       int y1,                //
                       int x2,                //
                       int y2,                //
                       unsigned int atr,      // �������
                       unsigned int brd       // �����
                      );

/* ������� ���������                            */
_RSPNLEXP void DrawBar(
                       int mode,              /* 0 - static, 1 - dynamic */
                       int x1,                /* ����������                                   */
                       int y1,                /*                                              */
                       int x2,                /*                                              */
                       int y2,                /*                                              */
                       unsigned int atr,      // �������
                       unsigned int brd       // �����
                      );

_RSPNLEXP void DrawShadow(PANEL *p) ;

_RSPNLEXP void PutPHead(PANEL *p, const char *Head) ;
_RSPNLEXP void PutPHeadStatic(PANEL *p, const char *Head);

_RSPNLEXP void SetFldRouting(
                             PANEL *p              // ��������� ������
                            );

_RSPNLEXP int SavePanelArea(
                            PANEL *p,              // ��������� ������
                            int sh                 // Shadow yes/no
                           );

// ���������� �������������� ������ �� ������
_RSPNLEXP int PutPanelBar(
                          PANEL *p,              // ��������� ������
                          int    savef            // ���� ����������
                         );

/************************************************************************/
/*                                                                      */
/*              ������� �������������� �����                            */
/*                                                                      */
/************************************************************************/

/* ���������� �������� � ������                 */
_RSPNLEXP char * valtostr(
                          void        *val,      /* ��������                                     */
                          int          w,        /* ������ ����                                  */
                          int          p,        /* ������ ����� �����                           */
                          unsigned int type,     /* ��� ��������                                 */
                          int         *size,     /* ������ ���������� ��� ������                 */
                          int          ljust     /* ������������ ����� - ON                      */
                         );

/* ���������� ������ � ��������                 */
_RSPNLEXP void strtoval(
                        char        *s,        /* ������                                       */
                        void        *V,        /* ��������                                     */
                        int          p,        /* ������ ����� ����� ��� ������������ �����    */
                        unsigned int type,     /* ��� ��������                                 */
                        int          size      /* ������ ���������� ������ ��� ������          */
                       );

// ���������� valtostr, ������ � ������ ������� � FIELD'�
_RSPNLEXP char *valtostrEx(void *val, int w, int p, unsigned int type, int *size, int ljust, FIELD *pFld);

/* ?�?����� -��?-�? � ���R��        */
_RSPNLEXP char* valtostrFmt(
                            void            *val,     /* ���祭��                            */
                            int              w,       /* ��ਭ� ����                         */
                            int              p,       /* ������ ��᫥ �窨                  */
                            unsigned int     type,    /* ��� ���祭��                        */
                            int             *size,    /* ������ �뤥������ ����� ��� ��ப� */
                            int              ljust,
                            const char      *fmtStr   /* 蠡��� */
                           );

// ���������� strtoval, ������ � ������ ������� � FIELD'�
_RSPNLEXP void strtovalEx(char *s, void *V, int p, unsigned int type, int size, FIELD *pFld);

/************************************************************************/
/*                                                                      */
/*                   ����� � �������������� ������                      */
/*                                                                      */
/************************************************************************/

_RSPNLEXP void outlenS          /* ����� ������ � ���������             */
                      (
                       int   x,                       /* �������                              */
                       int   y,                       /* �� ������                            */
                       const char *stro,              /* ������ ��� ������                    */
                       int   outlen,                  /* ����� ���� ��� ������                */
                       int   offs                     /* �������� ������� ���������� �������  */
                      );

_RSPNLEXP void outlenSX         /* ����� ������ � ���������             */
                       (
                        int   x,                       /* �������                              */
                        int   y,                       /* �� ������                            */
                        const char *stro,              /* ������ ��� ������                    */
                        int   outlen,                  /* ����� ���� ��� ������                */
                        int   offs,                    /* �������� ������� ���������� �������  */
                        unsigned long fflags           /* Field flags                          */
                       );

_RSPNLEXP void outlenS_2        /* ����� ������ � ���������             */
                        (
                         int   x,                       /* �������                              */
                         int   y,                       /* �� ������                            */
                         const char *stro,              /* ������ ��� ������                    */
                         int   outlen,                  /* ����� ���� ��� ������                */
                         int   offs,                    /* �������� ������� ���������� �������  */
                         unsigned selAtr                /* ������� ���������                    */
                        );

_RSPNLEXP void outlenSX_2       /* ����� ������ � ���������             */
                         (
                          int   x,                       /* �������                              */
                          int   y,                       /* �� ������                            */
                          const char *stro,              /* ������ ��� ������                    */
                          int   outlen,                  /* ����� ���� ��� ������                */
                          int   offs,                    /* �������� ������� ���������� �������  */
                          unsigned long fflags,          /* Field flags                          */
                          unsigned selAtr                /* ������� ���������                    */
                         );

_RSPNLEXP extern unsigned InsCur; /* ����� ������� ��� �������            */
_RSPNLEXP extern unsigned RepCur; /* ����� ������� ��� ����������         */

_RSPNLEXP extern int Ins;          /* ���� ������ �������                  */
_RSPNLEXP extern const char *PasswSubst; /* Password substitution string (can be NULL)  */

_RSPNLEXP unsigned int edcs       /* ��� ������� ��� ��������� ������             */
                           (
                            int insf               /* ���� ������ �������                          */
                           );

_RSPNLEXP int edstring  /* �������� ������                              */
                      (
                       int   x,               /* �������                                      */
                       int   y,               /* �� ������                                    */
                       char *s,               /* ������������� ������                         */
                       int  *offsp,           /* �������� ������ � ������                     */
                       int  *cursp,           /* ������� �������  � ������                    */
                       int   outlen,          /* ����� ���� ��� ������                        */
                       int   maxlen,          /* ����� ������ S                               */
                       int  *firstenter,      /* ���� ������� ����� = OFF                     */
                       int  *Insert,          /* ����� ������� = ON                           */
                       void *Pict             /* ������                                       */
                      );

_RSPNLEXP int edstringEx2    /* �������� ������            */
                         (
                          int   x,     /* �������                 */
                          int   y,               /* �� ������                                    */
                          char *s,     /* ������������� ������          */
                          int  *offsp,         /* �������� ������ � ������          */
                          int  *cursp,       /* ������� �������  � ������        */
                          int   outlen,      /* ����� ���� ��� ������         */
                          int   maxlen,      /* ������������ ����� ������ S ������� '\0'  */
                          int  *firstenter,  /* ���� ������� ����� = OFF         */
                          int  *Insert,      /* ����� ������� = ON            */
                          void *Pict,     /* ������               */
                          int   canEdit,
                          int  *ChangedFlag
                         );

_RSPNLEXP int FmtEdstringEx2     /* �������� ������ - � ������ ���������� ������� */
                            (
                             int   x,              /* ������� */
                             int   y,              /* �� ������  */
                             char *s,              /* ������������� ������ */
                             int  *offsp,          /* �������� ������ � ������ */
                             int  *cursp,          /* ������� �������  � ������ */
                             int   outlen,         /* ����� ���� ��� ������ */
                             int   maxlen,         /* ������������ ����� ������ S ������� '\0' */
                             int  *firstenter,     /* ���� ������� ����� = OFF */
                             int  *Insert,         /* ����� ������� = ON */
                             void *Pic,            /* ������ */
                             int   canEdit,
                             int  *ChangedFlag
                            );

// �������� ��������
_RSPNLEXP int CheckPict(char *s, FIELD *f);

// �������� ����������� ��������� ������ ���� ����� ��� ���� �������� type,
// default_len - ����� ��-���������
_RSPNLEXP int FIELD_GetMaxWidthByType(int type, int default_len);

// �������� ����������� ��������� ������ ���� ����� ��� ���� FIELD � ����������� �����
// (������� ��������� �� ������ CheckPict)
_RSPNLEXP int FIELD_GetMaxWidth(FIELD *f);

// �������� ��������� ������������� ����
_RSPNLEXP char *GetFldString(PANEL *p, int field);

// ��������/���������� ������������ ����
_RSPNLEXP const char *GetFldName(FIELD *f);
_RSPNLEXP void        SetFldName(FIELD *f, const char *text);

// ��������/���������� ����� ��������� � ����
_RSPNLEXP const char *GetFldTips(FIELD *f);
_RSPNLEXP void        SetFldTips(FIELD *f, const char *text);

// �������� ��������� � ����
_RSPNLEXP void        UpdateFieldTooltip(PANEL *p, int cf, const char *text);


/************************************************************************/
/*                                                                      */
/*                      ����� ��������� ������                          */
/*                                                                      */
/************************************************************************/

_RSPNLEXP void PutTexts /* ������� �����                                */
                       (
                        PANEL *p               /* ��������� ������                             */
                       );

_RSPNLEXP void PutFET  // ������� �������� ���� �� �����.
                     (
                      coord     x1,          // ����������
                      coord     y1,          // ����������.
                      unsigned  fatr,        // �������.
                      FIELD    *f            // ����.
                     );

_RSPNLEXP void PutFET2 // ������� �������� ���� �� �����.
                      (
                       coord     x1,          // ����������
                       coord     y1,          // ����������.
                       FIELD    *f,           // ����.
                       unsigned  ps,          // �������.
                       uchar     cmd          // �����.
                      );

_RSPNLEXP void PutFBT  // ������� �������� ������ �� �����.
                     (
                      coord     x1,          // ����������
                      coord     y1,          // ����������.
                      FIELD    *f,           // ����.
                      unsigned  ps,          // �������.
                      uchar     cmd          // �����.
                     );

_RSPNLEXP void PutFBS  // ������� ������ � ����� �� �����.
                     (
                      coord     x1,          // ����������
                      coord     y1,          // ����������.
                      FIELD    *f,           // ����.
                      unsigned  ps,          // �������.
                      uchar     cmd,         // �����.
                      uchar     fl           // ���� ���������.
                     );

_RSPNLEXP void PutWRP          /* ������� ������������� ����        */
                     (
                      PANEL    *p,    // ������.
                      FIELD    *f,    // ����.
                      int canEdit
                     );

_RSPNLEXP void PutHLP             /* ������� ������ ������                        */
                     (
                      PANEL *p,              /*                                              */
                      FIELD *f               /* ����                                         */
                     );

_RSPNLEXP extern int  HiLight;
_RSPNLEXP extern int  BlockNav;
_RSPNLEXP extern int  MousePressFlag;

#define  FPutOff()  HiLight = OFF;

_RSPNLEXP void PutField(PANEL *p, FIELD *f);

/* ������� ����                                 */
_RSPNLEXP void PutFields(
                         PANEL *p               /* ��������� ������                             */
                        );

_RSPNLEXP char *PrintField(PANEL *p, FIELD *f, char *buf, size_t bufsize);

_RSPNLEXP int IsCharLine(FIELD *f);

/************************************************************************/
/*                                                                      */
/*                     ����������� & ��������                           */
/*                                                                      */
/************************************************************************/


_RSPNLEXP extern panel_keyproc_t  cl;

#define  initcalc()  cl = calc

// �����������
_RSPNLEXP int calc(PANEL *p, int key);

_RSPNLEXP extern unsigned long  StorLeft;

_RSPNLEXP extern panel_keyproc_t  db;

#define  initdb()  db = debugrep

// ��������
_RSPNLEXP int debugrep(PANEL *p, int key);

/************************************************************************/
/*                                                                      */
/*                       ����� � �������� ������                        */
/*                                                                      */
/************************************************************************/

_RSPNLEXP extern panel_keyproc_t  hp;

/* ���������� �������� ������                   */
_RSPNLEXP int helper(PANEL *p, int key);


typedef int (* helpfunction)(int page, const char *context);
_RSPNLEXP extern helpfunction  bighelp;

/* ������� ���� ������ � ������ ��������� */
_RSPNLEXP int _bighelp(int HPage, const char *context);

// �� ��, ��� � _bighelp, �� � ���������� ������ ��������� ������ �������
int _bighelpProxy(int HPage, const char *context);


#define  inithelp(s)  { hp = helper; RslAddHelpRange(0, 0, s); }

_RSPNLEXP bool RslAddHelpRange(int ind1, int ind2, const char *name);

_RSPNLEXP extern int  ContextHelp;
_RSPNLEXP extern int  ContextTemp;

#define  SetContext(page)   (ContextTemp = ContextHelp,              \
                             ContextHelp = page,                     \
                             ContextTemp )

#define  ResetContext(temp)  ContextHelp = temp

// -----------------------------------------------------------------------------
// �������� �� ��������� ��� ��������������� ����� � ������ ��������
// [in]  ptr  - ������ ������ ���������
// [out] file - ���� ������ �� ������� ��������� (����� �������������)
// [out] ctx  - �������� (����� �������������)
// [ret]      - 0 (� ����� ������)
_RSPNLEXP int RslSplitHelpContext(const char *ptr, char *file, char *ctx);


/************************************************************************/
/*                                                                      */
/*                       ��������� �����                                */
/*                                                                      */
/************************************************************************/

_RSPNLEXP int procfld   /* ������������ ����                            */
                     (
                      PANEL *p               /* ��������� ������                             */
                     );

/* ������������ ���� ������������ ���� (��� ������ ������ ��� ����) */
_RSPNLEXP extern int unknownfld(PANEL *p);


/************************************************************************/
/*                                                                      */
/*                       ������ � �������                               */
/*                                                                      */
/************************************************************************/

_RSPNLEXP int Direction           // ���������� �������� ������ �����.
                       (                       // ���������� ������ ������ �������� ����.
                        FIELD *f,              // ������ �����.
                        int    n               // ���������� ����� � �������.
                       );

#define  SetDirec(p)    (p)->Pff = (unsigned char)Direction((p)->PFList, (p)->Pnumf)

// 15/02/96 (JNS)
_RSPNLEXP void testNSetFocus(PANEL *p, int disp, int d);

_RSPNLEXP char *psl      // ����� ������� ���������� ������� � ������.
                   (
                    PANEL *p
                   );

_RSPNLEXP int PrePanel            // ����������� ������
                      (
                       PANEL *p,    // ��������� ������
                       int   savef     // ���� ����������
                      );

_RSPNLEXP int PutPanel            /* ������� ������                               */
                      (
                       PANEL *p,              /* ��������� ������                             */
                       int   savef            /* ���� ����������                              */
                      );

_RSPNLEXP int UsePanel            /* ���� ������ ������                           */
                      (
                       PANEL *p               /* ��������� ������                             */
                      );

_RSPNLEXP void ResetField(PANEL *p, int fld);

_RSPNLEXP void RemPanel           /* ������  ������                               */
                       (
                        PANEL *p               /* ��������� ������                             */
                       );

_RSPNLEXP void DelPanel(PANEL *p);

_RSPNLEXP void MovePanel(PANEL *p, int x, int y, int from);


_RSPNLEXP int LoopPanel      // ���������� � �������.
                       (                       // ���������� -key ��� 0 ��� �������� ������.
                        PANEL *p     // ��������� ������.
                       );

int  PutMenuPanel(PANEL *p);
void RemMenuPanel(PANEL *p);

int  UseMenuPanel(
                  PANEL *p,              // ������ ����.
                  uchar  clf             // YES ��������� ����������� �� ESC.
                 );


// -----------------------------------------------------------------------------
// ������ ������ ��� ���� �������
// [in,out] p      - ��������� �� ������
// [in]     n      - ����� ����
// [in]     iconID - ������������� ������
// [in]     flags  - ����� ����������� ������
_RSPNLEXP void SetFieldIcon(PANEL *p, int n, int iconID, int flags);

// -----------------------------------------------------------------------------
// ������� ��� ������ � ����������� ������

// ���������� ������� ������������� ��������� ����������
int InitHelpContext(void);

// ���������� ������� ��������������� ��������� ����������
void DoneHelpContext(void);

// ��������� ��������� ������
_RSPNLEXP void SetPHelpContext(PANEL *p, const char *context);

// ��������� ��������� ������ ��� ��������� �������
// [in]  name    - ������������� ������� (������� �� �����������). ���� �������� � �������� name
//                 ��� �������� � ������, �� ����� ����������� ����� ���������.
// [in]  context - �������� ������ ��� ��������� �������. ���� ����� NULL, �� �������� ���
//                 ������� ������� ����� ����� �� ������.
// [ret] ���������� true � ������ ��������� ���������� ��������� � ������
_RSPNLEXP bool SetResHelpContext(const char *name, const char *context);

// ��������� ��������� ������ ��� ��������� �������
// [in]  name    - ������������� ������� (������� �� �����������)
// [in]  context - �������� ������ ��� ��������� �������. ����� ���� NULL - � ���� ������
//                 ������� ����� ������ ���������.
// [ret] ���������� ������ ������ ��������� (��� ����� 0-�������) ��� -1 � ���������
//       ������� (���� �������� ��� �������������� ������� �����������).
_RSPNLEXP int GetResHelpContext(const char *name, char *context);


// Kireev 29.5.00 ������� ��� �������������� ����� �������� ����
// ������������� ��� ������ ��������������� �� ������������.
_RSPNLEXP int PanGoToFld(PANEL *p, int disp);

//   ������� �������������� ������ ����� �� panel.c
_RSPNLEXP void  DisableEditPanel  (PANEL *p);
_RSPNLEXP void  EnableEditPanel   (PANEL *p);
_RSPNLEXP int   SavePanelTypes    (PANEL *p);
_RSPNLEXP int   RestorePanelTypes (PANEL *p, int del);
_RSPNLEXP int   _SavePanelTypes   (PANEL *p, unsigned char **buf);
_RSPNLEXP int   _RestorePanelTypes(PANEL *p, int del, unsigned char **buf);

// ���������� ����� ������
enum
   {
    BLOCK_DISABLE_FIELDS  = 1,
    BLOCK_READONLY_FIELDS = 2
   };

_RSPNLEXP int  BlockPanFields    (PANEL *p, int flags);
_RSPNLEXP void UnblockPanFields  (PANEL *p);
_RSPNLEXP int  IsBlockedPanFields(PANEL *p);

//*************************** ��������. **********************************

_RSPNLEXP void UpdateScrol       // ������� � ������������ �������.
                          (
                           SCROL *mn              // ��������� ���������.
                          );

_RSPNLEXP int PutScrol           // ������� ������� ���������.
                      (
                       SCROL *mn,             // ��������� ������.
                       int   savef            // ���� ����������.
                      );

_RSPNLEXP int UseScrol           // �������� �������.
                      (
                       SCROL *mn              // ��������� ���������.
                      );

_RSPNLEXP void RemScrol          // ������ �������� �������
                       (
                        SCROL *mn              // ��������� ���������.
                       );

_RSPNLEXP int LoopScrol          // ������� � ��������� ��������� �� ����������
                       (                                // ���������� -key ��� 0 ��� �������� ������.
                        SCROL *mn              // ��������� ����������.
                       );

_RSPNLEXP void AdjustScrol
                          (
                           SCROL *mn,             // ��������� ���������.
                           coord x,               // ���������� ������ � ���������� �����
                           coord y,
                           coord l
                          );

_RSPNLEXP int ScrolGoToFld       // �������������� ����� ����
                          (                                // ������������� ��� ������ ��������������� �� ������������.
                           SCROL *mn,             // ��������� ����������.
                           int    disp            // ����� ����.
                          );

// �������������� ����� ����
_RSPNLEXP int ScrolGoTo(SCROL *mn, int disp, int dir);

// ��������� ��������������� ����������
_RSPNLEXP void CopyScrolFields(SCROL *mn,int startF);
_RSPNLEXP void DrawScrolHeads(SCROL *mn);

// ��������� ������� ���������� ( ��������� �������� 0 )
_RSPNLEXP int ResizeScrol(PANEL *p, coord x, coord y, int fSizeMoveLoop);

int SResize(PANEL *p, int numRows, int l, bool isPartLine);

//************************** ���������. *********************************

_RSPNLEXP char **msgpars          // �������� ���������.
                        (
                         const char *msgs,            // ��������� c ������������� '|'.
                         int  *n                // ���������� �����.
                        );

_RSPNLEXP int msgwin              // ���� ���������
                    (                       // ���������� ��� ESC.
                     const char *msgs,            // ��������� c ������������� '|'.
                     int   x,               // ���������� ������,
                     int   y,               // ��� ������������� -1.
                     int   r                // ��������.
                    );

_RSPNLEXP int msgwinH              // ���� ���������
                     (                       // ���������� ��� ESC.
                      const char *msgs,            // ��������� c ������������� '|'.
                      const char *Head,     // ��������� ����
                      int   x,               // ���������� ������,
                      int   y,               // ��� ������������� -1.
                      int   r                // ��������.
                     );

_RSPNLEXP int meswin              // ���� ���������
                    (             // ���������� ��� ESC.
                     const char *msgs       // ��������� c ������������� '|'.
                    );

_RSPNLEXP int meswinh             // ���� ���������
                    (             // ���������� ��� ESC.
                     const char *msgs,      // ��������� c ������������� '|'.
                     int         page       // ����� �������� ������
                    );

_RSPNLEXP int meswinm             // ���� ���������
                    (             // ���������� ��� ESC.
                     const char *msgs,      // ��������� c ������������� '|'.
                     const char  *mname,    // ��� msg-�����               | ��� ������
                     int          mnumber   // ����� ��������� � msg-����� | �������� ������
                    );

_RSPNLEXP int formwin(int rf, const char *fmt, ...);

#define  freemsg(list) { free(list[0]); free(list); }

/************************************************************************/
/*                                                                      */
/*                         ���� �������������                           */
/*                                                                      */
/************************************************************************/

typedef struct
      {
       int                  version;  // Now it's 0
       const char * const  *msg;      /* ���������                                    */
       int                  nm;       /* ����� ������� ���������                      */
       const char * const  *btext;    /* ������ ������                                */
       int                  nb;       /* ����� ������                                 */
       int                  ncf;      /* ������� �������                              */
       const char          *Head;
       const char          *Statl;
       int                  infoType;
       long                 errCode;
      } CwinData;


typedef int (*CwinCallBackType)(const CwinData *p);

_RSPNLEXP CwinCallBackType SetCwinCallBack(CwinCallBackType newVal);


_RSPNLEXP const char *webwin(const char *url, const char *attr, const char *caption, const char *status,
                             const char *resField, size_t *outSz);

_RSPNLEXP void freeWebResult(const char *result);


_RSPNLEXP int confwin             /* ������ �������������                         */
                     (
                      const char * const *msg, /* ���������                                    */
                      int   nm,              /* ����� ����� ���������                        */
                      const char * const *btext,          /* ������ ������                                */
                      int   lt,              /* ����� ������                                 */
                      int   nb,              /* ����� ������                                 */
                      int   ncf,             /* ������� �������                              */
                      int   x,               /* ���������� ������,                           */
                      int   y,               /* ��� ������������� -1                         */
                      int   r                /* ��������                                     */
                     );

_RSPNLEXP int cwin               /* ������ �������������                         */
                  (
                   const char * const *msg,            /* ���������                                    */
                   int   nm,              /* ����� ������� ���������                      */
                   const char * const *btext,          /* ������ ������                                */
                   int   lt,              /* ����� ������                                 */
                   int   nb,              /* ����� ������                                 */
                   int   ncf,             /* ������� �������                              */
                   int   x,               /* ���������� ������,                           */
                   int   y,               /* ��� ������������� -1                         */
                   int   r,
                   const char  *Head,
                   const char  *Statl
                  );

_RSPNLEXP int cwinm              /* ������ �������������                         */
                  (
                   const char * const *msg,            /* ���������                                    */
                   int   nm,              /* ����� ������� ���������                      */
                   const char * const *btext,          /* ������ ������                                */
                   int   lt,              /* ����� ������                                 */
                   int   nb,              /* ����� ������                                 */
                   int   ncf,             /* ������� �������                              */
                   int   x,               /* ���������� ������,                           */
                   int   y,               /* ��� ������������� -1                         */
                   int   r,
                   const char  *Head,
                   const char  *Statl,
                   const char  *mname,    // ��� msg-�����               | ��� ������
                   int          mnumber   // ����� ��������� � msg-����� | �������� ������
                  );

_RSPNLEXP int cwinEx               /* ����� ���⢥ত����                         */
                    (
                     const char * const *msg,            /* ����饭��                                    */
                     int   nm,              /* ��᫮ ⥪�⮢ ᮮ�饭��                      */
                     const char * const *btext,          /* ������ ������                                */
                     int   lt,              /* ����� ⥪��                                 */
                     int   nb,              /* ��᫮ ������                                 */
                     int   ncf,             /* ������ �������                              */
                     int   x,               /* ���न���� ������,                           */
                     int   y,               /* ��� 業��஢���� -1                         */
                     int   r,
                     const char *Head,
                     const char *Statl,
                     int   infoType,    // One of RSINFO_...
                     long  errCode
                    );

#define  RSINFO_MESSAGE  0
#define  RSINFO_WARNING  1
#define  RSINFO_ERROR    2
#define  RSINFO_FATAL    3


_RSPNLEXP int fwin              /* ����� ��������� �� ������  */
                  (
                   const char  *msgs,    /* ������ ���������        */
                   PANEL       *p,       /* ������� ������    */
                   const char  *Head,
                   const char  *Statl
                  );

_RSPNLEXP int fwinm             /* ����� ��������� �� ������  */
                  (
                   const char  *msgs,    /* ������ ���������        */
                   PANEL       *p,       /* ������� ������    */
                   const char  *Head,
                   const char  *Statl,
                   const char  *mname,   // ��� msg-�����               | ��� ������
                   int          mnumber  // ����� ��������� � msg-����� | �������� ������
                  );

_RSPNLEXP int yesnowin                /* ������: Yes, No ?                            */
                      (
                       const char *msg,       /* ���������                                    */
                       int   ncf,             /* ������� �������                              */
                       int   x,               /* ���������� ������,                           */
                       int   y                /* ��� ������������� -1                         */
                      );

_RSPNLEXP int PanelSetDialogFlag(int newFlag);
_RSPNLEXP int NoUserActions(void);

_RSPNLEXP int YesNo(const char *msg, int ncf, int page);

_RSPNLEXP int fldwin                        // ����� ��������� �� ������
                    (
                     const char *msgs,      // ������ ���������
                     PANEL *p               // ������� ������
                    );

_RSPNLEXP int fldwinh                       // ����� ��������� �� ������
                     (
                      const char *msgs,     // ������ ���������
                      PANEL      *p,        // ������� ������
                      int         page      // ����� �������� ������
                     );

_RSPNLEXP int invwin    /* ���� ������ ��������                         */
                    (
                     const char   *mes,     /* ������ ����������� <= 75 ��������            */
                     unsigned char FVt,     /* ��� ����                                     */
                     void         *val,     /* ������ �� �������� ��������                  */
                     int           FVp,     /* ������ ����� �����                           */
                     coord         l        /* ������ ���� �����                            */
                    );

_RSPNLEXP int invwinEx  /* ���� ������ ��������                         */
                      (
                       const char   *mes,     /* ������ ����������� <= 75 ��������            */
                       unsigned char FVt,     /* ��� ����                                     */
                       void         *val,     /* ������ �� �������� ��������                  */
                       int           FVp,     /* ������ ����� �����                           */
                       coord         l,       /* ������ ���� �����                            */
                       int      passwd
                      );

_RSPNLEXP int invwinEx2 /* ���� ������ ��������                         */
                       (
                        const char   *mes,     /* ������ ����������� <= 75 ��������            */
                        unsigned char FVt,     /* ��� ����                                     */
                        void         *val,     /* ������ �� �������� ��������                  */
                        int           FVp,     /* ������ ����� �����                           */
                        coord         l,       /* ������ ���� �����                            */
                        int           passwd,
                        int           page     /* �������� ������                              */
                       );

_RSPNLEXP int invwinEx3 // ���� ������ ��������
                       (
                        const char   *mes,     // ������ ����������� <= 75 ��������
                        unsigned char FVt,     // ��� ����
                        void         *val,     // ������ �� �������� ��������
                        int           FVp,     // ������ ����� �����
                        coord         l,       // ������ ���� �����
                        int           passwd,  // �������� ��������
                        int           page,    // �������� ������
                        const char   *head,    // ��������� ����
                        const char   *stLine   // ��������� ������
                       );

_RSPNLEXP int infldval  /* ���� ������ ��������                         */
                      (
                       const char   *Invite,  /* ����������� � ����� �������� ������          */
                       FIELD        *f,       /* ���� ������                                  */
                       void         *v,       /* ����� ��� ��������                           */
                       size_t        sz       /* ������ ������                                */
                      );

_RSPNLEXP int infldval2 /* ���� ������ ��������                         */
                       (
                        const char   *Invite,  /* ����������� � ����� �������� ������          */
                        FIELD        *f,       /* ���� ������                                  */
                        void         *v,       /* ����� ��� ��������                           */
                        size_t        sz,      /* ������ ������                                */
                        int           page     /* �������� ������                              */
                       );

// -----------------------------------------------------------------------------
// ������ � �������������

// ��������� ����������� � ������-������ (������ EasyWin �����)
// [in] head    - ��������� �����������
// [in] text    - ����� �����������
// [in] secID   - ������������� ������ ������-������, ��� �������� ��������� ����������� (������)
// [in] iconID  - ������������� ������
// [in] timeOut - ����-��� (���)
// [ret]        - �� ������������
_RSPNLEXP int RSSendNotify(const char *head, const char *text, int secID, int iconID, int timeOut);

// ��������� ����������� � ������-������ (������ EasyWin �����)
// [in] head      - ��������� �����������
// [in] text      - ����� �����������
// [in] secID     - ������������� ������ ������-������, ��� �������� ��������� ����������� (������)
// [in] iconID    - ������������� ������
// [in] timeOut   - ����-��� (���)
// [in] colorText - ���� ������
// [in] colorBack - ���� ����
// [ret]          - �� ������������
_RSPNLEXP int RSSendNotifyEx(const char *head, const char *text, int secID, int iconID, int timeOut, int colorText, int colorBack);

// -----------------------------------------------------------------------------
//*********************** ��������� ����������. *************************

#define  MAX_INDIC  3

// ��������� ���������.
// [in] Nzp    - ����� �������.
// [in] Status - ��������� � ������.
// [in] Head   - ���������.
_RSPNLEXP int PutIndic(long Nzp, const char *Status, const char *Head);

// �������� ���������.
// [in] Nop - ���������� �������.
_RSPNLEXP void UseIndic(long Nop);

// ������ ���������.
_RSPNLEXP void RemIndic(void);

// -----------------------------------------------------------------------------
// ��������� � ������.
// [in] mes - ����� ���������.
_RSPNLEXP char *PutWarn(const char *mes);

// ������ ���������.
// [out] wsave - ������� ���������� �������.
_RSPNLEXP void RemWarn(char *wsave);


// -----------------------------------------------------------------------------
// ��������� ������������� ��� ���������������� ������ ������-����.
#define  ID_USER_PANE  100


// ������� ����� � ������-���
// ���� ������ � ����� ��������������� �����������, ��� ����� ���������.
// [in] id   - ������������� ������ ������-���� (��� ���������������� ������ >= ID_USER_PANE)
// [in] text - ����� ��������� � ������
// [in] tips - ����� ����������� ��������� � ������
_RSPNLEXP void PutStatusPane   (int id, const char *text, const char *tips);

// ������� ����� � ������-���
// ���� ������ � ����� ��������������� �����������, ��� ����� ���������.
// [in] id    - ������������� ������ ������-���� (��� ���������������� ������ >= ID_USER_PANE)
// [in] text  - ����� ��������� � ������
// [in] tips  - ����� ����������� ��������� � ������
// [in] flags - ����� ���� EW_SBT_
_RSPNLEXP void PutStatusPaneEx(int id, const char *text, const char *tips, int flags);

// �������� ����� � ������-����
// ��������� ����� � ������ � �������� ���������������. ������ � �������� ���������������
// ������ ������������.
// �������� ���������� - ���������� PutStatusPane.
_RSPNLEXP void UpdateStatusPane(int id, const char *text, const char *tips);

// �������� ����� � ������-����
// ��������� ����� � ������ � �������� ���������������. ������ � �������� ���������������
// ������ ������������.
// �������� ���������� - ���������� PutStatusPaneEx.
_RSPNLEXP void UpdateStatusPaneEx(int id, const char *text, const char *tips, int flags);

// ������� ������ � �������� ��������������� �� ������-����
// [in] id   - ������������� ������ ������-����
_RSPNLEXP void RemStatusPane(int id);

// ������� ������ � ������-���
// ���� ������ � ����� ��������������� �����������, ��� ����� ���������.
// [in] id   - ������������� ������ ������-���� (��� ���������������� ������ >= ID_USER_PANE)
// [in] icon - ������������� ������
// [in] tips - ����� ����������� ��������� � ������ (����������� NULL � "")
_RSPNLEXP void PutStatusIcon(int id, int icon, const char *tips);

// �������� ������ � ������-����
// ��������� ������ � ������ � �������� ���������������. ������ � �������� ���������������
// ������ ������������.
// �������� ���������� - ���������� PutStatusIcon.
_RSPNLEXP void UpdateStatusIcon(int id, int icon, const char *tips);


_RSPNLEXP int MouseInPanel(PANEL *p);
_RSPNLEXP int MouseInMenuPanel(PANEL *p);

_RSPNLEXP int MouseInScrol(SCROL *mn);

_RSPNLEXP int MouseInFTreeItem(PANEL *p, FIELD *f);

_RSPNLEXP int MouseInFTreeBox(PANEL *p, FIELD *f);

_RSPNLEXP int MouseInField(PANEL *p, FIELD *f);

_RSPNLEXP int IndMouseField(PANEL *p);

_RSPNLEXP int HorNav(PANEL *p, int key);

_RSPNLEXP void NoMem(void);   // ��������� "��� ������ ������� ESC".

// -----------------------------------------------------------------------------
_RSPNLEXP extern int CtrlBreakFlag;

_RSPNLEXP void SetCtrlBrk(void);

_RSPNLEXP void ResetCtrlBrk(void);

_RSPNLEXP int  IsCtrlBrk(int mouse);
_RSPNLEXP int  TestCtrlBrk(void);
_RSPNLEXP void ClearCtrlBrk(void);

_RSPNLEXP int  ForceSetCtrlBrk(int newFlag);
_RSPNLEXP void SendCtrlBrk(void);
_RSPNLEXP void SendKeyEvent(int key);


enum
   {
    RSCONFIRM_LEAVEEDIT = 1,
    RSCONFIRM_PRINTREP  = 2
   };

_RSPNLEXP void          SetConfirmFlags(unsigned long);
_RSPNLEXP unsigned long GetConfirmFlags(void);



/*   January 15,1998 (SD)
    ��������� ����������
*/
typedef int (*ctrlbrk_idle_proc)(void);
_RSPNLEXP extern ctrlbrk_idle_proc CtrlBrkIdle;

// ����������� CallBack-������� ��� ����������� CtrlBrk
// [in]   flagUserChoise - �������� ������ ������������ (YES/NO) � ����� �� ������
//                         "�� ������������� ������ �������� ������?"
// [ret]  0 - �� ��������� ������� �������� (��������� �������� ���������������)
typedef int (*ctrlbrk_idle_proc_ex)(int flagUserChoise);

// ��������� ����� CallBack-������� (���������� ���������� ��������)
_RSPNLEXP ctrlbrk_idle_proc_ex SetCtrlBrkEx(ctrlbrk_idle_proc_ex newHandler);

// -----------------------------------------------------------------------------
// ��������� ��������� ���� � ������
_RSPNLEXP TEXT *FInsertT
                        (
                         PANEL *p,
                         int    St,       // ����� ����������� ����
                         char  *TValue,   // �������� � ����
                         coord  x,        // ������������� ���������� ����
                         coord  y
                        );

// -----------------------------------------------------------------------------
// ��������� ������������� � ������
_RSPNLEXP BAR *InsertB
                      (
                       PANEL *p,
                       int    St,         // ����� ����������� ����
                       coord  x,          // ������������� ���������� ��������������
                       coord  y,
                       coord  l,
                       coord  h,
                       coord  fl          // ��� ����� (��. enum BORD � fs.h)
                      );

// -----------------------------------------------------------------------------
// ��������� ���� � ������
_RSPNLEXP FIELD *FInsertF
                         (
                          PANEL         *p,
                          unsigned char  Ftype,          // ��� ����
                          unsigned int   St,             // ����� ����������� ����
                          unsigned char  FVt,            // ��� ��������
                          void          *FValue,         // �������� � ����
                          int            FVp,            // ������ ����� �����, ����. ����� ������
                          coord          x,              // ������������� ���������� ����
                          coord          y,
                          coord          l,
                          coord          h,
                          coord          kl,             // ������� ����� ��� ���������
                          coord          kr,
                          coord          ku,
                          coord          kd,
                          int            FHelp           // ��������� � ����
                         );

_RSPNLEXP PANEL *CreateP
                        (
                         unsigned int    St,      // ����� ����������� ������
                         unsigned char   Pnumf,   // ���������� ����� ������
                         unsigned char   Pnumt,   // ���������� ������� � ������
                         coord           x1,      // ���������� ������
                         coord           y1,      // ���������� ������
                         coord           x2,      // ���������� ������
                         coord           y2,      // ���������� ������
                         int             PHelp,   // ��������� � ������
                         unsigned char   Pff,     // ����� ���������� ����
                         unsigned        adsize
                        );

_RSPNLEXP PANEL *FCreateP
                         (
                          unsigned int    St,      // ����� ����������� ������
                          unsigned char   Pnumf,   // ���������� ����� ������
                          unsigned char   Pnumt,   // ���������� ������� � ������
                          coord           x1,      // ���������� ������
                          coord           y1,      // ���������� ������
                          coord           x2,      // ���������� ������
                          coord           y2,      // ���������� ������
                          panel_keyproc_t proc,    // ���������� ����������
                          int             PHelp,   // ��������� � ������
                          void           *Parm,    // ��� �������� ����������
                          unsigned char   Pff,     // ����� ���������� ����
                          const char     *Pstatus, // ���������� ��� ������ ���������
                          const char     *Phead    // ���������
                         );

_RSPNLEXP SCROL *CreateS
                        (
                         unsigned int    St,      // ����� ����������� ������
                         unsigned char   Pnumf,   // ���������� ����� ������
                         unsigned char   Pnumt,   // ���������� ������� � ������
                         coord           x1,      // ���������� ������
                         coord           y1,      //
                         coord           x2,      //
                         coord           y2,      //
                         int             PHelp,   // ��������� � ������
                         unsigned char   Pff,     // ����� ���������� ����
                         int             Mn,      // ���������� ������� � �������
                         coord           x,       // ���������� �������
                         coord           y,
                         coord           l,
                         coord           h,
                         unsigned        adsize
                        );

_RSPNLEXP SCROL *FCreateS
                         (
                          unsigned int    St,      // ����� ����������� ������
                          unsigned char   Pnumf,   // ���������� ����� ������
                          unsigned char   Pnumt,   // ���������� ������� � ������
                          coord           x1,      // ���������� ������
                          coord           y1,      // ���������� ������
                          coord           x2,      // ���������� ������
                          coord           y2,      // ���������� ������
                          panel_keyproc_t proc,    // ���������� ����������
                          int             PHelp,   // ��������� � ������
                          void           *Parm,    // ��� �������� ����������
                          unsigned char   Pff,     // ����� ���������� ����
                          const char     *Pstatus, // ���������� ��� ������ ���������
                          const char     *Phead,   // ���������.
                          int             Mn,      // ���������� ������� � �������
                          coord           x,       // ���������� �������
                          coord           y,
                          coord           l,
                          coord           h,
                          panel_recproc_t rec,     // ��������� ������������� �������
                          void           *Ms,      // ������ �������
                          int             Mnumfl   // ���������� �������
                         );

// -----------------------------------------------------------------------------

#include <packpsh1.h>

// -----------------------------------------------
typedef struct PanelR_0
      {
       db_uint16    St;
       uchar        Pnumf, Pnumt, Nb, Nt;
       r_coord      x1, y1, x2, y2;
       db_int16     PHelp;
       uchar        Pff;

       db_int16     Mn;
       r_coord      x, y, l, h;

       db_int16     len;
      } PanelR_0;

// -----------------------------------------------
typedef struct PanelR_1
      {
       db_uint16    St;
       uchar        Pnumf, Pnumt, Nb, Nt;
       r_coord      x1, y1, x2, y2;
       db_int16     PHelp;
       uchar        Pff;

       db_int16     Mn;
       r_coord      x, y, l, h;

       db_int16     len;

       db_uint32    flags;  // RFP_... flags
      } PanelR_1;

// -----------------------------------------------
typedef struct PanelR
      {
       db_uint16    St;
       uchar        Pnumf, Pnumt, Nb, Nt;
       r_coord      x1, y1, x2, y2;
       db_int16     PHelp;
       uchar        Pff;

       db_int16     Mn;
       r_coord      x, y, l, h;

       db_int16     len;

       db_uint32    flags;    // RFP_... flags
       db_uint32    flagsEx1, // ������
                    flagsEx2; // ������
      } PanelR;

// -----------------------------------------------
typedef struct FieldR_0
      {
       uchar          Ftype;
       db_uint16      St;
       uchar          FVt;
       db_int16       FVp;
       r_coord        x, y, l, h;
       r_coord        kl, kr, ku, kd;
       db_int16       FHelp;

       r_coord        vfl, lens;
      } FieldR_0;

// -----------------------------------------------
typedef struct FieldR_1
      {
       uchar          Ftype;
       db_uint16      St;
       uchar          FVt;
       db_int16       FVp;
       r_coord        x, y, l, h;
       r_coord        kl, kr, ku, kd;
       db_int16       FHelp;

       r_coord        vfl, lens;


       r_coord        nameLen, formLen;
       db_uint32      flags;
       db_int16       group;
      } FieldR_1;

// -----------------------------------------------
typedef struct FieldR
      {
       uchar          Ftype;
       db_uint16      St;
       uchar          FVt;
       db_int16       FVp;
       r_coord        x, y, l, h;
       r_coord        kl, kr, ku, kd;
       db_int16       FHelp;

       r_coord        vfl, lens;


       r_coord        nameLen, formLen;
       db_uint32      flags;
       db_int16       group;
       r_coord        tooltipLen;         // ����� ������ ����������� ���������
      } FieldR;

// -----------------------------------------------
typedef struct TextR
      {
       db_uint16      St;
       r_coord        x, y;

       r_coord        vfl, lens;
      } TextR;

// -----------------------------------------------
typedef struct BordR
      {
       db_uint16  St;
       r_coord    x, y, l, h, fl;
      } BordR;

#include <packpop.h>

// -----------------------------------------------------------------------------

_RSPNLEXP int CrItem(int hd, PANEL *p, PanelR *pp, char *s, int ver, HRSLCVT hcvtRd, bool readName2);

_RSPNLEXP PANEL *CrPan            // ����������� PANEL.
                      (
                       ResFile    *rt,       // �������� ������.
                       const char *member    // ��� �������.
                      );

_RSPNLEXP SCROL *CrScrl            // ����������� SCROL.
                       (
                        ResFile    *rt,       // �������� ������.
                        const char *member    // ��� �������.
                       );

// ��������� SCROL.
_RSPNLEXP int GetScrol(ResFile *rf, void **pv);

_RSPNLEXP int ShowFV(const char *Fname, const char *Title);

_RSPNLEXP void SetFldsVa
                        (
                         PANEL          *p,
                         panel_keyproc_t proc,     // ���������� ����������
                         void           *Parm,     // ��� �������� ����������
                         void           *Pstatus,  // ���������� ��� ������ ���������
                         const char     *Phead,    // ���������.
                         va_list ap
                        );

_RSPNLEXP void SetFlds
                      (
                       PANEL          *p,
                       panel_keyproc_t proc,       // ���������� ����������
                       void           *Parm,       // ��� �������� ����������
                       void           *Pstatus,    // ���������� ��� ������ ���������
                       const char     *Phead,      // ���������.
                       ...
                      );

_RSPNLEXP void SSetFlds
                       (
                        SCROL          *m,
                        panel_keyproc_t proc,      // ���������� ����������
                        void           *Parm,      // ��� �������� ����������
                        void           *Pstatus,   // ���������� ��� ������ ���������
                        const char     *Phead,     // ���������.
                        panel_recproc_t rec,       // ��������� ������������� �������
                        void           *Ms,        // ������ �������
                        int             Mnumfl     // ���������� �������
                       );

_RSPNLEXP int MouseInField(PANEL *p, FIELD *f);

_RSPNLEXP int IndMouseField(PANEL *p);

_RSPNLEXP void setstyle           // ������������� ������
                       (
                        int mode,              // ����������
                        int mnt      // ��� ��������
                       );

// -----------------------------------------------------------------------------
// �������� �����
enum CLGET
   {
    CLBAR,          // ����� � �������������� �����
    CLTXT,          // �����
    CLSHW,          //
    CLSLB,          //
    CLBTB,          // ������������ ������ (������� ����)
    CLBSN,          // ������ � �����
    CLBSR,          // ������ � ����� ���������
    CLBTN,          // ��������������� ����
    CLBTR,          // ��������������� ���� ���������
    CLSCN,          //
    CLFLN,          // ������������� ����
    CLFLR,          // ������������� ���� ���������
    // New styles
    CLBDS,          // �������� ������
    CLDIS,          // �������� ����

    // ��������� ���������� � ���������
    CL_KEYWORD,     // �������� �����
    CL_TYPE,        // ���� ������
    CL_STDFUNC,     // ����������� ���������
    CL_NUMBER,      // �������� ���������
    CL_OPER,        // ���������
    CL_STRING,      // ������
    CL_SPECNAME,    // ����������� �����
    CL_COMMENT      // �����������
   };

// -----------------------------------------------------------------------------
// ����� �������
#define  SMES  0    // ����� ���������
#define  SRMS  1    // ����� ��������� ���������
#define  SHLP  2    // ����� ������
#define  SMEN  3    // ����� ����
#define  SCOM  4    // �������� �����
#define  SBCM  5    // ����� � ����� ������
#define  SCRL  6    // ����� ���������� �����������

#define  SUSR  7    // ���������������� �����

// ��������������� ������ ��� ��������� �����
// � ����������� ������������ � ���� St = ReS(SMEN, 0). � ���������� ���� ����������� ��������
// � ���� St = ReS(SMEN, St) � St = ReS(0, SHLP), � St = ReS(CLTXT, SHLP).
// ���� ����������, ��� st - ��� �����, � sp - ��������� ����� (CLGET).
#define  ReS(sp, st)  (((sp + 1) << 8) | st)

// �������� �� ����� ������� ��������� ����������
// [in] sp  - ����� ������ (SMES � �.�.)
// [in] cmd - ��������� ����� (CLTXT)
// [in] st  - ����� ���������� ������ (������ 0)
_RSPNLEXP unsigned char GetCl   (unsigned sp, int cmd, unsigned st);


_RSPNLEXP unsigned char GetHotCl(unsigned sp, int cmd, unsigned st);
_RSPNLEXP unsigned char GetBord (unsigned sp);

_RSPNLEXP void ErRes(int iErr, const char *szMember);

/*   10.31.94 (K78)
*/
_RSPNLEXP int GetFieldSize   (int FVt, int FVp);
_RSPNLEXP int PanelSetFocus  (PANEL *p, int disp);
_RSPNLEXP int FindFirstActiv (FIELD *f, int n);
_RSPNLEXP int JPanelSetFocus (PANEL *p, int disp, int d);

// 1996 (JNS) You can replace right mouse button function assigning this
//           variable. It's ESC by default
_RSPNLEXP extern int  rightMouseButton;

_RSPNLEXP int ProcSNRTemplate(char *s);

_RSPNLEXP char *SetSNRTemplate(char *templ);
_RSPNLEXP int   SetMaxSNRLen(int len);
_RSPNLEXP int   SetSNRPos(int pos);


// -----------------------------------------------------------------------------
// ��������� ��� �������� CheckBox
enum
   {
    eCB_ERROR     = -1,     // ������ (�������� ����� ���� ��� ��� ��������)
    eCB_UNCHECKED =  0,     // �� �������
    eCB_CHECKED,            // �������
    eCB_UNDEFINE            // ����������� (��� CheckBox � ����� �����������)
   };


// ���������� ������� ������ ��� �������� CheckBox
// �������� � ������� ����� ������. ��������� ����������� ����� ���������
// UNCHECKED -> CHECKED ��� �������� CheckBox'�, � UNCHECKED -> CHECKED -> UNDEFINE
// ��� CheckBox'� � ����� �����������.
_RSPNLEXP void Press_Space(PANEL *p);

// ���������� ����� ������ CheckBox'�
// [in] p          - ��������� �� ������
// [in] cf         - ����� ����
// [in] flag3state - ���� ������:
//                   false - � ����� ����������� (�������)
//                   true  - � ����� �����������
// [ret]           - true  - ����� ������� ����������
//                   false - ������ ��������� ������
_RSPNLEXP bool SetCheckBoxMode(PANEL *p, int cf, bool flag3state);

// �������� ��������� CheckBox'�
// [in] p          - ��������� �� ������
// [in] cf         - ����� ����
// [ret]           - ��������� ���� eCB_
_RSPNLEXP int GetCheckBoxState(PANEL *p, int cf);

// ���������� ��������� CheckBox'�. ���������� ���������� ���������.
// [in] p          - ��������� �� ������
// [in] cf         - ����� ����
// [in] state      - ��������� ���� eCB_
// [ret]           - ��������� ���� eCB_
_RSPNLEXP int SetCheckBoxState(PANEL *p, int cf, int state);


void RslSetAppServSignal(void);

// -----------------------------------------------------------------------------
// ���������� ���������������� ���������� ��������� ���� FSCALLBACK_.
// ���������� ��������� �� ���������� ���������������� ����������.
// ���������������� ���������� ���������� ������. ���� �� ������ �������� -1,
// ����������� ���������� ������ �� �����.
_RSPNLEXP fs_callback_t SetUserFSCallback(fs_callback_t proc);

// -----------------------------------------------------------------------------
_RSPNLEXP extern panel_keyproc_t  msgPtr;


// MSEL

// ������������� ������ ��� ������ ���������
_RSPNLEXP void InitMessageList(char **log);

// �������� ������ � ������ ���������
_RSPNLEXP int  AddMessageList(char *log, char *string);

// ���������� ������ ���������
_RSPNLEXP void FreeMessageList(char **log);

// �������� ������ ���������
_RSPNLEXP void ShowMessageList(char *head, char *log);


/* ������� ��������� ���� ������ */

_RSPNLEXP void SetHorMenu   (PANEL *p, MENU *m, int NumItems);
_RSPNLEXP void SetResHorMenu(PANEL *p, ResFile *rf, const char *member);

_RSPNLEXP int SetAddMenu   (PANEL *p, MENU *m, int NumItems, int key, const char *name);
_RSPNLEXP int SetResAddMenu(PANEL *p, ResFile *rf, const char *member, int key, const char *name);

void ClearAddMenu(PANEL* p);


/* ������� ������������ ������, ���������� ��� ����, ���������� �� ��������
   ��� ������ ������� SetResHorMenu. ������ ���� ������ ��������. */
_RSPNLEXP void FreeResHorMenu(PANEL *p);

/* ������� ��� ��������� ��������� ������
������ ����,  �������������� � ����������,  */
_RSPNLEXP int SetMenuItemFlag
                             (
                              MENU         *m,      // ��������� �� �������������� ����
                              unsigned char n,      // ���������� ������� � �������������� ����
                              int           key,    // ��� ������� �������, ��������������� ������ ���� (�.�.0)
                              const char   *name,   // �������� ������ ����, (�.�.NULL, ��� ������� key!=0)
                              int           flag,   // ������, ������� ����� ����������
                              int           active  // ON / OFF
                             );

_RSPNLEXP int SetPanMenuFlag  // ���������� ���� ������ ����, �����. � ������
                            (
                             PANEL      *p,      // ��������� �� ������, � ������� ����������� ����, ��� �� ������ ����
                             int         key,    // ��� �������, �������������� ������ ���� (�.�.0)
                             const char *name,   // ��� ������ ���� (�.�.NULL, ��� ������� key!=0)
                             int         flag,   // ������, ������� ����� ����������
                             int         active  // ON / OFF
                            );

// �������� ����� ����
_RSPNLEXP int HideMenuItem
                          (
                           PANEL      *p,       // ��������� �� ������, � ������� ����������� ����, ��� �� ������ ����
                           int         key,     // ��� �������, �������������� ������ ���� (�.�.0)
                           const char *name,    // ��� ������ ���� (�.�.NULL, ��� ������� key!=0)
                           int         active   // ON(�����) / OFF(�������)
                          );

// ���������� ������ ��� ������ ����
// [in]  p      - ��������� �� ������, � ������� ����������� ����, ��� �� ������ ����
// [in]  key    - ��� �������, �������������� ������ ���� (�.�.0)
// [in]  name   - ��� ������ ���� (�.�. NULL, ��� ������� key != 0)
// [in]  iconID - ������������� ������
// [ret]        - ���������� �������� �������������� ������
_RSPNLEXP int IconMenuItem(PANEL *p, int key, const char *name, int iconID);

// ���������� ����� ��������� ��� ������ ���� (ToolBar, ����������� ���� � �.�.)
// [in]  p      - ��������� �� ������, � ������� ����������� ����, ��� �� ������ ����
// [in]  key    - ��� �������, �������������� ������ ���� (�.�.0)
// [in]  name   - ��� ������ ���� (�.�. NULL, ��� ������� key != 0)
// [in]  flags  - ����� ��������� ������ ���� (��. ��������� ���� dFMENU_)
// [ret]        - ���������� �������� �����
_RSPNLEXP int FlagsMenuItem(PANEL *p, int key, const char *name, int flags);

// ������� ����� ���� �����������
_RSPNLEXP int DisableMenuItem
                             (
                              PANEL      *p,      // ��������� �� ������, � ������� ����������� ����, ��� �� ������ ����
                              int         key,    // ��� �������, �������������� ������ ���� (�.�.0)
                              const char *name,   // ��� ������ ���� (�.�.NULL, ��� ������� key!=0)
                              int         active  // ON(��������) / OFF(����������)
                             );


// -----------------------------------------------------------------------------
// ���������� ��� ���������� �������� � �������� ������� ��� ������
// [out] p       - ������
// [in]  lbrName - ������������ ���������� ��������
// [in]  resName - ������������ �������
_RSPNLEXP void SetResNamePanel(PANEL *p, const char *lbrName, const char *resName);

// ���������� ��� ���������� �������� � �������� ����
// [out] p       - ������ ����
// [in]  lbrName - ������������ ���������� ��������
// [in]  resName - ������������ �������
// [in]  toPanel - false: ��������� ������������ � p->HorMenu.rm
//                 true:  ���� ��������� p->HorMenu.rm �����������, ��������� ����� �����������
//                        ��� ������
_RSPNLEXP void SetResNameMenu(PANEL *p, const char *lbrName, const char *resName, bool toPanel);

// ���������� ��� ���������� �������� � �������� ���� �� ���� itemsyst.dbt/opermenu.dbt
// ���������� ���������� SetResNameMenu � ����������������� ������� D_MENUBTRLBRNAME � D_MENUBTRRESNAME
_RSPNLEXP void SetResNameOperMenu(PANEL *p);

// -----------------------------------------------------------------------------
_RSPNLEXP extern MENU vVarMenuPan[4];

_RSPNLEXP extern MENU vVarMenuScrol[11];
_RSPNLEXP extern MENU vVarMenuScrol_NoSel[10];

_RSPNLEXP extern MENU vVarMenuBs[15];
_RSPNLEXP extern MENU vVarMenuBs_NoSel[10];

#ifdef SQLBUILD
    _RSPNLEXP extern MENU vVarMenuBsA[17];
    _RSPNLEXP extern MENU vVarMenuBs_NoSelA[12];
#else
    _RSPNLEXP extern MENU vVarMenuBsA[16];
    _RSPNLEXP extern MENU vVarMenuBs_NoSelA[11];
#endif

// L������������� ����v ���������� ���� ����� ����v� ��������������
// vVarMenuBsA, vVarMenuBs_NoSelA.
_RSPNLEXP void InitVarMenu(void);

// ���������� ���������� ������ (������������ ������ � �����������)
int PanProc(PANEL *p, int key );


// ����� ����� ������������� ���� (������������ ������ � �����������)
MENU *FindMenuItem
                  (
                   const MENU   *m,              // ��������� �� ������������ ����
                   unsigned char n,              // ���������� ������� ����
                   int           key,            // ������� ������� (�.�. == 0)
                   const char   *name            // �������� ���� key == 0
                  );

// ����� ����� � ����, ������������� � ������
_RSPNLEXP MENU *FindPanMenuItem(PANEL *p, int key, const char *name);

// �������� �������� ������� ���� (������������ ������ � �����������)
_RSPNLEXP int CmpMenItemName(const char *name1, const char *name2);

// ���������� ���� "������" (������������ ������ � �����������)
_RSPNLEXP int SetVarMenu(PANEL *p, MENU *m, int n, MENU *vm, unsigned char vn);

// ��������� ��������� � ������ � ���� ��� ������ �� ������
_RSPNLEXP extern int SendFldMesOut(PANEL *p);

int SendFldMesIn(PANEL *p);


int FindMenuKeyAct(MENU *m, unsigned char n, int key);

/* ������� ������������ ������, ���������� ��� ������.
   ����������� ������ ��������������, ���� � ������ ����������� ����, ����������� �� �������  */
_RSPNLEXP void FreePanel(PANEL *p);

/* ������� ������������ ������, ���������� ��� ���������� */
_RSPNLEXP void FreeScrol(SCROL *s);


/************************ ������ SCROL ***********************************/
typedef int (*ScrollPrint)(SCROL *, const char *);

_RSPNLEXP void SetPrintS(ScrollPrint);         // ���������� ������
_RSPNLEXP void SetPrintSXml(ScrollPrint);      // ���������� ������� � XML

_RSPNLEXP void SetPrnDirS(const char *dir);    // ���������� ���.��� ������ � ������������� ����������

/************* �������������� ����������� ������ ������� *****************/

#define AUTO_LEN_SIZE  100

_RSPNLEXP int SetAutoWidth(int ind, int w);
_RSPNLEXP coord getAutoLen(FIELD *f);
_RSPNLEXP int SetFmtByW(int, const char *);
_RSPNLEXP int SetFmtByType(unsigned char, const char *);
_RSPNLEXP const char *GetFmtByType(unsigned char chType);
_RSPNLEXP const char *GetFmtByField(FIELD *);

const char *GetFmtByW(FIELD *);
const char *GetFmtByFldType(FIELD *);


void CreateFormatsForPanel(PANEL *);
void DeleteFormatsForPanel(PANEL *);

void RecreateFormatsByType(PANEL *);
_RSPNLEXP void RecreateFormatsForField(PANEL *, FIELD *);

void AllocFieldXData   (PANEL *p);
void FreeFieldXData    (PANEL *p);
void PreAllocFieldXData(PANEL *p, int numf);

/*************** ��������� ��������������� ���������� *********************/

int FirstActiv(PANEL *p);
int FirstVisible(PANEL *p);
int LastVisible(PANEL *p);
int FindFirstField(PANEL *p, int lastFld, int len);
int FieldVisible(coord l, FIELD *f, coord startX, coord *len);
coord SetUpFields(PANEL *p);

/****************************************************************************/

int  SIsFromMpiMode(SCROL *s);
void SUnsetFromMpiMode(SCROL *s);

int  SIsFromMpeMode(SCROL *s);
void SUnsetFromMpeMode(SCROL *s);

int ScrStandardProc(PANEL *p, int key);

// -----------------------------------------------------------------------------
_RSPNLEXP void SSetBlkPostRec(int val);  // ��������� ������� ��� ���������� � ��������� ���������� POSTREC
_RSPNLEXP void SSetDblStrHead(int val);  // ���������/���������� �������� �������� ���� �� ������ ������

_RSPNLEXP PANEL *ClonePanel  (PANEL *p, bool usePanelMenu);
_RSPNLEXP int    ResizePanel (PANEL *p, int l, int h, int flag);
_RSPNLEXP void   RemoveBorder(PANEL *p);

_RSPNLEXP MENU *CloneMenu    (const MENU *m, int n);
_RSPNLEXP MENU *CreateDynMenu(const MENU *m, unsigned char *n, ADDMENU *AddMenuInf);

// -----------------------------------------------------------------------------
extern int  SBlockPostRec;

int compleft(FIELD *cf, FIELD *nf, FIELD *f);
int compup  (FIELD *cf, FIELD *nf, FIELD *f);
int compdown(FIELD *cf, FIELD *nf, FIELD *f);

// ���������� / �������������� ���������� (������ ������� ����������)
typedef int (*SaveRestParm)
      (
       PANEL   *mp,               // ��������� �� ������ ������ ����������
       void    *userParm,         // ��������� �� ����� � �����������
       size_t  *userParmSize,     // ������ ������
       int      saveRestOperation // value from SaveRestOperationEnum
      );

enum SaveRestOperationEnum
   {
    restoreColumnWidth =   0,   saveColumnWidth,
    restoreFilterData  = 100,   saveFilterData,
    restoreSortData    = 200,   saveSortData
   };

_RSPNLEXP void SetBsSaveParm  (SaveRestParm);
_RSPNLEXP void SetScrlSaveParm(SaveRestParm);

_RSPNLEXP void setHideField(PANEL *p, int nColumn, unsigned int nWidth);

_RSPNLEXP void setChngField(PANEL *p);

const char * const *WrapStringList(const char * const *msg, int num, int outlen, int height, int *newNum);
void FreeWrapList(const char * const *msg, int num);

_RSPNLEXP void PutIndAct(const char *msg);
_RSPNLEXP void RemIndAct(void);


#define  RS_ACTION_NOCANCEL       1
#define  RS_ACTION_CLEAR_ALWAYS   0
#define  RS_ACTION_CLEAR_WAIT     1

_RSPNLEXP void RsBeginActionEx2(unsigned tm, const char *msg, const char *header, unsigned flags, bool useAnsi);
_RSPNLEXP void RsBeginActionEx (unsigned tm, const char *msg, unsigned flags);
_RSPNLEXP void RsBeginAction   (void);
_RSPNLEXP void RsEndAction     (void);
_RSPNLEXP void RsEndActionEx   (unsigned tm);
_RSPNLEXP void RsClearAction   (int flag);
_RSPNLEXP void RsChangeAction  (const char *msg);

void RsPutActionIndic   (void *data);
void RsRemActionIndic   (void *data);
void RsChangeActionIndic(void *data);


int TransferField(PANEL *Mps, int Index, int toIndex);

int changeFieldNav(PANEL *p, int N, int toN);
int restFieldNav  (PANEL *p, db_int16 *buf);

void InitDelayedUpdate(void);
void DoneDelayedUpdate(void);

bool IsDelayedUpdateTexts (PANEL *);
bool IsDelayedUpdateFields(PANEL *);

void UpdatePanelTexts (PANEL *);
void UpdatePanelFields(PANEL *);

bool UpdateStatLine      (void);
bool UpdateDelayedPanel  (void);
bool UpdateDelayedActions(void);

_RSPNLEXP void EnableDelayedUpdate (PANEL *pnl);
_RSPNLEXP void DisableDelayedUpdate(PANEL *pnl, int update);


_RSPNLEXP PANEL *GetActivePanel(void);
PANEL *SetActivePanel(PANEL *pnl);

void BPutFiltFlag(PANEL *p);
void BDelFiltFlag(PANEL *p);

PANEL_EX *CreatePXdata(PANEL *p);

_RSPNLEXP int  AddPanelSection(PANEL *p, int x, int cx, int align, bool visible);
_RSPNLEXP void SetPanelSection(PANEL *p, int x, const char *str);

_RSPNLEXP void SetPanelSectWd(PANEL *p, int x, int cx);
_RSPNLEXP void ClearAllSect  (PANEL *p);

_RSPNLEXP void PutPanelSection (PANEL *p, int x);
_RSPNLEXP void RemPanelSection (PANEL *p, int x);
_RSPNLEXP void PutPanelTextSect(PANEL *p, int x);

void PutAllSect(PANEL *p);

int   CheckFdataCondition(FDATA *fd);
char *ConvertFdataToSqlString(FDATA *fd, const char *fldname, char *buf, size_t size);

_RSPNLEXP MaskMatchingProc     SetMaskMatchingProc    (MaskMatchingProc     proc);
_RSPNLEXP ConvertMaskToSqlProc SetConvertMaskToSqlProc(ConvertMaskToSqlProc proc);

int UseMaskMatching(void);

// ��������� � �����/������ �� ������
int  SExitRec   (SCROL *s);
void SEntIntoRec(SCROL *s);

void PutFiltFlag(PANEL *p);
void DelFiltFlag(PANEL *p);

int  SelMake  (SCROL *s);
int  SelProc  (PANEL *p, int key);
void SelMouse (SCROL *s, int oldcr);

// -----------------------------------------------------------------------------
typedef int (*UnlockInputProc)(void *data);

_RSPNLEXP void LockUserInput(int flags, UnlockInputProc unlockProc, void *data);
_RSPNLEXP void UnlockUserInput(void);


_RSPNLEXP bool IsBsEditMode(PANEL *p);
void SetBsEditMode  (PANEL *p);
void UnsetBsEditMode(PANEL *p);

MENU *GetMenuPtrFrom(void *mn, int *num);

// ��������� ������ (���������� ����������)
// ��������� �������� � ��� BSCROL::Mp
void CenterPanelConsole(PANEL *p);

// ��������� ������ ��������� ��� ToolBar'�
// 1 (true)  - ���������� ����� ���������
// 0 (false) - ����� ����� ���������
_RSPNLEXP int RsSetBusy(int busy);

#ifdef __cplusplus
};
#endif

#include <packpop.h>

#endif

/* <-- EOF --> */