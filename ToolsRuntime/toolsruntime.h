// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef TOOLSRUNTIME_H
#define TOOLSRUNTIME_H

#include <QDir>
#include <QLoggingCategory>
#include "ToolsRuntime_global.h"

TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(highlighterLog);
TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(rslLog);

TOOLSRUNTIME_EXPORT QString toolGetRuntimeVersion();
TOOLSRUNTIME_EXPORT QString toolFullFileNameFromDir(const QString &file);
TOOLSRUNTIME_EXPORT QString toolReadTextFileContent(const QString &filename, const QString &encode = QString());
TOOLSRUNTIME_EXPORT QByteArray toolReadFileContent(const QString &filename);
TOOLSRUNTIME_EXPORT bool toolSaveResourceToFile(const QString &resname, const QString &filename); 
TOOLSRUNTIME_EXPORT bool toolGetPostgreSQLInstallLocation(QDir &dir);

#endif // TOOLSRUNTIME_H
