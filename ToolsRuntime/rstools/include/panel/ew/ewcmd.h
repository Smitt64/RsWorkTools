//-*--------------------------------------------------------------------------*-
//
// File Name   : ewcmd.h
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// October 02,2001  Alexey Lebedev - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __EWCMD_H
#define __EWCMD_H

#include "rstypes.h"

// -----------------------------------------------------------------------------
#define  EW_FS_BUFSIZE   8000
// - sizeof(TEWResult)
// - sizeof(TEWPutEvent)

// -----------------------------------------------------------------------------
enum TEWCommand
   {
    CMD_EW_INITINSTANCE = 1,
    CMD_EW_EXITINSTANCE,

    CMD_EW_SETAPPINFO,
    CMD_EW_SETCAPTURE,
    CMD_EW_GETEVENT,
    CMD_EW_SENDEVENT,

    CMD_EW_CREATEWINDOW,
    CMD_EW_DESTROYWINDOW,
    CMD_EW_OPENWINDOW,
    CMD_EW_CLOSEWINDOW,

    CMD_EW_SETTITLE,
    CMD_EW_GETTITLE,

    CMD_EW_MESWIN,
    CMD_EW_FLDWIN,
    CMD_EW_CONFWIN,

    CMD_EW_CREATEMENU,
    CMD_EW_APPENDMENU,
    CMD_EW_DESTROYMENU,
    CMD_EW_ENABLEMENUITEM,
    CMD_EW_DELETEMENUITEM,
    CMD_EW_INSERTMENUITEM,
    CMD_EW_LOOPMENU,

    CMD_EW_PUTPANEL,
    CMD_EW_PUTFIELDS,
    CMD_EW_PUTTEXTS,
    CMD_EW_PROCFIELD,
    CMD_EW_PUTFIELD,
    CMD_EW_RUNPANEL,
    CMD_EW_REMPANEL,
    CMD_EW_SWITCHPANEL,
    CMD_EW_SETFLDPARM,
    CMD_EW_GETFLDTEXT,

    CMD_EW_SETSCROLLBAR,

    CMD_EW_PUTSCROL,
    CMD_EW_PUTBSCROL,
    CMD_EW_MOVESCROLL,
    CMD_EW_CLEARITEM,
    CMD_EW_SCROLLTO,
    CMD_EW_SCROLLRESIZE,
    CMD_EW_SCROLLSAVPAR,
    CMD_EW_SHOWCOLUMN,
    CMD_EW_DRAWHEAD,
    CMD_EW_SELECTCOLUMN,
    CMD_EW_GETCOLORDWIDTH,
    CMD_EW_STARTVSCROLL,

    CMD_EW_PUTEDITOR,
    CMD_EW_SHOWCARET,
    CMD_EW_OUTPUTLINE,
    CMD_EW_UPDATEEDITOR,
    CMD_EW_SETEDITOROVRMODE,

    CMD_EW_PUTTABCTRL,
    CMD_EW_SETACTIVETAB,

    CMD_EW_PUTSTATLINE,
    CMD_EW_SAVESTATLINE,
    CMD_EW_RESTSTATLINE,
    CMD_EW_ADDSTATUSPANE,
    CMD_EW_DELSTATUSPANE,
    CMD_EW_SETSTATUSTEXT,
    CMD_EW_SETSTATUSICON,
    CMD_EW_SAVESTATUSPANE,
    CMD_EW_RESTSTATUSPANE,
    CMD_EW_PUTSTATUSTEXT,
    CMD_EW_PUTSTATUSPANE,
    CMD_EW_REMSTATUSPANE,

    CMD_EW_PUTINDACT,
    CMD_EW_REMINDACT,

 // Version 3.12
    CMD_EW_LOCKINPUT,
    CMD_EW_UNLOCKINPUT,

 // Version 3.13
    CMD_EW_RESETCOLUMNS,

 // Version 3.14
    CMD_EW_ENTRYUNLOCK,

    CMD_EW_MULTIMES = 99,

    CMD_EW_LOADSTREAM = 100,
    CMD_EW_LOADMENU,
    CMD_EW_MENUPROC,
    CMD_EW_SHOWHELP,
    CMD_EW_CALLRESIZE,
    CMD_EW_CALLHIDEF,
    CMD_EW_CALLCHNGF,
    CMD_EW_CALLTRANSF,
    CMD_EW_CALLUNLOCK,

 // Version 3.15
    CMD_EW_SCROLLEDITMODE,

    CMD_EW_WEBWIN,

    CMD_EW_CHINDIC,

 // Version 3.19
    CMD_EW_PUTEDITORBIG,
    CMD_EW_PUTPANELBIG,
    CMD_EW_PUTSCROLBIG,
    CMD_EW_PUTBSCROLBIG,
    CMD_EW_PUTTABCTRLBIG,
    CMD_EW_CREATEMENUBIG,
    CMD_EW_SHOWCOLUMNBIG,

    CMD_EW_PUTTEXTSBIG,


 // Version 3.21
    CMD_EW_PUTEDITORBIG2,
    CMD_EW_PUTPANELBIG2,
    CMD_EW_PUTSCROLBIG2,
    CMD_EW_PUTBSCROLBIG2,
    CMD_EW_PUTTABCTRLBIG2,

    CMD_EW_MESWIN2,
    CMD_EW_CONFWIN2,

 // Version 3.24
    CMD_EW_INITMESSAGEGATE,  // ���樠������ � �஡�᪨ ᮮ�饭�� �� EasyWin
    CMD_EW_DONEMESSAGEGATE,  // �����樠������ �
    CMD_EW_SENDMESSAGEGATE,  // ��ࠢ��� ᮮ�饭�� � ��

 // Version 4.26
    CMD_EW_PUTPANELBIG3,
    CMD_EW_PUTTEXTSBIG3,

    CMD_EW_BUSY,             // ��⠭����/��� �ਧ���� ������� ��⥬�
    CMD_EW_ICONMENUITEM,

 // Version 4.28
    CMD_EW_PUTBSCROLBIG3,
    CMD_EW_PUTSCROLBIG3,

 // Version 4.29
    CMD_EW_SETAPPSIZE,       // ���ᨬ�����/���������� ���� �ਫ������
    CMD_EW_SENDEDITOR,       // ��ࠢ��� ᮤ�ন��� ।���� (��� ���७�� ।����)
    CMD_EW_INITEDITOR,       // ���樠������ ��ࠬ��஢ ।���� (��⠫�� ���७�� � TxtFile)

    CMD_EW_SETHELPCONTEXT,   // ��⠭����� ���⥪�� �맮�� �ࠢ��

    CMD_EW_SETCOLUMNDATA,    // ��⠭����� ⥪�� ������� �஫����� � �ᯫ뢠�饩 ���᪠��� � ���

    CMD_EW_SETAPPSERVINFO,   // ��।��� � EW ��ࠬ���� �ᯮ��㥬��� �ࢥ� �ਫ������

    CMD_EW_SENDNOTIFY,       // �뢮� 㢥�������� � �����-��ப�

    CMD_EW_PUTBSCROLBIG4,
    CMD_EW_PUTSCROLBIG4,

    CMD_EW_UPDATEFIELDTOOLTIP,  // �������� �ᯫ뢠���� ���᪠��� � ���� ������

    CMD_EW_GET_FIELD_DATA,      // ����祭�� ������ �� ����
    CMD_EW_PUT_FIELD_DATA,      // ��ࠢ�� ������ � ����
   };

