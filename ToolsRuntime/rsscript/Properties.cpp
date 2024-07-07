#include "rsl/dlmintf.h"
#include "statvars.h"
#include "rsl/isymbol.h"
#include "typeinfo_p.h"
#include "registerobjlist.hpp"
#include "rslexecutor.h"
#include "rslibdynamicfuncs.h"
#include <cstring>
#include <QDebug>
#include <QTextCodec>
#include <QDate>
#include <QTime>
#include <QMetaProperty>
#include <QLibrary>
#include <QMetaType>

Q_GLOBAL_STATIC_WITH_ARGS(QLibrary, RSScriptLib, ("RSScript"))

LibRslTArrayCreate _LibRslTArrayCreate = nullptr;
LibRslTArrayPut _LibRslTArrayPut = nullptr;
LibRslIsTArray _LibRslIsTArray = nullptr;
LibRslTArraySize _LibRslTArraySize = nullptr;
LibRslTArrayGet _LibRslTArrayGet = nullptr;
LibSetParm _LibSetParm = nullptr;

void LoadFunctions()
{
    if (_LibRslTArrayCreate)
        return;

    _LibRslTArrayCreate = (LibRslTArrayCreate)RSScriptLib->resolve("RslTArrayCreate");
    _LibRslTArrayPut = (LibRslTArrayPut)RSScriptLib->resolve("RslTArrayPut");
    _LibRslIsTArray = (LibRslIsTArray)RSScriptLib->resolve("RslIsTArray");
    _LibRslTArraySize = (LibRslTArraySize)RSScriptLib->resolve("RslTArraySize");
    _LibRslTArrayGet = (LibRslTArrayGet)RSScriptLib->resolve("RslTArrayGet");
    _LibSetParm = (LibSetParm)RSScriptLib->resolve("SetParm");
}

QVariant SetFromRslValue(void *value, bool isStringListProp)
{
    QVariant result;
    VALUE *val = (VALUE*)value;
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    LoadFunctions();

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
        TGenObject *TArrayText = (TGenObject*)_LibRslIsTArray(P_GOBJ(val->value.obj));

        if (TArrayText)
        {
            if (isStringListProp)
            {
                QStringList lst;

                int size = _LibRslTArraySize(TArrayText);
                for (int i = 0; i < size; i++)
                {
                    VALUE *v = (VALUE*)_LibRslTArrayGet(TArrayText,i);
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

#define CHECK_TYPE(NeedType) ((((VALUE*)val)->v_type == NeedType) || (((VALUE*)val)->v_type == V_UNDEF && isOutParam))
bool CompareTypes(const int &MetaType, void *val, bool isOutParam)
{
    bool result = false;

    switch(MetaType)
    {
    case QVariant::String:
        result = CHECK_TYPE(V_STRING);
        break;
    case QVariant::Int:
    case QVariant::UInt:
    case QMetaType::Short:
        result = CHECK_TYPE(V_INTEGER);
        break;
    case QVariant::LongLong:
    case QVariant::ULongLong:
        result = CHECK_TYPE(V_BIGINT);
        break;
    case QVariant::Double:
        result = CHECK_TYPE(V_DOUBLE);
        break;
    case QVariant::Date:
        result = CHECK_TYPE(V_DATE);
        break;
    case QVariant::Time:
        result = CHECK_TYPE(V_TIME);
        break;

    case QVariant::Bool:
        result = CHECK_TYPE(V_BOOL);
        break;

    default:
        result = false;
    }
    return result;
}

int SetValueFromVariant(std::function<void(int,void*)> Setter, const QVariant &value)
{
    int result = 0;
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    LoadFunctions();

    int type = value.type();
    switch(type)
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
        TGenObject *ValueArray = (TGenObject*)_LibRslTArrayCreate(lst.size(), lst.size());

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

    case QMetaType::QObjectStar:
    {
        QObject *obj = value.value<QObject*>();
        const QMetaObject *meta = obj->metaObject();

        if (meta)
        {
            RegisterInfoBase *info = RegisterObjList::inst()->info(meta->className());

            //qDebug() << info << obj << meta->className();

            TGenObject *Child = nullptr;
            info->Create((void**)&Child, obj, RegisterInfoBase::CppOwner);
            Setter(V_GENOBJ, P_GOBJ(Child));
        }
    }
        break;

    default:
        //qDebug() << value.type() << (int)value.type() << value.userType();
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
    RegisterInfoBase *info = findInfo(meta->className());

    if (id < OBJ_RSL_ENUM_OFFSET)
    {
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
    else
    {
        int enumval = info->enumValue(id);
        ValueSet(val, V_INTEGER, &enumval);

        return 0;
    }

    return -1;
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
