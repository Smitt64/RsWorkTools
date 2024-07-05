#ifndef RSLMODULEPLUGININTERFACE_H
#define RSLMODULEPLUGININTERFACE_H

#include <QtPlugin>

class RslStaticModule;

class RslStaticModuleInterface
{
public:
    virtual ~RslStaticModuleInterface() = default;

    virtual QStringList staticModules() const = 0;
    virtual void registerStaticModules() = 0;
};

#define RslStaticModuleInterface_iid "org.toolsruntime.rsl.modules.RslStaticModuleInterface/1.0"
Q_DECLARE_INTERFACE(RslStaticModuleInterface, RslStaticModuleInterface_iid)

#endif // RSLMODULEPLUGININTERFACE_H
