// -------------------------------------------------------------------------------------------------
// 2015-08-28  ����䥩� ���७�� ।����

#ifndef _EWEDTEXT_H_
#define _EWEDTEXT_H_

#ifndef __EWEXTT_BUILD
    #define  EWEDEXT_API  __declspec(dllexport)
#else
    #define  EWEDEXT_API
#endif


// -----------------------------------------------------------------------------
// ������ ����� ����䥩�
#define  EW_EDEXT_VERSION             1

// -----------------------------------------------------------------------------
// ������ ���� (� ᨬ�����) ��� ���ᠭ�� �訡�� (����砥� 0-ᨬ���)
#define  EW_EDEXT_MAX_ERROR_SIZE    201

// -----------------------------------------------------------------------------
// ������, �����頥�� �ਪ����� ���७���
#define  EW_EDEXT_STATUS_OK           0   // �ᯥ譮
#define  EW_EDEXT_STATUS_BADMSG      -1   // ����ࠡ��뢠���� ᮮ�饭��
#define  EW_EDEXT_STATUS_BADPAR      -2   // ���������� �ॡ㥬� ���ଥ���
#define  EW_EDEXT_STATUS_ERROF       -3   // �訡�� ������ १������饣� 䠩��
#define  EW_EDEXT_STATUS_BADVER      -4   // �������ন������ ����� ����䥩�
#define  EW_EDEXT_STATUS_BADVAL      -5   // �����४�� ���祭�� ��ࠬ��஢
#define  EW_EDEXT_STATUS_BREAK       -6   // ��ࢠ�� ࠡ��� � ���७���

// -----------------------------------------------------------------------------
// ����饭�� ��� ���७�� ।����
#define  EW_EDEXT_MSG_LOAD          100   // ��᫥ ����㧪� �����
#define  EW_EDEXT_MSG_UNLOAD        101   // ��। ���㧪�� �����
#define  EW_EDEXT_MSG_BEFORE        102   // ��। �ᯮ�짮������
#define  EW_EDEXT_MSG_AFTER         103   // ��᫥ �ᯮ�짮�����
#define  EW_EDEXT_MSG_CALL          104   // �맮� �㭪樮���쭮��
#define  EW_EDEXT_MSG_VERSION       105   // ������� ����� ����䥩� ���७��
#define  EW_EDEXT_MSG_GETERROR      106   // ������� ���ᠭ�� �訡��
#define  EW_EDEXT_MSG_CALLBACK      107   // ��⠭����� callback-�㭪��

// -----------------------------------------------------------------------------
// ����饭�� ��� �㭪樨 ���⭮�� �맮��
#define  EW_EDEXT_CB_PB_INIT          1   // ���樠������ ��������
#define  EW_EDEXT_CB_PB_USE           2   // ���饭�� ��������
#define  EW_EDEXT_CB_PB_DONE          3   // �����樠������ ��������

// -----------------------------------------------------------------------------
// ���짮��⥫�᪨� �������� �����䨪��஢ ���� ��� ���७��
#define  EW_EDEXT_ID_BEGIN        10000   // ��砫�
#define  EW_EDEXT_ID_END          12000   // �����

// -----------------------------------------------------------------------------
// ��� �㭪樨-��ࠡ��稪� � ���㫥 ���७�� ।����
#define  EW_EDEXT_HANDLER  "EditorHandler"


// -----------------------------------------------------------------------------
// ������� ��� ᮮ�饭�� EW_EDEXT_MSG_LOAD
typedef struct TSExtInitData
      {
       int             version;    // ����� ����䥩� (���� 1)
       int             cmdMode;    // ����� ࠡ��� (0 - �����쭮, 1 - 㤠�����)
       int             lenTxt;     // ����� ��� � ��⠫��� TxtFile (��� 0-ᨬ����)
       const wchar_t  *pathTxt;    // �����⥫� �� ���� � ��⠫��� TxtFile
       int             lenExt;     // ����� ��� � ��⠫��� ���७�� ।���� (��� 0-ᨬ����)
       const wchar_t  *pathExt;    // �����⥫� �� ���� � ��⠫��� ���७�� ।���� (�����)
      } TSExtInitData;

// -----------------------------------------------------------------------------
// ������� ��� ᮮ�饭�� EW_EDEXT_MSG_CALL
typedef struct TSExtCallData
      {
       int             version;    // ����� ����䥩� (���� 1)
       int             len;        // ����� ����� 䠩�� (��� 0-ᨬ����)
       const wchar_t  *file;       // �����⥫� �� ���� � ����� ������ 䠩��
       int             size;       // ������ ⥪�� (��� 0-ᨬ����)
       const wchar_t  *text;       // �����⥫� �� ���� � ⥪�⮬
      } TSExtCallData;

// -----------------------------------------------------------------------------
// �����⥫� �� �㭪��-��ࠡ��稪 �� ���७�� ।����
// [in]      msg - �����䨪��� ᮮ�饭�� (EW_EDEXT_MSG_)
// [in]      cmd - �����䨪��� ������� (�����䨪��� �㭪� ����, ���९�������
//                 �� ����� ���७���)
// [in,out]  p1  - ���� ��ࠬ���
// [in,out]  p2  - ��ன ��ࠬ���
// [ret]         - �����頥��� ���祭�� (������ �� ⨯� ᮮ�饭��)
typedef int (*TFEExtHandler)(int msg, int cmd, void *p1, void *p2);

// -----------------------------------------------------------------------------
// �����⥫� �� callback-�㭪�� (�������� �ண���)
// [in]      msg - �����䨪��� ᮮ�饭�� (EW_EDEXT_CB_PB_)
// [in,out]  p1  - ���� ��ࠬ���
// [in,out]  p2  - ��ன ��ࠬ���
// [ret]         - �����頥��� ���祭�� (������ �� ⨯� ᮮ�饭��)
typedef int (CALLBACK *TFEProgress)(int msg, void *p1, void *p2);

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// �㭪��-��ࠡ��稪, �ᯮ���㥬�� �� ���७��
// [in]      msg - �����䨪��� ᮮ�饭�� (EW_EDEXT_MSG_)
// [in]      cmd - �����䨪��� ������� (�����䨪��� �㭪� ����, ���९�������
//                 �� ����� ���७���)
// [in,out]  p1  - ���� ��ࠬ���
// [in,out]  p2  - ��ன ��ࠬ���
// [ret]         - �����頥��� ���祭�� (������ �� ⨯� ᮮ�饭��)
EWEDEXT_API int EditorHandler(int msg, int cmd, void *p1, void *p2);

#ifdef __cplusplus
}
#endif

#endif // _EWEDTEXT_H_


/* <-- EOF --> */