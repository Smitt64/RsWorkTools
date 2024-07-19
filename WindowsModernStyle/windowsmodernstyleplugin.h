// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef WINDOWSMODERNSTYLEPLUGIN_H
#define WINDOWSMODERNSTYLEPLUGIN_H

#include <QStylePlugin>

class WindowsModernStylePlugin : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QStyleFactoryInterface_iid FILE "WindowsModernStyle.json")

public:
    explicit WindowsModernStylePlugin(QObject *parent = nullptr);

private:
    QStyle *create(const QString &key) override;
};

#endif // WINDOWSMODERNSTYLEPLUGIN_H
