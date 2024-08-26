// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
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

int ToolsRuntime::HighlighterByName(const QString &name)
{
    return toolHighlighterByName(name);
}