#include "packpsh1.h"

// -----------------------------------------------------------------------------
typedef struct
      {
       db_int32  result;
      } TEWResult;

//////////////////////////////////
// STARTUP AND CLEANUP
//////////////////////////////////
typedef struct
      {
       db_int16  dummy;
       db_int16  debugMode;
       db_int32  version;
       db_int16  isSql;     // In version 4 and higher
      } TEWInitInstance;

typedef struct
      {
       db_int16    dummy;
       db_uint32   client;
      } TEWExitInstance;

typedef struct
      {
       db_uint32   client;

       db_int32    version;
       db_int16    menuLoadingMode;
      } TEWModuleInfo;

//////////////////////////////////
// APP
//////////////////////////////////
typedef struct
      {
       db_int16  beepMode;
      } TEWSetAppInfo;

//////////////////////////////////
// APPSERV
//////////////////////////////////
typedef struct
      {
       db_int32  cmdll;         // ����� ࠡ���
       db_int32  port;          // ����� ���� ��
       db_int16  sizeHost;      // ����� ����� �������� (� ��⮬ 0-ᨬ����)
       db_int16  sizePipe;      // ����� ����� ������ (� ��⮬ 0-ᨬ����)
      } TEWSetAppServInfo;

//////////////////////////////////
// CAPTURE
//////////////////////////////////
typedef struct
      {
       db_int16  remote;
      } TEWSetCapture;

