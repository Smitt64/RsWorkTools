#include "RTTextCharFormat.h"

RTTextCharFormat::RTTextCharFormat(RTTextCharFormat *parent)
    : QObject()
    , m_format(parent->textCharFormat())
{
}

RTTextCharFormat::RTTextCharFormat(const QTextCharFormat &format, QObject *parent)
    : QObject(parent)
    , m_format(format)
{
}

RTTextCharFormat::RTTextCharFormat(const RTTextCharFormat &other)
    : QObject(other.parent())
    , m_format(other.m_format)
{
}

RTTextCharFormat::~RTTextCharFormat()
{
}

RTTextCharFormat &RTTextCharFormat::operator=(const RTTextCharFormat &other)
{
    if (this != &other) {
        m_format = other.m_format;
        emit formatChanged();
    }
    return *this;
}

QTextCharFormat RTTextCharFormat::textCharFormat() const
{
    return m_format;
}

void RTTextCharFormat::setTextCharFormat(const QTextCharFormat &format)
{
    m_format = format;
    emit formatChanged();
}

// Геттеры
QString RTTextCharFormat::anchorHref() const { return m_format.anchorHref(); }
QStringList RTTextCharFormat::anchorNames() const { return m_format.anchorNames(); }
QFont RTTextCharFormat::font() const { return m_format.font(); }
int RTTextCharFormat::fontCapitalization() const { return static_cast<int>(m_format.fontCapitalization()); }
QStringList RTTextCharFormat::fontFamilies() const { return m_format.fontFamilies().toStringList(); }
QString RTTextCharFormat::fontFamily() const { return m_format.fontFamily(); }
bool RTTextCharFormat::fontFixedPitch() const { return m_format.fontFixedPitch(); }
int RTTextCharFormat::fontHintingPreference() const { return static_cast<int>(m_format.fontHintingPreference()); }
bool RTTextCharFormat::fontItalic() const { return m_format.fontItalic(); }
bool RTTextCharFormat::fontKerning() const { return m_format.fontKerning(); }
qreal RTTextCharFormat::fontLetterSpacing() const { return m_format.fontLetterSpacing(); }
int RTTextCharFormat::fontLetterSpacingType() const { return static_cast<int>(m_format.fontLetterSpacingType()); }
bool RTTextCharFormat::fontOverline() const { return m_format.fontOverline(); }
qreal RTTextCharFormat::fontPointSize() const { return m_format.fontPointSize(); }
int RTTextCharFormat::fontStretch() const { return m_format.fontStretch(); }
bool RTTextCharFormat::fontStrikeOut() const { return m_format.fontStrikeOut(); }
int RTTextCharFormat::fontStyleHint() const { return static_cast<int>(m_format.fontStyleHint()); }
QString RTTextCharFormat::fontStyleName() const { return m_format.fontStyleName().toString(); }
int RTTextCharFormat::fontStyleStrategy() const { return static_cast<int>(m_format.fontStyleStrategy()); }
bool RTTextCharFormat::fontUnderline() const { return m_format.fontUnderline(); }
int RTTextCharFormat::fontWeight() const { return m_format.fontWeight(); }
qreal RTTextCharFormat::fontWordSpacing() const { return m_format.fontWordSpacing(); }
bool RTTextCharFormat::isAnchor() const { return m_format.isAnchor(); }
bool RTTextCharFormat::isValid() const { return m_format.isValid(); }
QPen RTTextCharFormat::textOutline() const { return m_format.textOutline(); }
QString RTTextCharFormat::toolTip() const { return m_format.toolTip(); }
QColor RTTextCharFormat::underlineColor() const { return m_format.underlineColor(); }
int RTTextCharFormat::underlineStyle() const { return static_cast<int>(m_format.underlineStyle()); }
int RTTextCharFormat::verticalAlignment() const { return static_cast<int>(m_format.verticalAlignment()); }

