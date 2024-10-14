// -------------------------------------------------------------------------------------------------

#ifndef _TOOLVER_H_
#define _TOOLVER_H_

#define  RSL_COMPLECT_VER_HI    19
#define  RSL_COMPLECT_VER_LO    14
#define  RSL_COMPLECT_VER_STR   "19.14."


#define  RSL_TOOLS_VER_HI        6
#define  RSL_TOOLS_VER_LO       57
#define  RSL_TOOLS_VER_BUILD   714
#define  RSL_TOOLS_VER_PATCH     6


#define  RSL_TOOLS_VERSION    (RSL_TOOLS_VER_HI*100000000 + RSL_TOOLS_VER_LO*100000 + RSL_TOOLS_VER_BUILD*100 + RSL_TOOLS_VER_PATCH)
#define  RSL_TOOLS_VER_STR    "6.57.714.6\0"


// �ᯮ������ � rsrtlwm.rc ��� 㭨���쭮�� ���ᨨ.
#define  RSL_TOOLS_VER_STRING  RSL_TOOLS_VER_STR

// -----------------------------------------------------------------------------
// #2850 ����᫥���騥 ����⠭�� ᮤ�ঠ� �������쭮 �ॡ㥬�� ����� �� ���
//       ���४⭮� ࠡ��� � ����� �����㬥�⠫�� �������⮬.
#define  APPSERV_MIN_VER_STRING    "7.16.300.0"


