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
