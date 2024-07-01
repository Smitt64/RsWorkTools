#include "registerobjlist.hpp"
#include <QMap>

RegisterObjList *RegisterObjList::m_inst = nullptr;

class RegisterObjListPrivate
{
    Q_DECLARE_PUBLIC(RegisterObjList)
public:
    RegisterObjListPrivate(RegisterObjList *obj)
    {
        q_ptr = obj;
    }

    QMap<QString, RegisterInfoBase*> m_Info;
    RegisterObjList *q_ptr;
};

// -------------------------------------------------------

RegisterObjList::RegisterObjList():
    d_ptr(new RegisterObjListPrivate(this))
{

}

RegisterObjList *RegisterObjList::inst()
{
    if (!RegisterObjList::m_inst)
        RegisterObjList::m_inst = new RegisterObjList();

    return RegisterObjList::m_inst;
}

RegisterInfoBase *RegisterObjList::info(const QString &name)
{
    Q_D(RegisterObjList);

    if (!d->m_Info.contains(name))
        return nullptr;

    return d->m_Info[name];
}

RegisterInfoBase *findInfo(const QString &name)
{
    return RegisterObjList::inst()->info(name);
}

bool RegisterObjList::isExists(const QString &name) const
{
    Q_D(const RegisterObjList);

    return d->m_Info.contains(name);
}

void RegisterObjList::InsertInfo(const QString &name, RegisterInfoBase *info)
{
    Q_D(RegisterObjList);
    d->m_Info.insert(name, info);
}
