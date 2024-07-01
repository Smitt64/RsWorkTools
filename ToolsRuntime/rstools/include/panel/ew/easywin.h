//-*--------------------------------------------------------------------------*-
//
// File Name   : easywin.h
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// February 22,2001  Alexey Lebedev - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __EASYWIN_H
#define __EASYWIN_H

#include <string>

#ifndef _INC_WINDOWS
#define  STRICT
#include <windows.h>
#endif

#include <commctrl.h>
#pragma hdrstop

#include <vector>

#include "menu.h"
#include "listobj.h"

#ifdef UNICODE
    #define  rsmemset(p, ch, count)  wmemset(p, ch, count)
#else
    #define  rsmemset(p, ch, count)  memset(p, ch, count)
#endif


#ifndef WC_EDIT
    #define  WC_EDIT            "EDIT"
#endif

#ifndef WC_BUTTON
    #define  WC_BUTTON          "BUTTON"
#endif

#ifndef WC_STATIC
    #define  WC_STATIC          "STATIC"
#endif

#ifndef ICON_SMALL
    #define  ICON_SMALL         0
#endif

#ifndef ICON_BIG
    #define  ICON_BIG           1
#endif

#ifndef WM_MOUSEWHEEL
    #define  WM_MOUSEWHEEL      0x020A
#endif

#ifndef WM_GETOBJECT
    #define  WM_GETOBJECT      0x003D
#endif

#ifndef SB_SIMPLEID
    #define  SB_SIMPLEID        0x00ff   // refers to the data saved for simple mode
#endif

#ifndef SBARS_TOOLTIPS
    #define  SBARS_TOOLTIPS     0x0800
#endif

#ifndef CX_PANE_BORDER
    #define  CX_PANE_BORDER     6        // 3 pixels on each side of each pane
#endif

#ifndef TTS_BALLOON
    #define  TTS_BALLOON        0x40
#endif

#ifndef TTM_SETTITLE
    #define  TTM_SETTITLE      (WM_USER + 32)
#endif

#ifndef GET_X_LPARAM
    #define  GET_X_LPARAM(lParam)  ((int)(short)LOWORD(lParam))
#endif

#ifndef GET_Y_LPARAM
    #define  GET_Y_LPARAM(lParam)  ((int)(short)HIWORD(lParam))
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ������ ���������� ���������
#define  EW_TERMEXT         "EWEXTT"

// ������� ��������� �� ���������� ���������
#define  EW_MES_TABLE       "EWTBL"

// ������ ������ � ��������� ������� ���������
#define  EW_MES_TABLE_SIZE  9

// ���������� � ��������� ������
#define  EW_FILTR_TEXT      TEXT("FILTR")
#define  EW_INS_TEXT        TEXT("INS")
#define  EW_OVR_TEXT        TEXT("OVR")
#define  EW_NUMLOCK_TEXT    TEXT("NUM")
#define  EW_CAPSLOCK_TEXT   TEXT("CAPS")
#define  EW_SCROLLLOCK_TEXT TEXT("SCRL")

// ������ ���������� ������
#define  EW_BUFSIZE              512

// ����� ������ � �������
#define  EW_MAXCOLOR               7

// ����� ������ � ��������� ������
#define  EW_MAXPANE               64

// ������ ���������
#define  EW_ELLIPSIS_CHAR_A      133
#define  EW_ELLIPSIS_CHAR_W   0x2026

// ������������ ����� �������� ������ ���� � ����������� � ����
#define  EW_MAXEXTEDITOR_NAME     31
#define  EW_MAXEXTEDITOR_COMMENT  81

// ������������ ����� ������-������ (� ��������)
#define  EW_MAX_STATUS_LEN        80

// ������������ ������ ��������� � ������ �����������
#define  EW_MAX_BALLOON_HEAD      81
#define  EW_MAX_BALLOON_TEXT     301

// ������������ ����� ������ ������
#define  EW_MAX_BTTEXT_SIZE      101

// ������ ������
#define  EW_SMALLICONSIZE         16
#define  EW_LARGEICONSIZE         24

// ������ ����������� ������
#define  EW_BUTTON_API_H          23  // ������

// ����� ��� ��������� ������-������ (��� ew_setStatusText, �������� flags)
#define  EW_SBT_NO                 0  // ������������ ������ ������, ������������ ����� ew_addStatusPane
#define  EW_SBT_TEXT               1  // ������ ������ ����������� �� ������ ������
#define  EW_SBT_TEXTTIPS           2  // ������� ����� � ��������� � ����
#define  EW_SBT_ICON               4  // ������� ������ (� ��������� � ��� ��� �������)
#define  EW_SBT_ALL                (EW_SBT_TEXT | EW_SBT_TEXTTIPS | EW_SBT_ICON)


#ifdef UNICODE
    #define  EW_ELLIPSIS_CHAR  EW_ELLIPSIS_CHAR_W
#else
    #define  EW_ELLIPSIS_CHAR  EW_ELLIPSIS_CHAR_A
#endif


// ���������� ���������� ����� (��� ������� ���������� ������� � ����������)
#define  EW_COLOR_DECREASE        50

// ����� ������� �����
#define  EW_COLOR_SHIFT          100

// �������� ������ �� ���������
#define  EW_YELLOW          RGB(255, 255, 200)
#define  EW_ROSE            RGB(255, 200, 240)
#define  EW_GREEN           RGB(200, 255, 200)
#define  EW_GRAY            RGB(220, 220, 220)
#define  EW_BLUE            RGB(100, 255, 255)
#define  EW_PURPLE          RGB(220, 200, 255)
#define  EW_DOTLINE         RGB( 90,  90,  90)
#define  EW_RED             RGB(255,   0,   0)


// ��������� �������� ������ (������� CGA) ��� ��������� ����� (������ EWGlobalData::staticTextColor)
#define  EW_TEXT_CGA_BLACK            RGB(  0,   0,   0)
#define  EW_TEXT_CGA_BLUE             RGB(  0,   0, 170)
#define  EW_TEXT_CGA_GREEN            RGB(  0, 170,   0)
#define  EW_TEXT_CGA_CYAN             RGB(  0, 170, 170)
#define  EW_TEXT_CGA_RED              RGB(170,   0,   0)
#define  EW_TEXT_CGA_MAGENTA          RGB(170,   0, 170)
#define  EW_TEXT_CGA_BROWN            RGB(170,  85,   0)
#define  EW_TEXT_CGA_LTGRAY           RGB(170, 170, 170)
#define  EW_TEXT_CGA_DKGRAY           RGB( 85,  85,  85)
#define  EW_TEXT_CGA_LTBLUE           RGB( 85,  85, 255)
#define  EW_TEXT_CGA_LTGREEN          RGB( 85, 255,  85)
#define  EW_TEXT_CGA_LTCYAN           RGB( 85, 255, 255)
#define  EW_TEXT_CGA_LTRED            RGB(255,  85,  85)
#define  EW_TEXT_CGA_LTMAGENTA        RGB(255,  85, 255)
#define  EW_TEXT_CGA_YELLOW           RGB(255, 255,  85)
#define  EW_TEXT_CGA_WHITE            RGB(255, 255, 255)


// ��������� �������� ������ (������� WEB) ��� ��������� �����
#define  EW_TEXT_WEB_BLACK            RGB(  0,   0,   0)
#define  EW_TEXT_WEB_BLUE             RGB(  0,   0, 255)
#define  EW_TEXT_WEB_GREEN            RGB(  0, 128,   0)
#define  EW_TEXT_WEB_AQUA             RGB(  0, 255, 255)
#define  EW_TEXT_WEB_RED              RGB(255,   0,   0)
#define  EW_TEXT_WEB_FUCHSIA          RGB(255,   0, 255)
#define  EW_TEXT_WEB_MAROON           RGB(128,   0,   0)
#define  EW_TEXT_WEB_SILVER           RGB(192, 192, 192)
#define  EW_TEXT_WEB_GRAY             RGB(128, 128, 128)
#define  EW_TEXT_WEB_NAVI             RGB(  0,   0, 128)
#define  EW_TEXT_WEB_LIME             RGB(  0, 255,   0)
#define  EW_TEXT_WEB_TEAL             RGB(  0, 128, 128)
#define  EW_TEXT_WEB_OLIVE            RGB(128, 128,   0)
#define  EW_TEXT_WEB_PURPLE           RGB(128,   0, 128)
#define  EW_TEXT_WEB_YELLOW           RGB(255, 255,   0)
#define  EW_TEXT_WEB_WHITE            RGB(255, 255, 255)


// ��������� �������� ������ (������� WEB) ��� ���� ��������� �����
#define  EW_BACK_WEB_WHITE            RGB(255, 255, 255)
#define  EW_BACK_WEB_BLUE             RGB(  0, 191, 255)
#define  EW_BACK_WEB_GREEN            RGB(128, 128,   0)
#define  EW_BACK_WEB_CYAN             RGB(  0, 255, 255)
#define  EW_BACK_WEB_RED              RGB(250, 128, 114)
#define  EW_BACK_WEB_MAGENTA          RGB(238, 130, 238)
#define  EW_BACK_WEB_BROWN            RGB(160,  82,  45)
#define  EW_BACK_WEB_LTGRAY           RGB(211, 211, 211)
#define  EW_BACK_WEB_DKGRAY           RGB(128, 128, 128)
#define  EW_BACK_WEB_LTBLUE           RGB(176, 196, 222)
#define  EW_BACK_WEB_LTGREEN          RGB(173, 255,  47)
#define  EW_BACK_WEB_LTCYAN           RGB(224, 255, 255)
#define  EW_BACK_WEB_LTRED            RGB(255, 192, 203)
#define  EW_BACK_WEB_LTMAGENTA        RGB(230, 230, 250)
#define  EW_BACK_WEB_YELLOW           RGB(255, 255, 224)
#define  EW_BACK_WEB_BLACK            RGB(  0,   0,   0)


