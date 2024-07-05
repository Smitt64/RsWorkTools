#define BUILD_DLM
#include "rsl/dlmintf.h"
#include "statvars.h"
#include "rsl/isymbol.h"
#include "typeinfo_p.h"
#include "registerobjlist.hpp"
#include <cstring>
#include <QDebug>
#include <QTextCodec>
#include <QDate>
#include <QTime>
#include <QMetaProperty>
#include <QLibrary>
#include <QMetaType>

typedef TGenObject *(*LibRslTArrayCreate)(int,int);
typedef TGenObject *(*LibRslTArrayPut)(TGenObject*,long, VALUE*);
typedef TGenObject *(*LibRslIsTArray)(TGenObject *obj);
typedef long (*LibRslTArraySize)(TGenObject *obj);
typedef VALUE *(*LibRslTArrayGet)(TGenObject *obj, long id);

Q_GLOBAL_STATIC_WITH_ARGS(QLibrary, RSScriptLib, ("RSScript"))

LibRslTArrayCreate _LibRslTArrayCreate = nullptr;
LibRslTArrayPut _LibRslTArrayPut = nullptr;
LibRslIsTArray _LibRslIsTArray = nullptr;
LibRslTArraySize _LibRslTArraySize = nullptr;
LibRslTArrayGet _LibRslTArrayGet = nullptr;

void LoadArrayFunctions()
{
    if (_LibRslTArrayCreate)
        return;

    _LibRslTArrayCreate = (LibRslTArrayCreate)RSScriptLib->resolve("RslTArrayCreate");
    _LibRslTArrayPut = (LibRslTArrayPut)RSScriptLib->resolve("RslTArrayPut");
    _LibRslIsTArray = (LibRslIsTArray)RSScriptLib->resolve("RslIsTArray");
    _LibRslTArraySize = (LibRslTArraySize)RSScriptLib->resolve("RslTArraySize");
    _LibRslTArrayGet = (LibRslTArrayGet)RSScriptLib->resolve("RslTArrayGet");
}

QVariant SetFromRslValue(void *value, bool isStringListProp = false)
{
    QVariant result;
    VALUE *val = (VALUE*)value;
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    LoadArrayFunctions();

    switch(val->v_type)
    {
    case V_STRING:
        result = QVariant::fromValue(codec->toUnicode(val->value.string));
        break;

    case V_INTEGER:
        result = QVariant::fromValue(val->value.intval);
        break;

    case V_BIGINT:
        result = QVariant::fromValue(val->value.bigint);
        break;

    case V_BOOL:
        result = QVariant::fromValue(val->value.boolval);
        break;

    case V_DATE:
    {
        bdate dt = val->value.date;
        result = QVariant::fromValue(QDate(dt.year, dt.mon, dt.day));
    }
        break;

    case V_TIME:
    {
        btime dt = val->value.time;
        result = QVariant::fromValue(QTime(dt.hour, dt.min, dt.sec));
    }
        break;

    case V_GENOBJ:
    {
        TGenObject *TArrayText = _LibRslIsTArray(P_GOBJ(val->value.obj));

        if (TArrayText)
        {
            if (isStringListProp)
            {
                QStringList lst;

                int size = _LibRslTArraySize(TArrayText);
                for (int i = 0; i < size; i++)
                {
                    VALUE *v = _LibRslTArrayGet(TArrayText,i);
                    QVariant vl = SetFromRslValue(v);
                    lst.append(vl.toString());
                }

                result = QVariant::fromValue(lst);
            }
        }
    }
        break;
    }

    return result;
}

int GenObjSetId(TGenObject *obj, long id, VALUE *val)
{
    QObjectRsl *rsl = (QObjectRsl*)obj;
    const QMetaObject *meta = rsl->object->metaObject();
    QMetaProperty property = meta->property(id - 1);
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    const char *prop = property.name();

    if (!property.isWritable())
        return 1;

    QVariant var = SetFromRslValue(val);
    if (!var.isValid())
        return -1;

    rsl->object->setProperty(prop, var);

    return 0;
}

