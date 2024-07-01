/*************************************************************************
        File Name               : hook.hpp                      05.06.2001
        Programmer              : E.Lebedeva

        Description             : ����� HookList �।�����祭 ��� ॠ����樨 ᯨ᪠ 
                    ���墠�稪�� ����������

        Copyright(C) 2001 by R-Style Software Lab.
        All Rights Reserved.

*************************************************************************/


#ifndef __HOOK_HPP
#define __HOOK_HPP

#include "menu.h"
#include "listobj.h"

// ���⨯ �㭪樨 hook'a
typedef int (*HookProc)(PANEL *p, int key, void *data);

#ifdef __cplusplus
    class HookList
        {
         public :

           HookList()
           {
            InitCntList(&h_list, LST_DEFAULT, offsetof(HookObject, list_elem));
           };

          ~HookList()
           {
            Clear();
           };
            
           void Add(const char *PName, int k, HookProc f, void *dt = 0,
                    const char *MIName = 0, const char *status = 0, int h = 0);

           void Clear();

           int  Nitems()
           {
            return LobjNitems(&h_list);
           };

           int  FindAndExecute(PANEL *p, int key);

           void CreateMenuItems(PANEL *p);

         private :

           struct HookObject
                {
                 HookObject(const char *PName, int k, HookProc f, void *dt = 0, 
                            const char *MIName = 0, const char *status = 0, int h = 0);

                ~HookObject();

                 const char *PanelName;
                 int         key;
                 HookProc    proc;
                 void       *data;
                 char       *MenuItemName;
                 const char *par;
                 int         help;
                 LOBJ        list_elem;
                };

           CNTLIST  h_list;
        };
#endif


#ifdef __cplusplus
extern "C" {
#endif

// ��⠭����� ���墠�稪 (hook)
_RSPNLEXP void SetHook( 
                       const char *ResPanName,       // ��� ����� ������
                       int         key,              // ������, �� ���. ��⠭���������� hook
                       HookProc    f_hook,           // �㭪��-hook
                       void       *data,             // ��� ��।�� ������ (�.�.0)
                       const char *MenItemName,      // ��� ������塞��� �㭪� � ����"������"(�.�.0)
                       const char *status,           // ����饭�� � ������� ��ப� (�.�.0)
                       int         hlp               // ����� ��࠭��� ����� (�.�.0)
                      );

_RSPNLEXP void ClearHookList(void);  // ������ ᯨ᮪ ���墠�稪��

// �������� � ������� "������" �㭪�� �� ᯨ᪠ ���墠�稪�� 
// (�ᯮ������ ⮫쪮 � �����㬥��)
extern void CreateHookMenuItems(PANEL *p);

extern int IsHook(void); // (�ᯮ������ ⮫쪮 � �����㬥��)

extern int UseHookList(PANEL *p, int key);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */