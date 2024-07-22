// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "codeeditor/highlighterstyle.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QTextCharFormat>
#include <QDir>
#include <QApplication>

HighlighterStyle *HighlighterStyle::m_Inst = nullptr;

#define NO_ELEMENT_TEMPLATE QString("Element \"%1\" not exists")
#define HEX_COLOR_REGEX QString("#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})")

typedef struct FmtGenHighlightingRule
{
    QRegularExpression pattern;
    QString alias;
}FmtGenHighlightingRule;
typedef QList<FmtGenHighlightingRule> FmtGenHighlightingRuleList;

typedef QPair<qint32, qint32> Range;
typedef QList<Range> RangeList;

void CheckColorFormat(const QString &hex, const QString &field)
{
    QRegExp rx(HEX_COLOR_REGEX);

    if (!rx.exactMatch(hex))
        throw QString("Invalid \"%1\" value").arg(field);
}

class StyleItemPrivate
{
    Q_DECLARE_PUBLIC(StyleItem);
public:
    StyleItemPrivate(StyleItem *parent)
    {
        q_ptr = parent;
    }

    StyleItem *q_ptr;

    QMap<QString, QTextCharFormat> m_Formats;
    QColor m_EditorBackground, m_EditorCurLine, m_EditorCurWord;
};

StyleItem::StyleItem() :
    d_ptr(new StyleItemPrivate(this))
{

}

QTextCharFormat StyleItem::format(const QString &alias)
{
    Q_D(StyleItem);
    if (d->m_Formats.contains(alias))
        return d->m_Formats[alias];

    return QTextCharFormat();
}

bool StyleItem::hasFormat(const QString &alias) const
{
    Q_D(const StyleItem);
    return d->m_Formats.contains(alias);
}

const QColor &StyleItem::editorBackground() const
{
    Q_D(const StyleItem);
    return d->m_EditorBackground;
}

const QColor &StyleItem::editorCurrentLine() const
{
    Q_D(const StyleItem);
    return d->m_EditorCurLine;
}

const QColor &StyleItem::editorCurrentWord() const
{
    Q_D(const StyleItem);
    return d->m_EditorCurWord;
}

