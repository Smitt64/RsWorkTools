// -------------------------------------------------------------------------------------------------
// 2013-08-13  ����䥩� ���७�� ��� WebForm

#ifndef _EWWEB_I_H_
#define _EWWEB_I_H_


// -----------------------------------------------------------------------------
// ����饭��, �����ন����� ��ࠡ��稪�� THandler
// ���� ᮮ�饭�� ��������� ⮫쪮 � ����� ����᫥���!
enum
   {
    EW_WEB_Version = 1,
    EW_WEB_Init,
    EW_WEB_CleanUp,
    EW_WEB_GetTypeInfoCount,
    EW_WEB_GetTypeInfo,
    EW_WEB_GetIDsOfNames,
    EW_WEB_Invoke
  };


// -----------------------------------------------------------------------------
// ���⨯ �㭪樨-��ࠡ��稪�, �ᯮ���㥬� �� DLL ���७��
// [in]      - �����䨪��� ᮮ�饭�� (EW_WEB_)
// [in, out] - 㪠��⥫� �� ��������
// [in, out] - १��
typedef HRESULT (CALLBACK *THandler)(int msg, void *hi, void *low);



// -----------------------------------------------------------------------------
// ��������, ��।������ � ��ࠡ��稪

// ����饭�� EW_WEB_Version
typedef struct
      {
       int32            version;
      } TEWWVersion;


// ����饭�� EW_WEB_Init
typedef struct
      {
       void            *appIntf;
       HRESULT          retVal;
      } TEWWInit; 


// ����饭�� EW_WEB_CleanUp
typedef struct
      {
       HRESULT          retVal;
      } TEWWCleanUp;


// ����饭�� EW_WEB_GetTypeInfoCount
typedef struct
      {
       UINT            *nInfo;
       HRESULT          retVal;
      } TEWWGetTypeInfoCount;


// ����饭�� EW_WEB_GetTypeInfo
typedef struct
      {
       UINT             iTInfo;
       LCID             lcid;
       ITypeInfo      **ppTInfo;
       HRESULT          retVal;
      } TEWWGetTypeInfo;


// ����饭�� EW_WEB_GetIDsOfNames
typedef struct
      {
       IID              riid;
       LPOLESTR        *rgszNames;
       UINT             cNames;
       LCID             lcid;
       DISPID          *rgDispId;
       HRESULT          retVal;
      } TEWWGetIDsOfNames;

// ����饭�� EW_WEB_Invoke
typedef struct
      {
       DISPID           dispIdMember;
       IID              riid;
       LCID             lcid;
       WORD             wFlags;
       DISPPARAMS      *pDispParams;
       VARIANT         *pVarResult;
       EXCEPINFO       *pExcepInfo;
       UINT            *puArgErr;
       HRESULT          retVal;
      } TEWWInvoke;

#endif

/* <-- EOF --> */