// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef TOOLSRUNTINERSL_H
#define TOOLSRUNTINERSL_H

#include <QObject>
#include <QDir>

class ToolsRuntime : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString rslLibVersion READ GetRuntimeVersion);
public:
    Q_INVOKABLE ToolsRuntime(QObject *parent = nullptr);

    Q_INVOKABLE bool GetPostgreSQLInstallLocation(QString &dir);
    Q_INVOKABLE QString ReadTextFileContent(const QString &filename);
    Q_INVOKABLE QString GetRuntimeVersion() const;
    Q_INVOKABLE QString FullFileNameFromDir(const QString &file);

    Q_INVOKABLE int HighlighterByName(const QString &name);
};

#endif // TOOLSRUNTINERSL_H