void StyleItem::load(const QString &filename)
{
    Q_D(StyleItem);
    QFile f(filename);
    f.open(QIODevice::ReadOnly);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &error);

    if (error.error != QJsonParseError::NoError)
        throw error;

    QJsonObject obj = doc.object();

    if (!obj.contains("formats"))
        throw (NO_ELEMENT_TEMPLATE).arg("formats");

    if (obj.contains("editor.background"))
    {
        QString hex = obj["editor.background"].toString();
        CheckColorFormat(hex, "editor.background");
        d->m_EditorBackground = QColor(hex);
    }

    if (obj.contains("editor.selection.line"))
    {
        QString hex = obj["editor.selection.line"].toString();
        CheckColorFormat(hex, "editor.selection.line");
        d->m_EditorCurLine = QColor(hex);
    }

    if (obj.contains("editor.selection.word"))
    {
        QString hex = obj["editor.selection.word"].toString();
        CheckColorFormat(hex, "editor.selection.word");
        d->m_EditorCurWord = QColor(hex);
    }

    QJsonArray formats = obj["formats"].toArray();

    for (auto format : formats)
    {
        QJsonObject elem = format.toObject();

        if (!elem.contains("alias"))
            throw (NO_ELEMENT_TEMPLATE).arg("alias");

        QString Alias = elem["alias"].toString();

        QTextCharFormat textFormat;
        if (elem.contains("fontFamily"))
            textFormat.setFontFamily(elem["fontFamily"].toString());

        if (elem.contains("fontFixedPitch"))
            textFormat.setFontFixedPitch(elem["fontFixedPitch"].toBool());

        if (elem.contains("fontItalic"))
            textFormat.setFontItalic(elem["fontItalic"].toBool());

        if (elem.contains("fontKerning"))
            textFormat.setFontKerning(elem["fontKerning"].toBool());

        if (elem.contains("fontLetterSpacing"))
            textFormat.setFontLetterSpacing(elem["fontLetterSpacing"].toInt());

        if (elem.contains("fontOverline"))
            textFormat.setFontOverline(elem["fontOverline"].toBool());

        if (elem.contains("fontPointSize"))
            textFormat.setFontPointSize(elem["fontPointSize"].toInt());

        if (elem.contains("fontStretch"))
            textFormat.setFontStretch(elem["fontStretch"].toInt());

        if (elem.contains("fontStrikeOut"))
            textFormat.setFontStrikeOut(elem["fontStrikeOut"].toBool());

        if (elem.contains("fontUnderline"))
            textFormat.setFontUnderline(elem["fontUnderline"].toBool());

        if (elem.contains("fontWeight"))
        {
            int fontWeight = QFont::Normal;
            QString cons = elem["fontWeight"].toString();

            if (cons == "Thin")
                fontWeight = QFont::Thin;
            else if (cons == "ExtraLight")
                fontWeight = QFont::ExtraLight;
            else if (cons == "Light")
                fontWeight = QFont::Light;
            else if (cons == "Normal")
                fontWeight = QFont::Normal;
            else if (cons == "Medium")
                fontWeight = QFont::Medium;
            else if (cons == "DemiBold")
                fontWeight = QFont::DemiBold;
            else if (cons == "Bold")
                fontWeight = QFont::Bold;
            else if (cons == "ExtraBold")
                fontWeight = QFont::ExtraBold;
            else if (cons == "ExtraBold")
                fontWeight = QFont::Black;
            else
            {
                if (!elem["fontWeight"].isDouble())
                    throw QString("Invalid \"fontWeight\" value");
                else
                    fontWeight = elem["fontWeight"].toInt();
            }

            textFormat.setFontWeight(fontWeight);
        }

        if (elem.contains("fontWordSpacing"))
            textFormat.setFontWordSpacing(elem["fontWordSpacing"].toInt());

        if (elem.contains("foreground"))
        {
            QString hex = elem["foreground"].toString();
            CheckColorFormat(hex, "foreground");
            textFormat.setForeground(QColor(hex));
        }

        if (elem.contains("background"))
        {
            QString hex = elem["background"].toString();
            CheckColorFormat(hex, "background");
            textFormat.setBackground(QColor(hex));
        }

        if (elem.contains("underlineColor"))
        {
            QString hex = elem["underlineColor"].toString();
            CheckColorFormat(hex, "underlineColor");
            textFormat.setUnderlineColor(QColor(hex));
        }

        if (elem.contains("textOutline"))
        {
            QString hex = elem["textOutline"].toString();
            CheckColorFormat(hex, "textOutline");
            textFormat.setTextOutline(QColor(hex));
        }

        if (elem.contains("underlineStyle"))
        {
            QTextCharFormat::UnderlineStyle underlineStyle = QTextCharFormat::NoUnderline;
            QString cons = elem["underlineStyle"].toString();

            if (cons == "NoUnderline")
                underlineStyle = QTextCharFormat::NoUnderline;
            else if (cons == "SingleUnderline")
                underlineStyle = QTextCharFormat::SingleUnderline;
            else if (cons == "DashUnderline")
                underlineStyle = QTextCharFormat::DashUnderline;
            else if (cons == "DotLine")
                underlineStyle = QTextCharFormat::DotLine;
            else if (cons == "DashDotLine")
                underlineStyle = QTextCharFormat::DashDotLine;
            else if (cons == "DashDotDotLine")
                underlineStyle = QTextCharFormat::DashDotDotLine;
            else if (cons == "WaveUnderline")
                underlineStyle = QTextCharFormat::WaveUnderline;
            else if (cons == "SpellCheckUnderline")
                underlineStyle = QTextCharFormat::SpellCheckUnderline;
            else
                throw QString("Invalid \"underlineStyle\" value");

            textFormat.setUnderlineStyle(underlineStyle);
        }

        textFormat.setFontStyleHint(QFont::Courier);
        d->m_Formats.insert(Alias, textFormat);
    }

    f.close();
}