/*
24.06.2011
- #167564 -�訡�� � ��⥬��� rsl-��楤�� StringIsNumber
- #167584 - �஡��� � ��� �� �ନ���� �� ���� ࠡ���� ������ "��ᬮ�� ��" � ������� �롮� 䠩��.

16.06.2011
- #164121 - ��������� ��⥬��� ��楤�� � RSL -  StrIsNumber(), �믮������ �஢�ઠ �� �, �� �� ᨬ���� ��ப� - �᫠ (isdigit() )

14.06.2011
- #166036 - ��ࠡ�⪠ ������ �롮� 䠩�� (rstools\panel\c\openfile.c).

03.06.2011
- #165511  - �⪫�祭�� ��ୠ����樨 �� ࠡ�� �� RunScroll

10.05.2011
- #166327 - ��������� ����ன�� � RSREQ.INI    TESTMODE.
����ன�� ����� �ᯮ�짮������ ��� ����ன�� ��⮬�⨧�஢����� ��⮢ � RS-Bank.
�᫨ TESTMODE=1,  RSL-�㭪樨 Date() � Time() �������� ����� ⥪�饩 �������୮� ���� � ⥪�饣� ��⥬���� �६��� ᫥���騥 ���祭��:
Date()  31.12.2099
Time()  23:59:59:00

03.05.2011
- #165561 - ��ࠡ�⪨ xml-��ୠ����樨
  - ���� ����஫��� �㬬 crc32 ��� xml-����ᥩ � ��ୠ�� ⥯��� ��� ��� ���ᠭ�� � FMT_FIELDS. crc32 ��⠥� � RSDB �� RecBuf+xml-��ப�.
  - ���������� �� ��� ���� T_RESERV ����� ����� � �������� OPERLOG_3 - T_VARLENXML,T_VARLENBIG.
    � �� ᪮�४�஢��� ⮫쪮 xml-⠡����, � ᮮ⢥���饩 ������ன: DXML_FISCLOG_DBT,DXML_FISCLOG_TMP,DXML_OPERLOG_DBT,DXML_OPERLOG_TMP.

25.03.2011
- ��ࠢ����� ��� xml-��ୠ����樨: #164976,#164965,#164944,#164939,#164896
- 㢥��祭�� ����� ���� Reserve � OPERMAN � 80 �� 130: #147198
- �뢮� ������ ����� � ������� � �஫������: #159331

18.03.2011 - ��ࠢ��� easywin.
- #162258 - �������⥫쭮 ॠ�������� ����������� ��⠭�������� ����ࠦ���� -䮭 ��� �������� ����
����ன�� BACK_PICTURE � easywin.ini
- #164806 - ��ࠡ�⠭ ��堭��� �����樨 ����������.
��������� ����ன�� ����祭��\�몫�祭�� � rsreq.ini � term.ini ("MINIDUMP")

16.03.2011 - ��ࠢ��� easywin.
- #162258 - ����������� ����ࠨ���� 梥� 䮭� �������� ���� rs-bank, ��ࠬ��� � easywin.ini BACK_COLOR.

09.03.2011
- #163854 - ॠ������� ���� ०�� xml-��ୠ����樨 XMLLOG=2 (��ୠ������ ⮫쪮 � XML-⠡����)

14.02.2011
- #163573 - �� ࠡ�⠥� AddMultiAction ��� AddScroll

11.02.2011
- #122413 - 㢥��祭�� ࠧ��� sql-����ᮢ � ���஢��
- #155389 - ॠ������ ࠡ��� � xml-��ୠ����

07.02.2011
- #163456,#163153 - ��ࠢ���� exception � ���஢�� ���.

02.02.2011
- #163117 - �� ��뢠���� html-help �� ���� ��� ����ᮢ.
- �163170 - exception � ���஢�� ���.

19.01.2011
- #157461,#157524 - ��� ������ � RunScroll,AddScroll ��������� �㭪樨 GetValueScrollField, SetValueScrollField
- #156859 - ��� ࠡ��� � RunScroll,AddScroll ��������� �㭪�� IsScrollEditMode

24.12.2010
- #160719 - ���ࠡ�⪠ rsl-�㭪権 - RunDialog,AddScroll,RunScroll.
����������� ����������� ���������� �� ������ ��᪮�쪨� �஫������.

13.12.2010 VERSION 4.52.43  (16.61, 17.39)
- #162292 - RSL. ����������� ��������� ⨯�� STRING->DATE, STRING->TIME �� ��᢮����.
- #162293 - ॠ�������� ����७��� �࠭���� ��⠨��ଠ樨 � ॣ���஧���ᨬ�� ����.
��ࠡ�⠭� ��楤��� rsl-����� RCW:
PrintObject, GetObjProps, GetObjMethods - �������� bool-��ࠬ��� CaseSeinsitive.
��-㬮�砭�� �� �㭪樨 �� �०���� �������� ��⠨��ଠ�� � ���孥� ॣ����.

07.12.2010 VERSION 4.52.42  (16.60, 17.38)
- #162069 - �訡�� � DBinit, ��������� BTRIEVE-⠡��� � ORACLE, ������� ������� �᫠ (���ਬ�� 49980.0000)  � 10000 ࠧ �����.
- #161479 - � �஫����� ��᫥ ����� � �믮������ �㭪樨 BsGoToPos, �㡫������� ��ப�

24.11.2010 VERSION 4.52.41  (16.59, 17.37)
- #160923 - RunDialog+AddScroll, ��ࠡ�⪠ - �� ���室� ����� ������ � �஫������ ����� �� ���뫠���� ᮡ�⨥ DLG_DESTROY
- #160723 - RunDialog+AddScroll, ��ࠡ�⪠ - �� ��ࠡ�⪥ ᮡ��� DLG_BUTTON (����⨥ ������) ⥯��� ����� ������ CM_UPDATE_ADDSCROLL

08.11.2010 VERSION 4.52.40  (16.58, 17.36)
- #160832 - �訡�� �஢�ન 楫��⭮�� � ��ୠ�� �㤨� (��ࢨ� ����)

27.10.2010 VERSION 4.52.39  (16.57, 17.35)
- #151419 - ���������� ������ ���� � �ଠ⮬ /w11

21.10.2010 VERSION 4.52.38  (16.56, 17.34)
- #160679 - �孮����� �⫠����� ������ (MINIDUMP)

16.06.2010 VERSION 4.52.37  (16.54, 17.32)
- #156915 - �����४⭠� ॠ������ �����㬥�⠫쭮� �㭪樨 getmsgfile()

25.05.2010 VERSION 4.52.36  (16.53, 17.31)
- #156038 - �����㬥�� ��ࠡ�⠭, �� ���᪥ �� �⮫��� � �஫���� ࠧ��� ���� ����� ���祭�� ���᪠ ��� �᫮��� � �������� �����
  ������ ���ᨬ��쭮 �������� ��� ������� ⨯� ����.

13.05.2010 VERSION 4.52.35  (16.51, 17.30)
- ��ࠡ�⠭� RSL-�㭪樨 RunDialog, AddScroll (ॠ������ �� RSL ���짮��⥫�᪮�� ����䥩� - ������-�������� + �஫����).
  ��������� ᮡ�⨥ �����頥��� �� RSL-��� ��ࠡ��稪�� ������ � �஫����� - CM_UPDATE_ADDSCROLL.
  �����㬥�� ��ࠡ��뢠�� �� ᮡ�⨥ ᫥���騬 ��ࠧ�� - ����뢠�� �஫���� � ������ ��� ���樠������� � ���ᮢ뢠��, ��뢠�
  ��� �⮣� ��ࠡ��稪 ������ � ᮡ�⨥� DLG_PREINIT, �� ���஬� ���짮��⥫� ������ �믮����� ���������� �஫�����
  �� ����� AddScroll.  ����� ��ࠧ�� ����� ॠ�������� �� RSL ���������� ����� ⮫쪮 ������ �஫�����
  ��� ����ᮢ�� �ᥣ� �������, ���ਬ�� �� ������ ��।������� ������.

07.05.2010 VERSION 4.52.34  (16.50, 17.29)
- ��ࠢ���� ��� �訡�� �� ࠡ�� � ����� � rsl-�㭪樨 AddScroll (playrep2.c).
  1. ����� ����� �뫮 ��४����� 䮪�� � ��᫥���� ������ ��������� ������������ �஫�����.
  2. �� �ப��稢���� �஫����� ����ᮬ ���, � ���짮��⥫�᪨� ����� �� ���뫠���� ᮡ�⨥ DLG_INREC.

23.04.2010 VERSION 4.52.33  (16.49, 17.28)
- ��ࠢ���� �訡��. � �㭪樨 TrnEx (trn.c) �ந�室�� exception, � ��砥 �᫨ �� �६� �࠭���樨
��������� �訡��, �易���� � �����७�� ����㯮�.

15.03.2010 VERSION 4.52.32  (16.47, 17.26)
- ��ࠢ���� �訡��. �� ��뢠��� ���⥪��� help (F1) �� ��⥬��� ᮮ�饭�� �� �訡��� RS-Bank.

01.03.2010 VERSION 4.52.31  (16.46, 17.25)
- ������� �訡�� �������樨 � �ਪ������ ���� (���⭮���). ������ #define stat( �� #define rs_macro_stat(

16.02.2010 VERSION 4.52.31  (16.45, 17.24)
- �᪮७� �஢�ઠ 楫��⭮�� � �᪠�쭮� ��ୠ�� (�� F2)

08.02.2010 VERSION 4.52.30  (16.44, 17.23)
- � ������ ���� ⨯� Numeric �ᥣ�� �⮡ࠦ����� � ������䠬�-ࠧ����⥫ﬨ ࠧ�冷� �㬬

25.12.2009 VERSION 4.52.29  (16.43)
- ��ࠬ��� edit_user � edit_item ? ��-㬮�砭� ���樠����஢�� 1

11.11.2009 VERSION 4.52.28  (16.40, 17.19)
- ����������� ����������� �롮� 䠩��� �� �ନ���� �� ����� �⠭���⭮�� ���� �롮� 䠩���. ����� 2679.
- ����뫠�� 䠩��� �� �ନ��� �ᯮ���� ���� �롮� 䠩��� �� �ନ����. ����� 2750.
05.11.2009
- ��ࠡ�⪠ Tab Control. ��� ��४��祭�� �������� ��誮� ���뫠���� ᮮ�饭�� K_TABSWITCH. ����� 2763.
- �訡�� � ���⮬ LSCROL. ����� 2768.
08.10.2009 VERSION 4.52.26  (16.38, 17.17)
- ��ࠢ����� �� ������ 2753.
08.09.2009 VERSION 4.52.24  (16.35)
- ��ࠢ����� ���� � Ecxel �� ������ 2744
02.09.2009 VERSION 4.52.22  (16.33)
- XML ���૮�
22.07.2009 VERSION 4.52.19  (16.29)
16.07.2009 VERSION 4.52.18  (16.28)
07.07.2009 VERSION 4.52.17  (16.27)
- ���ࠨ����� �஫���� � ������ ��� RSL
01.06.2009 VERSION 4.52.16  (16.26)
- ��������� RunScrol ��� TBfile � RSL.
- �맮� 䨪⨢��� ����樨 Bopenposblk
- � bfOpenUser �������� ��ࠬ��� RecBuf
- � ����䥩� IBtUserDataSource ��������� ��⮤� ����祭�� ��⠨��ଠ樨.
01.06.2009
- ��ࠡ�⪠ �� ������ 2684
21.05.2009 VERSION 4.52.14  (16.24)
- ��������� ����㯠 � �������� ��६���� BEDIT �१ TLS. ����� 2671.
27.04.2009
- �����প� ��ࠡ�⪨ ������⢥����� �롮� � RunScroll �� ������ 2661.
27.02.2009 VERSION 4.52.08  (16.16)
- ��ࠡ�⪠ �� ������ 2604 (���墠� berr).
30.01.2009 VERSION 4.52.07  (16.13)
- ���࠭���� ���������� ���� RTL � ५����� ���ᨨ �����㬥�� �� ������ 2587.
09.12.2008 VERSION 4.52.06  (16.8)
- �����প� ���᫥��� ���ࠪ⨢��� ��ࠦ���� �� ������ 2548.
04.12.2008 VERSION 4.52.04  (16.7)
- ��ࠡ�⪠ �� ������ 2559.
09.10.2008 VERSION 4.52.01  (16.1)
- ��ࠡ�⪠ �� ������ 2513.
07.10.2008 VERSION 4.52.00  (16.0)
- ���� ��࠭�祭�� �� ࠧ��� ����� � ���૮�� �� ������ 2518
04.09.2008 VERSION 4.51.77  (15.103)
- ��������� ���짮��⥫�᪮�� ���筨�� ������ �� ������ 2490
22.08.2008 VERSION 4.51.75  (15.101)
30.06.2008 VERSION 4.51.74  (15.99)
- ��ࠢ����� �� ������ 2454
24.06.2008 VERSION 4.51.72  (15.97)
- ��ࠢ����� GetCurDir �� ������ 2448
- ��ࠢ����� ������� rslmsp �� ������ 2445
07.06.2008 VERSION 4.51.71  (15.96)
- ��⮬���᪠� ॣ������ RSEM �� ������ 2437.
03.06.2008 VERSION 4.51.70  (15.95)
- �����প� ���᪨� ��� 䠩��� (�த�������)
- ��ࠢ����� �⮡ࠦ���� NUmeric � ���૮�� ��� Btrieve ०��� �� ������ 2410.
30.05.2008 VERSION 4.51.69  (15.94)
- �����প� ���᪨� ��� 䠩���
29.04.2008 VERSION 4.51.67  (15.92)
- ��ࠢ����� �� ������ 2380 (����� ���譥� �ணࠬ�� � ���᪨�� ᨬ������ � ��������� ��ப�)
- ��ࠢ����� �� ������ 2374
15.04.2008 VERSION 4.51.66  (15.91)
- ��ࠢ����� �� ������ 2343
15.04.2008
- ��ࠢ����� �� ������ 2267
02.04.2008
- ��������� ��� ��୨����.
31.03.2008 VERSION 4.51.65  (15.89)
- ��ࠢ����� �� ������ 2344
20.03.2008 VERSION 4.51.64  (15.88)
- ��ࠢ����� �� ������ 2336
28.02.2008 VERSION 4.51.62  (15.84)
- �����প� ����� �� �ନ���� �ਫ������ RS-Forms
28.02.2008 VERSION 4.51.61  (15.83)
- ��ࠢ����� �� ������ 2313.
- ��ࠢ����� �� ������ 2310.
18.02.2008 VERSION 4.51.60  (15.82)
- ��ࠡ�⪠ ।���� ��� �����প� 䠩��� � �ଠ� Unicode.
12.02.2008
- �����প� ��⮤�� ����䥩� �⫠�稪� RSL ��� ����祭�� � ��⠭���� ������� ��ப���� ���祭�� ��६�����.
05.02.2008 VERSION 4.51.59  (15.81)
- ��ࠢ����� �� ������ 2274
- ��ࠢ����� �� ������ 2275
- ��������� �㭪樨 �८�ࠧ������ ��ப �� ������ 2296
- ��������� ��ࠡ�⪠ ᮮ�饭�� RSL �� ��⠭���� ⠩��� �� ������ 2287.
- ��������� �����஢�� ��������� �맮��� ⠩����� ��楤��.
21.01.2008 VERSION 4.51.57   (15.79)
- ��ࠡ�⪠ �㭪権 �⥭�� ini-䠩��� ��� �����প� Unicode. ����� 2272.
11.01.2008 VERSION 4.51.56
- ��ࠢ����� �� ������ 2267
20.12.2007 VERSION 4.51.55
- ��������� ॠ������ ����� TUniFile
18.12.2007
- �믮����� ������ Ctl-Shift- � ���᪠���� ���� �� Alt-Shift-
14.12.2007 VERSION 4.51.54
05.12.2007 VERSION 4.51.53
- ��������� �㭪�� valtostrFmt
08.11.2007 VERSION 4.51.51
- ��ࠡ�⪠ ���૮�� �� ������ 2213
23.10.2007 VERSION 4.51.50
- ��������� �����প� ��ࠬ��� NLS_LANG � rsreq.ini
11.10.2007 VERSION 4.51.49
- ��������� �����প� ������樨 �ய��⥩. (ToolsSetProperty, ToolsGetProperty)
13.09.2007 VERSION 4.51.47
- ��ࠡ�⪠ �ଠ��� 蠡����� �� ������ 2171
05.09.2007
- ��ࠡ�⪠ Numeric �� ������ 2176.
03.09.2007
- ��������� �맮�� rsleng.chm �� ������ 2169.
27.08.2007 VERSION 4.51.46
- ��ࠢ����� �� ������ 2164.
27.07.2007 VERSION 4.51.44
- ��ࠢ����� �� ������ 2150.
18.07.2007 VERSION 4.51.43
- ��ࠢ����� �� ������ 2142.
28.06.2007 VERSION 4.51.42
- ��ࠢ���� ��横������� �� ��४��祭� popup WEB ��.
25.06.2007
- ��������� �஢�ઠ �� �ॢ�襭�� ����� ���� � �㭪�� CheckPict. ����� 2126.
21.06.2007
- �� �������樨 � DDF ����� Numeric �ᯮ�짮������ ������⢮ ������ ��᫥ �窨 �� ����ࠦ����.
20.06.2007 VERSION 4.51.41
- ��ࠢ���� ����� �஫����� � Numeric ���ﬨ � Btrieve ०���. ����� 2124.
06.06.2007 VERSION 4.51.39
- �������� ��⨢���� �����ন���� ���������� �� ���� ���� �ண�� �믮������.
  ����� 2070.
- ��������� �㭪�� BGetNewEditMode. ����� 2107.
24.05.2007 VERSION 4.51.38
- �����প� UseLocalRsFm ().
- ��������� �� ������ 2075
24.04.2007 VERSION 4.51.37
- �����প� WebForm ����� 2063.
- fs_version ��� DLM SDK �� �������� ��譥� ����, ���������饥 � DLM SDK.
- ��������� �㭪樨 ���� CurSumToStr. ����� 2061.
- ����������� ����祭�� ���ᨬ��쭮�� � �������쭮�� ���祭�� ��� BtrDecimal. ����� 2059.
16.04.2007 VERSION 4.51.36
- ��ࠢ����� � MDB
10.04.2007 VERSION 4.51.35
- ������訥 ��ࠡ�⪨ �� �����প� �ଠ��� 蠡����� ��� Numeric
04.04.2007 VERSION 4.51.34
- ��� ⨯� ���� FT_NUMERIC � FVp ᮤ�ন��� ������⢮ ������ ��᫥ �窨 ��� �⮡ࠦ���� � ����襬 ���� �
  ��� ��࠭���� � Btrive ⨯� ������ DECIMAL!
- ��ࠢ���� Numeric2BtrDec. 0.1 �८�ࠧ��뢠���� � 0.0.
20.03.2007 VERSION 4.51.33
- �����প� ��� RSL ��楤�� GoToScroll � UpdateScroll
14.03.2007 VERSION 4.51.32
- ��ࠢ����� MDB �� ������ 2006.
06.03.2007 VERSION 4.51.31
- �����প� � ini-䠩��� ����⠭���� �� ��६����� �।� �� ������ 1959.
28.02.2007 VERSION 4.51.30
- ���࠭��� �᪫�祭�� � UseIndic �� ������ 1977
21.02.2007 VERSION 4.51.29
- �㭪�� StartProgram �����⢫�� ���� �ணࠬ�� � ॥��� � "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\"
13.02.2007 VERSION 4.51.28
- ��ࠢ����� �� ������ 1957. ���� ���� ⥪��� MDB � Tree �� V6.
05.02.2007
- �����প� ⨯� ������ Numeric ��� Btrieve.
29.12.2006
- ��ࠢ����� �� ������ 1850. � 䠩� ewdialog.c �������� ��� ��⠭���� 䮪�� �� ���� ���୨� �������, �᫨
  ���� tab ����஫. ����⭮ �� �ᥣ�� ��⨢��� �ଠ.
22.12.2006 VERSION 4.51.24
- ��ࠡ�⪠ ।���஢���� ���� � ०��� 3-� �� ����� SourceEditor.
14.12.2006 VERSION 4.51.23
- � RSFM 3-� ०��� ��� ������ �� �ନ��� ����뫠���� 䠩� ��� ࠧ���
- CNum ���樠��������� �� 㬮�砭�� � ID �����.
12.12.2003 VERSION 4.51.22
- ��ᯮ�� �㭪樨 fs_setSleepTime
- rscomi.hpp �����ন���� IRsStream � ��ᯥ����᪨� ����䥩ᮬ.
29.11.2003 VERSION 4.51.21
- �����஢�� ����⮢ � 3-� ०��� �믮������ �� �᭮�� ������� ��६����� �।� RSAPPNAME
28.11.2003
- ��ࠢ����� ewscroll.c � EasyWin �� ������ #1838 (�ꥧ� ���୥�� ���� � �஫�������)
27.11.2003
- ��ࠢ����� ����஢�� ��������� tooltip � EasyWin �� ������ #1842
27.11.2003 version 4.51.20
- RSASSERT -�ᯮ���� _ASSERTE ��� MS VC. � 3-� ०��� �뢮� �������� �����஢��. �뢮����� � rsassert.log
- ����������� �ᯮ�짮����� �⠭���⭮�� ����������� ���� termw ��� MsgBox �᫨ ������㯥� ������ RSFM.
- �� ᨭ�஭���樨 � RSFM ���� �� �ᯮ������ RSFM ��������, �᫨ �।� 䠩��� ��� ᨭ�஭���樨 ���� 䠩�
  RSDlgs.d32.
*/

#endif // _TOOLVER_H_

/* <-- EOF --> */