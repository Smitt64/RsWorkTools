/**********************************************************************
*        File Name:   ierrfpu.h          March 1,1994            
*       Programmer:   S. Kubrin              
*      Description:   ��ࠡ�⪠ �⠫��� �訡��
*  Public routines:  
* Private routines:  
***********************************************************************/

#ifndef __IERRFPU_H
#define __IERRFPU_H

#include "pexpdef.h"

#ifdef __cplusplus
extern "C" {
#endif

// �����⥫� �� �㭪�� �뢮�� ᮮ�饭�� �� �訡���
// �� ��ࠡ��稪� FPU-�訡��
typedef void (*FpeShowMes_f)(const char *);

extern _RSPNLEXP FpeShowMes_f  FpeShowMes;

// ��⠭�������� ��ࠡ��稪� �訡�� FPU
void FpeInstall(FpeShowMes_f ShowMes);

#ifdef __cplusplus
}
#endif

#endif  // __IERRFPU_H

/* <-- EOF --> */