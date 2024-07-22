#include "codehighlighter.h"

class CodeHighlighterPrivate
{
    Q_DECLARE_PUBLIC(CodeHighlighter)
public:
    CodeHighlighterPrivate(CodeHighlighter *parent)
    {
        q_ptr = parent;
    }

    CodeHighlighter *q_ptr;
};

CodeHighlighter::CodeHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent),
    d_ptr(new CodeHighlighterPrivate(this))
{

}

CodeHighlighter::~CodeHighlighter()
{

}
