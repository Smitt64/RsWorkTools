// -------------------------------------------------------------------------------------------------

#ifndef __MSEL_H
#define __MSEL_H

#include "listobj.h"

// -----------------------------------------------------------------------------
// ��� �஢����� �����७�� ���������
#define  MSEL_CHECK_NO                0  // �����
#define  MSEL_CHECK_CRC32             1  // crc32
#define  MSEL_CHECK_SAVEB             2  // ���� �����

// �஢��� �஢�ન �� 㬮�砭��
#define  MSEL_DEFAULT_CHECK_LEVEL     MSEL_CHECK_SAVEB

// ᮮ�饭��, ��।������ � ����. �㭪�� ��室� ᯨ᪠
#define  MSEL_ACTION_STARTWALK        1  // ��砫� ��室�
#define  MSEL_ACTION_PROCESSRECORD    2  // ��ࠡ�⪠ �����
#define  MSEL_ACTION_ENDWALK          3  // ����� ��室�


// �� ������ ��᫥ ��ࠡ�⪨ �����
#define  MSEL_WALK_CONTINUE_KEEPSEL   1  // ��� �����, ��࠭��� �뤥�����
#define  MSEL_WALK_CONTINUE_CLEARSEL  2  // ��� �����, ���� �뤥�����
#define  MSEL_WALK_STOP_KEEPSEL       3  // ��ࢠ��, ��࠭��� �뤥�����
#define  MSEL_WALK_STOP_CLEARSEL      4  // ��ࢠ��, ���� �뤥�����
#define  MSEL_WALK_STOP_CLEARALLSEL   5  // ��ࢠ��, ���� �뤥����� � ���

// ��ࠬ���� �롮� (��⮢�� ��᪠ - long)
#define  MSEL_OPT_NOINVERSION         1  // ����� �롮� � �����ᨥ�
#define  MSEL_OPT_NODELETE            2  // ����� ��⥬���� ���ᮢ��� 㤠�����
#define  MSEL_OPT_FILEKEYWALK         4  // ��室 ᯨ᪠ �� ����� �ᥣ�� ���� �� 䠩��
#define  MSEL_OPT_PROCESSERR          8  // �맮� ����⢨� � ��砥 �訡�� �⥭�� �����

// ��ࠬ���� �� 㬮�砭��
#define  MSEL_DEFAULT_OPTIONS         0

// -----------------------------------------------
typedef struct tagMSEL_COUNT_PARM
      {
       db_int32   num;
       db_int32   err;
       db_int32   partly;
       db_int32   all;  
       PANEL     *Pn;
       char      *Pstatus;
       char      *Phead;
      } MSEL_COUNT_PARM;

// -----------------------------------------------
// ������� ��ࠬ��஢ ���ᮢ��� �롮�
typedef struct tagMULTISELECT
      {
       int      Disabled;         // ���ᮢ� �롮� �⪫�祭
       int      SaveBufCheck;     // �஢��� �஢�ન �����७��� ���������
       int      AutoDestruct;     // ��⮬���᪨ �ਡ��� �� ��室� �� �஫�����
       long     Options;          // ����� ��ࠬ��஢ (MSEL_OPT_*)

       int      Invert;           // ������� �롮ન
       int      Running;          // 䫠� �믮������ ���ᮢ�� ��楤���
       char    *log;              // ��ப� � ����஬ ᮮ�饭�� � 室� ���ᮢ�� ��楤���

       CNTLIST  RecList;          // ᯨ᮪ ��࠭��� ����ᥩ
       CNTLIST  FunList;          // ᯨ᮪ ��ॣ����஢����� ����⢨�

       int               BlockAction;  // ���ᮢ�� ��ࠡ�⪠ �������஢���
       const char       *BlockMessage; // ⥪�� ᮮ�饭��, ������饣� ���ᮢ�� ��ࠡ���

       int               LockSection;  // ���稪 �����஢�� ��� �।���饭�� ��⮣� ���������� ᥪ樨

       MSEL_COUNT_PARM  *counter;
      } MULTISELECT;

// -----------------------------------------------
// ������� ��⥬��� ��ࠬ��஢, ��।������� � ���짮��⥫���� �㭪��
// ���ᮢ�� ��ࠡ�⪨ ���ᥩ
typedef struct tagMSEL_KEYACTIONPARM
      {
       int  error;   // �訡�� �� ��室� ᯨ᪠; ॠ�쭮 ���� ⮫쪮 0 ��� 70
                     // (������ �����७⭮ ��������). �� ������ � �⮩ �訡���
                     // (�����஢���, ��ࢠ�� ࠡ���) - �蠥� �ணࠬ����
       int  key;     // ᮡ�⢥��� ������, ������ ��ࠡ��뢠��.
      } MSEL_KEYACTIONPARM;

#endif

/* <-- EOF --> */