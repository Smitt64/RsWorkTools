// -------------------------------------------------------------------------------------------------
// 2019-05-22  ����䥩� ࠡ��� � ���⨭���� (�ᯮ������ � ����� ������� �� ࠡ�� � EW-०���)

#ifndef _RSIMAGE_H_
#define _RSIMAGE_H_

#include <stdio.h>
#include <list>

#include "pexpdef.h"
#include "rstypes.h"
#include "bedit.h"
#include "bdate.h"
#include "menu.h"

#include "rsl/common.h"
#include "rsl/stdobj.h"


// -----------------------------------------------------------------------------
// �࠭��� ���祭�� ��� ��ࠬ��஢

// ��ࠬ��� MAXPICTURESIZE
#define dRSIMAGE_MAXSIZE_MIN        1
#define dRSIMAGE_MAXSIZE_MAX     3000
#define dRSIMAGE_MAXSIZE_DEF      500

// ��ࠬ��� MAXPICTURECOUNT
#define dRSIMAGE_MAXCOUNT_MIN       1
#define dRSIMAGE_MAXCOUNT_MAX      50
#define dRSIMAGE_MAXCOUNT_DEF      10

// ��ࠬ��� EXTERNALPICTUREMODE
#define dRSIMAGE_EXTMODE_ND         1   // �� 㤠���� �६���� 䠩� (No Delete)
#define dRSIMAGE_EXTMODE_UAP        2   // �ᯮ�짮���� ��᮫��� ���� � 䠩�� � ���⨭��� (Use Abs Path)
#define dRSIMAGE_EXTMODE_DTTM       4   // ��������� � ��� �६������ 䠩�� ⥪�騥 ����/�६�


// -----------------------------------------------------------------------------
// ������� ���⥪�⭮�� ���� (� �� ⮫쪮) ���� � ���⨭���
#define  dCMD_IMAGE_PREV          200   // �।��饥 ����ࠦ���� � ᯨ᪥
#define  dCMD_IMAGE_NEXT          201   // ������饥 ����ࠦ���� � ᯨ᪥
#define  dCMD_IMAGE_FIRST         202   // ��ࢮ� ����ࠦ���� � ᯨ᪥
#define  dCMD_IMAGE_LAST          203   // ��᫥���� ����ࠦ���� � ᯨ᪥

#define  dCMD_IMAGE_PREVFRAME     204   // �।��騩 ���� � ���⨭��
#define  dCMD_IMAGE_NEXTFRAME     205   // ������騩 ���� � ���⨭��

#define  dCMD_IMAGE_ADD           210   // �������� ����ࠦ����
#define  dCMD_IMAGE_UPDATE        211   // �������� ����ࠦ����
#define  dCMD_IMAGE_REMOVE        212   // ������� ����ࠦ����

#define  dCMD_IMAGE_PASTE         213   // ��⠢�� ���⨭�� �� ���� ������

#define  dCMD_IMAGE_SAVEAS        220   // ���࠭��� ����ࠦ���� ���

#define  dCMD_IMAGE_INFO          230   // �������� ���ଠ�� �� ����ࠦ����

#define  dCMD_IMAGE_INIT_TT       240   // ��砫쭠� ��⠭���� �ᯫ뢠�饩 ���᪠��� ��� ᯨ᪠ ����ࠦ����

#define  dCMD_IMAGE_EXTVIEWER     250   // ��ᬮ�� ����ࠦ���� � ������� ���譥� �ணࠬ��
#define  dCMD_IMAGE_VIEWER        251   // ��ᬮ�� ����ࠦ���� � �⤥�쭮� ����


// -----------------------------------------------------------------------------
// �����䨪���� ����ᮢ
#define  dCLASSID_IMAGE           100
#define  dCLASSID_IMAGELIST       200
#define  dCLASSID_IMAGEBASE       300

