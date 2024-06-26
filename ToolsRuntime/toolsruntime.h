#ifndef TOOLSRUNTIME_H
#define TOOLSRUNTIME_H

#include <QDir>
#include "ToolsRuntime_global.h"

class TOOLSRUNTIME_EXPORT ToolsRuntime
{
public:
    ToolsRuntime();
};

TOOLSRUNTIME_EXPORT QString toolFullFileNameFromDir(const QString &file);
TOOLSRUNTIME_EXPORT QString toolReadTextFileContent(const QString &filename);
TOOLSRUNTIME_EXPORT bool toolSaveResourceToFile(const QString &resname, const QString &filename); 
TOOLSRUNTIME_EXPORT bool toolGetPostgreSQLInstallLocation(QDir &dir);

#endif // TOOLSRUNTIME_H
