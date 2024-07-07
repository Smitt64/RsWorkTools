#include "registerobjlist.hpp"
#include "registerinfobase.h"
#include "rsldirs.h"
#include "rsscript/RslModulePluginInterface.h"
#include <QMap>
#include <QPluginLoader>

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
    QMap<QString, RslStaticModule*> m_StaticModules;
    RegisterObjList *q_ptr;

    QPluginLoader m_Loader;
};

// -------------------------------------------------------

RegisterObjList::RegisterObjList():
    d_ptr(new RegisterObjListPrivate(this))
{

}

void RegisterObjList::loadPlugins()
{
    Q_D(RegisterObjList);
    QObjectList lst = QPluginLoader::staticInstances();

    for (auto obj : lst)
    {
        RslStaticModuleInterface *plugin = dynamic_cast<RslStaticModuleInterface*>(obj);

        if (plugin)
        {
            qDebug() << plugin->staticModules();
            plugin->registerStaticModules();
        }
    }
}

RegisterObjList *RegisterObjList::inst()
{
    if (!RegisterObjList::m_inst)
    {
        RegisterObjList::m_inst = new RegisterObjList();
        RegisterObjList::m_inst->loadPlugins();
    }

    return RegisterObjList::m_inst;
}

RegisterInfoBase *RegisterObjList::info(const QString &name)
{
    Q_D(RegisterObjList);

    if (!d->m_Info.contains(name))
        return nullptr;

    return d->m_Info[name];
}

RegisterInfoBase *RegisterObjList::info(const Qt::HANDLE &rslID)
{
    Q_D(RegisterObjList);

    RegisterInfoBase *result = nullptr;
    for (auto item : d->m_Info)
    {
        if (item->rslID() == rslID)
        {
            result = item;
            break;
        }
    }

    return result;
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

bool RegisterObjList::addStaticModulePrivate(const QString &name, RslStaticModule *module, RslStaticModuleCallerBase *caller)
{
    Q_D(RegisterObjList);

    if (!d->m_StaticModules.contains(name.toUpper()))
    {
        d->m_StaticModules.insert(name.toUpper(), module);
        module->setCaller(caller);
        return true;
    }

    return false;
}

RslStaticModule *RegisterObjList::staticModule(const QString &name)
{
    Q_D(RegisterObjList);

    if (d->m_StaticModules.contains(name.toUpper()))
        return d->m_StaticModules[name.toUpper()];

    return nullptr;
}

void RegisterObjList::setIncDir(const QString &path)
{
    ToolsSetIncDir(path.toLocal8Bit().data());
}

void RegisterObjList::setIncDir(const QStringList &path)
{
    setIncDir(path.join(";"));
}

void RegisterObjList::setTextDir(const QString &path)
{
    ToolsSetTextDir(path.toLocal8Bit().data());
}

bool RegisterObjList::AddObject(const QString &name, const bool &canCreate)
{
    Q_D(RegisterObjList);
    RegisterInfoBase *_info = info(name);

    if (!_info)
        return false;

    _info->importObject(canCreate);

    return true;
}

RegisterObjList *rslObjList()
{
    return RegisterObjList::inst();
}