// -----------------------------------------------------------------------------
// ����� ���ᨨ ��-㬮�砭��
#define  dRSIMAGE_VERSION           1   // ��� CRSImage
#define  dRSIMAGELIST_VERSION       1   // ��� CRSImageList


#include <packpsh1.h>

// -----------------------------------------------------------------------------
// ��騩 ��������� (��� ��।������ ⨯� ��ꥪ�)
typedef struct
      {
       db_int32   version;    // ����� ����� (���� 1)
       db_int16   classid;    // �����䨪��� ����� ��ꥪ�
       db_int16   offset;     // ���饭�� (�� ��砫� �����) �� ��砫� ᮤ�ন����
      } TSImageBaseHeader;

// -----------------------------------------------------------------------------
// ����� 䠩��
typedef struct
      {
       TSImageBaseHeader  h;  // ���� ���� ���������

       db_int32   size;       // ������ 䠩�� (����)
       char       name[_MAX_PATH];  // �������� 䠩��
       bdate      cdate;      // ��� ᮧ����� 䠩��
       btime      ctime;      // �६� ᮧ����� 䠩��
       bdate      mdate;      // ��� ��᫥���� ����䨪�樨
       btime      mtime;      // �६� ��᫥���� ����䨪�樨
       bdate      ldate;      // ��� ����㧪�
       btime      ltime;      // �६� ����㧪�

       db_uint32  crc32;      // ����஫쭠� �㬬� 䠩��
      } TSImageHeader;

// -----------------------------------------------------------------------------
// ����� ᯨ᪠
typedef struct
      {
       TSImageBaseHeader  h;  // ���� ���� ���������

       db_int16   count;      // ������⢮ ��ꥪ⮢ � ᯨ᪥
       db_int16   curpos;     // ����� ⥪�饣� ��ꥪ�
       db_int32   size;       // ������ ������ ����⮢ ᯨ᪠ (����)
      } TSImageListHeader;