//////////////////////////////////
// EVENT
//////////////////////////////////
typedef struct
      {
       db_int16  wait;
      } TEWGetEvent;

typedef struct
      {
       db_int16    key;
       db_int16    kbflags;
       db_int16    scan;
       db_int16    Xm;
       db_int16    Ym;
       uchar       Butn;
      } TEWPutEvent;


//////////////////////////////////
// WINDOW
//////////////////////////////////
typedef struct
      {
       db_int16  numcols;
       db_int16  numrows;
      } TEWCreateWindow;

typedef struct
      {
       db_int16  dummy;
      } TEWDestroyWindow;

typedef struct
      {
       db_int16  dummy;
      } TEWOpenWindow;

typedef struct
      {
       db_int16  dummy;
      } TEWCloseWindow;

//////////////////////////////////
// TITLE
//////////////////////////////////
typedef struct
      {
       db_uint32   handle;
      } TEWSetTitle;

typedef struct
      {
       db_uint32   handle;
       db_int16    size;
      } TEWGetTitle;

//////////////////////////////////
// MESWIN, FLDWIN
//////////////////////////////////
typedef struct
      {
       db_int16    type;
       db_int16    help;

       db_uint32   client;
      } TEWMesWin;

typedef struct
      {
       db_int16    type;
       db_int16    help;

       db_uint32   client;

       db_int16    infoType;
       db_int32    errCode;
      } TEWMesWin2;

typedef struct
      {
       db_int16    type;
       db_int16    help;

       db_uint32   handle;
       db_uint32   client;

       db_int16    curfld;
      } TEWFldWin;

typedef struct
      {
       db_int16    type;
       db_int16    help;

       db_uint32   client;

       db_int16    curfld;
       db_int16    length;
      } TEWConfWin;

typedef struct
      {
       db_int16    type;
       db_int16    help;

       db_uint32   client;

       db_int16    curfld;
       db_int16    length;

       db_int16    infoType;
       db_int32    errCode;
      } TEWConfWin2;

typedef struct
      {
       db_int16    type;
       db_int16    help;

       db_uint32   client;
      } TEWWebWin;


//////////////////////////////////
// MENU
//////////////////////////////////

// -----------------------------------------------
typedef struct
      {
       db_int16    x;
       db_int16    help;
       db_int16    flags;

       uchar       n;
       db_uint32   handle;
       uchar       type; // 0 submenu, 1 callback, 2 retCode
       uchar       ChPos;

       db_uint32   iconID;  // �����䨪��� ���⨭�� ��� ToolBar'�
       db_uint32   mflags;  // ����� ��⮢�� 䫠��� (ᮮ⢥����� ���� MENU::flags)
      } TEWMenu;

// -----------------------------------------------
typedef struct
      {
       db_uint32   handle;
       db_uint32   client;

       db_int8     x;
       db_int8     y;
       uchar       popup;

       uchar       nMenu;
       db_uint16   menuCount;
       db_uint32   addSize;

       db_uint32   stream;
      } TEWCreateMenu;

// -----------------------------------------------
typedef struct
      {
       db_uint32   handle;
       db_uint32   client;

       db_int16     x;
       db_int16     y;
       uchar       popup;

       uchar       nMenu;
       db_uint16   menuCount;
       db_uint32   addSize;

       db_uint32   stream;
      } TEWCreateMenuBig;

// -----------------------------------------------
typedef struct
      {
       db_uint32   handle;

       uchar       nMenu;
       db_uint16   menuCount;
       db_uint32   addSize;
      } TEWAppendMenu;

// -----------------------------------------------
typedef struct
      {
       db_uint32   handle;
      } TEWDestroyMenu;

// -----------------------------------------------
typedef struct
      {
       db_uint32   handle;

       db_uint32   itemHandle;
       uchar       enable;
      } TEWEnableMenuItem;

