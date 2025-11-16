#ifndef RTTEXTCHARFORMAT_H
#define RTTEXTCHARFORMAT_H

#include <QTextCharFormat>
#include <QObject>
#include <QFont>
#include <QPen>
#include <QColor>
#include "ToolsRuntime_global.h"

class TOOLSRUNTIME_EXPORT RTTextCharFormat : public QObject
{
    Q_OBJECT

    // Свойства
    Q_PROPERTY(QString anchorHref READ anchorHref WRITE setAnchorHref NOTIFY formatChanged)
    Q_PROPERTY(QStringList anchorNames READ anchorNames WRITE setAnchorNames NOTIFY formatChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY formatChanged)
    Q_PROPERTY(int fontCapitalization READ fontCapitalization WRITE setFontCapitalization NOTIFY formatChanged)
    Q_PROPERTY(QStringList fontFamilies READ fontFamilies WRITE setFontFamilies NOTIFY formatChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY formatChanged)
    Q_PROPERTY(bool fontFixedPitch READ fontFixedPitch WRITE setFontFixedPitch NOTIFY formatChanged)
    Q_PROPERTY(int fontHintingPreference READ fontHintingPreference WRITE setFontHintingPreference NOTIFY formatChanged)
    Q_PROPERTY(bool fontItalic READ fontItalic WRITE setFontItalic NOTIFY formatChanged)
    Q_PROPERTY(bool fontKerning READ fontKerning WRITE setFontKerning NOTIFY formatChanged)
    Q_PROPERTY(qreal fontLetterSpacing READ fontLetterSpacing WRITE setFontLetterSpacing NOTIFY formatChanged)
    Q_PROPERTY(int fontLetterSpacingType READ fontLetterSpacingType WRITE setFontLetterSpacingType NOTIFY formatChanged)
    Q_PROPERTY(bool fontOverline READ fontOverline WRITE setFontOverline NOTIFY formatChanged)
    Q_PROPERTY(qreal fontPointSize READ fontPointSize WRITE setFontPointSize NOTIFY formatChanged)
    Q_PROPERTY(int fontStretch READ fontStretch WRITE setFontStretch NOTIFY formatChanged)
    Q_PROPERTY(bool fontStrikeOut READ fontStrikeOut WRITE setFontStrikeOut NOTIFY formatChanged)
    Q_PROPERTY(int fontStyleHint READ fontStyleHint WRITE setFontStyleHint NOTIFY formatChanged)
    Q_PROPERTY(QString fontStyleName READ fontStyleName WRITE setFontStyleName NOTIFY formatChanged)
    Q_PROPERTY(int fontStyleStrategy READ fontStyleStrategy WRITE setFontStyleStrategy NOTIFY formatChanged)
    Q_PROPERTY(bool fontUnderline READ fontUnderline WRITE setFontUnderline NOTIFY formatChanged)
    Q_PROPERTY(int fontWeight READ fontWeight WRITE setFontWeight NOTIFY formatChanged)
    Q_PROPERTY(qreal fontWordSpacing READ fontWordSpacing WRITE setFontWordSpacing NOTIFY formatChanged)
    Q_PROPERTY(bool isAnchor READ isAnchor WRITE setAnchor NOTIFY formatChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY formatChanged)
    Q_PROPERTY(QPen textOutline READ textOutline WRITE setTextOutline NOTIFY formatChanged)
    Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip NOTIFY formatChanged)
    Q_PROPERTY(QColor underlineColor READ underlineColor WRITE setUnderlineColor NOTIFY formatChanged)
    Q_PROPERTY(int underlineStyle READ underlineStyle WRITE setUnderlineStyle NOTIFY formatChanged)
    Q_PROPERTY(int verticalAlignment READ verticalAlignment WRITE setVerticalAlignment NOTIFY formatChanged)

public:
    Q_INVOKABLE RTTextCharFormat(RTTextCharFormat *parent = nullptr);
    RTTextCharFormat(const QTextCharFormat &format, QObject *parent = nullptr);
    Q_INVOKABLE RTTextCharFormat(const RTTextCharFormat &other);
    ~RTTextCharFormat();

    // Оператор присваивания
    RTTextCharFormat &operator=(const RTTextCharFormat &other);

    // Методы доступа к внутреннему формату
    QTextCharFormat textCharFormat() const;
    void setTextCharFormat(const QTextCharFormat &format);

    // Свойства-геттеры
    QString anchorHref() const;
    QStringList anchorNames() const;
    QFont font() const;
    int fontCapitalization() const;
    QStringList fontFamilies() const;
    QString fontFamily() const;
    bool fontFixedPitch() const;
    int fontHintingPreference() const;
    bool fontItalic() const;
    bool fontKerning() const;
    qreal fontLetterSpacing() const;
    int fontLetterSpacingType() const;
    bool fontOverline() const;
    qreal fontPointSize() const;
    int fontStretch() const;
    bool fontStrikeOut() const;
    int fontStyleHint() const;
    QString fontStyleName() const;
    int fontStyleStrategy() const;
    bool fontUnderline() const;
    int fontWeight() const;
    qreal fontWordSpacing() const;
    bool isAnchor() const;
    bool isValid() const;
    QPen textOutline() const;
    QString toolTip() const;
    QColor underlineColor() const;
    int underlineStyle() const;
    int verticalAlignment() const;

    // Свойства-сеттеры
    void setAnchor(bool anchor);
    void setAnchorHref(const QString &value);
    void setAnchorNames(const QStringList &names);
    void setFont(const QFont &font);
    void setFontCapitalization(int capitalization);
    void setFontFamilies(const QStringList &families);
    void setFontFamily(const QString &family);
    void setFontFixedPitch(bool fixedPitch);
    void setFontHintingPreference(int hintingPreference);
    void setFontItalic(bool italic);
    void setFontKerning(bool enable);
    void setFontLetterSpacing(qreal spacing);
    void setFontLetterSpacingType(int letterSpacingType);
    void setFontOverline(bool overline);
    void setFontPointSize(qreal size);
    void setFontStretch(int factor);
    void setFontStrikeOut(bool strikeOut);
    void setFontStyleHint(int hint, int strategy = 0); // QFont::PreferDefault
    void setFontStyleName(const QString &styleName);
    void setFontStyleStrategy(int strategy);
    void setFontUnderline(bool underline);
    void setFontWeight(int weight);
    void setFontWordSpacing(qreal spacing);
    void setTextOutline(const QPen &pen);
    void setToolTip(const QString &text);
    void setUnderlineColor(const QColor &color);
    void setUnderlineStyle(int style);
    void setVerticalAlignment(int alignment);

signals:
    void formatChanged();

private:
    QTextCharFormat m_format;
};

#endif // RTTEXTCHARFORMAT_H
