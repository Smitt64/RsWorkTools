// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "windowsmodernstyleplugin.h"
#include "style/windowsmodernstyle.h"

WindowsModernStylePlugin::WindowsModernStylePlugin(QObject *parent)
    : QStylePlugin(parent)
{
}

QStyle *WindowsModernStylePlugin::create(const QString &key)
{
    if (!key.compare("WindowsModernStyle", Qt::CaseInsensitive))
        return new WindowsModernStyle();
    else if (!key.compare("WindowsModernStyleBlue", Qt::CaseInsensitive))
        return new WindowsModernStyle(WindowsModernStyle::Blue);
    else if (!key.compare("WindowsModernStyleSilver", Qt::CaseInsensitive))
        return new WindowsModernStyle(WindowsModernStyle::Silver);
    else if (!key.compare("WindowsModernStyleOlive", Qt::CaseInsensitive))
        return new WindowsModernStyle(WindowsModernStyle::Olive);

    return nullptr;
}
