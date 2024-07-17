#include "windowsmodernstyleplugin.h"
#include "style/windowsmodernstyle.h"

WindowsModernStylePlugin::WindowsModernStylePlugin(QObject *parent)
    : QStylePlugin(parent)
{
}

QStyle *WindowsModernStylePlugin::create(const QString &key)
{
    if (key == "WindowsModernStyle")
        return new WindowsModernStyle();
    else if (key == "WindowsModernStyleBlue")
        return new WindowsModernStyle(WindowsModernStyle::Blue);
    else if (key == "WindowsModernStyleSilver")
        return new WindowsModernStyle(WindowsModernStyle::Silver);
    else if (key == "WindowsModernStyleOlive")
        return new WindowsModernStyle(WindowsModernStyle::Olive);

    return nullptr;
}