// ------------------------------------------------------------

class HighlighterStylePrivate
{
    Q_DECLARE_PUBLIC(HighlighterStyle)
public:
    HighlighterStylePrivate(HighlighterStyle *parent)
    {
        q_ptr = parent;
        m_DefaultTheme = "Default";
    }

    void loadStyles()
    {
        bool hr = true;
        QDir syntaxhighlighter = QDir::currentPath();

        //qInfo(logCore()) << "Try detect syntaxhighlighter folder";
        if (!syntaxhighlighter.cd("syntaxhighlighter"))
        {
            //qWarning(logCore()) << QString("Working directory %1: false").arg(syntaxhighlighter.path());
            syntaxhighlighter = QDir(qApp->applicationDirPath());

            if (!syntaxhighlighter.cd("syntaxhighlighter"))
            {
                hr = false;
                //qWarning(logCore()) << QString("Application directory %1: false").arg(syntaxhighlighter.path());
            }
            //else
                //qInfo(logCore()) << QString("Syntaxhighlighter folder: %1").arg(syntaxhighlighter.path());
        }
        else
            //qInfo(logCore()) << QString("Syntaxhighlighter folder: %1").arg(syntaxhighlighter.path());

        if (hr)
        {
            QFileInfoList lst = syntaxhighlighter.entryInfoList(QStringList() << "*.json");

            for (auto fi : lst)
            {
                QString name = fi.baseName();

                try
                {
                    //qInfo(logCore()) << "Try load highlighter style:" << name;

                    QSharedPointer<StyleItem> item(new StyleItem());
                    item->load(fi.absoluteFilePath());
                    m_Styles.insert(name, item);

                    m_Themes.append(name);
                    //qInfo(logCore()) << "Highlighter style successfully loaded";
                }  catch (QJsonParseError e)
                {
                    //qWarning(logCore()) << QString("Can't load highlighter style [%1]: %2")
                                              // .arg(name, e.errorString());
                }
                catch (QString e) {
                    //qWarning(logCore()) << QString("Can't load highlighter style [%1]: %2")
                                              // .arg(name, e);
                }
            }
        }
        else
        {
            //qWarning(logCore()) << QString("Syntaxhighlighter folder not found... Used default scheme...");

            QSharedPointer<StyleItem> item(new StyleItem());
            item->load(":/../codeeditor/Default.json");
            m_Styles.insert("Default", item);

            m_Themes.append("Default");
        }
    }

    QMap<QString, QSharedPointer<StyleItem>> m_Styles;
    QStringList m_Themes;
    QString m_DefaultTheme;
    HighlighterStyle *q_ptr;
};

HighlighterStyle::HighlighterStyle() :
    d_ptr(new HighlighterStylePrivate(this))
{
    Q_D(HighlighterStyle);
    d->loadStyles();
}

HighlighterStyle::~HighlighterStyle()
{

}

HighlighterStyle *HighlighterStyle::inst()
{
    if (!m_Inst)
        m_Inst = new HighlighterStyle();

    return m_Inst;
}

QSharedPointer<StyleItem> HighlighterStyle::style(const QString &name) const
{
    Q_D(const HighlighterStyle);
    if (name.isEmpty())
        return style(d->m_DefaultTheme); // Default

    if (!d->m_Styles.contains(name))
        return nullptr;

    return d->m_Styles[name];
}

QStringList HighlighterStyle::themes() const
{
    Q_D(const HighlighterStyle);
    return d->m_Themes;
}

void HighlighterStyle::setDefaultTheme(const QString &theme)
{
    Q_D(HighlighterStyle);
    d->m_DefaultTheme = theme;
}

QString HighlighterStyle::defaultTheme() const
{
    Q_D(const HighlighterStyle);
    return d->m_DefaultTheme;
}
