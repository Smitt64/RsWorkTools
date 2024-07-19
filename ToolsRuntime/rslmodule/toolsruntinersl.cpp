#include "toolsruntinersl.h"
#include "toolsruntime.h"

ToolsRuntime::ToolsRuntime(QObject *parent) :
    QObject(parent)
{

}

bool ToolsRuntime::GetPostgreSQLInstallLocation(QString &dir)
{
    QDir qdir;
    bool result = toolGetPostgreSQLInstallLocation(qdir);

    dir = qdir.path();
    return result;
}

QString ToolsRuntime::ReadTextFileContent(const QString &filename)
{
    return toolReadTextFileContent(filename);
}

QString ToolsRuntime::GetRuntimeVersion() const
{
    return toolGetRuntimeVersion();
}

QString ToolsRuntime::FullFileNameFromDir(const QString &file)
{
    return toolFullFileNameFromDir(file);
}