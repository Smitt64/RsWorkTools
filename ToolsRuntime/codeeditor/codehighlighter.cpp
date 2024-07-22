// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
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
