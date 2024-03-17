#ifndef TOOLSRUNTIME_H
#define TOOLSRUNTIME_H

#include "ToolsRuntime_global.h"

class TOOLSRUNTIME_EXPORT ToolsRuntime
{
public:
    ToolsRuntime();
};

TOOLSRUNTIME_EXPORT QString toolFullFileNameFromDir(const QString &file);
TOOLSRUNTIME_EXPORT QString toolReadTextFileContent(const QString &filename);

#endif // TOOLSRUNTIME_H
