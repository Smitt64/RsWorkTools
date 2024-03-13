#ifndef TOOLSRUNTIME_H
#define TOOLSRUNTIME_H

#include "ToolsRuntime_global.h"

class TOOLSRUNTIME_EXPORT ToolsRuntime
{
public:
    ToolsRuntime();
};

TOOLSRUNTIME_EXPORT QString toolFullFileNameFromDir(const QString &file);

#endif // TOOLSRUNTIME_H
