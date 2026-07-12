// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef MARKDOWNSETTINGS_H
#define MARKDOWNSETTINGS_H

#include <QString>

struct MarkdownSettings
{
    enum Renderer
    {
        LocalQt,
        GitHubApi
    };

    Renderer renderer = GitHubApi;
    QString cssUrl = QStringLiteral("https://cdnjs.cloudflare.com/ajax/libs/github-markdown-css/5.2.0/github-markdown-light.min.css");
    QString githubApiVersion = QStringLiteral("2022-11-28");
    QString githubMode = QStringLiteral("gfm");

    bool operator==(const MarkdownSettings &other) const;
    bool operator!=(const MarkdownSettings &other) const;
};

#endif // MARKDOWNSETTINGS_H
