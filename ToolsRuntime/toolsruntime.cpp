// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "toolsruntime.h"
#include <Windows.h>
#include <QDir>
#include <QApplication>
#include <QSettings>
#include <QPluginLoader>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>

Q_LOGGING_CATEGORY(logHighlighter, "Highlighter.Style")
Q_LOGGING_CATEGORY(logRsl, "Rsl")
Q_LOGGING_CATEGORY(logSql, "Sql")
Q_LOGGING_CATEGORY(logSettings, "Settings")

Q_IMPORT_PLUGIN(RslToolsRuntimeModule)

QString toolFullFileNameFromDir(const QString &file)
{
    QDir dir = QDir::current();
    QString fullfilename = dir.absoluteFilePath(file);

    if (QFile::exists(fullfilename))
        return fullfilename;

    dir = QDir(QApplication::applicationDirPath());
    fullfilename = dir.absoluteFilePath(file);

    if (QFile::exists(fullfilename))
        return fullfilename;

    return QString();
}

QString toolReadTextFileContent(const QString &filename, const QString &encode)
{
    QString content;
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (encode.isEmpty())
            content = f.readAll();
        else
        {
            QTextStream stream(&f);
            stream.setCodec(encode.toLocal8Bit().data());

            content = stream.readAll();
        }

        f.close();
    }
    return content;
}

QByteArray toolReadFileContent(const QString &filename)
{
    QByteArray content;
    QFile f(filename);

    if (f.open(QIODevice::ReadOnly))
    {
        content = f.readAll();
        f.close();
    }

    return content;
}

bool toolSaveResourceToFile(const QString &resname, const QString &filename) 
{ 
    QFile res(resname), out(filename); 
    if (res.open(QIODevice::ReadOnly)) 
    { 
        if (out.open(QIODevice::WriteOnly)) 
        { 
            out.write(res.readAll()); 
            out.close(); 
        } 
        else 
        { 
            res.close(); 
            return false; 
        } 
        res.close(); 
 
        return true; 
    } 
    return false; 
} 

bool toolGetPostgreSQLInstallLocation(QDir &dir)
{
    QSettings settings64("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", QSettings::NativeFormat);
    QStringList childKeys = settings64.childGroups();

    for (const QString &key : childKeys)
    {
        if (key.contains("PostgreSQL"))
        {
            settings64.beginGroup(key);
            QString InstallLocation = settings64.value("InstallLocation").toString();

            if (!InstallLocation.isEmpty())
            {
                QDir tmp(InstallLocation);

                if (tmp.cd("bin"))
                {
                    if (QFile::exists(tmp.absoluteFilePath("pg_dump.exe")) &&
                        QFile::exists(tmp.absoluteFilePath("psql.exe")))
                    {
                        dir = tmp;
                        settings64.endGroup();
                        return true;
                    }
                }
            }
            settings64.endGroup();
        }
    }
    return false;
}

QString toolGetRuntimeVersion()
{
    QString versionNumberString;
    char moduleName[MAX_PATH + 1];

    HMODULE hm = NULL;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                       GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       (LPWSTR) &toolGetRuntimeVersion, &hm);

    GetModuleFileNameA(hm, moduleName, MAX_PATH);

    DWORD dummyZero;
    DWORD versionSize = GetFileVersionInfoSizeA(moduleName, &dummyZero);

    if(!versionSize)
        return QString();

    void* pVersion = malloc(versionSize);
    if(!pVersion)
        return QString();

    if(!GetFileVersionInfoA(moduleName, NULL, versionSize, pVersion))
    {
        free(pVersion);
        return QString();
    }

    UINT length;
    VS_FIXEDFILEINFO* pFixInfo;
    if (VerQueryValueA(pVersion, "\\", (LPVOID*)&pFixInfo, &length))
    {
        versionNumberString = QString("%1.%2.%3.%4")
                .arg((pFixInfo->dwFileVersionMS >> 16) & 0xffff)
                .arg((pFixInfo->dwFileVersionMS >>  0) & 0xffff)
                .arg((pFixInfo->dwFileVersionLS >> 16) & 0xffff)
                .arg((pFixInfo->dwFileVersionLS >>  0) & 0xffff);
    }
    free(pVersion);

    return versionNumberString;
}

int toolExecuteQuery(QSqlQuery *query, QString *err)
{
    int stat = 0;

    QMap<QString, QVariant> values = query->boundValues();
    QMapIterator<QString, QVariant> i(values);
    while(i.hasNext())
    {
        i.next();
        qCInfo(logSql()) << i.key() << ": " << i.value();
    }

    bool result = query->exec();
    if (!result)
    {
        stat = 1;
        qCCritical(logSql()) << query->lastError().text();

        if (err != Q_NULLPTR)
            *err = query->lastError().text();
    }
    qCInfo(logSql()) << query->executedQuery();
    qCInfo(logSql()) << "Result:" << result;

    return stat;
}