// -----------------------------------------------
typedef struct
      {
       db_uint32   handle;

       db_uint32   itemHandle;
      } TEWDeleteMenuItem;

// -----------------------------------------------
typedef struct
      {
       db_uint32   handle;

       db_uint32   add;
       db_uint32   parent;
       uchar       posMenu;

       uchar       nMenu;
       db_uint16   menuCount;
       db_uint32   addSize;
      } TEWInsertMenuItem;

// -----------------------------------------------
typedef struct
      {
       db_uint32   handle;
       uchar       close;
      } TEWLoopMenu;

// -----------------------------------------------
typedef struct
      {
       db_uint32   handle;
       db_uint32   itemHandle;

       db_uint32   iconID;
       db_uint32   flags;
      } TEWIconMenuItem;

//////////////////////////////////
// PANEL
//////////////////////////////////
typedef struct
      {
       db_uint16   St;
       db_int8     x;
       db_int8     y;
      } TEWPnlText;

typedef struct
      {
       db_uint16   St;
       db_int16     x;
       db_int16     y;
      } TEWPnlTextBig;

typedef struct
      {
       db_uint32    St;
       db_int16     x;
       db_int16     y;
      } TEWPnlTextBig3;

typedef struct
      {
       db_uint16   St;
       db_int8     x;
       db_int8     y;
       db_int8     l;
       db_int8     h;
      } TEWPnlBar;

typedef struct
      {
       db_uint16   St;
       db_int16     x;
       db_int16     y;
       db_int16     l;
       db_int16     h;
      } TEWPnlBarBig;

typedef struct
      {
       db_uint32    St;
       db_int16     x;
       db_int16     y;
       db_int16     l;
       db_int16     h;
      } TEWPnlBarBig3;

typedef struct
      {
       uchar       Ftype;
       db_uint16   St;
       uchar       FVt;
       db_int16    FVp;
       db_int8     x;
       db_int8     y;
       db_int8     l;
       db_int8     h;
       db_int16    FHelp;
       db_uint32   FFlags;
      } TEWPnlField;

typedef struct
      {
       uchar       Ftype;
       db_uint32   St;
       uchar       FVt;
       db_int16    FVp;
       db_int16     x;
       db_int16     y;
       db_int16     l;
       db_int16     h;
       db_int16    FHelp;
       db_uint32   FFlags;
       db_int32    iconID;
       db_int32    iconFlag;
       db_uint32   St2;
      } TEWPnlFieldBig;

typedef struct
      {
       db_int8  x;
       db_int8  cx;
       uchar    visible;
       uchar    align;
      } TEWPnlSect;

typedef struct
      {
       db_int16 x;
       db_int16 cx;
       uchar    visible;
       uchar    align;
      } TEWPnlSectBig;

typedef struct
      {
       db_uint32   handle;
       db_uint32   client;

       db_uint16   St;
       uchar       Pnumb;
       uchar       Pnumt;
       uchar       Pnumf;
       uchar       Pnums;
       db_int8     x1;
       db_int8     y1;
       db_int8     x2;
       db_int8     y2;
       db_uint32   flags;
       uchar       proc;
       db_int16    PHelp;
      } TEWPutPanel;

typedef struct
      {
       db_uint32   handle;
       db_uint32   client;

       db_uint32   St;
       uchar       Pnumb;
       uchar       Pnumt;
       uchar       Pnumf;
       uchar       Pnums;
       db_int16    x1;
       db_int16    y1;
       db_int16    x2;
       db_int16    y2;
       db_uint32   flags;
       uchar       proc;
       db_int16    PHelp;
       char        PHelpContext[255]; // ��. ��䠩� D_HELPCONTEXTLEN �� panel\h\menu.h
       uchar       hasMenu;           // #3392
       db_uint32   flags2;            // �த������� flags
      } TEWPutPanelBig;


typedef struct
      {
       uchar       mask;
      } TEWFldValue;

typedef struct
      {
       db_uint32   handle;

       db_int16    item;
      } TEWPutFields;

typedef struct
      {
       db_uint32   handle;
      } TEWPutTexts;

