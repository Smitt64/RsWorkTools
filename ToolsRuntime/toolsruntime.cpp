#include "toolsruntime.h"
#include <QDir>
#include <QApplication>
#include <QSettings>

ToolsRuntime::ToolsRuntime()
{

}

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

QString toolReadTextFileContent(const QString &filename)
{
    QString content;
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
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

