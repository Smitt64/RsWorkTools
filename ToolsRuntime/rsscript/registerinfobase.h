#ifndef REGISTERINFOBASE_H
#define REGISTERINFOBASE_H

#include <QObject>
#include "ToolsRuntime_global.h"

#define OBJ_RSL_METHOD_OFFSET 1001

typedef const char* (*ObjTypeName)(void*);
typedef int (*ObjFindMember)(const char*,long *);
typedef int (*ObjGetUniqID)(void*,const char*);
typedef int (*ObjMemberFromID)(void*,long, long*);
typedef int (*ObjEnumProps)(void *obj, int cmd, void *data);

typedef int(*ObjInitProvider)(void*);
typedef void(*ObjDoneProvider)(void*);
typedef void*(*ObjCreateObject)(void*, const char*);
typedef void*(*ObjClassProc)(void*, const char*);
typedef void(*ObjConstructorProc)(void);

class RegisterInfoBasePrivate;
class TOOLSRUNTIME_EXPORT RegisterInfoBase
{
    friend void CansrtuctorCaller(RegisterInfoBase *info);
    friend int EnumPropsCaller(RegisterInfoBase *info, void *obj, int cmd, void *data);
    friend int InitProviderCaller(RegisterInfoBase *info, void *clntId);
    friend void DoneProviderCaller(RegisterInfoBase *info, void *clntId);
    friend void *GetTypeInfoCaller(RegisterInfoBase *info, void *clntId, const char *typeName);
public:
    enum QObjectRslOwner
    {
        RslOwner = 0,
        CppOwner
    };

    RegisterInfoBase();
    virtual ~RegisterInfoBase();

    int findMember(const char *name, long *id);
    void importObject();

    virtual void Create(void **GenObject, QObject *cls, const QObjectRslOwner &owner = CppOwner);

protected:
    void FillFromMetaObject(const QMetaObject &meta,
                            ObjTypeName ObjTypeNameFunc,
                            ObjFindMember ObjFindMemberFunc);

    void SetFunctions(ObjGetUniqID ObjGetUniqIDFunc, ObjEnumProps ObjEnumProps);
    void AddObjectFuncs(ObjInitProvider _InitProvider,
                        ObjDoneProvider _DoneProvider,
                        ObjCreateObject _CreateObject,
                        ObjClassProc _ClassProc,
                        ObjConstructorProc _ObjConstructorProc);

    void CallCansrtuctor();
    virtual int EnumProps(void *obj, int cmd, void *data);

    virtual int InitProvider(void *clntId);
    virtual void DoneProvider(void *clntId);
    virtual void *GetTypeInfo(void *clntId, const char *typeName);

private:
    RegisterInfoBasePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(RegisterInfoBase);
};

void CansrtuctorCaller(RegisterInfoBase *info);
int EnumPropsCaller(RegisterInfoBase *info, void *obj, int cmd, void *data);

int InitProviderCaller(RegisterInfoBase *info, void *clntId);
void DoneProviderCaller(RegisterInfoBase *info, void *clntId);
void *GetTypeInfoCaller(RegisterInfoBase *info, void *clntId, const char *typeName);

#endif // REGISTERINFOBASE_H