typedef struct
      {
       db_uint32   handle;

       db_int16    item;
       db_int16    field;
       db_int16    nitems;
      } TEWProcFld;

typedef struct
      {
       db_uint32   handle;

       db_int16    field;
      } TEWPutField;

typedef struct
      {
       db_uint32   handle;
       db_uint32   use;
      } TEWRunPanel;

typedef struct
      {
       db_uint32   handle;
      } TEWRemPanel;

typedef struct
      {
       db_uint32   handle;
      } TEWSwitchPanel;

typedef struct
      {
       db_int16    breakYear;
       uchar       ena32dec;
       uchar       overMode;

       db_int16    SNR_maxLen;
       db_int16    SNR_pos;
      } TEWSetFldParm;

typedef struct
      {
       db_uint32   handle;

       db_int16    field;
      } TEWGetFldText;

typedef struct
      {
       db_uint32   handle;
       db_int16    size;
      } TEWSetHelpContext;

typedef struct
      {
       db_uint32   handle;
       db_int16    field;
       db_int16    size;
      } TEWUpdateFieldTooltip;

//////////////////////////////////
// SCROLLBAR INFO
//////////////////////////////////
typedef struct
      {
       db_uint32   handle;

       db_int16    flags;
      } TEWSetScrollBar;

//////////////////////////////////
// SCROL, BSCROL
//////////////////////////////////
typedef struct
      {
       db_int8     x;
       db_int8     y;
       db_int8     l;
       db_int8     h;
       db_int16    Mn;
      } TEWPutScrol;

typedef struct
      {
       db_int16     x;
       db_int16     y;
       db_int16     l;
       db_int16     h;
       db_int16    Mn;
      } TEWPutScrolBig;


typedef struct
      {
       db_int8     x;
       db_int8     y;
       db_int8     l;
       db_int8     h;
       db_int16    Mn;
      } TEWPutBScrol;

typedef struct
      {
       db_int16     x;
       db_int16     y;
       db_int16     l;
       db_int16     h;
       db_int16    Mn;
      } TEWPutBScrolBig;

typedef struct
      {
       db_uint32   handle;

       db_int16    row;
       db_int16    num;
       db_int16    delta;
      } TEWMoveScroll;

typedef struct
      {
       db_uint32   handle;

       db_int16    item;
      } TEWClearItem;

typedef struct
      {
       db_uint32   handle;

       db_int16    field;
      } TEWScrollTo;

typedef struct
      {
       db_uint32   handle;

       uchar       mode;
      } TEWScrollEditMode;

typedef struct
      {
       db_uint32   handle;

       db_int16    deltaX;
       db_int16    deltaY;
      } TEWScrollResize;

typedef struct
      {
       db_uint32   handle;
      } TEWScrollSavPar;

typedef struct
      {
       db_uint32   handle;

       uchar       nColumn;
       db_int8     l;
      } TEWShowColumn;

typedef struct
      {
       db_uint32   handle;

       db_int16     nColumn;
       db_int16     l;
      } TEWShowColumnBig;


typedef struct
      {
       db_uint32   handle;
      } TEWResetColumns;

typedef struct
      {
       db_uint32   handle;
      } TEWDrawHead;

typedef struct
      {
       db_uint32   handle;

       db_uint32   mode,     // ����� (�� �ᯮ������)
                   flags,    // ���� (�� �ᯮ������)
                   column;   // ����� �������
       db_uint16   sizeText, // ������ ���������
                   sizeTips; // ������ ���᪠���
      } TEWColumnData;

typedef struct
      {
       db_uint32   handle;

       db_int16    nfield;
       db_int32    type;
       uchar       mode;
      } TEWSelectColumn;

typedef struct
      {
       db_uint32   handle;

       db_int16    size;
      } TEWGetColOrdWidth;

typedef struct
      {
       db_uint32   handle;
      } TEWStartVScroll;

//////////////////////////////////
// TEXT EDITOR
//////////////////////////////////
typedef struct
      {
       db_int8     x;
       db_int8     y;
       db_int8     l;
       db_int8     h;
      } TEWPutEditor;

typedef struct
      {
       db_int16     x;
       db_int16     y;
       db_int16     l;
       db_int16     h;
      } TEWPutEditorBig;

