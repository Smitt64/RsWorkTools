// -------------------------------------------------------------------------------------------------
// 2013-12-09  ��ࠢ��� �஫����� ��� �롮� ����ᮢ

#ifndef _RSHELPRESH_H_
#define _RSHELPRESH_H_

#include "menu.h"


// -----------------------------------------------------------------------------
// �� �ࠢ�筨�� �������� ���祭�� ENTER � ��砥 �롮� �����,
// � ���祭�� ESC �᫨ �ࠢ�筨� �� ������ ��� �롮�.

// -----------------------------------------------------------------------------
// ��ࠢ��� �஫���� �롮� �����
// [in]     lbr  - ������������ ������⥪� ����ᮢ
// [in,out] type - ⨯ �⡨ࠥ��� ����ᮢ (����易⥫�� ��ࠬ���)
// [out]    name - ������������ ��࠭���� �����
// [out]    help - ��࠭�� ����� ��࠭���� �����
// �᫨ ��ࠬ��� type �����, � ����� ���㫥��� ���祭��, � � �ࠢ�筨�� ����
// �⮡ࠦ��� ⮫쪮 ������ ��������� ����. �᫨ ��ࠬ��� �����, �� ࠢ�� ���,
// � �ࠢ�筨�� ���� �⮡ࠦ��� ������ ��� ⨯��, � ��᫥ �롮� ����� �
// ��ࠬ��� type �㤥� ����ᠭ ��� ⨯� ��࠭���� �����.
_RSRESEXP int RSH_SelectRes(const char *lbr, int32 *type, char *name = NULL, int *help = NULL);


// -----------------------------------------------------------------------------
// ��ࠢ��� �஫���� �롮� ������� ����
// [in]  lbr  - ������������ ������⥪� ����ᮢ
// [in]  name - ������������ �����
// [out] id   - �����䨪��� �㭪� ���� (����易⥫�� ��ࠬ���)
// [out] help - ����� ��࠭��� ����� (����易⥫�� ��ࠬ���)
_RSRESEXP int RSH_SelectMenuItem(const char *lbr, const char *name, int *id = NULL, int *help = NULL);

#endif // _RSHELPRESH_H_

/* <-- EOF --> */