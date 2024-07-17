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