// Сеттеры
void RTTextCharFormat::setAnchor(bool anchor) { m_format.setAnchor(anchor); emit formatChanged(); }
void RTTextCharFormat::setAnchorHref(const QString &value) { m_format.setAnchorHref(value); emit formatChanged(); }
void RTTextCharFormat::setAnchorNames(const QStringList &names) { m_format.setAnchorNames(names); emit formatChanged(); }
void RTTextCharFormat::setFont(const QFont &font) { m_format.setFont(font); emit formatChanged(); }
void RTTextCharFormat::setFontCapitalization(int capitalization) { m_format.setFontCapitalization(static_cast<QFont::Capitalization>(capitalization)); emit formatChanged(); }
void RTTextCharFormat::setFontFamilies(const QStringList &families) { m_format.setFontFamilies(families); emit formatChanged(); }
void RTTextCharFormat::setFontFamily(const QString &family) { m_format.setFontFamily(family); emit formatChanged(); }
void RTTextCharFormat::setFontFixedPitch(bool fixedPitch) { m_format.setFontFixedPitch(fixedPitch); emit formatChanged(); }
void RTTextCharFormat::setFontHintingPreference(int hintingPreference) { m_format.setFontHintingPreference(static_cast<QFont::HintingPreference>(hintingPreference)); emit formatChanged(); }
void RTTextCharFormat::setFontItalic(bool italic) { m_format.setFontItalic(italic); emit formatChanged(); }
void RTTextCharFormat::setFontKerning(bool enable) { m_format.setFontKerning(enable); emit formatChanged(); }
void RTTextCharFormat::setFontLetterSpacing(qreal spacing) { m_format.setFontLetterSpacing(spacing); emit formatChanged(); }
void RTTextCharFormat::setFontLetterSpacingType(int letterSpacingType) { m_format.setFontLetterSpacingType(static_cast<QFont::SpacingType>(letterSpacingType)); emit formatChanged(); }
void RTTextCharFormat::setFontOverline(bool overline) { m_format.setFontOverline(overline); emit formatChanged(); }
void RTTextCharFormat::setFontPointSize(qreal size) { m_format.setFontPointSize(size); emit formatChanged(); }
void RTTextCharFormat::setFontStretch(int factor) { m_format.setFontStretch(factor); emit formatChanged(); }
void RTTextCharFormat::setFontStrikeOut(bool strikeOut) { m_format.setFontStrikeOut(strikeOut); emit formatChanged(); }
void RTTextCharFormat::setFontStyleHint(int hint, int strategy) { m_format.setFontStyleHint(static_cast<QFont::StyleHint>(hint), static_cast<QFont::StyleStrategy>(strategy)); emit formatChanged(); }
void RTTextCharFormat::setFontStyleName(const QString &styleName) { m_format.setFontStyleName(styleName); emit formatChanged(); }
void RTTextCharFormat::setFontStyleStrategy(int strategy) { m_format.setFontStyleStrategy(static_cast<QFont::StyleStrategy>(strategy)); emit formatChanged(); }
void RTTextCharFormat::setFontUnderline(bool underline) { m_format.setFontUnderline(underline); emit formatChanged(); }
void RTTextCharFormat::setFontWeight(int weight) { m_format.setFontWeight(weight); emit formatChanged(); }
void RTTextCharFormat::setFontWordSpacing(qreal spacing) { m_format.setFontWordSpacing(spacing); emit formatChanged(); }
void RTTextCharFormat::setTextOutline(const QPen &pen) { m_format.setTextOutline(pen); emit formatChanged(); }
void RTTextCharFormat::setToolTip(const QString &text) { m_format.setToolTip(text); emit formatChanged(); }
void RTTextCharFormat::setUnderlineColor(const QColor &color) { m_format.setUnderlineColor(color); emit formatChanged(); }
void RTTextCharFormat::setUnderlineStyle(int style) { m_format.setUnderlineStyle(static_cast<QTextCharFormat::UnderlineStyle>(style)); emit formatChanged(); }
void RTTextCharFormat::setVerticalAlignment(int alignment) { m_format.setVerticalAlignment(static_cast<QTextCharFormat::VerticalAlignment>(alignment)); emit formatChanged(); }
