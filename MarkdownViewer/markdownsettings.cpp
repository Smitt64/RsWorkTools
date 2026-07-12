// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "markdownsettings.h"

bool MarkdownSettings::operator==(const MarkdownSettings &other) const
{
    return renderer == other.renderer
           && cssUrl == other.cssUrl
           && githubApiVersion == other.githubApiVersion
           && githubMode == other.githubMode;
}

bool MarkdownSettings::operator!=(const MarkdownSettings &other) const
{
    return !(*this == other);
}
