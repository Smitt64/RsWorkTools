#ifndef RSLTOOLSRUNTIMEMODULE_H
#define RSLTOOLSRUNTIMEMODULE_H

#include <QObject>
#include <QtPlugin>
#include <rsscript/RslModulePluginInterface.h>

class RslToolsRuntimeModule : public QObject,
        public RslStaticModuleInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.toolsruntime.rsl.modules.RslStaticModuleInterface/1.0" FILE "RslToolsRuntimeModule.json")
    Q_INTERFACES(RslStaticModuleInterface)

public:
    virtual QStringList staticModules() const Q_DECL_OVERRIDE;
    virtual void registerStaticModules() Q_DECL_OVERRIDE;
};

#endif // RSLTOOLSRUNTIMEMODULE_H
