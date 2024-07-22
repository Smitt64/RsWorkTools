// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef CODEHIGHLIGHTER_H
#define CODEHIGHLIGHTER_H

#include "ToolsRuntime_global.h"
#include <QSyntaxHighlighter>

class CodeHighlighterPrivate;
class TOOLSRUNTIME_EXPORT CodeHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    CodeHighlighter(QTextDocument *parent = Q_NULLPTR);
    virtual ~CodeHighlighter();

private:
    CodeHighlighterPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(CodeHighlighter);
};

#endif // CODEHIGHLIGHTER_H
