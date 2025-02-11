// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "toolsruntinersl.h"
#include "toolsruntime.h"
#include <QTime>

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

QString ToolsRuntime::ReadTextFileContent(const QString &filename, const QString &encode)
{
    return toolReadTextFileContent(filename, encode);
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

QString ToolsRuntime::toString(const QTime &time, const QString &format)
{
    return time.toString(format);
}

QString ToolsRuntime::toString(const QDate &date, const QString &format)
{
    return date.toString(format);
}

QStringList ToolsRuntime::split(const QString &str, const QString &sep)
{
    return str.split(sep);
}

QString ToolsRuntime::join(const QStringList &lst, const QString &separator)
{
    return lst.join(separator);
}

QString ToolsRuntime::number(int n, int base)
{
     return QString::number(n, base);
}

QString ToolsRuntime::number(qint64 n, int base)
{
     return QString::number(n, base);
}

QString ToolsRuntime::number(double n, char format, int precision)
{
     return QString::number(n, format, precision);
}

QString ToolsRuntime::replace(QString str, const int &pos, const int &len, const QString &replace)
{
     return str.replace(pos, len, replace);
}

QString ToolsRuntime::replace(QString str, const QString &before, const QString &after, const bool &CaseSensitive)
{
     return str.replace(before, after, CaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
}

QString ToolsRuntime::simplified(QString str)
{
    return str.simplified();
}

QStringList ToolsRuntime::getJavaHomes(const quint32 &homeflags)
{
    return toolGetJavaHomes(homeflags);
}

int ToolsRuntime::levenshteinDistance(const QString &s1, const QString &s2)
{
    return toolLevenshteinDistance(s1, s2);
}

double ToolsRuntime::levenshteinSimilarity(const QString &s1, const QString &s2)
{
    return toolLevenshteinSimilarity(s1, s2);
}
