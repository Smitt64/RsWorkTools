#include "xmlcodehighlighter.h"
#include "highlighterstyle.h"

XmlCodeHighlighter::XmlCodeHighlighter(QObject *parent)
    : CodeHighlighter{parent}
{
    m_xmlElementRegex.setPattern("<[?\\s]*[/]?[\\s]*([^\\n][^>]*)(?=[\\s/>])");
    m_xmlAttributeRegex.setPattern("\\w+(?=\\=)");
    m_xmlValueRegex.setPattern("\"[^\\n\"]+\"(?=[?\\s/>])");
    m_xmlCommentRegex.setPattern("<!--[^\\n]*-->");

    m_xmlKeywordRegexes = QList<QRegExp>() << QRegExp("<\\?") << QRegExp("/>")
                                           << QRegExp(">") << QRegExp("<") << QRegExp("</")
                                           << QRegExp("\\?>");
}

XmlCodeHighlighter::~XmlCodeHighlighter()
{

}

void XmlCodeHighlighter::reset()
{
    CodeHighlighter::reset();
}

void XmlCodeHighlighter::highlightBlock(const QString &text)
{
    highlightByRegex(style()->format(FormatComment), m_xmlCommentRegex, text);
    highlightByRegex(style()->format(FormatStrig), m_xmlValueRegex, text);

    int xmlElementIndex = m_xmlElementRegex.indexIn(text);
    while(xmlElementIndex >= 0)
    {
        int matchedPos = m_xmlElementRegex.pos(1);
        int matchedLength = m_xmlElementRegex.cap(1).length();

        if (!isInCommentOrString(matchedPos, text))

        xmlElementIndex = m_xmlElementRegex.indexIn(text, matchedPos + matchedLength);
    }

    typedef QList<QRegExp>::const_iterator Iter;
    Iter xmlKeywordRegexesEnd = m_xmlKeywordRegexes.end();
    for(Iter it = m_xmlKeywordRegexes.begin(); it != xmlKeywordRegexesEnd; ++it)
    {
        const QRegExp & regex = *it;
        int index = regex.indexIn(text);
        while(index >= 0)
        {
            if (!isInCommentOrString(index, text))
            {
                int matchedLength = regex.matchedLength();
                setFormat(index, matchedLength, style()->format(FormatKeyword));
            }
            index = regex.indexIn(text, index + regex.matchedLength());
        }
    }

    int attrIndex = m_xmlAttributeRegex.indexIn(text);
    while(attrIndex >= 0)
    {
        if (!isInCommentOrString(attrIndex, text))
        {
            int matchedLength = m_xmlAttributeRegex.matchedLength();
            setFormat(attrIndex, matchedLength, style()->format(FormatKeyword2));
        }
        attrIndex = m_xmlAttributeRegex.indexIn(text, attrIndex + m_xmlAttributeRegex.matchedLength());
    }
}

void XmlCodeHighlighter::highlightByRegex(const QTextCharFormat &format,
                                          const QRegExp &regex, const QString &text)
{
    int index = regex.indexIn(text);

    while(index >= 0)
    {
        int matchedLength = regex.matchedLength();
        setFormat(index, matchedLength, format);

        index = regex.indexIn(text, index + matchedLength);
    }
}
