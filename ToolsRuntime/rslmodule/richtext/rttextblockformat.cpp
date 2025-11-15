#include "RTTextBlockFormat.h"
#include <QTextCharFormat>

RTTextBlockFormat::RTTextBlockFormat(RTTextBlockFormat *parent)
    : RTTextCharFormat(parent)
    , m_blockFormat()
{
}

RTTextBlockFormat::RTTextBlockFormat(const QTextBlockFormat &format, QObject *parent)
    : RTTextCharFormat(format.toCharFormat())
    , m_blockFormat(format)
{
}

RTTextBlockFormat::RTTextBlockFormat(const RTTextBlockFormat &other)
    : RTTextCharFormat(other)
    , m_blockFormat(other.m_blockFormat)
{
}

RTTextBlockFormat::~RTTextBlockFormat()
{
}

RTTextBlockFormat &RTTextBlockFormat::operator=(const RTTextBlockFormat &other)
{
    if (this != &other) {
        RTTextCharFormat::operator=(other);
        m_blockFormat = other.m_blockFormat;
        emit blockFormatChanged();
    }
    return *this;
}

QTextBlockFormat RTTextBlockFormat::textBlockFormat() const
{
    return m_blockFormat;
}

void RTTextBlockFormat::setTextBlockFormat(const QTextBlockFormat &format)
{
    m_blockFormat = format;

    setTextCharFormat(format.toCharFormat()); // Обновляем также базовый формат
    emit blockFormatChanged();
}

// Геттеры
int RTTextBlockFormat::alignment() const { return static_cast<int>(m_blockFormat.alignment()); }
qreal RTTextBlockFormat::bottomMargin() const { return m_blockFormat.bottomMargin(); }
int RTTextBlockFormat::headingLevel() const { return m_blockFormat.headingLevel(); }
int RTTextBlockFormat::indent() const { return m_blockFormat.indent(); }
qreal RTTextBlockFormat::leftMargin() const { return m_blockFormat.leftMargin(); }
qreal RTTextBlockFormat::lineHeight() const { return m_blockFormat.lineHeight(); }
qreal RTTextBlockFormat::lineHeight(qreal scriptLineHeight, qreal scaling) const { return m_blockFormat.lineHeight(scriptLineHeight, scaling); }
int RTTextBlockFormat::lineHeightType() const { return static_cast<int>(m_blockFormat.lineHeightType()); }
int RTTextBlockFormat::marker() const { return static_cast<int>(m_blockFormat.marker()); }
bool RTTextBlockFormat::nonBreakableLines() const { return m_blockFormat.nonBreakableLines(); }
int RTTextBlockFormat::pageBreakPolicy() const { return static_cast<int>(m_blockFormat.pageBreakPolicy()); }
qreal RTTextBlockFormat::rightMargin() const { return m_blockFormat.rightMargin(); }
QVariantList RTTextBlockFormat::tabPositions() const
{
    QVariantList result;
    QList<QTextOption::Tab> tabs = m_blockFormat.tabPositions();
    for (const QTextOption::Tab &tab : tabs) {
        QVariantMap tabMap;
        tabMap["position"] = tab.position;
        tabMap["type"] = static_cast<int>(tab.type);
        tabMap["delimiter"] = tab.delimiter;
        result.append(tabMap);
    }
    return result;
}
qreal RTTextBlockFormat::textIndent() const { return m_blockFormat.textIndent(); }
qreal RTTextBlockFormat::topMargin() const { return m_blockFormat.topMargin(); }

// Сеттеры
void RTTextBlockFormat::setAlignment(int alignment) { m_blockFormat.setAlignment(static_cast<Qt::Alignment>(alignment)); emit blockFormatChanged(); }
void RTTextBlockFormat::setBottomMargin(qreal margin) { m_blockFormat.setBottomMargin(margin); emit blockFormatChanged(); }
void RTTextBlockFormat::setHeadingLevel(int level) { m_blockFormat.setHeadingLevel(level); emit blockFormatChanged(); }
void RTTextBlockFormat::setIndent(int indentation) { m_blockFormat.setIndent(indentation); emit blockFormatChanged(); }
void RTTextBlockFormat::setLeftMargin(qreal margin) { m_blockFormat.setLeftMargin(margin); emit blockFormatChanged(); }
void RTTextBlockFormat::setLineHeight(qreal height, int heightType) { m_blockFormat.setLineHeight(height, heightType); emit blockFormatChanged(); }
void RTTextBlockFormat::setMarker(int marker) { m_blockFormat.setMarker(static_cast<QTextBlockFormat::MarkerType>(marker)); emit blockFormatChanged(); }
void RTTextBlockFormat::setNonBreakableLines(bool b) { m_blockFormat.setNonBreakableLines(b); emit blockFormatChanged(); }
void RTTextBlockFormat::setPageBreakPolicy(int policy) { m_blockFormat.setPageBreakPolicy(static_cast<QTextFormat::PageBreakFlags>(policy)); emit blockFormatChanged(); }
void RTTextBlockFormat::setRightMargin(qreal margin) { m_blockFormat.setRightMargin(margin); emit blockFormatChanged(); }
void RTTextBlockFormat::setTabPositions(const QVariantList &tabs)
{
    QList<QTextOption::Tab> tabList;
    for (const QVariant &tabVar : tabs) {
        if (tabVar.canConvert<QVariantMap>()) {
            QVariantMap tabMap = tabVar.toMap();
            QTextOption::Tab tab;
            tab.position = tabMap.value("position", 0).toDouble();
            tab.type = static_cast<QTextOption::TabType>(tabMap.value("type", 0).toInt());
            tab.delimiter = tabMap.value("delimiter", QChar()).toChar();
            tabList.append(tab);
        }
    }
    m_blockFormat.setTabPositions(tabList);
    emit blockFormatChanged();
}
void RTTextBlockFormat::setTextIndent(qreal indent) { m_blockFormat.setTextIndent(indent); emit blockFormatChanged(); }
void RTTextBlockFormat::setTopMargin(qreal margin) { m_blockFormat.setTopMargin(margin); emit blockFormatChanged(); }
