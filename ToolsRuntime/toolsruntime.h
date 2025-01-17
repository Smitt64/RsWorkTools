// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef TOOLSRUNTIME_H
#define TOOLSRUNTIME_H

#include <QDir>
#include <QLoggingCategory>
#include "ToolsRuntime_global.h"

class QSqlQuery;
class QSqlDatabase;
class QProcess;
class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logUnknown);
TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logHighlighter);
TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logRsl);
TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logSql)
TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logSettings)
TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logProcess)
TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logUpdate)
TOOLSRUNTIME_EXPORT Q_DECLARE_LOGGING_CATEGORY(logNetApi)

TOOLSRUNTIME_EXPORT void toolMakeSqlDatabaseObj(QSqlDatabase &db, QObject **obj);

TOOLSRUNTIME_EXPORT QString toolLogginFileName();
TOOLSRUNTIME_EXPORT bool toolIsLoggingEnabled();
TOOLSRUNTIME_EXPORT void toolDisableLogging();
TOOLSRUNTIME_EXPORT bool toolInitLogging(const QString &prefix, const QString &rules);
TOOLSRUNTIME_EXPORT void toolLoggingCategoryListAdd(const QLoggingCategory &category);
TOOLSRUNTIME_EXPORT QStringList toolLoggingCategoryList();
TOOLSRUNTIME_EXPORT void toolSetLoggingRules(const QString &rules);
TOOLSRUNTIME_EXPORT const QLoggingCategory &toolLoggingCategory(const QString &name);

TOOLSRUNTIME_EXPORT QString toolGetRuntimeVersion();
TOOLSRUNTIME_EXPORT QString toolFullFileNameFromDir(const QString &file);
TOOLSRUNTIME_EXPORT QString toolReadTextFileContent(const QString &filename, const QString &encode = QString());
TOOLSRUNTIME_EXPORT QByteArray toolReadFileContent(const QString &filename);
TOOLSRUNTIME_EXPORT bool toolSaveResourceToFile(const QString &resname, const QString &filename); 
TOOLSRUNTIME_EXPORT bool toolGetPostgreSQLInstallLocation(QDir &dir);
TOOLSRUNTIME_EXPORT int toolExecuteQuery(QSqlQuery *query, QString *err = Q_NULLPTR);

TOOLSRUNTIME_EXPORT int toolShowCodeDialog(QWidget *parent, const QString &title, const int &type, const QString &code);

TOOLSRUNTIME_EXPORT int toolHighlighterByName(const QString &name);

enum JavaHomes
{
    JavaRuntimeHomes32 = 1 << 1,
    JavaDevelopmentHomes32 = 1 << 2,
    JavaRuntimeHomes64 = 1 << 3,
    JavaDevelopmentHomes64 = 1 << 4,

    JavaAllHomes = JavaRuntimeHomes32 | JavaDevelopmentHomes32 |
        JavaRuntimeHomes64 | JavaDevelopmentHomes64,
};
TOOLSRUNTIME_EXPORT QStringList toolGetJavaHomes(const quint32 &homeflags);
TOOLSRUNTIME_EXPORT int toolStartProcess(QProcess *exe, const QString &program,
                                             const QStringList& arguments,
                                             bool waitForFinished = false,
                                             bool waitForStarted = false,
                                             int timeout = 30000,
                                             bool waitForReadyRead = false);

typedef std::tuple<QString, QString> UserDomainTuple;
TOOLSRUNTIME_EXPORT UserDomainTuple toolGetCurrentUserAndDomain();
TOOLSRUNTIME_EXPORT QString toolGetApiUrl(const QString &method);
TOOLSRUNTIME_EXPORT QNetworkReply *toolGetApiReply(QNetworkRequest *request, QNetworkAccessManager *ApiManager, bool wait = false);
TOOLSRUNTIME_EXPORT QNetworkReply *toolPostApiReply(QNetworkRequest *request, QNetworkAccessManager *ApiManager,
                                                    const QByteArray &data,
                                                    bool wait = false);

#endif // TOOLSRUNTIME_H
