// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef TOOLSRUNTIME_H
#define TOOLSRUNTIME_H

#include <QDir>
#include <QLoggingCategory>
#include "ToolsRuntime_global.h"

class QSqlQuery;

TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logHighlighter);
TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logRsl);
TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logSql)
TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logSettings)

TOOLSRUNTIME_EXPORT QString toolLogginFileName();
TOOLSRUNTIME_EXPORT bool toolIsLoggingEnabled();
TOOLSRUNTIME_EXPORT void toolDisableLogging();
TOOLSRUNTIME_EXPORT bool toolInitLogging(const QString &prefix, const QString &rules);

TOOLSRUNTIME_EXPORT QString toolGetRuntimeVersion();
TOOLSRUNTIME_EXPORT QString toolFullFileNameFromDir(const QString &file);
TOOLSRUNTIME_EXPORT QString toolReadTextFileContent(const QString &filename, const QString &encode = QString());
TOOLSRUNTIME_EXPORT QByteArray toolReadFileContent(const QString &filename);
TOOLSRUNTIME_EXPORT bool toolSaveResourceToFile(const QString &resname, const QString &filename); 
TOOLSRUNTIME_EXPORT bool toolGetPostgreSQLInstallLocation(QDir &dir);
TOOLSRUNTIME_EXPORT int toolExecuteQuery(QSqlQuery *query, QString *err = Q_NULLPTR);

TOOLSRUNTIME_EXPORT int toolShowCodeDialog(QWidget *parent, const QString &title, const int &type, const QString &code);

TOOLSRUNTIME_EXPORT int toolHighlighterByName(const QString &name);

#endif // TOOLSRUNTIME_H