int GenObjSet(TGenObject *obj, const char *parm, VALUE *val, long *id)
{
    QObjectRsl *rsl = (QObjectRsl*)obj;
    const QMetaObject *meta = rsl->object->metaObject();

    *id = 0;
    bool found = false;
    for (int i = 0; i < meta->propertyCount(); i++)
    {
        QMetaProperty property = meta->property(i);

        if (!QString(property.name()).compare(parm, Qt::CaseInsensitive))
        {
            found = true;
            *id = i + 1;
            break;
        }
    }

    if(found) // property
        return GenObjSetId(obj, *id, val);

    return -1;
}

int SetValueFromVariant(std::function<void(int,void*)> Setter, const QVariant &value)
{
    int result = 0;
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    LoadArrayFunctions();

    switch(value.type())
    {
    case QVariant::String:
        Setter(V_STRING, codec->fromUnicode(value.toString()).data());
        //ValueSet(val, V_STRING, codec->fromUnicode(value.toString()).data());
        break;
    case QVariant::Int:
    case QVariant::UInt:
    {
        long v = value.toInt();
        //ValueSet(val, V_INTEGER, &v);
        Setter(V_INTEGER, &v);
    }
        break;
    case QVariant::LongLong:
    case QVariant::ULongLong:
    {
        qint64 v = value.value<qint64>();
        //ValueSet(val, V_BIGINT, &v);
        Setter(V_BIGINT, &v);
    }
        break;

    case QVariant::Double:
    {
        qreal v = value.value<qreal>();
        //ValueSet(val, V_DOUBLE, &v);
        Setter(V_DOUBLE, &v);
    }
        break;

    case QVariant::Date:
    {
        QDate dt = value.toDate();
        bdate rsldt;
        rsldt.day = dt.day();
        rsldt.mon = dt.month();
        rsldt.year = dt.year();
        //ValueSet(val, V_DATE, &rsldt);
        Setter(V_DATE, &rsldt);
    }
        break;

    case QVariant::Time:
    {
        QTime dt = value.toTime();
        qDebug() << dt;
        btime rsldt;
        rsldt.hour = dt.hour();
        rsldt.min = dt.minute();
        rsldt.sec = dt.second();

        //ValueInit(val, V_TIME);
        //ValueSet(val, V_TIME, &rsldt);
        Setter(V_TIME, &rsldt);
    }
        break;

    case QVariant::Bool:
    {
        bool b = value.toBool();
        //ValueSet(val, V_BOOL, &b);
        Setter(V_BOOL, &b);
    }
        break;

    case QVariant::StringList:
    {
        QStringList lst = value.toStringList();
        TGenObject *ValueArray = _LibRslTArrayCreate(lst.size(), lst.size());

        int i = 0;
        for (const QString &item : lst)
        {
            VALUE val;
            ValueSet(&val, V_STRING, codec->fromUnicode(item).data());
            _LibRslTArrayPut(ValueArray, i++, &val);
            ValueClear(&val);
        }
        Setter(V_GENOBJ, ValueArray);
    }
        break;

    case QVariant::UserType:
    {
        const QMetaObject *meta = QMetaType::metaObjectForType(value.userType());

        if (meta)
        {
            RegisterInfoBase *info = RegisterObjList::inst()->info(meta->className());
            QObject *obj = value.value<QObject*>();

            qDebug() << info << obj << meta->className();

            TGenObject *Child = nullptr;
            info->Create((void**)&Child, obj, RegisterInfoBase::CppOwner);
            Setter(V_GENOBJ, P_GOBJ(Child));
        }
    }
        break;

    default:
        result =  -1;
    }

    return result;
}

void StdValueSetFunc(void *val, int type, void *ptr)
{
    ValueSet((VALUE*)val, type, ptr);
}

#define ValueVariantSet(type, ptr) SetValueFromVariant(SetterFunc, type, ptr)