#include <packpop.h>


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// ������ ����� (��� ⨯���樨)
class _RSPNLEXP CRSImageBase
    {
     private:
       int   m_pv_Version,    // ����� ��ꥪ�
             m_pv_Type;       // ��� ��ꥪ�
       bool  m_pv_Changed,    // �ਧ��� ��������� ��ꥪ�
             m_pv_RO;         // �ਧ��� "���쪮 ��� �⥭��"
       int   m_pv_Size;       // ������ ������ (� ����⢥ 䫠��)


     protected:
       int mRestoreFromBLOB(BTRVFILE *bf, bool flagAsStream = false, bool flagReadSize = false);

     public:
       // ----------------------------------------
       CRSImageBase(int ver = dRSIMAGE_VERSION, int type = dCLASSID_IMAGEBASE);

       virtual ~CRSImageBase();

       // ----------------------------------------
       // ������� ����� ��ꥪ�
       int GetVersion(void) const;

       // ----------------------------------------
       // ��⠭����� ����� ��ꥪ�
       int SetVersion(int ver);

       // ----------------------------------------
       // ������� �����䨪��� �����
       int GetClassID(void) const;

       // ----------------------------------------
       // ��⠭����� �����䨪��� �����
       void SetClassID(int type = dCLASSID_IMAGEBASE);

       // ----------------------------------------
       // ������� �ਧ��� ��������� ��ꥪ�
       bool IsChanged(void) const;

       // ----------------------------------------
       // ��⠭����� �ਧ��� ��������� ��ꥪ�
       void SetChanged(bool flag);

       // ----------------------------------------
       // ������� �ਧ��� "���쪮 ��� �⥭��"
       bool IsRO(void) const;

       // ----------------------------------------
       // ��⠭����� �ਧ��� "���쪮 ��� �⥭��"
       void SetRO(bool flag);

       // ----------------------------------------
       // "����䥩��"
       // ----------------------------------------

       // ----------------------------------------
       // ���⪠ ��ꥪ�
       virtual void Clear(bool flDataOnly = false);

       // ----------------------------------------
       // ���࠭��� ��ꥪ� � BLOB'� dbt-䠩��
       virtual int  StoreBF(BTRVFILE *bf, bool flagAsStream = false);

       // ----------------------------------------
       // ����⠭����� ��ꥪ� �� BLOB'� dbt-䠩��
       // [in] flagAsStream - ����� ����� � �ଠ� TStream
       virtual int  RestoreBF(BTRVFILE *bf, bool flagAsStream = false);

       // ----------------------------------------
       // ���࠭��� ��ꥪ� � ����
       virtual int  Store(void *ptr, bool flDataOnly = false);

       // ----------------------------------------
       // ����⠭����� ��ꥪ� �� ���� � ����஫�� ࠧ���
       // [in] sizePtr - ࠧ��� ����� ptr
       virtual int  Restore(const void *ptr, int sizePtr);

       // ----------------------------------------
       // �����頥� �᫮ ����, ����室���� ��� ��࠭���� ��ꥪ⮢
       // �᫨ ��ࠬ��� flDataOnly ��⠭����� � ���祭�� true, �㤥� �����⠭ ⮫쪮
       // ��騩 ࠧ��� ����ࠦ���� � ᯨ᪥
       virtual int  Size(bool flDataOnly = false) const;
    };


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// �����-����⪠ ��� ������ ����ࠦ����
class _RSPNLEXP CRSImage : public CRSImageBase
    {
     private:
       int            m_pv_ImageSize;            // ������ ���⨭��

       mutable void  *m_pv_ImageData;            // ���� ������ ���⨭��

       char           m_pv_FileName[_MAX_PATH];  // ��� 䠩��

       btimestamp     m_pv_CreateDT,             // ��� � �६� ᮧ����� 䠩��
                      m_pv_ModifyDT,             // ��� � �६� ����䨪�樨 䠩��
                      m_pv_LoadDT;               // ��� � �६� ����㧪� 䠩��

       uint32         m_pv_crc32;                // ����஫쭠� �㬬� ������

       bool           m_pv_IsOwner;              // ��ꥪ� ᮧ��� �������᪨ ��� �� �⥪� (�㦭� ��� CRSImageList)

     protected:
       // ----------------------------------------
       void mInit(bool fromConstr = false);

       // ----------------------------------------
       int mRestore(const void *ptr, int sizePtr, bool flDataOnly, int *readSize);

     public:
       // ----------------------------------------
       // ��������� � ���ᨥ� � ����㧪�� 䠩��
       CRSImage(int ver = dRSIMAGE_VERSION, const char *name = NULL);

       // ----------------------------------------
       // ��������� � ����㧪�� 䠩�� (����� �ᯮ������ ⥪���)
       CRSImage(const char *name);

       // ----------------------------------------
       // ������ ��������� ����஢���� �१ 㪠��⥫�
       CRSImage(const CRSImage &obj, bool flDataOnly);

       // ----------------------------------------
       // ��������� ����஢����
       CRSImage(const CRSImage &obj);

       // ----------------------------------------
       // ��������
      ~CRSImage();

       // ----------------------------------------
       // ������ ��ᢠ������ (�㦭� ��� ᯨ᪠)
       CRSImage &operator = (CRSImage &rObj);

       // ----------------------------------------
       // ������� �ਧ��� ᮧ����� ��ꥪ�
       bool IsOwner(void);

       // ----------------------------------------
       // ��⠭����� �ਧ��� ᮧ����� ��ꥪ�
       void SetIsOwner(bool isOwner);

       // ----------------------------------------
       // ����㧨�� ����ࠦ���� �� 䠩��
       int Load(const char *name);

       // ----------------------------------------
       // ����㧨�� ����ࠦ���� �� �����
       int Load(size_t size, void *data);

       // ----------------------------------------
       // ������� ࠧ��� 䠩�� () � �����. � ��砥 �訡�� �����頥� 0.
       // ��࠭�祭�� �� ࠧ��� - �� ����� 2 ��
       int FileSize(const char *name);

       // ----------------------------------------
       // ���࠭��� ����ࠦ���� � 䠩� (� ⥪�騬 ������), ���� ���
       // ��㣨� ������ (�᫨ ����� ��ࠬ��� name).
       int Save(const char *name = NULL);

       // ----------------------------------------
       // ���࠭��� ��ꥪ� � ����. ����室��� ࠧ��� ���� ����� 㧭���
       // � ������� ��⮤� Size().
       // �᫨ ��ࠬ��� flDataOnly ��⠭����� � ���祭�� true, ���� ��࠭���
       // ⮫쪮 ������ ����� ����ࠦ����.
       int Store(void *ptr, bool flDataOnly = false);

       // ----------------------------------------
       // ���࠭��� ��ꥪ� � BLOB'� ⥪�饩 ����� dbt-䠩��. ����室��� ࠧ��� ����
       // ����� 㧭��� � ������� ��⮤� Size().
       // [in] flagAsStream - ������� ����� � �ଠ� TStream
       int StoreBF(BTRVFILE *bf, bool flagAsStream = false);

       // ----------------------------------------
       // ����⠭����� ��ꥪ� �� ����. ��। ����⠭�������� ��ꥪ� ��� ⥪�饥
       // ᮤ�ন��� 㤠�����!
       // �᫨ ��ࠬ��� flDataOnly ࠢ�� true, �㤥� ����⠭����� ⮫쪮 ���� ����ࠦ����.
       // �� ��⠫�� ����� ��⠭����� �०����.
       // ��ࠬ��� readSize �����頥� ����� ࠧ��� ��ꥪ� (����)
       int Restore(const void *ptr, bool flDataOnly = false, int *readSize = NULL);

       // ----------------------------------------
       // ����⠭����� ��ꥪ� �� ���� � ����஫�� ࠧ���
       // [in] sizePtr - ࠧ��� ����� ptr
       int Restore(const void *ptr, int sizePtr);

       // ----------------------------------------
       // ����⠭����� ��ꥪ� �� BLOB'� ⥪�饩 ����� dbt-䠩��. ��। ����⠭�������� ��ꥪ�
       // ��� ⥪�饥 ᮤ�ন��� 㤠�����!
       // [in] flagAsStream - ����� ����� � �ଠ� TStream
       int RestoreBF(BTRVFILE *bf, bool flagAsStream = false);

       // ----------------------------------------
       // �����頥� ������⢮ ���஢ � ����ࠦ���� (TIFF � �.�.)
       // ���� �� ࠡ�⠥� (�����頥� 0 ��� 1, � ����ᨬ��� �� ������ ���⨭��).
       int  Count(void) const;

       // ----------------------------------------
       // �����頥� �᫮ ����, ����室���� ��� ��࠭���� ��ꥪ�.
       // �᫨ ��ࠬ��� flDataOnly ࠢ�� true, �����頥� ⮫쪮 ࠧ��� ���⨭��.
       int  Size(bool flDataOnly = false) const;

       // ----------------------------------------
       // ��⠭����� ��ꥪ� (���� �㤥� 㤠���).
       // �᫨ ��ࠬ��� flDataOnly ࠢ�� true, �㤥� ������� ⮫쪮 ���� ����ࠦ����.
       // �� ��⠫�� ����� ��⠭����� �०����.
       bool Set(const CRSImage &obj, bool flDataOnly = false);

       // ----------------------------------------
       // �����頥� 㪠��⥫� �� ���� ����� � ᮤ�ন�� ����ࠦ����.
       // ������ ���� ����� 㧭��� � ������� �㭪樨 Size(true).
       void *GetData(void) const;

       // ----------------------------------------
       // ������� ����஫��� �㬬�
       uint32 GetCRC32(void) const;

       // ----------------------------------------
       // ���᫨�� ����஫��� �㬬�
       // [in] flagUpdate - �᫨ ࠢ�� true, �ந�室�� ���������� m_pv_crc32.
       //                   ���� - ⮫쪮 ���᫥���.
       uint32 CalcCRC32(bool flagUpdate = true);

       // ----------------------------------------
       // ������� ���� � �६� ᮧ����� 䠩��
       void GetCreateDT(btimestamp *dt) const;

       // ----------------------------------------
       // ������� ���� � �६� ����䨪�樨 䠩��
       void GetModifyDT(btimestamp *dt) const;

       // ----------------------------------------
       // ������� ���� � �६� ����㧪� 䠩��
       void GetLoadDT(btimestamp *dt) const;

       // ----------------------------------------
       // ������� ��� 䠩��
       void GetName(char *name) const;

       // ----------------------------------------
       // ������ ��ꥪ�
       // �᫨ ��ࠬ��� flDataOnly ࠢ�� true, ���� 㤠��� ⮫쪮 ���� ����ࠦ����.
       void Clear(bool flDataOnly = false);
    };


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// ����䥩�� ����� ��� ᯨ᪠ ����ࠦ���� (�ᯮ������ � FIELD::FValue)
class _RSPNLEXP CRSImageList : public CRSImageBase
    {
       typedef std::list<CRSImage *>            TLImageList;
       typedef std::list<CRSImage *>::iterator  TIImageList;

     private:
       int          m_pv_Pos;        // ������ ⥪�饣� �����
       TIImageList  m_pv_Curr;       // ����騩 ��ꥪ�

       CRSImage    *m_pv_Tmp;        // �����⥫� �� �६���� ��ꥪ�
                                     // �ᯮ������ � ���樨, ����� ᯨ᮪ ���� (� � ewextt ��� ࠢ��
                                     // �㦭� ��-� ��।���).

       TLImageList  m_pv_ImageList;  // ���᮪ ����ࠦ����


     protected:
       // ----------------------------------------
       void mInit(bool fromConstr = false);

       // ----------------------------------------
       bool mSetPos(int pos);

       // ----------------------------------------
       TIImageList mCheckPos(int pos);

       // ----------------------------------------
       bool mCreateTmp(void);

       // ----------------------------------------
       void mDeleteTmp(void);

     public:
       // ----------------------------------------
       // ��������� � �������� ���ᨨ ᯨ᪠
       CRSImageList(int ver = dRSIMAGELIST_VERSION);

       // ----------------------------------------
       // ��������
      ~CRSImageList();

       // ----------------------------------------
       // ���࠭��� ᯨ᮪ � ����. ����室��� ࠧ��� ���� ����� 㧭���
       // � ������� ��⮤� Size(-1).
       // ��ࠬ��� flDataOnly �ᯮ������ ⮫쪮 ��� ᮢ���⨬��� � ������ ����ᮬ !
       int Store(void *ptr, bool flDataOnly = false);

       // ----------------------------------------
       // ���࠭��� ᯨ᮪ � BLOB ⥪�饩 �����. ����室��� ࠧ��� ���� ����� 㧭���
       // � ������� ��⮤� Size(-1).
       // [in] flagAsStream - ������� ����� � �ଠ� TStream
       int StoreBF(BTRVFILE *bf, bool flagAsStream = false);

       // ----------------------------------------
       // ����⠭����� ᯨ᮪ �� ����. ��। ����⠭�������� ᯨ᪠ ��� ⥪�饥
       // ᮤ�ন��� 㤠�����!
       int Restore(const void *ptr);

       // ----------------------------------------
       // ����⠭����� ��ꥪ� �� ���� � ����஫�� ࠧ���
       // [in] sizePtr - ࠧ��� ����� ptr
       int Restore(const void *ptr, int sizePtr);

       // ----------------------------------------
       // ����⠭����� ᯨ᮪ �� BLOB ⥪�饩 �����. ��। ����⠭�������� ᯨ᪠ ��� ⥪�饥
       // ᮤ�ন��� 㤠�����!
       // [in] flagAsStream - ����� ����� � �ଠ� TStream
       int RestoreBF(BTRVFILE *bf, bool flagAsStream = false);

       // ----------------------------------------
       // �����頥� ������⢮ ��ꥪ⮢ � ᯨ᪥
       int  Count(void);

       // ----------------------------------------
       // �����頥� �᫮ ����, ����室���� ��� ��࠭���� ��ꥪ� (��� ��� ��ꥪ⮢)
       // �᫨ ��ࠬ��� flDataOnly ��⠭����� � ���祭�� true, �㤥� �����⠭ ⮫쪮
       // ��騩 ࠧ��� ����ࠦ���� � ᯨ᪥
       int  Size(int pos, bool flDataOnly = false);

       // ----------------------------------------
       // �����頥� �᫮ ����, ����室���� ��� ��࠭���� ��� ��ꥪ⮢
       // �᫨ ��ࠬ��� flDataOnly ��⠭����� � ���祭�� true, �㤥� �����⠭ ⮫쪮
       // ��騩 ࠧ��� ����ࠦ���� � ᯨ᪥
       int  Size(bool flDataOnly = false) const;

       // ----------------------------------------
       // �����頥� ⥪�騩 ��ꥪ�, ��� ��ꥪ� �� ��������� �������
       // �᫨ �ॡ㥬� ��ꥪ� ���������, �����頥� NULL.
       CRSImage *Get(int pos = -1);

       // ----------------------------------------
       // ������� �६���� ��ꥪ�
       // �᫨ �ॡ㥬� ��ꥪ� ���������, �����頥� NULL.
       CRSImage *GetTmp(void);

       // ----------------------------------------
       // �����頥� (��⠭��������) ⥪���� ������
       int Pos(int pos = -1);

       // ----------------------------------------
       // �����樮��஢����� �� ���� ����� ᯨ᪠
       // �᫨ �ॡ㥬� ��ꥪ� ���������, �����頥� NULL.
       CRSImage *First(void);

       // ----------------------------------------
       // �����樮��஢����� �� ��᫥���� ����� ᯨ᪠
       // �᫨ �ॡ㥬� ��ꥪ� ���������, �����頥� NULL.
       CRSImage *Last(void);

       // ----------------------------------------
       // �����樮��஢����� �� ᫥���騩 ����� ᯨ᪠
       // �᫨ �ॡ㥬� ��ꥪ� ���������, �����頥� NULL.
       CRSImage *Next(void);

       // ----------------------------------------
       // �����樮��஢����� �� �।��騩 ����� ᯨ᪠
       // �᫨ �ॡ㥬� ��ꥪ� ���������, �����頥� NULL.
       CRSImage *Prev(void);

       // ----------------------------------------
       // �������� ���� ����� � ����� ᯨ᪠
       bool Add(CRSImage *obj, bool isOwner = true);

       // ----------------------------------------
       // �������� ���� ����� � ����� ᯨ᪠
       bool Add(const char *name);

       // ----------------------------------------
       // ��⠢��� ���� ����� ��। ������� (�᫨ before = 0, ���� �����
       // �㤥� �������� � ��砫� ᯨ᪠)
       bool Insert(CRSImage *obj, bool isOwner = true, int before = 0);

       // ----------------------------------------
       // �������� ⥪�騩 ����� (��� ����� � �������� ����樨)
       bool Update(CRSImage *obj, int pos = -1);

       // ----------------------------------------
       // ������� ⥪�騩 ����� (��� ����� � �������� ����樨)
       bool Delete(int pos = -1);

       // ----------------------------------------
       // ������ ���� ᯨ᮪
       // �᫨ ��ࠬ��� flDataOnly ��⠭����� � ���祭�� true, ���� 㤠����
       // ⮫쪮 ����� ����ࠦ����
       void Clear(bool flDataOnly = false);

       // ----------------------------------------
       // ������ �ਧ��� ��������� ��ꥪ� (ᯨ᪠)
       void ClearChanged(void);
    };