typedef struct
      {
       db_int16     result;
       db_int16     l;
       db_int16     h;
       db_int16     flag;
      } TEWGetEditorBig;


typedef struct
      {
       db_uint32   handle;

       db_int16    line;
       db_int16    pos;
       db_int16    caret;
      } TEWShowCaret;

typedef struct
      {
       db_uint32   handle;

       db_int16    line;
       db_int16    len;
       db_int16    selB;
       db_int16    selE;
      } TEWOutputLine;

typedef struct
      {
       db_uint32   handle;
      } TEWUpdateEditor;

typedef struct
      {
       db_uint32   handle;    // ����� 㤠������ ������

       db_int16    cp;        // ����஢�� ।����

       db_int32    nameLen;   // ����� ����� 䠩�� (��� 0 ᨬ����)
       db_int32    textLen;   // ����� ᮤ�ন���� ।���� (��� 0 ᨬ����)
      } TEWSendEditor;

typedef struct
      {
       db_int16    cmdll;    // ����� ࠡ��� (������/��姢����)

       db_int16    extLen;   // ����� ��� � ���७�� (��� 0 ᨬ����)
       db_int16    txtLen;   // ����� ��� � ��⠫��� TxtFile (��� 0 ᨬ����)
      } TEWInitEditor;

//////////////////////////////////
// TAB CONTROL
//////////////////////////////////
typedef struct
      {
       db_int8     x;
       db_int8     y;
       db_int8     l;
       db_int8     h;
      } TEWPutTabCtrl;

typedef struct
      {
       db_int16     x;
       db_int16     y;
       db_int16     l;
       db_int16     h;
      } TEWPutTabCtrlBig;

typedef struct
      {
       db_uint32   handle;
       db_int16    curtab;
      } TEWSetActiveTab;

//////////////////////////////////
// STATUS LINE
//////////////////////////////////
typedef struct
      {
       db_int16 dummy;
      } TEWPutStatLine;

typedef struct
      {
       db_uint32   handle;
      } TEWSaveStatLine;

typedef struct
      {
       db_uint32   handle;
      } TEWRestStatLine;

typedef struct
      {
       db_int16    id;
       db_int16    cx;
       db_int16    before;
      } TEWAddStatusPane;

typedef struct
      {
       db_int16    id;
      } TEWDelStatusPane;

typedef struct
      {
       db_int16    id;
       db_int32    flags;
      } TEWSetStatusText;

typedef struct
      {
       db_int16    id;
       db_int32    flags;
       db_int32    icon;     // �����䨪��� ������
       db_int16    size;     // ������ �ᯫ뢠�饩 ���᪠��� (� ��⮬ 0-ᨬ����)
      } TEWSetStatusIcon;

typedef struct
      {
       db_int16    id;
       db_uint32   handle;
      } TEWSaveStatusPane;

typedef struct
      {
       db_int16    id;
       db_uint32   handle;
      } TEWRestStatusPane;

typedef struct
      {
       db_uint32   handle;

       db_int16    id;
      } TEWPutStatusText;

typedef struct
      {
       db_uint32   handle;

       db_int16    id;
       db_int16    cx;
      } TEWPutStatusPane;

typedef struct
      {
       db_uint32   handle;

       db_int16    id;
      } TEWRemStatusPane;


// ��� ᮮ�饭�� CMD_EW_SENDNOTIFY
typedef struct
      {
       db_int16    cmd;      // ���������� (0 ��� dCMD_TT_* �� easywin.h)
       db_int16    paneID;   // �����䨪��� ᥪ樨 �����-��ப�
       db_int32    iconID;   // �����䨪��� ������
       db_int32    timeOut;  // ����-���
       db_int16    sizeHead; // ������ ��������� 㢥�������� (� ��⮬ 0-ᨬ����)
       db_int16    sizeText; // ������ ⥪�� 㢥�������� (� ��⮬ 0-ᨬ����)

       db_int32    colorText;  // ���� ⥪�� ᮮ�饭��
       db_int32    colorBack;  // ���� 䮭� ᮮ�饭��
      } TEWNotify;

//////////////////////////////////
// ACTIVE INDICATOR
//////////////////////////////////
typedef struct
      {
       db_int16    dummy;
       db_uint32   flags;
      } TEWPutIndAct;

