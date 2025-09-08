#include "sqlcodehighlighter.h"
#include "highlighterstyle.h"
#include <QFile>
#include <QTextStream>

SqlCodeHighlighter::SqlCodeHighlighter(QObject *parent)
    : CodeHighlighter{parent}
{

}

SqlCodeHighlighter::~SqlCodeHighlighter()
{

}

void SqlCodeHighlighter::reset()
{
    CodeHighlighter::reset();

    HighlightingRule rule;
    QStringList keywordPatterns;

    QSharedPointer<StyleItem> style = CodeHighlighter::style();

    keywordPatterns << "\\bVARCHAR2\\b"
                    << "\\bNVARCHAR2\\b"
                    << "\\bVARCHAR\\b"
                    << "\\bNUMBER\\b"
                    << "\\bLONG\\b"
                    << "\\bDATE\\b"
                    << "\\bCHAR\\b";

    QFile f(":/highlighter/sql_keys.txt");
    if (f.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&f);
        QString line;
        while (!stream.atEnd())
        {
            line = stream.readLine().trimmed();
            if (!line.isEmpty())
            {
                QStringList l = line.remove(" ").split(",");
                foreach (const QString &pattern, l)
                {
                    if (!pattern.isEmpty())
                        keywordPatterns.append(QString("\\b%1\\b").arg(pattern));
                }
            }
        }
        f.close();
    }

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = style->format(FormatKeyword);
        rule.isNotCaseInsensitive = false;
        addHighlightingRuleToEnd(rule);
    }

    QString excludedKeywords = keywordPatterns.join("|").replace("\\b", "");
    rule.pattern = QRegularExpression(QString("\\b(?!(?:%1)\\b)[a-zA-Z_][a-zA-Z0-9_]*\\s*(?=\\()").arg(excludedKeywords));
    rule.format = style->format(FormatFunction);
    rule.isNotCaseInsensitive = false;
    addHighlightingRuleToEnd(rule);

    rule.pattern = QRegularExpression("((?<![\\\\])['\"])((?:.(?!(?<![\\\\])\\1))*.?)\\1");
    rule.format = style->format(FormatStrig);
    addHighlightingRuleToEnd(rule);

    if (style->hasFormat(FormatNumber))
    {
        rule.pattern = QRegularExpression("\\b[\\-\\+]?([0-9]+(\\.[0-9]+)?)\\b");
        rule.format = style->format(FormatNumber);
        addHighlightingRuleToEnd(rule);
    }

    setSingleLineCommentStr("--");
}