// ������� ������� (CGA)
// ����� ��������
#define  EW_TEXT_STYLE_BLACK          EW_TEXT_CGA_BLACK
#define  EW_TEXT_STYLE_BLUE           EW_TEXT_CGA_BLUE
#define  EW_TEXT_STYLE_GREEN          EW_TEXT_CGA_GREEN
#define  EW_TEXT_STYLE_CYAN           EW_TEXT_CGA_CYAN
#define  EW_TEXT_STYLE_RED            EW_TEXT_CGA_RED
#define  EW_TEXT_STYLE_MAGENTA        EW_TEXT_CGA_MAGENTA
#define  EW_TEXT_STYLE_BROWN          EW_TEXT_CGA_BROWN
#define  EW_TEXT_STYLE_LTGRAY         EW_TEXT_CGA_LTGRAY
#define  EW_TEXT_STYLE_DKGRAY         EW_TEXT_CGA_DKGRAY
#define  EW_TEXT_STYLE_LTBLUE         EW_TEXT_CGA_LTBLUE
#define  EW_TEXT_STYLE_LTGREEN        EW_TEXT_CGA_LTGREEN
#define  EW_TEXT_STYLE_LTCYAN         EW_TEXT_CGA_LTCYAN
#define  EW_TEXT_STYLE_LTRED          EW_TEXT_CGA_LTRED
#define  EW_TEXT_STYLE_LTMAGENTA      EW_TEXT_CGA_LTMAGENTA
#define  EW_TEXT_STYLE_YELLOW         EW_TEXT_CGA_YELLOW
#define  EW_TEXT_STYLE_WHITE          EW_TEXT_CGA_WHITE

// ����� ����
#define  EW_BACK_STYLE_WHITE          EW_BACK_WEB_WHITE
#define  EW_BACK_STYLE_BLUE           EW_BACK_WEB_BLUE
#define  EW_BACK_STYLE_GREEN          EW_BACK_WEB_GREEN
#define  EW_BACK_STYLE_CYAN           EW_BACK_WEB_CYAN
#define  EW_BACK_STYLE_RED            EW_BACK_WEB_RED
#define  EW_BACK_STYLE_MAGENTA        EW_BACK_WEB_MAGENTA
#define  EW_BACK_STYLE_BROWN          EW_BACK_WEB_BROWN
#define  EW_BACK_STYLE_LTGRAY         EW_BACK_WEB_LTGRAY
#define  EW_BACK_STYLE_DKGRAY         EW_BACK_WEB_DKGRAY
#define  EW_BACK_STYLE_LTBLUE         EW_BACK_WEB_LTBLUE
#define  EW_BACK_STYLE_LTGREEN        EW_BACK_WEB_LTGREEN
#define  EW_BACK_STYLE_LTCYAN         EW_BACK_WEB_LTCYAN
#define  EW_BACK_STYLE_LTRED          EW_BACK_WEB_LTRED
#define  EW_BACK_STYLE_LTMAGENTA      EW_BACK_WEB_LTMAGENTA
#define  EW_BACK_STYLE_YELLOW         EW_BACK_WEB_YELLOW
#define  EW_BACK_STYLE_BLACK          EW_BACK_WEB_BLACK

// -----------------------------------------------------------------------------
// ���� �������
#define  EW_TYPE_FONT_TEXT            1
#define  EW_TYPE_FONT_FIELD           2
#define  EW_TYPE_FONT_DISABLE         3
#define  EW_TYPE_FONT_LIST            4
#define  EW_TYPE_FONT_HEADER          5
#define  EW_TYPE_FONT_EDITOR          6
#define  EW_TYPE_FONT_MENU            7

// -----------------------------------------------------------------------------
// ����������� ������ ���� ���������
#define  EW_EDITOR_MINSIZE_X   (2 * ew_data->editorCX)
#define  EW_EDITOR_MINSIZE_Y   (2 * ew_data->editorCY)

// -----------------------------------------------------------------------------
// ����������� ������ ���� ������� ����������� ������
#define  EW_VIEWER_MINSIZE_W   400
#define  EW_VIEWER_MINSIZE_H   300

// ����� �������
#define  EW_CHILD           0x00000001
#define  EW_POPUP           0x00000002
#define  EW_NOTITLE         0x00000004
#define  EW_NOCLOSE         0x00000008
#define  EW_BORDER          0x00000010
#define  EW_NOTITLETC       0x00000100  // ��� ��������� ��� TabControl (��������� � EW_NOTITLE)
#define  EW_CENTERED        0x00001000  // ������ ������ ���� ����������� �� �������� ���� ����������
#define  EW_FIXED           0x00010000

// ����� ����������
#define  EW_AUTOHEAD        0x00000001
#define  EW_HORLINES        0x00000002
#define  EW_VERLINES        0x00000004
#define  EW_DOTLINES        0x00000008
#define  EW_AUTOFILL        0x00000010
#define  EW_MAXIMIZED       0x00000020
#define  EW_ELLIPSIS        0x10000000
#define  EW_MULTIHEAD       0x00000040
//#define EW_

// ����� ������������
#define  EW_LEFTJUST        0x00000000
#define  EW_RIGHTJUST       0x00000001
#define  EW_CENTERJUST      0x00000002

#define  EW_JUSTSTYLE       (EW_LEFTJUST | EW_RIGHTJUST | EW_CENTERJUST)

// Places text on the left side of the radio button or check box.
#define  EW_LEFTTEXT        0x00000004
// Positions a radio button's circle or a check box's square
// on the right side of the button rectangle.
#define  EW_RIGHTBUTTON     EW_LEFTTEXT

#define  EW_SCROL_FIELD     0x00000010

// ���� ����������
#define  EW_DOUBLE_EDGE     2
#define  EW_SINGLE_EDGE     1
#define  EW_EMPTY_EDGE      0

//
#define  EW_APPDATA_MAGIC      0x0A12B9E3
#define  EW_APPDATA_NAME       "EasyWin Application Data File."
#define  EW_APPDATA_COPYRIGHT  "Copyright (c) 2001-2019 R-Style Softlab."

// ������ ��� ������
#define  EW_FRAME_DATA      0x00000001

// ew_addEvent target:
// 0 - send to the server
// 1 - leave on terminal if capture is on
// 2 - leave on terminal
#define  LOCAL_TARGET       2
#define  REMOTE_TARGET      0
#define  CAPTURE_TARGET     1

// ������ (�� ����������� ����)
#define  EW_EMPTY_MENU      ((void *)-1)

// -----------------------------------------------------------------------------
// ����� ��� ���������� ��������� ������ � ����� shift-������
#define  EW_SPLIT_LEFT_RIGHT_ALT     1
#define  EW_SPLIT_LEFT_RIGHT_SHIFT   2
#define  EW_SPLIT_LEFT_RIGHT_CTRL    4

// -----------------------------------------------------------------------------
// ������� ������������ ���� ������ �����������
#define  dCMD_TT_SHOW_HISTORY     100
#define  dCMD_TT_CLEAR_HISTORY    101
#define  dCMD_TT_USE_HISTORY      102
#define  dCMD_TT_CLEAR_STATE      110
#define  dCMD_TT_SILENT_MODE      120

// ���������� ��� ������ �����������

// ��� dCMD_TT_
#define  dCMD_TT_VALUE_ON           1
#define  dCMD_TT_VALUE_OFF          2   // ��������� ������� ���� � ����������� ���� (��� ��������)


// -----------------------------------------------
// ��������� ��� ��������������� ������
#define  dEW_ToolRangeBegin     3000   // ������ ��������� ���������������� ������
#define  dEW_ToolRangeEnd       4999   // ��������� ��������� ���������������� ������

#define  dEW_UserRangeBegin     7000   // ������ ��������� ���������������� ������
#define  dEW_UserRangeEnd       8999   // ��������� ��������� ���������������� ������

#define  dEW_IconShift24        2000   // ����� �������������� ��� ������ ������� 24�24

// -----------------------------------------------------------------------------
// �������� ��� ��������� NOGRIDSCROL
#define  dEW_HideScrolLineH        1   // �� �������� �������������� �����
#define  dEW_HideScrolLineV        2   // �� �������� ������������ �����


// -----------------------------------------------------------------------------
// �������� ��� ��������� ENABLEBUTTONICON
#define  dEW_ButtonIconNone        0   // �� ���������� �������� �� �������
#define  dEW_ButtonIconSmall       1   // ������ ���������� ��������� ��������
#define  dEW_ButtonIconAuto        2   // ���������� ��� ��������� ������ ��������� ��������,
                                       // � ��� ������� - �������


// -----------------------------------------------------------------------------
// �������������� ����� ��������� (���� otype)

#define  EW_TYPEOBJ_MENU         100
#define  EW_TYPEOBJ_FRAME        101
#define  EW_TYPEOBJ_LINE         102
#define  EW_TYPEOBJ_TEXT         103
#define  EW_TYPEOBJ_BOX          104
#define  EW_TYPEOBJ_FIELD        105
#define  EW_TYPEOBJ_COLUMN       106
#define  EW_TYPEOBJ_CEIL         107
#define  EW_TYPEOBJ_SCROL        108
#define  EW_TYPEOBJ_EDITOR       109
#define  EW_TYPEOBJ_TAB          110
#define  EW_TYPEOBJ_POPUP        111
#define  EW_TYPEOBJ_DLG          112
#define  EW_TYPEOBJ_WND          113
#define  EW_TYPEOBJ_BALLOON      114
#define  EW_TYPEOBJ_STATUSBAR    115