typedef struct
      {
       db_int16 dummy;
      } TEWRemIndAct;

//////////////////////////////////
// LOCK INPUT
//////////////////////////////////
typedef struct
      {
       db_int32    flags;
       db_uint32   handle;
      } TEWPutLockInput;

typedef struct
      {
       db_int16 dummy;
      } TEWPutUnlockInput;

typedef struct
      {
       db_uint32   handle;

       db_int16    entry;
      } TEWEntryUnlock;

//////////////////////////////////
// MULTI MESSAGE
//////////////////////////////////
typedef struct
      {
       db_int16 cmd;
       db_int16 sz;
      } TEWSingleMes;

typedef struct
      {
       db_int16 nmes;
      } TEWMultiMes;

//////////////////////////////////
// LOAD STREAM
//////////////////////////////////
typedef struct
      {
       db_uint32 handle;
       db_int32 size;
      } TEWLoadStream;

//////////////////////////////////
// LOAD MENU
//////////////////////////////////
typedef struct
      {
       db_uint32   handle;
       db_uint32   key;

       uchar       nMenu;
      } TEWLoadMenu;

//////////////////////////////////
// MENU PROC
//////////////////////////////////
typedef struct
      {
       db_uint32   handle;
       db_uint32      key;
      } TEWMenuProc;

//////////////////////////////////
// HELP
//////////////////////////////////
typedef struct
      {
       db_int32 page;
       char     context[255];  // ��. ��䠩� D_HELPCONTEXTLEN �� panel\h\menu.h
       db_int32 key;           // �����䨪��� �㭪� ����
      } TEWShowHelp;

//////////////////////////////////
// CALL RESIZE
//////////////////////////////////
typedef struct
      {
       db_uint32   handle;

       db_int16    x;
       db_int16    y;
       uchar       flag;
      } TEWCallResize;

//////////////////////////////////
// CALL SETHIDEF
//////////////////////////////////
typedef struct
      {
       db_uint32   handle;

       db_int16    nColumn;
       db_uint16   nWidth;
      } TEWCallHideF;

//////////////////////////////////
// CALL SETCHNGF
//////////////////////////////////
typedef struct
      {
       db_uint32   handle;
      } TEWCallChngF;

//////////////////////////////////
// CALL TRANSFERFIELD
//////////////////////////////////
typedef struct
      {
       db_uint32   handle;

       db_int16    oldi;
       db_int16    newi;
      } TEWCallTransf;

//////////////////////////////////
// CALL UNLOCK
//////////////////////////////////
typedef struct
      {
       db_uint32   handle;
      } TEWCallUnlock;

//////////////////////////////////
// ������� ��� ᮮ�饭�� CMD_EW_INITMESSAGEGATE
//////////////////////////////////
typedef struct
      {
       db_int32  version;   // ����� ��������
       db_int32  pMessage;  // �ਢ������ 㪠��⥫� �� �㭪�� ��ࠢ�� ᮮ�饭��
      } TEWInitMessageGate;

//////////////////////////////////
// ������� ��� ᮮ�饭�� CMD_EW_SENDMESSAGEGATE
//////////////////////////////////
typedef struct
      {
       db_int32  version; // ����� ��������
       db_int32  stat;    // ���祭��, �����頥��� ��ࠡ��稪�� ᮮ�饭��
       db_int32  cmd;     // �����䨪��� ᮮ�饭��
       db_int32  data;    // �᭮��� ����� ᮮ�饭��
       db_int32  add;     // �������⥫�� ����� ᮮ�饭��
      } TEWMessageGate;

//////////////////////////////////
// ������� ��� ᮮ�饭�� CMD_EW_BUSY
//////////////////////////////////
typedef struct
      {
       db_int32  version; // ����� ��������
       db_int32  busy;    // ���� �������
      } TEWBusy;

//////////////////////////////////
// ������� ��� ᮮ�饭�� CMD_EW_SETAPPSIZE
//////////////////////////////////
typedef struct
      {
       db_int32  cmdSize; // �����
      } TEWSetAppSize;

// -----------------------------------------------------------------------------
// �������� ��� ᮮ�饭�� CMD_EW_PUTBSCROLBIG4/CMD_EW_PUTCROLBIG4

