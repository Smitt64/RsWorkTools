#ifndef __MULTISEL_H
#define __MULTISEL_H

#include "bedit.h"
#include "msel.h"

#ifdef __cplusplus
 extern "C" {
#endif

// -----------------------------------------------------------------------------
// ���짮��⥫�᪨� ��ࠡ��稪 ���ᮢ�� ����権 #3044
typedef int (*TSelCallback)(BSCROL *);

// ���⨯ ���짮��⥫�᪮� �㭪樨 ���ᮢ�� ��ࠡ�⪨.
typedef int (*msel_iterator_t)(PANEL *, int, MSEL_KEYACTIONPARM *, void *);

// -----------------------------------------------
// �������, ᮤ�ঠ�� ��ࠬ���� ��࠭��� �����
typedef struct tagMSEL_RECORD
      {
       LOBJ     node;    // ��� ࠡ��� ᯨ᪠

       recpos_t RecPos;  // ������ ��࠭��� ����� � 䠩��
       long     CRC;     // CRC ��࠭��� ����� (�᫨ �㦭�)
       void    *RecBuf;  // ���� ��࠭��� ����� (�᫨ �㦭�)
       int      RecLen;  // ����� ����
       void    *KeyBuf;  // ���� ���� ��࠭��� �����
       int      KeyLen;  // ����� ���祢��� ����
      } MSEL_RECORD;

// -----------------------------------------------
// �������, ᮤ�ঠ�� ��ࠬ���� ��ॣ����஢������ ��ࠡ��稪�
typedef struct tagMSEL_ACTION
      {
       LOBJ             node;        // ��� ࠡ��� ᯨ᪠

       int              key;         // ������, ������ ��ࠡ��뢠��
       int              KeyWalk;     // ��室 �� ����� (�᫨ !0) ��� � ���浪� �롮�
                                     // ����ᥩ (�᫨ 0)
       msel_iterator_t  func;        // ���짮��⥫�᪠� �㭪�� ���ᮢ�� ��ࠡ�⪨
       void            *userparm;    // ���짮��⥫�᪨� ��ࠬ����
      } MSEL_ACTION;


// �������� �������� ���ᮢ��� �롮�. �㦭� � ⮬ ��砥, �᫨ �� ���ࠨ����
// �, �� ᮧ������ �� 㬮�砭��, ���� �᫨ �㦭� �࠭��� ᯨ᮪ �롮� ��᫥
// ��室� �� �஫����� � �.�.
_BEDITFUNC int  MSelCreate(
                           MULTISELECT  **lpSel ,        // 㪠��⥫� �� 㪠��⥫� �� ��������
                           int            AutoDestruct,  // ��⮬���᪨ 㡨�� �� ��室� �� �஫�����
                           int            SaveBufChk ,   // �஢��� �஢�ન �����७��� ���������
                           long           Options        // ����� ��ࠬ��஢ (MSEL_OPT_*)
                          );

// �������� �������� ���ᮢ��� �롮� � ��ࠬ��ࠬ� �� 㬮�砭��
_BEDITFUNC  int  MSelCreateDefault(MULTISELECT **lpSel);

// �⪫�祭�� ���ᮢ��� �롮� ��� �஫�����
_BEDITFUNC int  MSelDisable(BSCROL *Bs);

// �� ����, �᫨ �믮������ ���ᮢ�� ��楤��
_BEDITFUNC int  MSelIsRunning(BSCROL *Bs);

// ���⪠ �������� ���ᮢ��� �롮�. ������� �� �᢮��������� (�� ��砩,
// �᫨ ��� �뤥���� �� �⥪� ��� ����� ����᪠�)
_BEDITFUNC int  MSelClear(MULTISELECT **lpSel);

// ����⮦���� �������� ���ᮢ��� �롮� (� �᢮��������� �����)
_BEDITFUNC int  MSelDestroy(MULTISELECT **lpSel);

// ��������� ���ᮢ��� ����⢨� 
_BEDITFUNC int  MSelAddKeyAction(
                                 BSCROL         *Bs,       // ��஫����
                                 int             key,      // ������, �� ������ ��蠥� ��ࠡ��稪
                                 int             keywalk,  // ����室������ ��室� ᯨ᪠ �� �����
                                 msel_iterator_t func,     // ���짮��⥫�᪠� ��楤�� ���ᮢ�� ��ࠡ�⪨
                                 void           *parm      // ���짮��⥫�᪨� ��ࠬ����
                                );

// Make ��� ���ᮢ��� �롮�. �ᯮ������ ⮫쪮 � �����㬥��.
_BEDITFUNC int  MSelMake(BSCROL *Bs);

// ��ࠡ��稪 ���������� ���ᮢ��� �롮�. �ᯮ������ ⮫쪮 � �����㬥��.
_BEDITFUNC int  MSelProc(PANEL *p, int key);

// ����娢���� ᮮ�饭�� � ��� ࠡ��� ��楤��� ���ᮢ��� �롮�
_BEDITFUNC void MSelAddMessageToLog(BSCROL *Bs, char *str);

// ��������� ��⥬��� ���ᮢ�� ����⢨� �� 㬮�砭��. �ᯮ������ ⮫쪮 � �����㬥��.
_BEDITFUNC void MSelAddDefaultKeys(BSCROL *Bs);


_BEDITFUNC void MSelDeselectAll(BSCROL *Bs);
_BEDITFUNC int  MSelListWalk(PANEL *p, msel_iterator_t func, void *userparm, int key);
_BEDITFUNC int  MSelKeyWalk(PANEL *p, msel_iterator_t func, void *userparm, int key);
_BEDITFUNC int  MSelInvertSelection(BSCROL *Bs);
_BEDITFUNC int  MSelSelectAll(BSCROL *Bs , int strict);
_BEDITFUNC int  MSelAddRecord(BSCROL *Bs);

_BEDITFUNC int  MSelRemoveNode(BSCROL *Bs, MSEL_RECORD *node);   
_BEDITFUNC int  MSelRefreshCRC(BSCROL *Bs, MSEL_RECORD *node);

_BEDITFUNC MSEL_RECORD *MSelIsInList(BSCROL *Bs, recpos_t RecPos);


// �஢�ઠ, ���� �� � �஫����� ��� �� ���� �뤥������ ������
_BEDITFUNC int  MSelAnyRecordsInSelection(BSCROL *Bs);

// ��⠭���� ���짮��⥫�᪮�� ��ࠡ��稪�. �����頥� 㪠��⥫� �� �।��騩 ��ࠡ��稪.
_BEDITFUNC TSelCallback MSelSetCallback(TSelCallback SelClbk);

// �����஢�� ���ᮢ�� ����⢨�
void MSelBlockAction(BSCROL *Bs, int BlockAction, const char *BlockMessage);

void MSelMouse(BSCROL *Bs, int oldcr);

void MSelLockSection(BSCROL *Bs);
void MSelUnlockSection(BSCROL *Bs);

void MSelAddCounter(BSCROL *Bs, int num, int err, int partly);

#ifdef __cplusplus
 }
#endif

#endif

/* <-- EOF --> */