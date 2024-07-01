#ifndef REGISTEROBJLIST_HPP
#define REGISTEROBJLIST_HPP

#include <QObject>
#include "registerinfobase.h"
#include "ToolsRuntime_global.h"

RegisterInfoBase *findInfo(const QString &name);

template<class Obj>
class RegisterObjInfo : public RegisterInfoBase
{
    RegisterObjInfo() :
        RegisterInfoBase()
    {
        const QMetaObject meta = Obj::staticMetaObject;
        FillFromMetaObject(meta,
                           RegisterObjInfo<Obj>::GenObjTypeName,
                           RegisterObjInfo<Obj>::GenObjFindMember);

        SetFunctions(RegisterObjInfo<Obj>::GenObjGetUniqID,
                     RegisterObjInfo<Obj>::GenObjEnumProps);

        AddObjectFuncs(RegisterObjInfo<Obj>::InitProvider,
                  RegisterObjInfo<Obj>::DoneProvider,
                  RegisterObjInfo<Obj>::CreateObject,
                  RegisterObjInfo<Obj>::GetTypeInfo);
    }

    static const char *GenObjTypeName(void *o)
    {
        const QMetaObject meta = Obj::staticMetaObject;
        return meta.className();
    }

    static int GenObjFindMember(const char *name, long *id)
    {
        const QMetaObject meta = Obj::staticMetaObject;
        RegisterInfoBase *info = findInfo(meta.className());

        return info->findMember(name, id);
    }

    static int GenObjEnumProps(void *obj, int cmd, void *data)
    {
        const QMetaObject meta = Obj::staticMetaObject;
        RegisterInfoBase *info = findInfo(meta.className());

        return EnumPropsCaller(info, obj, cmd, data);
    }

    static long GenObjGetUniqID(void *pObj, const char *name)
    {
        const QMetaObject meta = Obj::staticMetaObject;
        RegisterInfoBase *info = findInfo(meta.className());

        long  id = -1;
        info->findMember(name, &id);
        return id;
    }

    static int InitProvider(void *clntId)
    {
        const QMetaObject meta = Obj::staticMetaObject;
        RegisterInfoBase *info = findInfo(meta.className());
        return InitProviderCaller(info, clntId);
    }

    static void DoneProvider(void *clntId)
    {
        const QMetaObject meta = Obj::staticMetaObject;
        RegisterInfoBase *info = findInfo(meta.className());
        return DoneProviderCaller(info, clntId);
    }

    static void *CreateObject(void *clntId, const char *typeName)
    {
        const QMetaObject meta = Obj::staticMetaObject;
        RegisterInfoBase *info = findInfo(meta.className());

        if(qstricmp(typeName, meta.className()) != 0)
            return NULL;

        void *GenObject = nullptr;
        info->Create(&GenObject, nullptr);
        return GenObject;
    }

    static void RslConstructorFunc()
    {
        const QMetaObject meta = Obj::staticMetaObject;
        RegisterInfoBase *info = findInfo(meta.className());

        CansrtuctorCaller(info);
    }

    static void *GetTypeInfo(void *clntId, const char *typeName)
    {
        const QMetaObject meta = Obj::staticMetaObject;
        RegisterInfoBase *info = findInfo(meta.className());

        return GetTypeInfoCaller(info, clntId, typeName);
    }

};

class RegisterObjListPrivate;
class TOOLSRUNTIME_EXPORT RegisterObjList
{
public:
    static RegisterObjList *inst();

    RegisterInfoBase *info(const QString &name);
    bool isExists(const QString &name) const;

    template<class Obj>
    void RegisterRslObject()
    {
        static_assert(std::is_base_of_v<QObject, Obj>, "QObject is not base of class");
        const QMetaObject meta = Obj::staticMetaObject;

        if (!isExists(meta.className()))
        {
            RegisterObjInfo<Obj> *info = new RegisterObjInfo<Obj>();
            InsertInfo(info->TypeName, info);
        }

        /*AddObjectProviderModEx(
                    RegisterObjInfo<Obj>::InitProvider,
                    RegisterObjInfo<Obj>::DoneProvider,
                    RegisterObjInfo<Obj>::CreateObject,
                    RegisterObjInfo<Obj>::GetTypeInfo);

        AddStdProc(V_GENOBJ, meta.className(), RegisterObjInfo<Obj>::RslCreate, 0);*/
    }

private:
    RegisterObjList();
    void InsertInfo(const QString &name, RegisterInfoBase *info);
    static RegisterObjList *m_inst;

    RegisterObjListPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(RegisterObjList);
};

#endif // REGISTEROBJLIST_HPP
