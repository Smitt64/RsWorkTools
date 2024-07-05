#define BUILD_DLM
#include "rsl/dlmintf.h"
#include "statvars.h"
#include "rsl/isymbol.h"
#include "typeinfo_p.h"
#include "registerobjlist.hpp"
#include "rslexecutor.h"
#include <cstring>
#include <QDebug>
#include <QTextCodec>
#include <QDate>
#include <QTime>
#include <QMetaProperty>
#include <QLibrary>
#include <QMetaType>

static bool CompareParamTypes(const QMetaMethod &method)
{
    bool result = true;

    for (int i = 0; i < method.parameterCount(); i++)
    {
        VALUE *val = nullptr;
        GetParm(i + 1, &val);

        if (!CompareTypes(method.parameterType(i), (void*)val))
            result = false;
    }

    return result;
}

static int FindMethod(const QMetaObject *meta, const QString &name, int NumParams)
{
    int id = -1;

    for (int i = 0; i < meta->methodCount(); i++)
    {
        QMetaMethod tmpmethod = meta->method(i);

        if (!name.compare(tmpmethod.name()))
        {
            if (tmpmethod.parameterCount() == NumParams)
            {
                if (NumParams == 0)
                {
                    id = i;
                    break;
                }
                else
                {
                    // compare params types
                    if (CompareParamTypes(tmpmethod))
                    {
                        id = i;
                        break;
                    }
                }
            }
        }
    }

    return id;
}

int GenObjRunId(TGenObject *obj, long id)
{
    QObjectRsl *rsl = (QObjectRsl*)obj;
    const QMetaObject *meta = rsl->object->metaObject();
    QMetaMethod method = meta->method(id - OBJ_RSL_METHOD_OFFSET);
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");

    int NumParam = GetNumParm() - 1;
    id = FindMethod(meta, method.name(), NumParam);

    if (id < 0)
    {
        iError(IER_RUNMETHOD, "Param count missmatch");
        return -1;
    }

    method = meta->method(id);

    int parmaOffset = 0;
    if (method.returnType() != QMetaType::Void)
        parmaOffset = 1;

    int MethodParams = method.parameterCount();
    auto AllocaTeParam = [=](const int &Type, void **param, VALUE *val) -> void
    {
        *param = QMetaType::create(Type);

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
        else if (Type == QMetaType::LongLong || Type == QMetaType::ULongLong)
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

    qDebug() << id << method.name() << method.methodIndex();
    rsl->object->qt_metacall(QMetaObject::InvokeMetaMethod, id, params);

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
        QMetaType::destroy(method.parameterType(i), params[i]);

    delete[] params;

    return 0;
}

int GenObjRun(TGenObject *obj, const char *methodname, long *id)
{
    QObjectRsl *rsl = (QObjectRsl*)obj;
    const QMetaObject *meta = rsl->object->metaObject();
    RegisterInfoBase *info = findInfo(meta->className());

    if (info->findMember(methodname, id) == 0)
        return GenObjRunId(obj, *id);

    return -1;
}
