#ifndef CATEGORYCONTENTWIDGETSTYLE_H
#define CATEGORYCONTENTWIDGETSTYLE_H

#include <QProxyStyle>
#include <QStyleOption>
#include <QPainter>
#include <QColor>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QScrollBar>
#include <QTabWidget>
#include <QGroupBox>
#include <QLabel>
#include <QScrollArea>
#include <QTreeView>
#include <QHeaderView>

class CategoryContentWidgetStyle : public QProxyStyle
{
public:
    explicit CategoryContentWidgetStyle(QStyle *style = nullptr);

    void setBaseColor(const QColor &color);
    QColor baseColor() const { return m_baseColor; }

    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const { return m_backgroundColor; }

    void setTextColor(const QColor &color);
    QColor textColor() const { return m_textColor; }

    void polish(QWidget *widget) override;
    void unpolish(QWidget *widget) override;

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = nullptr) const override;

    void drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget = nullptr) const override;

    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget = nullptr) const override;

    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr,
                    const QWidget *widget = nullptr) const override;

    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const override;

    QRect subElementRect(SubElement element, const QStyleOption *option,
                         const QWidget *widget = nullptr) const override;

    QRect subControlRect(ComplexControl control, const QStyleOptionComplex *option,
                         SubControl subControl, const QWidget *widget = nullptr) const override;

private:
    QColor m_baseColor;
    QColor m_hoverColor;
    QColor m_selectedColor;
    QColor m_pressedColor;
    QColor m_borderColor;
    QColor m_textColor;
    QColor m_disabledColor;
    QColor m_backgroundColor;
    QColor m_alternateColor;
    bool m_groupBoxFlat;
    int m_cornerRadius;

    QFont m_GroupBoxFont;

    void updateColors();

    // Методы отрисовки
    void drawComboBox(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;
    void drawPushButton(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    void drawToolButton(const QStyleOptionToolButton *option, QPainter *painter, const QWidget *widget) const;
    void drawLineEdit(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    void drawSpinBox(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;
    void drawCheckBox(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    void drawScrollBar(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;
    void drawListItem(const QStyleOptionViewItem &option, QPainter *painter, const QWidget *widget) const;
    void drawTreeViewItem(const QStyleOptionViewItem *option, QPainter *painter, const QWidget *widget) const;
    void drawGroupBox(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;
    void drawProgressBar(const QStyleOptionProgressBar *option, QPainter *painter, const QWidget *widget) const;
    void drawHeaderSection(const QStyleOptionHeader *option, QPainter *painter, const QWidget *widget) const;
};

#endif // CATEGORYCONTENTWIDGETSTYLE_H