int GenObjGetId(TGenObject *obj, long id, VALUE *val)
{
    QObjectRsl *rsl = (QObjectRsl*)obj;
    const QMetaObject *meta = rsl->object->metaObject();
    QMetaProperty property = meta->property(id - 1);

    if (!property.isReadable())
        return 1;

    QVariant value = rsl->object->property(property.name());
    //qDebug() << "GenObjGetId" << value;
    auto SetterFunc = [=](int type, void *ptr) -> void
    {
        ValueSet(val, type, ptr);
    };

    return SetValueFromVariant(SetterFunc, value);
}

int GenObjGet(TGenObject *obj, const char *parm, VALUE *val, long *id)
{
    QObjectRsl *rsl = (QObjectRsl*)obj;
    const QMetaObject *meta = rsl->object->metaObject();
    RegisterInfoBase *info = RegisterObjList::inst()->info(meta->className());

    int  tp = info->findMember(parm, id);

    if(tp == 1) // property
        return GenObjGetId(obj, *id, val);

    if(tp == 0)
        return 1;

    return -1;
}

int GenObjRunId(TGenObject *obj, long id)
{
    QObjectRsl *rsl = (QObjectRsl*)obj;
    const QMetaObject *meta = rsl->object->metaObject();
    QMetaMethod method = meta->method(id - OBJ_RSL_METHOD_OFFSET);
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");

    int parmaOffset = 0;
    if (method.returnType() != QMetaType::Void)
        parmaOffset = 1;

    int NumParam = GetNumParm();
    int MethodParams = method.parameterCount();

    if (NumParam - 1 != MethodParams)
    {
        iError(IER_RUNMETHOD, "Param count missmatch");
        return -1;
    }

    auto AllocaTeParam = [=](const int &Type, void **param, VALUE *val) -> void
    {
        switch(Type)
        {
        case QMetaType::Bool:
            *param = new bool();
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::Void:
        case QMetaType::Long:
        case QMetaType::Short:
        case QMetaType::ULong:
        case QMetaType::UShort:
            *param = new int();
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            *param = new qint64();
            break;
        case QMetaType::Double:
            *param = new qreal();
            break;
        case QMetaType::QString:
            *param = new QString();
            break;
        case QMetaType::QDate:
            *param = new QDate();
            break;
        case QMetaType::QTime:
            *param = new QTime();
            break;
        }

        if (!val)
            return;

        VALUE NewVal;
        ValueMake(&NewVal);
        ValueCopy(val, &NewVal);
        if (Type == QMetaType::QString)
        {
            if (!CnvType(&NewVal, V_STRING))
                iError(IER_RUNTIME, "Param type missmatch, required string");

            (*reinterpret_cast<QString*>(*param)) = codec->toUnicode(NewVal.value.string);
        }
        else if (Type == QMetaType::Bool)
        {
            if (!CnvType(&NewVal, V_BOOL))
                iError(IER_RUNTIME, "Param type missmatch, required boolean");

            (*reinterpret_cast<bool*>(*param)) = NewVal.value.boolval;
        }
        else if (Type == QMetaType::Int || Type == QMetaType::UInt ||
                 Type == QMetaType::Long ||
                 Type == QMetaType::Short ||
                 Type == QMetaType::ULong ||
                 Type == QMetaType::UShort
        )
        {
            if (!CnvType(&NewVal, V_INTEGER))
                iError(IER_RUNTIME, "Param type missmatch, required integer");

            (*reinterpret_cast<int*>(*param)) = NewVal.value.intval;
        }
        else if (Type == QMetaType::LongLong || Type == QMetaType::ULongLong
        )
        {
            if (!CnvType(&NewVal, V_BIGINT) && !CnvType(&NewVal, V_INTEGER))
                iError(IER_RUNTIME, "Param type missmatch, required integer");

            (*reinterpret_cast<int*>(*param)) = NewVal.value.intval;
        }
        else if (Type == QMetaType::Double)
        {
            if (!CnvType(&NewVal, V_DOUBLE))
                iError(IER_RUNTIME, "Param type missmatch, required double");

            (*reinterpret_cast<qreal*>(*param)) = NewVal.value.doubval;
        }
        else if (Type == QMetaType::QDate)
        {
            if (!CnvType(&NewVal, V_DATE))
                iError(IER_RUNTIME, "Param type missmatch, required date");

            (*reinterpret_cast<QDate*>(*param)) = QDate(NewVal.value.date.year,
                                                        NewVal.value.date.mon,
                                                        NewVal.value.date.day);
        }
        else if (Type == QMetaType::QTime)
        {
            if (!CnvType(&NewVal, V_TIME))
                iError(IER_RUNTIME, "Param type missmatch, required time");

            (*reinterpret_cast<QTime*>(*param)) = QTime(NewVal.value.time.hour,
                                                        NewVal.value.time.min,
                                                        NewVal.value.time.sec);
        }
    };

    void **params = new void*[MethodParams + 1];
    AllocaTeParam(method.returnType(), &params[0], nullptr);

    for (int i = 0; i < MethodParams; i++)
    {
        VALUE *val = nullptr;
        GetParm(i + 1, &val);

        AllocaTeParam(method.parameterType(i), &params[i + 1], val);
    }

    rsl->object->qt_metacall(QMetaObject::InvokeMetaMethod, id - OBJ_RSL_METHOD_OFFSET, params);

    if (method.returnType() != QMetaType::Void)
    {
        VALUE ret;
        ValueMake(&ret);
        switch(method.returnType())
        {
        case QMetaType::Bool:
            ValueSet(&ret, V_BOOL, reinterpret_cast<bool*>(params[0]));
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::Void:
        case QMetaType::Long:
        case QMetaType::Short:
        case QMetaType::ULong:
        case QMetaType::UShort:
            ValueSet(&ret, V_INTEGER, reinterpret_cast<int*>(params[0]));
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            ValueSet(&ret, V_BIGINT, reinterpret_cast<int*>(params[0]));
            break;
        case QMetaType::Double:
            ValueSet(&ret, V_DOUBLE, reinterpret_cast<int*>(params[0]));
            break;
        case QMetaType::QString:
            ValueSet(&ret, V_STRING, codec->fromUnicode(*reinterpret_cast<QString*>(params[0])));
            break;
        case QMetaType::QDate:
        {
            QDate qdate = *reinterpret_cast<QDate*>(params[0]);
            bdate rsldate;
            rsldate.day = qdate.day();
            rsldate.mon = qdate.month();
            rsldate.year = qdate.year();
            ValueSet(&ret, V_DATE, &rsldate);
        }
            break;
        case QMetaType::QTime:
        {
            QTime qdate = *reinterpret_cast<QTime*>(params[0]);
            btime rsldate;
            rsldate.hour = qdate.hour();
            rsldate.min = qdate.minute();
            rsldate.sec = qdate.second();
            ValueSet(&ret, V_TIME, &rsldate);
        }
            break;
        }

        ReturnVal2(&ret);
        ValueClear(&ret);
    }

    for (int i = 0; i < MethodParams; i++)
    {
        switch(method.parameterType(i))
        {
        case QMetaType::Bool:
            delete reinterpret_cast<bool*>(params[i]);
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::Void:
            delete reinterpret_cast<int*>(params[i]);
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            delete reinterpret_cast<qint64*>(params[i]);
            break;
        case QMetaType::QString:
            delete reinterpret_cast<QString*>(params[i]);
            break;
        case QMetaType::QDate:
            delete reinterpret_cast<QDate*>(params[i]);
            break;
        case QMetaType::QTime:
            delete reinterpret_cast<QTime*>(params[i]);
            break;
        }
    }

    delete[] params;

    return 0;
}

int GenObjRun(TGenObject *obj, const char *methodname, long *id)
{
    QObjectRsl *rsl = (QObjectRsl*)obj;
    const QMetaObject *meta = rsl->object->metaObject();
    RegisterInfoBase *info = RegisterObjList::inst()->info(meta->className());

    if (info->findMember(methodname, id) == 0)
        return GenObjRunId(obj, *id);

    return -1;
}