// -----------------------------------------------------------------------------
// ��������� �� �������-���� �������� ���������
typedef  int (*TFMsgGate)(int msg, void *data, void *add);

// -----------------------------------------------
// ������������ �������� � ������ ����
enum
   {
    DLGOBJECT_ID,
    POPUPOBJECT_ID,
    WINDOWOBJECT_ID
   };

// -----------------------------------------------
// �������������� ��������� � ��������
enum
   {
    // Control base-identifiers
    ID_BOX      = 0x0100,
    ID_TEXT     = 0x0200,
    ID_FIELD    = 0x0300,

    ID_DLGBTN   = 0x0400,
    ID_DLGTEXT  = 0x0500,

    ID_FRAMEWND = 0x0600,
    ID_REBARWND,
    ID_CLIENTWND,
    ID_MENUBAR,
    ID_STATUSBAR,
    ID_TOOLBAR,

    ID_POPUPWND = 0x0700,

    ID_PANEL    = 0x0800,
    ID_SCROLL   = 0x0900,
    ID_HEADER   = 0x0A00,
    ID_EDITOR   = 0x0B00,
    ID_TABCTRL  = 0x0C00,
    ID_TOOLTIP  = 0x0D00,
    ID_BALLOON  = 0x0E00,

    // Timer identifier
    ID_INDICTIMER = 0x0100,
    ID_MOUSETIMER,
    ID_FOCUSTIMER,
   };

// -----------------------------------------------
// �������������� ������ � �������
enum
   {
    MES_COLOR = 0,
    RMES_COLOR,
    HLP_COLOR,
    MENU_COLOR,
    COM_COLOR,
    SCRL_COLOR,
    BACK_COLOR,

    USER_COLOR
   };

// -----------------------------------------------
enum ColorTypes
   {
    NormalColor   = 0x11,
    SelectedColor = 0x12,
    ErrorColor    = 0x13,

    KeyColor      = 0xF1,
    TypeColor     = 0xF2,
    StdFuncColor  = 0xF3,
    NumericColor  = 0xF4,
    OperatorColor = 0xF5,
    StringColor   = 0xF6,
    SpecVarColor  = 0xF7,
    CommentColor  = 0xF8
   };

#include "packpsh1.h"

// -----------------------------------------------
class EwAccessibleObj;

// -----------------------------------------------
typedef struct
      {
       int32          type;
       uint32         size;
      } APPDATA_ITEM;

// -----------------------------------------------
typedef struct
      {
       int32          type;
       uint32         size;

       UINT           flags;
       UINT           showCmd;
       RECT           rcNormal;

       char           reserv[480];
      } FRAMEDATA_ITEM;

// -----------------------------------------------
typedef struct
      {
       int32          magic;
       char           name[31];
       char           copyright[41];

       char           reserv[180];
      } APPDATA_HEADER;

#include "packpop.h"


// -----------------------------------------------
typedef struct MENUOBJ
      {
       int            otype;            // EW_TYPEOBJ_MENU

       MENU          *pMenu;
       int            nMenu;
       bool           popup;
       int            x, y;

       HMENU          hMenu;
       HMENU          hPreviousMenu;
       struct MENUOBJ *previousMenu;

       BOOL           fUseMenu;
       BOOL           fLoopMenu;
       BOOL           fEnableExit;
       BOOL           fExitMenuLoop;

       PANEL         *pnl;
       void          *psave;

       void          *mapExt;           // ��������� ���� map ��� ���������� ��������� (CDataItem)
       BOOL           fHasExt;          // ������� ������� ���������� ���������
       int            itemId;           // ������������� ������ ����
      } MENUOBJ;

    
// -----------------------------------------------
typedef struct BALLOONOBJ
      {
       int            otype;            // EW_TYPEOBJ_BALLOON

       TEXT           head[EW_MAX_BALLOON_HEAD];  // ��������� �����������
       TEXT           text[EW_MAX_BALLOON_TEXT];  // ����� �����������

       int            timeOut;          // ����� (��) ����������� �����������

       HWND           hWnd;             // ���������� ���� �����������
       HWND           hStatusBar;       // ���������� ���� ������-������

       HIMAGELIST     hImageList;       // ������ ��������
       CNTLIST        lstIcons;         // ������ ��������������� ������
       int            iIcon;            // ����� ������� ������ � ������ hImageList (�.� -1 ���� ��� ������)

       bool           flSilentMode;     // �� ���������� �����������
       bool           flHistory;        // ������������/�� ������������ �������
       bool           flBlock;          // ���������� �������� ����� � ������������ ����
      } BALLOONOBJ;
      

