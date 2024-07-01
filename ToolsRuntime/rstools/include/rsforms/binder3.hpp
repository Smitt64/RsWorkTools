
//-*--------------------------------------------------------------------------*-
//
// File Name: binder3.hpp
//
// Copyright (c) 1991 - 2005 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// Thu Mar 22 17:56:32 2007  Automatically generated by code generator
//-*--------------------------------------------------------------------------*-

#ifndef __BINDER3_HPP__
#define __BINDER3_HPP__

/*ENUM_SECTION*/

/*CLASS_SECTION*/

/*{{RSCOM_WRAPPER(TBinderPtr)*/
class TBinderPtr: public TRsPtr<IDispObj>
{
public:
   void getName(TRsString& retVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x80010000, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      retVal = TRsString (prm.retVal ());
   }
   void putName(const char* Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0x80010000, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   void getObject(IDispObj** retVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x80018009, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      if(prm.retVal ().getType() == RSCOM_OBJ)
         {
         if (!((IRsObj*)prm.retVal ())->getInterface (IID_RSDISP, (void**)retVal))
            throw XRSFormsError (p->getAppObj (), RSL_STAT_NOINTF);
         }
      else
         *retVal = NULL;
   }

   void getParent(IDispObj** retVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x80010008, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      if(prm.retVal ().getType() == RSCOM_OBJ)
         {
         if (!((IRsObj*)prm.retVal ())->getInterface (IID_RSDISP, (void**)retVal))
            throw XRSFormsError (p->getAppObj (), RSL_STAT_NOINTF);
         }
      else
         *retVal = NULL;
   }

};
/*RSCOM_WRAPPER}}*/

/*{{RSCOM_WRAPPER(TConstBinderPtr)*/
class TConstBinderPtr: public TBinderPtr
{
public:
   void Apply(bool bWithSave) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 1, false, RSFM_CODEPAGE);
      prm [0] = bWithSave;
      TRsStat stat = p->invoke (0x00000001, RS_DISP_METHOD, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   void Restore() throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000002, RS_DISP_METHOD, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   void Save() throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000007, RS_DISP_METHOD, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   void Update(bool bSetVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 1, false, RSFM_CODEPAGE);
      prm [0] = bSetVal;
      TRsStat stat = p->invoke (0x00000004, RS_DISP_METHOD, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   void getItems(IDispObj** retVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000000, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      if(prm.retVal ().getType() == RSCOM_OBJ)
         {
         if (!((IRsObj*)prm.retVal ())->getInterface (IID_RSDISP, (void**)retVal))
            throw XRSFormsError (p->getAppObj (), RSL_STAT_NOINTF);
         }
      else
         *retVal = NULL;
   }

   bool getEnabledIn() throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0xfffffdfe, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      return prm.retVal ();
   }
   void putEnabledIn(bool Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0xfffffdfe, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   bool getEnabledOut() throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000006, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      return prm.retVal ();
   }
   void putEnabledOut(bool Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0x00000006, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

};
/*RSCOM_WRAPPER}}*/

/*{{RSCOM_WRAPPER(TSaveBinderPtr)*/
class TSaveBinderPtr: public TBinderPtr
{
public:
   void Update(bool bSetVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 1, false, RSFM_CODEPAGE);
      prm [0] = bSetVal;
      TRsStat stat = p->invoke (0x00000004, RS_DISP_METHOD, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   void getItems(IDispObj** retVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000000, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      if(prm.retVal ().getType() == RSCOM_OBJ)
         {
         if (!((IRsObj*)prm.retVal ())->getInterface (IID_RSDISP, (void**)retVal))
            throw XRSFormsError (p->getAppObj (), RSL_STAT_NOINTF);
         }
      else
         *retVal = NULL;
   }

   bool getEnabledIn() throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0xfffffdfe, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      return prm.retVal ();
   }
   void putEnabledIn(bool Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0xfffffdfe, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   bool getEnabledOut() throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000006, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      return prm.retVal ();
   }
   void putEnabledOut(bool Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0x00000006, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

};
/*RSCOM_WRAPPER}}*/

/*{{RSCOM_WRAPPER(TDataSrcBinderPtr)*/
class TDataSrcBinderPtr: public TBinderPtr
{
public:
   void Update(bool bSetVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 1, false, RSFM_CODEPAGE);
      prm [0] = bSetVal;
      TRsStat stat = p->invoke (0x00000004, RS_DISP_METHOD, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   void getItems(IDispObj** retVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000000, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      if(prm.retVal ().getType() == RSCOM_OBJ)
         {
         if (!((IRsObj*)prm.retVal ())->getInterface (IID_RSDISP, (void**)retVal))
            throw XRSFormsError (p->getAppObj (), RSL_STAT_NOINTF);
         }
      else
         *retVal = NULL;
   }

   bool getEnabledIn() throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0xfffffdfe, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      return prm.retVal ();
   }
   void putEnabledIn(bool Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0xfffffdfe, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   bool getEnabledOut() throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000006, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      return prm.retVal ();
   }
   void putEnabledOut(bool Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0x00000006, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   bool getAutoUpdate() throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000005, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      return prm.retVal ();
   }
   void putAutoUpdate(bool Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0x00000005, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   void getDataSource(IDispObj** retVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000003, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      if(prm.retVal ().getType() == RSCOM_OBJ)
         {
         if (!((IRsObj*)prm.retVal ())->getInterface (IID_RSDISP, (void**)retVal))
            throw XRSFormsError (p->getAppObj (), RSL_STAT_NOINTF);
         }
      else
         *retVal = NULL;
   }
   template<typename  T1>
   void putDataSource(T1 Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0x00000003, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }
   template<typename  T1>
   void putRefDataSource(T1 Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0x00000003, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

};
/*RSCOM_WRAPPER}}*/

/*{{RSCOM_WRAPPER(TCtrlBinderPtr)*/
class TCtrlBinderPtr: public TBinderPtr
{
public:
   void Update(bool bSetVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 1, false, RSFM_CODEPAGE);
      prm [0] = bSetVal;
      TRsStat stat = p->invoke (0x00000004, RS_DISP_METHOD, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   void getItems(IDispObj** retVal) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000000, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      if(prm.retVal ().getType() == RSCOM_OBJ)
         {
         if (!((IRsObj*)prm.retVal ())->getInterface (IID_RSDISP, (void**)retVal))
            throw XRSFormsError (p->getAppObj (), RSL_STAT_NOINTF);
         }
      else
         *retVal = NULL;
   }

   bool getEnabledIn() throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0xfffffdfe, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      return prm.retVal ();
   }
   void putEnabledIn(bool Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0xfffffdfe, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

   bool getEnabledOut() throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, true, RSFM_CODEPAGE);
      TRsStat stat = p->invoke (0x00000006, RS_DISP_PROPERTYGET, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
      return prm.retVal ();
   }
   void putEnabledOut(bool Var1) throw (XRSFormsError)
   {
      TParmArray prm (p->getAppObj (), 0, false, RSFM_CODEPAGE);
      prm.putVal () = Var1;
      TRsStat stat = p->invoke (0x00000006, RS_DISP_PROPERTYPUT, prm, RSCOM_AX_LEVEL);
      if (stat) throw XRSFormsError (p->getAppObj (), stat);
   }

};
/*RSCOM_WRAPPER}}*/

#endif // !__BINDER3_HPP__
