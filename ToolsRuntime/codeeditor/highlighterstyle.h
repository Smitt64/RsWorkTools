#ifndef HIGHLIGHTERSTYLE_H_
#define HIGHLIGHTERSTYLE_H_

#include "ToolsRuntime_global.h"
#include <QTextCharFormat>

#define FormatKeyword "keyword"
#define FormatKeyword2 "keyword2"
#define FormatComment "comment"
#define FormatDefault "plain"
#define FormatFunction "function"
#define FormatNumber "number"
#define FormatStrig "string"
#define FormatEnum "enum"
#define FormatDefine "define"
#define FormatElemType "type"

class QColor;
class HighlighterStylePrivate;
class StyleItemPrivate;
class QTextCharFormat;
class TOOLSRUNTIME_EXPORT StyleItem
{
public:
    StyleItem();
    void load(const QString &filename);

    QTextCharFormat format(const QString &alias);
    bool hasFormat(const QString &alias) const;

    const QColor &editorBackground() const;
    const QColor &editorCurrentLine() const;
    const QColor &editorCurrentWord() const;

private:
    StyleItemPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(StyleItem);
};

class TOOLSRUNTIME_EXPORT HighlighterStyle
{
public:
    virtual ~HighlighterStyle();

    static HighlighterStyle *inst();
    QSharedPointer<StyleItem> style(const QString &name = QString()) const;
    QStringList themes() const;

    void setDefaultTheme(const QString &theme);
    QString defaultTheme() const;

private:
    HighlighterStyle();
    static HighlighterStyle *m_Inst;

    HighlighterStylePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(HighlighterStyle);
};

#endif // HIGHLIGHTERSTYLE_H
