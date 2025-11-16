#ifndef RTTEXTBLOCKFORMAT_H
#define RTTEXTBLOCKFORMAT_H

#include "RTTextCharFormat.h"
#include <QTextBlockFormat>
#include <QTextOption>
#include "ToolsRuntime_global.h"

class TOOLSRUNTIME_EXPORT RTTextBlockFormat : public RTTextCharFormat
{
    Q_OBJECT

    // Свойства
    Q_PROPERTY(int alignment READ alignment WRITE setAlignment NOTIFY blockFormatChanged)
    Q_PROPERTY(qreal bottomMargin READ bottomMargin WRITE setBottomMargin NOTIFY blockFormatChanged)
    Q_PROPERTY(int headingLevel READ headingLevel WRITE setHeadingLevel NOTIFY blockFormatChanged)
    Q_PROPERTY(int indent READ indent WRITE setIndent NOTIFY blockFormatChanged)
    Q_PROPERTY(qreal leftMargin READ leftMargin WRITE setLeftMargin NOTIFY blockFormatChanged)
    Q_PROPERTY(qreal lineHeight READ lineHeight NOTIFY blockFormatChanged)
    Q_PROPERTY(int lineHeightType READ lineHeightType NOTIFY blockFormatChanged)
    Q_PROPERTY(int marker READ marker WRITE setMarker NOTIFY blockFormatChanged)
    Q_PROPERTY(bool nonBreakableLines READ nonBreakableLines WRITE setNonBreakableLines NOTIFY blockFormatChanged)
    Q_PROPERTY(int pageBreakPolicy READ pageBreakPolicy WRITE setPageBreakPolicy NOTIFY blockFormatChanged)
    Q_PROPERTY(qreal rightMargin READ rightMargin WRITE setRightMargin NOTIFY blockFormatChanged)
    Q_PROPERTY(QVariantList tabPositions READ tabPositions WRITE setTabPositions NOTIFY blockFormatChanged)
    Q_PROPERTY(qreal textIndent READ textIndent WRITE setTextIndent NOTIFY blockFormatChanged)
    Q_PROPERTY(qreal topMargin READ topMargin WRITE setTopMargin NOTIFY blockFormatChanged)

public:
    explicit RTTextBlockFormat(RTTextBlockFormat *parent = nullptr);
    RTTextBlockFormat(const QTextBlockFormat &format, QObject *parent = nullptr);
    RTTextBlockFormat(const RTTextBlockFormat &other);
    ~RTTextBlockFormat();

    // Оператор присваивания
    RTTextBlockFormat &operator=(const RTTextBlockFormat &other);

    // Методы доступа к внутреннему формату
    QTextBlockFormat textBlockFormat() const;
    void setTextBlockFormat(const QTextBlockFormat &format);

    // Свойства-геттеры
    int alignment() const;
    qreal bottomMargin() const;
    int headingLevel() const;
    int indent() const;
    qreal leftMargin() const;
    qreal lineHeight() const;
    qreal lineHeight(qreal scriptLineHeight, qreal scaling = 1.0) const;
    int lineHeightType() const;
    int marker() const;
    bool nonBreakableLines() const;
    int pageBreakPolicy() const;
    qreal rightMargin() const;
    QVariantList tabPositions() const;
    qreal textIndent() const;
    qreal topMargin() const;

    // Свойства-сеттеры
    void setAlignment(int alignment);
    void setBottomMargin(qreal margin);
    void setHeadingLevel(int level);
    void setIndent(int indentation);
    void setLeftMargin(qreal margin);
    void setLineHeight(qreal height, int heightType);
    void setMarker(int marker);
    void setNonBreakableLines(bool b);
    void setPageBreakPolicy(int policy);
    void setRightMargin(qreal margin);
    void setTabPositions(const QVariantList &tabs);
    void setTextIndent(qreal indent);
    void setTopMargin(qreal margin);

signals:
    void blockFormatChanged();

private:
    QTextBlockFormat m_blockFormat;
};

#endif // RTTEXTBLOCKFORMAT_H