// -----------------------------------------------
// ���������
typedef struct
      {
       db_int16   version;        // ����� ����䥩� (���� 0)
       db_int16   num;            // ��᫮ ����஫��
       db_int32   size;           // ����� ࠧ��� ������ ���������
       db_int16   y;              // BSCROL::old_y
       db_int16   n;              // BSCROL::old_Mn
      } TEWMLHeader;

// -----------------------------------------------
// ����஫ ���������
typedef struct
      {
       db_int8    ch;             // �ਧ��� ����஫� ('k')
       db_int16   num;            // ��᫮ �祥� � ����஫�
       db_int16   x0, y0, x1, y1; // ���न���� ������ ���孥�� � �ࠢ��� ������� 㣫�� ����஫�
      } TEWMLControl;

// -----------------------------------------------
// �祩�� ����஫�
typedef struct
      {
       db_int8    ch;             // �ਧ��� �祩�� ('c')
       db_int16   width;          // ��ਭ� �祩��
       db_int16   last;           // �ਧ��� ��᫥���� �祩��
       db_int16   idx;            // ����� �祩�� � ��᫥���� ��ப� (��稭����� � 1-��)
       db_int16   size;           // ����� ���� text
       char       text[1];        // �������� �祩��
      } TEWMLCell;


// -----------------------------------------------------------------------------
// ����� ��� ���� TEWPicture::flags

#define  dTEWP_FLAGS_READONLY     0x01
#define  dTEWP_FLAGS_EXTVIEWER    0x02


// -----------------------------------------------------------------------------
// ���� �訡��, �����頥�� � TEWFieldDataOut::status
#define  dTEWFDO_ERROR_Ok           0   // �ᯥ譮
#define  dTEWFDO_ERROR_ZeroSize     5   // �㫥��� ࠧ��� ������ (����� ���⨭��)
#define  dTEWFDO_ERROR_NoMem       33   // �� 㤠���� �뤥���� ���� (᫨誮� ����訥 �����)

// -----------------------------------------------------------------------------
// ���� ������� ��� ��।�� ���⨭��
// ���� classid, count � curpos �㦭� ��� ���४⭮� ��ࠡ�⪨ ���⥪�⭮�� ����
typedef struct
      {
       db_int32   version;        // �����
       db_int16   classid;        // �����䨪��� ����� ��ꥪ�
       db_int16   count;          // ������⢮ ��ꥪ⮢ � ᯨ᪥
       db_int16   curpos;         // ����� ⥪�饣� ��ꥪ�
       db_int32   flags;          // ����� 䫠��� (dTEWP_FLAGS_*)
       db_int32   maxsize;        // ���ᨬ���� ࠧ��� ���⨭��
       db_int16   maxcount;       // ���ᨬ��쭮� �᫮ ���⨭�� � ᯨ᪥
       db_int32   size;           // ������ ����� ������ �� ������ன
       // ...
      } TEWPicture;

// -----------------------------------------------------------------------------
// ������� ��� ����� ������ �ࢥ஬ �� EasyWin (ᮮ�饭�� CMD_EW_GET_FIELD_DATA � CMD_EW_PUT_FIELD_DATA)
typedef struct
      {
       db_int32   version;        // �����
       db_uint32  handle;         // ���ਯ�� ������
       db_int16   field;          // ����� ���� � ������
       db_int16   type;           // ��� ������ (dCLASSID_* �� rsimage.h)
       db_int32   flags;          // ����� 䫠��� (dTEWP_FLAGS_*)
       db_int32   size;           // ������ ����� ������ �� ������ன (��� ᮮ�饭�� CMD_EW_PUT_FIELD_DATA)
       // ...
      } TEWFieldDataIn;

// -----------------------------------------------------------------------------
// ������� ��� ��।�� ������ �� EasyWin �� �ࢥ� (ᮮ�饭�� CMD_EW_GET_FIELD_DATA)
typedef struct
      {
       db_int32   result;         // ��� ᮢ���⨬��� � TEWResult
       db_int32   status;         // ������� ����樨 (�. dTEWFDO_ERROR_)
       db_int32   size;           // ������ ����� ������ �� ������ன
       // ...
      } TEWFieldDataOut;

#include "packpop.h"

#endif //__EWCMD_H

/* <-- EOF --> */