#ifdef __cplusplus
 extern "C" {
#endif

// -----------------------------------------------------------------------------
// ��ࠡ��稪 ���� PICTURE
int RSPictureHandler(PANEL *fld, int cf, int cmd);

// -----------------------------------------------------------------------------
// ��������� ����ᮢ TImage � TImageList ��� ࠡ��� � RSL
void RSImageTable(void);

// -----------------------------------------------------------------------------
// �������� RSL-���� ��ꥪ� ��� CRSImage
_TOOLEXP TGenObject *RSImageCreateTImage(CRSImage *obj = NULL);

// -----------------------------------------------------------------------------
// �������� RSL-���� ��ꥪ� ��� CRSImageList
_TOOLEXP TGenObject *RSImageCreateTImageList(CRSImageList *obj = NULL);

// -----------------------------------------------------------------------------
// ����饭��� �㭪�� ᮧ����� RSL-���� ��ꥪ� ��� CRSImage/CRSImageList
_TOOLEXP TGenObject *RSImageCreateTImageBase(CRSImageBase *obj);

// -----------------------------------------------------------------------------
// ����祭�� �-譮�� ��ꥪ� ��� TImage
_TOOLEXP CRSImage *RSImageGetTImage(TGenObject *obj);

// -----------------------------------------------------------------------------
// �������� C-譮�� ��ꥪ� ��� TImageList
_TOOLEXP CRSImageList *RSImageGetTImageList(TGenObject *obj);

// -----------------------------------------------------------------------------
// ����饭��� �㭪�� ����祭�� �-譮�� ��ꥪ� ��� TImage/TImageList
_TOOLEXP CRSImageBase *RSImageGetTImageBase(TGenObject *obj);

// -----------------------------------------------------------------------------
// ��⠭���� �।��쭮�� ࠧ��� ���⨭�� (��) (��ࠬ��� MAXPICTURESIZE)
// �����頥� �।��饥 ���祭��
_TOOLEXP int RSImageSetImageSize(int val);

// -----------------------------------------------------------------------------
// ��⠭���� ���ᨬ��쭮�� ࠧ��� ᯨ᪠ (��ࠬ��� MAXPICTURECOUNT)
// �����頥� �।��饥 ���祭��
_TOOLEXP int RSImageSetImageCount(int val);

// -----------------------------------------------------------------------------
// ��⠭���� �ணࠬ�� ��� ���譥�� ��ᬮ�� (��ࠬ��� EXTERNALPICTUREVIEWER)
_TOOLEXP void RSImageSetExternalViewer(const char *prog);

// -----------------------------------------------------------------------------
// ��⠭���� ०���� ��� �ணࠬ�� ���譥�� ��ᬮ�� (��ࠬ��� EXTERNALPICTUREMODE)
// � ����⢥ val ����� ���� ������ ��������� 䫠��� dRSIMAGE_EXTMODE_*
// �����頥� �।��饥 ���祭��
_TOOLEXP int RSImageSetExternalMode(int val);


// -----------------------------------------------------------------------------
// ��ᬮ�஢騪 ����᪨� 䠩��� (�� ����� 䠩��)
_TOOLEXP int RSImageViewerF(const char *fname, const char *title = NULL);

// -----------------------------------------------------------------------------
// ��ᬮ�஢騪 ����᪨� 䠩��� (�� ��ꥪ�� CRSImage)
_TOOLEXP int RSImageViewerO(CRSImage *obj, const char *title = NULL);

#ifdef __cplusplus
 };
#endif

#endif _RSIMAGE_H_

/* <-- EOF --> */