// -----------------------------------------------
// ��������� �� ������� ���������
typedef LRESULT (CALLBACK *TFWndProc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// -----------------------------------------------------------------------------
// ��������� ��� ������ ��������� ����� StatusBar'�
typedef struct
      {
       TCHAR    text[EW_MAX_STATUS_LEN],  // ����� ��������
                tips[EW_MAX_STATUS_LEN];  // ����� ����������� ���������
       int      id;                       // ������������� ������
       int      pane;                     // ������ ������
       int      flags;                    // ����� ��� ������ (EW_SBT_*)
       HBITMAP  hIcon;                    // ����� ��������� ��������
      } EWStatusItem;

// -----------------------------------------------
typedef struct STATUSBAROBJ
      {
       int           otype;               // EW_TYPEOBJ_STATUSBAR

       bool          bSizeGrip;           // ������� ������� � ������-���� "������" ��� ��������� �������
       bool          bHasFilter;          // ������� �������� ����������

       int           npanes;              // ����������� ������ � ������-���� (������� �������� id/cx/item)
       short         id[EW_MAXPANE];      // ������ ��������������� ������
       short         cx[EW_MAXPANE];      // ������ ����� ������
       EWStatusItem  item[EW_MAXPANE];    // ������ ������ �� �������

       CNTLIST       lstIcons;            // ������ ������ ��� ������-����

       HWND          hToolTips;           // ������� ��� ����������� ���������

       TFWndProc     pWndProc;            // ��������� �� "������" ������� ��������� ������-����

       BALLOONOBJ   *balloon;             // ������ �����������
      } STATUSBAROBJ;


// -----------------------------------------------
class CCustomizeToolBar;

// -----------------------------------------------
typedef struct
      {
       int            otype;              // EW_TYPEOBJ_FRAME

       int            numcols;
       int            numrows;

       bool           showFrame;
       bool           blockOpen;
       bool           lostForeground;
       bool           restMaximized;

       HWND           hFrameWnd;
       HWND           hClientWnd;
       HWND           hParentWnd;

       HWND           hMenuBar;
       HWND           hRebarWnd;
       HWND           hStatusBar;
       HWND           hToolBar;           // ���������� ToolBar'�
       HIMAGELIST     hImageListTB;       // ������ �������� ��� ToolBar'�
CCustomizeToolBar*    pCustomizeTB;

       HWND           hOldMainWnd;

       MENUOBJ       *menu;
       void          *psave;

       BOOL           fPostResize;
       BOOL           fParentDisabled;

       void          *overlappedWnd;

       BOOL           prevToolBarBusy;   // ���������� ��������� ToolBar'�
      } FRAMEOBJ;

// -----------------------------------------------
typedef struct
      {
       char     fldType;
       int      fldTypeF;                // ������ DLGFIELD::typeF
       unsigned fldLength;
       int      fldPoint;

       bool     oemFont;

       char    *minVal;
       char    *maxVal;

       int    (*userProc)(const char *, void *);
       void    *userParm;
      } VLDOBJ;

// -----------------------------------------------
typedef struct
      {
       void  *fmtedit;
      } FMTOBJ;

// -----------------------------------------------
typedef struct
      {
       int            x;
       int            y;
       int            cx;
       int            cy;
      } DLGCOORD;

// -----------------------------------------------
typedef struct
      {
       int            otype;            // EW_TYPEOBJ_LINE

       int            x1;
       int            y1;
       int            x2;
       int            y2;

       DLGCOORD       dlg;
      } DLGLINE;

// -----------------------------------------------
typedef struct
      {
       int            otype;            // EW_TYPEOBJ_TEXT

       int            x;
       int            y;
       char          *value;
       int            align;
       int            format;
       int            color;

       DLGCOORD       dlg;
      } DLGTEXT;

// -----------------------------------------------
typedef struct
      {
       int            otype;            // EW_TYPEOBJ_BOX

       int            x;
       int            y;
       int            l;
       int            h;
       char          *text;

       DLGCOORD       dlg;
      } DLGBOX;

// -----------------------------------------------
typedef struct
      {
       int            otype;            // EW_TYPEOBJ_FIELD

       FIELD         *fld;

       unsigned char  Ftype;            // ��� ����
       unsigned int   St;               // ����� ����������� ����
       unsigned char  FVt;              // ��� ��������

       int            x;
       int            l;

       unsigned char  typeF;
       unsigned long  FFlags;

       bool           enable;
       bool           blocked;
       bool           oemFont;

       char          *text;
       int            style;            // ������������ � �.�.
       int            color,            // ����� ���� (������ �� ���)
                      colorText,        // ���� ������ ���� (������ ������ �����)
                      colorBack;        // ���� ���� ����
       int            format;           // ������ (B/I/U). ���� �� ������������, �� ����� "��� ���� �� ��������"
       int            focusState;

       HWND           hWnd;
       WNDPROC        wndProc;

       VLDOBJ        *vldobj;
       FMTOBJ        *fmtobj;

       DLGCOORD       dlg;
       int            IndOffset;

       EwAccessibleObj *acc;

       unsigned int   St2;              // ���������� ����������� St

       TCHAR         *tooltipStr;       // ����������� ��������� � ���� (������������ �����)

       int            iconID;           // ������������� ������, ��������� � ����
       int            iconFlag;         // ����� � ������ ������ (dICON_ALIGN_)

       int            iconIndex;        // ������ ������ � ������ �������� (DLGOBJ::hImageListFld)
      } DLGFIELD;

// -----------------------------------------------
typedef struct
      {
       int            otype;            // EW_TYPEOBJ_COLUMN

       FIELD         *fld;

       unsigned int   St;
       unsigned char  Ftype;
       int            x;
       int            l;

       unsigned char  typeF;
       unsigned long  FFlags;
       unsigned char  fieldNum;

       bool           selected;
       int            style;

       int            xColumn;          // ������ ������� �������
       int            xColumnSrc;       // �������� �������� xColumn (��� ��������������)
       int            xTreePos;

       int            lMargin;
       int            rMargin;
       int            nMinCX;           // ����������� ������ �������
       int            nWidth;           // ������������ ������ ������� (������������ ��� ��������������
                                        // ���� ��-���������.
       DLGCOORD       dlg;

       unsigned int   St2;              // ���������� ����������� St
      } DLGCOLUMN;

// -----------------------------------------------
typedef struct
      {
       int            otype;            // EW_TYPEOBJ_CEIL

       int            x;
       int            l;

       unsigned char  typeF;
       unsigned long  FFlags;

       short          needOffs;

       int            align;
       int            color,            // ����� ���� (������ �� ���)
                      colorText,        // ���� ������ ���� (������ ������ �����)
                      colorBack;        // ���� ���� ����
       int            format;           // ������ (B/I/U). ���� �� ������������, �� ����� "��� ���� �� ��������"

       char          *text;
       char          *tree;

       int            mark1;
       int            mark2;

       bool           IsTrunc;

       int            iconID;           // ������������� ������, ��������� � ����
       int            iconFlag;         // ����� � ������ ������ (���� �� ������������)

       bool           IsMark;           // ������� ���������� ������ � ����������
      } DLGCEIL;

// -----------------------------------------------
typedef struct
      {
       int            otype;            // EW_TYPEOBJ_SCROL

       int               x;
       int               y;
       int               l;
       int               h;             // ������ ����� ������ � ����������

       int               ncolumns;
       int               nrows;

       DLGCOLUMN        *columns;
       DLGCEIL         **ceilPtr;

       int               xColumn;
       int               focusState;

       int               curItem;
       int               curCeil;

       int               numRows;
       int               numCols;
       int               numRec;

       const int        *firstRecPtr;   // �������� �� ����� ������ ������ � ����������
       const int        *numRecPtr;     // �������� �� ����� ����� ������� � ����������

       int               style;
       bool              bRunState;

       HWND              hEditWnd;
       HWND              hScrollWnd;
       HWND              hHeaderWnd;
       HWND              hToolTips;
       HWND              hHeaderToolTips;
       TCHAR            *lpTip;
       int               TTLastHitRow;
       int               TTLastHitCol;

       HFONT             hScrollFont;
       HFONT             hHeaderFont;

       DLGCOORD          dlg;

       size_t            parSize;
       void             *parBuf;

       bool              isPartRow;
       bool              isViewMode;

       EwAccessibleObj  *acc;

       void             *ewHeader;  // ��������� �� ������ ���� std:vector

       int               old_y;     // BSCROL::old_y
       int               old_n;     // BSCROL::old_Mn (DLGSCROL::nrows)
      } DLGSCROL;

// -----------------------------------------------
typedef struct
      {
       int            otype;            // EW_TYPEOBJ_EDITOR

       int            x;
       int            y;
       int            l;
       int            h;
       int            flag;

       TCHAR        **buffer;
       void          *markAreas;

       int            caretX;
       int            caretY;
       int            caretType;
       int            focusState;

       bool           bRunState;

       HWND           hWnd;
       HFONT          hFont;
       HBITMAP        hBitmap;
       HRGN           hInvalidRgn;

       DLGCOORD       dlg;

       unsigned char  **attrArray;

       bool           isImager;         // ������� ����, ��� �������� ������������ ��� ��������� ��������
       void          *imgData;          // ��������� �� ��������� EWPictureData
      } DLGEDITOR;

// -----------------------------------------------
typedef struct
      {
       int            otype;            // EW_TYPEOBJ_TAB

       int            x;
       int            y;
       int            l;
       int            h;
       char          *tabs;

       HWND           hWnd;
       HFONT          hFont;

       DLGCOORD       dlg;

       // #3359
       bool           canResize;        // TabCtrl ����� ������ ������

       int            deltaX,           // ��������� ��������� TabCtrl'� � ��������
                      deltaY;

       RECT           rcDialogOld,      // ������ ������ �������� (�������)
                      rcElemOld;        // ������ ������ �������� �������� (����������)

       // #3463
       int            style;
      } DLGTAB;

// -----------------------------------------------
typedef struct POPUPOBJ
      {
       int            otype;            // EW_TYPEOBJ_POPUP

       char          *text;
       char          *btext;
       char          *status;
       char          *caption;

       int            type;
       int            style;
       int            curfld;
       int            length;

       int            help;
       bool           enableClosing;

       int            ntexts;
       int            nbuttons;

       void          *psave;

       HWND           hPopupWnd;
       HWND           hToolTip;

       HWND           hFrameWnd;
       HWND           hParentWnd;

       HFONT          hFont;
       POINT          pt;

       DLGCOORD       dlg;
       DLGCOORD       textDlg;
       DLGCOORD      *buttonDlg;

       void          *webCrtl;

       EwAccessibleObj  *acc;
       long              errCode;       // ercode  cwinEx
       int               infoType;      // message, warning, error, fatal.

       BOOL           prevToolBarState;  // ���������� ������ ToolBar'� ������������/������������� (��� ����������� ��������������)
      } POPUPOBJ;

// -----------------------------------------------
typedef  POPUPOBJ DLGPOPUP;

// -----------------------------------------------
typedef struct DLGOBJ
      {
       int            otype;            // EW_TYPEOBJ_DLG

       PANEL         *pnl;

       struct DLGOBJ *parent;
       struct DLGOBJ *oldUseObject;

       LOBJ           _lobj;            // ������� ������ �������� �������
       CNTLIST        child_list;       // ������ �������� �������

       int            nfields;
       int            nlines;
       int            ntexts;
       int            nboxs;

       DLGTAB        *tab;
       DLGSCROL      *scrol;
       DLGEDITOR     *editor;

       DLGFIELD      *fields;
       DLGLINE       *lines;
       DLGTEXT       *texts;
       DLGBOX        *boxs;

       char          *values;
       int            style;

       POPUPOBJ      *popup;

       HWND           hWnd;
       HWND           hParentWnd;
       HWND           hStatusBar;
       HWND           hToolTips;

       HWND           hFocusWnd;

       HFONT          hTextFont;
       HFONT          hFieldFont;
       HFONT          hDisableFont;

       DLGCOORD       dlg;

       EwAccessibleObj  *acc;

       BOOL           prevToolBarState; // ���������� ������ ToolBar'� ������������/������������� (��� ����������� ��������������)
       BOOL           hasMenu;          // #3392

       HIMAGELIST     hImageListFld;    // ������ �������� � �����

       BOOL           fSizeCmd;         // ������ ������ MAXIMIZE/RESTORE
      } DLGOBJ;

// -----------------------------------------------
typedef struct
      {
       int            otype;            // EW_TYPEOBJ_WND

       HWND           hWnd;
       LOBJ           _lobj;

       void          *object;
       int            objectID;

       void          *overlappedWnd;
       bool           externalObject;
      } WNDOBJ;

// -----------------------------------------------
typedef struct
      {
       DLGOBJ        *dlgobj;
      } ew_ExtPan;

// -----------------------------------------------
typedef struct
      {
       FRAMEOBJ      *frame;

       WNDOBJ        *topWindow;

       DLGOBJ        *useObject;

       BOOL           fAutoDelete;
       BOOL           fRemoteCapture;

       LOBJ           _lobj;
       CNTLIST        wnd_list;
      } EWContext;

// -----------------------------------------------
typedef struct
      {
       int            version;          // ������ ����������

       TFMsgGate      SendMessageGate;  // ��������� �� �������-���� ��� �������� ���������
      } TSMessageGate;

// -----------------------------------------------------------------------------
// ��������� ��� �������� ���������� ������ ��� ����� (��� ������ B/I/U)
typedef struct
      {
       LOBJ           lobj;             // ����������� ��������� ��� ������

       int            type;             // ��� ������ (EW_TYPE_FONT_*)
       int            format;           // ���� ��� ������ (����� B/I/U)

       LOGFONT        font;             // ������ ������
       int            cx, cy;           // ������� ������������� ���������� ������� ������
       HFONT          hFont;            // ���������� ������
      } TSFontData;

// -----------------------------------------------
typedef struct
      {
       void       *appIntf;

       char       *appid;
       char       *system;
       char       *version;
       char       *copyright;

       HINSTANCE   hInstance;
       int         nShowCmd;
       LPTSTR      szCmdLine;

       char        szFrameIcon[_MAX_PATH];
       HICON       hFrameIcon;
       HICON       hSmallFrameIcon;

       char       *szRegClass;

       int         edgeType;
       bool        edgeForDisable;
       bool        edgeForReadOnly;
       int         editorThumbMode;
       int         menuLoadingMode;

       BOOL        bOldWindows;

       BOOL        fSizeMoveLoop;
       BOOL        fScrollBarLoop;
       BOOL        fOverWriteMode;
       BOOL        fEditorOvrMode;

       LOGFONT     lfTextFont;           // ����� ��� ��������� �����
       CNTLIST     lstFormatFont;        // ������ ������� ��� ������ B/I/U/S


       LOGFONT     lfFieldFont;
       LOGFONT     lfDisableFont;

       LOGFONT     lfScrollFont;
       LOGFONT     lfHeaderFont;

       LOGFONT     lfEditorFont;

       LOGFONT     lfMenuFont;

       bool        fOemTextFont;
       bool        fOemFieldFont;
       bool        fOemDisableFont;

       bool        fOemScrollFont;
       bool        fOemHeaderFont;

       bool        fOemEditorFont;

       int         textCX, textCY;
       int         fieldCX, fieldCY;
       int         editorCX, editorCY;
       int         listCX;

       int         coordX, coordY;              // ������� ���������� (STEPX, STEPY)

       int         virtualScreenW,              // ������ ������������ ������ (��� ��������)
                   virtualScreenH;

       HPALETTE    hPalette;
       COLORREF    nColorRef[EW_MAXCOLOR],
                   nColorSel[EW_MAXCOLOR];      // ����� ��� ������� ���������� �������

       HBRUSH      hBkBrush[EW_MAXCOLOR];

       COLORREF    editTextClr[256];
       COLORREF    editBkClr[256];

       COLORREF    textColor[dTEXT_MAX_COLOR];  // ������� ��� ����� (���� ������)
       COLORREF    backColor[dBACK_MAX_COLOR];  // ������� ��� ����� (���� ����)

       COLORREF    nColorSel2[dBACK_MAX_COLOR];
       HBRUSH      hBkBrush2[dBACK_MAX_COLOR];

       CNTLIST     context_list;

       PANEL      *pInd;
       void       *pLock;                       // ��������� �� ������ ������ CLockUserInputWnd

       HIMAGELIST  himl;

       HHOOK       hCallWndProcHook;
       HHOOK       hCallWndProcRetHook;

       void       *appInfo;
       void       *appData;
       size_t      appSize;

       // ----------------------------------------
       // ��������� ������������� ��
       int         hostPort;
       char       *hostName,
                  *pipeName;
       // ----------------------------------------

       void       *wndMgr;
       void       *extWndMgr;
       bool        ownWndMgr;

       bool        beepMode;

       bool        noRecreateCtrl;

       TSMessageGate  msgGate;           // ���� ��� ���������

       // AV 17.03.2011 picure for background main window
       char        szBackPicture[_MAX_PATH];

       int         splitLeftRightKeys;   // ��������� ����� � ������ shift-�������

       // #3061 - ���������� �������� ���������� �� ������� ������������
       int         selectStatusFilter;   // ��������� ������ ����������
       COLORREF    colorStatusFilterBG,  // ���� ����
                   colorStatusFilterFG;  // ���� �������

       BOOL        flStatusUpdate;       // ������� ��������� �������� INS/OVR, CAPS � �.�.
       BOOL        flForceStatusUpdate;  // ������� ��������������� ���������� �������� INS/OVR, CAPS � �.�.
       BOOL        flIsBusy;             // ������� ��������� ������� (���������������/������������ CMD_EW_BUSY)

       int         toolBarMode;          // ����� ������ ToolBar'� (���� 0 ��� 1)
       int         toolBarCustom;        // ����� ��������� ToolBar'a (0 ��� 1)
       int         toolBarImageSize;     // ������ �������� �� ToolBar'� (0 - 16�16, 1 - 24�24)
       int         toolBarShowErrIcons;  // ����� ����������� ���������� �������� �� ToolBar'�.

       COLORREF    crTransparentBmp,     // "����������" ���� � ���������
                   crGrayedText,         // ���� ����������� ������ � ����
                   crSelText,            // ���� ������ � ���� ��� ��������
                   crMenuBkgnd,          // ���� ���� � ����
                   crSelBkgnd;           // ���� ���� � ���� ��� ��������

       char        szDirResFile[_MAX_PATH],  // ������� ��������
                   szToolResDll[_MAX_PATH],  // ���������������� ���������� ��������
                   szUserResDll[_MAX_PATH];  // ���������������� ���������� ��������
                   

       HMODULE     hToolResDll,          // ����� ���������������� ���������� ��������
                   hUserResDll;          // ����� ���������������� ���������� ��������

       HGDIOBJ     hMenuFont;            // ����� ���������� ������ ��� ����
       bool        flUseUserMenuFont;    // ������������ ��� ���� ���������������� ����� (��. lfMenuFont)

       int         cmdll;                    // ����� ������ (������/����������)
       char        szEditorExt[_MAX_PATH];   // ������� ���������� ���������
       char        szTxtFile[_MAX_PATH];     // ������� TxtFile

       int         newFeatureMode;       // ����� ������ ����������
       int         noGridScrol;          // �� �������� ����� � ����������� (��. dEW_HideScrolLine*)
       int         flInputAsPic;         // ������ ������� 'R' � ������-������ �������� ��������
       int         flWorkModeAsPic;      // ��� ����������� � ������-������ ������ ������ �������� ��������
       int         enableScrolIcon;      // ��������� ����������� ������ � �����������
       int         enableTextFormat;     // �������� ��������� �����/������� ��� ��������� �����
       int         enableTTDialog;       // �������� ����������� ��������� � ����� �������

       int         treeIconClose,        // �������������� ������ (������ ��������) ���
                   treeIconOpen;         // �������������� ����������.

       int         lockInputMode;        // ����� ���������� ���� ����������

       int         enableButtonIcon;     // ��������� ����������� ������ �� �������
       int         useAPIButtonH;        // ������������ ����������� ������ ������

       int         enableSysButton;      // ��������� ������������ ������ ������������ ��� �����������

       int         workGraphCharMode;    // ����� ��������� �������������
      } EWGlobalData;

// -----------------------------------------------------------------------------
typedef struct
      {
       int   selB;
       int   selE;
      } EWMarkArea;


// -----------------------------------------------------------------------------
// ����� ������ ������������ ����
#define  dEWMD_None     -1   // ������������ � �������� ����������������� �������� ��� EWMenuData::flagsTextSave
#define  dEWMD_OutAll    0   // ������� ������ ������������
#define  dEWMD_OutLeft   1   // ������� ����� �����
#define  dEWMD_OutRight  2   // ������� ������ �����

// -----------------------------------------------------------------------------
// ������ ��� MENU::ewdata. ������ ��� ����������� ���������� ��������� ����
// � �������� � ��������.
typedef struct
      {
       char  *Text;             // ������ �������� ������ ���� � ������������ ��� GUI �������
       char  *TextL,            // ����� ����� �������� ���� (������������)
             *TextR;            // ������ ����� �������� ���� (������������ ����������)
       int    flagsText,        // ����� ����� ���� ����� ��������� (��. dEWMD_)
              flagsTextSave;    // ���������� �������� flagsText. ��-��������� ���������������� dEWMD_None.
      } EWMenuData;


// -----------------------------------------------------------------------------
// ������ ��� ���� � ���������
typedef struct
      {
       int       size;              // ������ ����� � ���������
       int       classID;           // ������������� ������ �������
       int       countList;         // ���������� �������� � ������
       int       countFrame;        // ���������� ������ � �����
       int       countMax;          // ������������ ����� ����������� � ������
       int       sizeMax;           // ������������ ������ �����������
       int       currPos;           // ����� �������� �������
       bool      readOnly;          // ����� "������ ��� ������"
       TCHAR     name[_MAX_PATH];   // �������� �������� ����� (� �����������)
       HGLOBAL   hMem;              // ���������� ������, ��������� � ���������
       void     *oImage;            // ��������� �� ������ Gdiplus::Image
       bool      externalViewer;    // �������� ������� ����������
       // ----- ������ �� ����������
       void     *cbData;            // ��������� �� ������
       int       cbSize;            // ������ ������
      } EWPictureData;

// -----------------------------------------------------------------------------
extern EWGlobalData  *ew_data;

// -----------------------------------------------------------------------------
// Global data
bool  ew_initData(void *appIntf, HMODULE hModule);
void  ew_cleanupData(void);

// -----------------------------------------------------------------------------
// ATL module
void  ew_initAtlModule(HINSTANCE hInstance);
void  ew_doneAtlModule(void);

// -----------------------------------------------------------------------------
// Context
bool  ew_initContextList(void);
void  ew_doneContextList(void);

EWContext *ew_createContext(void);
void       ew_deleteContext(EWContext *context);

void       ew_setActiveContext(EWContext *context);
EWContext *ew_findContextForMenu(MENUOBJ *menu);
EWContext *ew_findContextForFrame(FRAMEOBJ *frame);
EWContext *ew_findContextForFrameWnd(void *overlappedWnd);
EWContext *ew_findContextWithFrame(void);
EWContext *ew_findContextForObject(void *object, int objectID);

void  ew_addToContext(EWContext *context, WNDOBJ *wndobj);
void  ew_dropFromContext(EWContext *context, WNDOBJ *wndobj);

void *ew_getTopObject(EWContext *context, int objectID);

void  ew_activateContext(EWContext *context, bool activate, bool focus);
void  ew_sendToAllWindow(EWContext *context, int iMessage, WPARAM wParam, LPARAM lParam);
void  ew_updateContextLayout(EWContext *context);

// -----------------------------------------------------------------------------
// WndObj
WNDOBJ *ew_addWndObj(HWND hWnd, void *object, int objectID);
WNDOBJ *ew_findWndObj(void *object, int objectID, EWContext **pContext);
bool    ew_dropWndObj(void *object, int objectID);

WNDOBJ *ew_addExternalWndObj(HWND hWnd, void *object, int objectID, void *overlappedWnd);
bool    ew_dropExternalWndObj(EWContext *context, WNDOBJ *wndobj);

WNDOBJ *ew_findOverlappedWnd(void *overlappedWnd, EWContext **pContext);

void  ew_activateWndObj(WNDOBJ *wndobj);
void  ew_deactivateWndObj(WNDOBJ *wndobj);

// -----------------------------------------------------------------------------
// Window manager
bool  ew_initWndManager(void);
void  ew_doneWndManager(void);

bool  ew_canCloseExtWndManager(void);
void  ew_destroyExtWndManager(void);

void  ew_addToWndManager(WNDOBJ *wndobj, bool disableAllFlag);
void  ew_dropFromWndManager(WNDOBJ *wndobj, bool enableAllFlag);

void  ew_addFrameToWndManager(FRAMEOBJ *frame);
void  ew_dropFrameFromWndManager(FRAMEOBJ *frame);

void  ew_addContextToWndManager(EWContext *context);
void  ew_dropContextFromWndManager(EWContext *context);

void  ew_activateOverlapped(void *overlapped);
void  ew_onActivateOverlapped(void *overlapped);

// -----------------------------------------------------------------------------
// Windows hook
void  ew_initWindowsHook(void);
void  ew_doneWindowsHook(void);
HWND  ew_getSysLoopWnd(void);

// -----------------------------------------------------------------------------
// Window class
int   ew_regWndClass(WNDCLASS *);
int   ew_regWndClassEx(WNDCLASSEX *);
void  ew_doneWndClass(void);

// Icon
HICON ew_loadIcon(const char *szIconName, int cx, int cy, unsigned opt);

// Font support
bool  ew_initFont (void);
void  ew_initFontMetrics (void);
int   ew_parseFontString (LOGFONT *lf, char *str);
HFONT ew_createFont (LOGFONT *);

// -----------------------------------------------------------------------------
// �������� ��������� ������ ��� ���� ���������� �������
// ���� ����� ���������� ���, ��� ����� �������
// [in] type   - ��� ������ (EW_TYPE_FONT_*)
// [in] format - ������ ������ (dTEXT_FORMAT_*)
// [in] parent - ��������� �� ��������� ��� ������������� ������
TSFontData *ew_getFont(int type, int format, const LOGFONT *parent);

// -----------------------------------------------------------------------------
// Color support
bool  ew_initColor (void);
void  ew_doneColor (void);
int   ew_parseColorString (COLORREF *cr, char *str);
void  ew_initColorObjects (void);

// -----------------------------------------------------------------------------
// Events, session
int   ew_addEvent(TFSEvent *ev, int target);
int   ew_getEvent(TFSEvent *ev, int waitTime);
void  ew_clearAllEvents(void);
void  ew_stopSession(bool doAbort);


// -----------------------------------------------------------------------------
#define  ew_canSetFocus()       (!ew_context->frame || ::GetActiveWindow())
#define  ew_context             ((EWContext *)((char*)ew_data->context_list.last->next - ew_data->context_list.delta))
#define  ew_getTopDlgObject()   ((DLGOBJ *)(ew_getTopObject(ew_context, DLGOBJECT_ID)))


HWND  ew_getTopWnd(void);
HWND  ew_getTopFrameWnd(void);

DLGOBJ *ew_getObjectFromWnd(HWND);

bool  ew_isTopObject(void *object, int objectID);

int   ew_messageBox(const char *text, const char *caption, unsigned style);
void  ew_errorBox(DWORD dwErrorCode, const char *fmt, ...);

// -----------------------------------------------------------------------------
// �������������� ����
bool ew_centerWindow(HWND hWnd, HWND hWndCenter, bool centrX, bool centerY, bool asText);

// �������������� ����
void ew_setActiveWindow(HWND hWnd, bool bActivate);

// ���������� �����
bool ew_setFocus(HWND hWnd);

// ������� ��� API-������� ������ � ������� (������ ��� �������� �����������)
HWND ew_SetFocus(HWND hWnd);
HWND ew_GetFocus(void);

// �������� ����� ����
void ew_changeWindowStyleEx(HWND hWnd, DWORD dwStyle, DWORD dwExStyle);

// ��������� ������� ���� �� �������� ��� ���������� �������
bool ew_setWindowSize(HWND, RECT *, int flags);

// �������� ������� ����
bool ew_changeWindowPos(HWND, int deltaX, int deltaY, int flags);

// �������� ������� ����
bool ew_changeWindowSize(HWND, int deltaX, int deltaY, int flags);

// �������� � ������ ����������
TCHAR *ew_makeEndEllipsis    (HDC hDC, TCHAR *text, int cx, int *plen, TCHAR *psave, bool *isTrunc);
TCHAR *ew_makeBeginEllipsisEx(HDC hDC, TCHAR *text, int cx, int needOffs, bool *isTrunc);

// ����������� ���������
void ew_cvtNum2Size(int, int, long *, long *);
void ew_cvtSize2Num(long, long, int *, int *);

// ���������� ������ ���������� ������� � ������ ����
void ew_adjustDlgCoord(DLGCOORD *dlg, DWORD dwStyle, DWORD dwExStyle);
void ew_alignDlgCoord(DLGOBJ *, int, int, int, int, DLGCOORD *);

// ���������� ���������� ������� � ��� ���������
void ew_calcDlgCoord(DLGOBJ * dlgobj);

// ����������� �����
void ew_convMesStr(char *);
void ew_convMenuStr(char *);
void ew_convStatusStr(LPTSTR str);
void ew_convButtonStr(char *);
void ew_convFieldStr(char *);
char  *ew_convToMultiLine(const char *, char *, size_t);
size_t ew_convFromMultiLine(LPCTSTR src, LPTSTR dst, size_t size);

// -----------------------------------------------------------------------------
// FrameObj
FRAMEOBJ *ew_createFrame(const char *title, int x, int y, int cx, int cy, HWND hParentWnd);
void ew_destroyFrame(FRAMEOBJ *);

bool ew_showFrame(FRAMEOBJ *, int show);
void ew_openFrame(FRAMEOBJ *frame);
void ew_closeFrame(FRAMEOBJ *frame);

bool ew_initFrameSize(FRAMEOBJ *frame);
void ew_initFrameStatus(FRAMEOBJ *frame);

bool ew_calcFrameSize(int numcols, int numrows, int *cx, int *cy);
bool ew_restoreFrameData(int *x, int *y, int *cx, int *cy, int *show);
void ew_saveFrameData(FRAMEOBJ *);

void ew_setFrameMenu(MENUOBJ *menu);
void ew_restoreFrameMenu(MENUOBJ *menu);
void ew_updateFrameMenu(MENUOBJ *menu);

void ew_scrollFrameToRect(FRAMEOBJ *frame, long left, long top, long right, long bottom, int mode);
void ew_updateFrameScrollBars(FRAMEOBJ *frame);

// -----------------------------------------------------------------------------
// StatusBar
HWND ew_createStatusBar(HWND hParentWnd, bool bToolTips, bool bSizeGrip, bool bBalloon, SECTIONS *sect);
void ew_destroyStatusBar(HWND hStatusBar);
void ew_layoutStatusBar(HWND hStatusBar);
void ew_processIdleStatus(HWND hStatusBar);

void ew_layoutDialogStatus(HWND hStatusBar);
void ew_setDialogStatusText(HWND hStatusBar, int, const char *);
int  ew_getStatusPane(HWND hStatusBar, int id);

// -----------------------------------------------------------------------------
// Notify (Balloon)
BALLOONOBJ *ew_createBalloonObject(HWND hStatusBar, int timeOut);
void ew_deleteBalloonObject(BALLOONOBJ *obj);
bool ew_createBalloon(BALLOONOBJ *obj, const char *head, const char *text, int secID, int iconID, int timeOut, int colorText, int colorBack);
void ew_destroyBalloon(BALLOONOBJ *obj);
BOOL ew_processCMStatus(HWND hwnd, WPARAM wParam, LPARAM lParam, BALLOONOBJ *obj, bool isCM);
void ew_cmdNotify(int cmd, int sub);

// -----------------------------------------------------------------------------
// Menu
BOOL ew_isMenuEnabled(HWND hWnd);
BOOL ew_initDropDownMenu(MENUOBJ *menu, HMENU hMenu, UINT uItem);
BOOL ew_processMenuHelp(MENUOBJ *menu, HELPINFO *helpInfo);
BOOL ew_processMenuSelect(HWND hStatusBar, HMENU hMenu, UINT uFlags, UINT uItem);
BOOL ew_processMenuCommand(MENUOBJ *menu, UINT uItem, BOOL fromToolBar = FALSE);
void ew_processIdleMenu(MENUOBJ *);

#ifdef __EWEXTT_BUILD
    bool ew_downloadMenu(PANEL *pnl, MENU *pMenu);
#endif //__EWEXTT_BUILD

// -----------------------------------------------------------------------------
// DlgObj
DLGOBJ *ew_createDlgObject(PANEL *, DLGOBJ *);
void    ew_deleteDlgObject(DLGOBJ *);

// -----------------------------------------------------------------------------
// DlgScroll
DLGSCROL *ew_createDlgScrol(int, int, int, int, int, int, void *, size_t);
void ew_deleteDlgScrol(DLGSCROL *);

// -----------------------------------------------------------------------------
// DlgColumn
void ew_initDlgColumn(const DLGOBJ *);

// -----------------------------------------------------------------------------
// DlgItems
void ew_initDlgItems(DLGOBJ *);

// -----------------------------------------------------------------------------
// DlgCeil
DLGCEIL **ew_allocDlgCeil(DLGSCROL *);
void ew_deleteDlgCeil(DLGSCROL *);
void ew_moveDlgCeil(DLGSCROL *scrol, int dest, int src, int count);

// -----------------------------------------------------------------------------
// DlgEditor
DLGEDITOR *ew_createDlgEditor(int, int, int, int, bool isImager);
bool       ew_reallocDlgEditor(DLGEDITOR *, int dx, int dy, int flag);
void       ew_deleteDlgEditor(DLGEDITOR *);


// -----------------------------------------------------------------------------
// TxtBuffer
TCHAR **ew_allocTxtBuffer(int dx, int dy);
TCHAR **ew_reallocTxtBuffer(TCHAR **, int dx, int dy, int new_dx, int new_dy);
void ew_freeTxtBuffer (TCHAR **);

// -----------------------------------------------------------------------------
// DlgTab
DLGTAB *ew_createDlgTab(int, int, int, int, const char *);
void    ew_deleteDlgTab(DLGTAB *);

// -----------------------------------------------------------------------------
// PopupObj
POPUPOBJ  *ew_createPopupObject(const char *text, const char *btext, const char *status,
                                const char *caption, bool isWeb, const char* result);
void       ew_deletePopupObject(POPUPOBJ *);

// -----------------------------------------------------------------------------
// Dialog
bool ew_createDialog(DLGOBJ *);
void ew_destroyDialog(DLGOBJ *);
bool ew_isActiveDlg(DLGOBJ *dlgobj);
RECT ew_getDlgItemRect(const DLGOBJ *, int);

// -----------------------------------------------------------------------------
// Scroll
bool ew_createScroll(DLGOBJ *);
void ew_runScroll(const DLGOBJ *, bool);
RECT ew_getScrollRect(const DLGOBJ *, int, int);
RECT ew_getScrollEditRect(const DLGOBJ *);
void ew_invalidScrollRect(const DLGOBJ *, int, int, BOOL);
void ew_checkNumRec(DLGOBJ *dlgobj, int numRec, int op);
UINT ew_getColumnWidth(const DLGOBJ *, int nColumn);
void ew_setColumnWidth(const DLGOBJ *, int nColumn, UINT nWidth);
void ew_setColumnWidthMH(const DLGOBJ *, int nColumn, UINT nWidth);
void ew_moveScrollColumn(const DLGOBJ *, int deltaX);
void ew_moveScrollRow(const DLGOBJ *, int row, int num, int delta);
BOOL ew_hittestScroll(const DLGOBJ *, POINT *, int *, int *);
void ew_scrollGoToCol(const DLGOBJ * dlgobj, int col);
int  ew_scrolFldNToColN(const DLGOBJ *blgobj, int fld);
void ew_updateHeaderMH(const DLGOBJ *dlgobj, int col, int new_cx, int right);
void ew_storeHeaderMH(const DLGOBJ *dlgobj);
void ew_restoreHeaderMH(const DLGOBJ *dlgobj);
void ew_cutHeaderMH(const DLGOBJ *dlgobj, int dx);

// -----------------------------------------------------------------------------
// Field
int  ew_getFieldAlign(const FIELD* fld, int* needOffs);
void ew_addFieldStyle(const DLGFIELD *, DWORD *, DWORD *, DWORD_PTR *);
void ew_addFieldBorder(const DLGFIELD *, DWORD *pStyle, DWORD *pExStyle);
void ew_remFieldBorder(const DLGFIELD *, DWORD *pStyle, DWORD *pExStyle);

void ew_setFieldFont(const DLGOBJ *, int field);
void ew_setFieldValidator(DLGFIELD *);

void ew_enableField(HWND hFieldWnd, DLGFIELD *dlgfield, bool enable);
int  ew_processField(DLGOBJ *dlgobj, DLGFIELD *dlgfield, HWND hWnd);

void ew_initDlgField(DLGOBJ *dlgobj, DLGFIELD *dlgfield);
void ew_initListField(DLGOBJ *dlgobj, DLGFIELD *dlgfield);

void ew_putDlgFields(DLGOBJ *, int, int);
void ew_putListFields(DLGOBJ *, int, int, int);

int  ew_processDlgField(DLGOBJ *);
int  ew_processListField(DLGOBJ *, PANEL * pnl);

void ew_checkDlgFields(DLGOBJ *dlgobj);
void ew_checkListFields(DLGOBJ *dlgobj, int);

char * ew_setLViewItemText (DLGOBJ *, int, int, char *);
void ew_setLViewTreeItem (DLGOBJ *, int, int, char *);
void ew_setLViewFindData(DLGOBJ *, int, int, int, char *, unsigned char);

char * ew_field2str(FIELD *fld, char *buf, size_t sizebuf, bool fEdit, bool fOemFont);
char * ew_field2str2(FIELD *fld, char *buf, size_t sizebuf, bool fEdit, bool fOemFont);

// -----------------------------------------------------------------------------
// Header
bool ew_createHeader(DLGOBJ *dlgobj);
void ew_initHeader(DLGOBJ *dlgobj);
void ew_setHeaderText(const DLGOBJ *dlgobj);
void ew_layoutHeader(const DLGOBJ *dlgobj, int flags);
BOOL ew_notifyHeader(DLGOBJ *dlgobj, WPARAM wParam, LPARAM lParam);
void ew_updateHeaderToolTip(const DLGOBJ *dlgobj);
int  ew_headerIndToOrd(HWND hWnd, int Ind);
void ew_setColumnData(const DLGOBJ *dlgobj, int col);
BOOL ew_getHeaderRect(const DLGOBJ *dlgobj, RECT &rch);

// -----------------------------------------------------------------------------
// Editor
bool ew_createEditor(DLGOBJ *dlgobj);
void ew_runEditor(DLGEDITOR *, bool);
bool ew_resizeEditor(DLGEDITOR *editor, int deltaX, int deltaY);
BOOL ew_hittestEditor(const DLGEDITOR *, POINT *);

// -----------------------------------------------------------------------------
// Tab
bool ew_createTab(DLGTAB *, HWND);
void ew_destroyTab(DLGTAB *tab);

void ew_adjustTabCtrl(DLGTAB *);
void ew_adjustTabItem(DLGTAB *, const DLGOBJ *);
BOOL ew_notifyTab(DLGTAB *, WPARAM, LPARAM);

// -----------------------------------------------------------------------------
// Popup
bool ew_createPopup(DLGPOPUP *popup);
void ew_destroyPopup(DLGPOPUP *popup);
void ew_closePopup(DLGPOPUP *popup);

// -----------------------------------------------------------------------------
// ToolTip
bool ew_createToolTip(POPUPOBJ *popup, bool);
void ew_trackToolTip(DLGPOPUP *popup);

// ���������� ������� �������� ����������� ���������
HWND ew_createToolTips(DWORD dwStyle, DWORD dwExStyle, HWND hParentWnd, HINSTANCE hInstance);

// -----------------------------------------------------------------------------
// VldObj
VLDOBJ *ew_createVldObj(char, ...);
void ew_deleteVldObj(VLDOBJ *);
BOOL ew_processVldObj(VLDOBJ *, HWND, UINT, WPARAM, LPARAM);

// -----------------------------------------------------------------------------
// FmtObj
FMTOBJ *ew_createFmtObj(DLGFIELD *dlgfield);
void  ew_deleteFmtObj(FMTOBJ *);
BOOL  ew_processFmtObj(FMTOBJ *, HWND, UINT, WPARAM, LPARAM);
void  ew_drawFmtObj(FMTOBJ *fmtobj, HDC, HBRUSH, int isScrol);
bool  ew_isFmtEditMode(void *p);

// -----------------------------------------------------------------------------
// Format
int  ew_getFormatAlign (void *fmt, int *needOffs);
const char *ew_getFormatResult(void *fmt);
bool ew_getFormatModify(void *fmt);
bool ew_getFormatValue(void *fmt, FIELD *fld);
void ew_getFormatMode(void *fmt, BOOL *fOverWriteMode);

void ew_setFromatSelection(void *fmt, bool);
void ew_setFormatValue(void *fmt, FIELD *fld);
void ew_setFormatMode(void *fmt, BOOL fOverWriteMode);

// -----------------------------------------------------------------------------
// Clipboard
BOOL ew_processClipboard(HWND hWnd, int wID, UINT iMessage, WPARAM wParam, LPARAM lParam, int keystat);
BOOL ew_processClipboardKey(HWND hWnd, int wID, int virtkey, int kbflags);

// -----------------------------------------------------------------------------
// Application Data
void ew_initAppData(const char *appid, const char *system, const char *version, const char *copyright, bool beepMode);
void ew_cleanupAppData(void);
bool ew_readAppData(const char *key, void **pbuf, size_t *psize);
bool ew_writeAppData(const char *key, void *buf, size_t size);
void ew_loadAppData(void);
void ew_saveAppData(void);
APPDATA_ITEM *ew_addAppData(int type, size_t size);
APPDATA_ITEM *ew_findAppData(int type);

void ew_initAppServData(int cmdll, const char *hostName, const char *pipeName, int port);
void ew_cleanupAppServData(void);

RSL_CFUNC void ew_arrangeDlgItems (const DLGOBJ *);
RSL_CFUNC void ew_blinkDlgItems (const DLGOBJ *);
RSL_CFUNC void ew_movedDlgItems (const DLGOBJ *);

// -----------------------------------------------------------------------------
// Color support
void  ew_setFldColor (DLGOBJ *, int);
void  ew_setCeilColor (DLGOBJ *, int, int);
LRESULT  ew_processColor (HWND, WPARAM, LPARAM);
LRESULT  ew_processColorInLView (HWND, WPARAM, LPARAM);

// -----------------------------------------------------------------------------
// Process palette messages
LRESULT ew_processPaletteChanged(HWND hWnd, WPARAM wParam);
LRESULT ew_processQueryNewPalette(HWND hWnd);

// -----------------------------------------------------------------------------
// Window title
void *ew_saveTitle(HWND);
void  ew_restTitle(HWND, void *);

// -----------------------------------------------------------------------------
// Mouse and keyboard support
void  ew_setMessagePos(DWORD);
DWORD ew_getMessagePos(void);

void ew_putKey(int key, int target);
void ew_putChar(int ch, int target);
void ew_putCharEx(int ch, int target, bool from_WM_CHAR);
void ew_putMouseEvent(int key, int x, int y, int item, int target);
void ew_putMenuEvent(int menu);
void ew_putResizeEvent(int numcols, int numrows);
void ew_putScrolEvent(int key, int pos, int item, int target);
void ew_putWheelEvent(int key, int target);
void ew_putHeaderEvent(int key, int item, int butn, int target);

BOOL ew_processKeyboard(HWND hWnd, int virtkey, int kbflags, MSG *pMsg);
BOOL ew_processMenuKeyboard(HWND hWnd, int virtkey, int kbflags, MSG *pMsg);
BOOL ew_processPopupKeyboard(HWND hWnd, int virtkey, int kbflags, MSG *pMsg);

BOOL ew_processMouseInField(DLGFIELD *, HWND, UINT, WPARAM, LPARAM);
BOOL ew_processMouseInLView(DLGOBJ *, HWND, UINT, WPARAM, LPARAM);
BOOL ew_processMouseInEditor(DLGEDITOR *, UINT, WPARAM, LPARAM);
BOOL ew_processMouseInMenu(HWND, UINT, WPARAM, LPARAM);

BOOL ew_processScrollMessage(HWND, UINT, WPARAM, LPARAM, int);

// -----------------------------------------------------------------------------
// Message filter and idle routines
BOOL ew_filterProc(MSG *,void *);
void ew_idleProc(void *,unsigned level);

// -----------------------------------------------------------------------------
// Local message queue
void ew_addMessage(MSG *, int kbflags);
bool ew_getMessage(MSG *, int *kbflags);
void ew_pumpMessage(HWND hFocusWnd);
void ew_clearMessage(void);

int  ew_moveColumn(DLGOBJ *dlgobj, int oldOrder, int iOrder);

BOOL ew_restOrderHead(DLGOBJ *dlgobj);

void ew_setFieldAlign(const DLGFIELD *dlgfield, DWORD *pStyle);

void ew_entryUnlock(int entry);
bool ew_isLockMessage(MSG *pMsg);
void ew_destroyLockWnd(void);

// -----------------------------------------------------------------------------
// ������ �� ������������ ������ ���� ������ � ������� GDI
// [in]  buf  - ����� ��� �������������� ������
// [in]  name - �������� ��� ������ ����
// [in]  pos  - ������� "��������" �������
// [ret] ���������� ��������� �� buf ��� NULL
char *ew_MakeMenuItemString(char *buf, const char *name, int pos);

// -----------------------------------------------------------------------------
// ��������� ������ ���� �� ��� �����. ������������ ������ ������� ���������.
// ������� ��������� � �������������� ������ �� ��������.
// [in]  name  - �������� ������
// [out] buffL - ����� ����� ������
// [out] buffR - ������ ����� ������
void ew_SplitMenuItemString(const char *name, char *buffL, char *buffR);

// -----------------------------------------------------------------------------
// ToolBar
void ew_clearToolBar(FRAMEOBJ *frame);

// -----------------------------------------------------------------------------
// ��������� ToolBar
class CCustomizeToolBar 
{
 private:
   HWND      m_hToolBar;
   int       m_nResetCount; 
   TBBUTTON* m_pResetButtons; 
   std::vector<TBBUTTON> m_SavedButtons;
 public: 
   CCustomizeToolBar() {} 
   CCustomizeToolBar::CCustomizeToolBar(HWND hToolBar) 
     : m_hToolBar(hToolBar), m_nResetCount(0), m_pResetButtons(NULL){}

   ~CCustomizeToolBar(){}  

   void AddSavedButtons(TBBUTTON* tb);
   std::string NeedText(NMHDR* pNMHDR, bool bFromToolBar = true);
   void GetButtons(int& nSavedCount, TBBUTTON*& pSavedButtons);   
   void SetButtons(int nSavedCount, TBBUTTON* pSavedButtons);   

   BOOL OnBeginAdjust(NMHDR* pNMHDR, LRESULT* pResult);   
   BOOL OnEndAdjust(NMHDR* pNMHDR, LRESULT* pResult);   
   BOOL OnGetButtonInfo(NMHDR* pNMHDR, LRESULT* pResult);   
   BOOL OnQueryDelete(NMHDR* pNMHDR, LRESULT* pResult);   
   BOOL OnQueryInsert(NMHDR* pNMHDR, LRESULT* pResult);   
   BOOL OnReset(NMHDR* pNMHDR, LRESULT* pResult);   
   BOOL OnToolBarChange(NMHDR* pNMHDR, LRESULT* pResult);  
   BOOL OnNeedText(NMHDR* pNMHDR, LRESULT* lResult);   
};

// -----------------------------------------------------------------------------
// ��������� �������� �� ������� � ������ �������� bmpId
HBITMAP ew_LoadBitmap(int bmpId);
HICON   ew_LoadIcon(int iconId);

void* CreateWebFormData(const char *attr, const char *resultField);
void DeleteWebFormData(void *ptr);
HWND CreateWebControl(void *ptr, HWND parent, RECT *rect, const char *url);
void MoveWebControl(void *ptr, DWORD x, DWORD y, DWORD cx, DWORD cy, BOOL repaint);
void OnDestroyWebControl(void *ptr);
BOOL PretranslateWebControl(void *ptr, LPMSG msg);
const char* GetWebControlResult(void *ptr, size_t *sz);
void FreeWebControlResult(const char *buff);
void SetFocusWebControl(void *ptr);
bool GetDispatchWebControl(void *ptr, IDispatch **outPtr);

bool GetIntProp(const char *attr, const char *key, long *result, bool *isPerSent);


void ewInitAccessibility();
void ewDoneAccessibility(bool force);

void DeleteAccessibleObj(EwAccessibleObj *ptr);
bool CreateAccessibleProxyFor(EwAccessibleObj *ptr, WPARAM wParam, LPARAM lParam, LRESULT *pResult);

EwAccessibleObj *CreateAccessibleObjForDLGFIELD(DLGFIELD *pObj);
EwAccessibleObj *CreateAccessibleObjForDLGOBJ(DLGOBJ *pObj);
EwAccessibleObj *CreateAccessibleObjForPOPUPOBJ(POPUPOBJ *pObj);
EwAccessibleObj *CreateAccessibleObjForDLGSCROL(DLGOBJ *pObj);

EwAccessibleObj *CreateAccessibleObjForMenu(HWND hWnd, HMENU hMenu);
void RepairAccessibleObjForMenu(EwAccessibleObj *pAcc, HMENU hMenu);

void SetSatusPaneTo(const char *str);

// �������� ������ MENU, ��������������� � ������� ����
MENU *GetMenuItemData(HMENU hMenu, int item, MENUITEMINFO *mii);

// ��������� ���� hSrcMenu � ���� hDstMenu
void AppendContextMenu(HMENU hDstMenu, HMENU hSrcMenu, bool addSeparator = false);

// ��������������� ������ ���� (� ����� EWMenuData)
void RestoreMenu(HMENU hMenu);

// ��������� ��������� WM_MEASUREITEM ��� ����
BOOL Menu_ON_MEASUREITEM(HWND hWnd, LPMEASUREITEMSTRUCT lpMI);

// ��������� ��������� WM_DRAWITEM ��� ����
BOOL Menu_ON_DRAWITEM(LPDRAWITEMSTRUCT lpDIS);

// ��������� ��������� WM_CONTEXTMENU
BOOL Menu_ON_CONTEXTMENU(DLGFIELD *dlgfield, DLGOBJ *dlgobj, HWND hWnd, WPARAM wParam, LPARAM lParam);

int  ew_resizeDialog2(DLGOBJ *dlgobj, int deltaX, int deltaY);

void ew_DateToStr(TCHAR *str, size_t size, int year, int month, int day);

// ������ ��������� ������ � WM_DRAWITEM
void Button_ON_DRAWITEM(DLGOBJ *dlgobj, int fldID, DRAWITEMSTRUCT *pDIS);

// -----------------------------------------------------------------------------
// �������� �������� �� ������
void *LoadImageFromBuff(EWPictureData *data, const char *ptr, int size);

// -----------------------------------------------------------------------------
// ����������� �������� ��� ����������� ���������� � ���� � ����������� ���������.
RECT IMG_ConvertRect(RECT &rcScreen, RECT &sizePicture, bool bCenter);

// -----------------------------------------------------------------------------
// ��������� ����������� �������� ������. � ������:
//   - ������� ��������� �������
//   - ������� ������� '~'
//   - ������� �������� �������
void ew_CvtButtonName(char *str);

#ifdef __cplusplus
}
#endif

#endif //__EASYWIN_H

/* <-- EOF --> */