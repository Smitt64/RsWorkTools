#include "CategoryContentWidgetStyle.h"
#include <QPainter>
#include <QPainterPath>
#include <QTreeView>
#include <QDebug>

CategoryContentWidgetStyle::CategoryContentWidgetStyle(QStyle *style)
    : QProxyStyle(style)
    , m_baseColor(0x21, 0x73, 0x46)
    , m_borderColor(0xAB, 0xAB, 0xAB)
    , m_textColor(0x33, 0x33, 0x33)
    , m_disabledColor(0xAB, 0xAB, 0xAB)
    , m_backgroundColor(0xF5, 0xF5, 0xF5)
    ,m_groupBoxFlat(true)
{
    updateColors();
}

void CategoryContentWidgetStyle::setBaseColor(const QColor &color)
{
    m_baseColor = color;
    updateColors();
}

void CategoryContentWidgetStyle::updateColors()
{
    // Цвета как в списке
    m_hoverColor = QColor(
        m_baseColor.red() + (255 - m_baseColor.red()) * 85 / 100,
        m_baseColor.green() + (255 - m_baseColor.green()) * 85 / 100,
        m_baseColor.blue() + (255 - m_baseColor.blue()) * 85 / 100
        ); // 85% белого

    m_selectedColor = QColor(
        m_baseColor.red() + (255 - m_baseColor.red()) * 70 / 100,
        m_baseColor.green() + (255 - m_baseColor.green()) * 70 / 100,
        m_baseColor.blue() + (255 - m_baseColor.blue()) * 70 / 100
        ); // 70% белого

    m_pressedColor = QColor(
        m_baseColor.red() * 80 / 100,
        m_baseColor.green() * 80 / 100,
        m_baseColor.blue() * 80 / 100
        ); // 20% черного
}

void CategoryContentWidgetStyle::polish(QWidget *widget)
{
    QProxyStyle::polish(widget);

    // Устанавливаем атрибуты для всех виджетов, которые будем отрисовывать сами
    if (qobject_cast<QComboBox*>(widget) ||
        qobject_cast<QPushButton*>(widget) ||
        qobject_cast<QLineEdit*>(widget) ||
        qobject_cast<QSpinBox*>(widget) ||
        qobject_cast<QCheckBox*>(widget) ||
        qobject_cast<QListWidget*>(widget) ||
        qobject_cast<QTreeView*>(widget) ||
        qobject_cast<QScrollBar*>(widget))
    {
        widget->setAttribute(Qt::WA_StyledBackground, true);
        widget->setMouseTracking(true);
    }

    if (QPushButton *button = qobject_cast<QPushButton*>(widget))
    {
        button->setAttribute(Qt::WA_StyledBackground, true);
        button->setMouseTracking(true);
    }

    // Устанавливаем фиксированную высоту для QComboBox
    if (QComboBox *combo = qobject_cast<QComboBox*>(widget))
    {
        combo->setMinimumHeight(40);
        combo->setMaximumHeight(40);
    }

    // Устанавливаем параметры для QListWidget
    if (QListWidget *list = qobject_cast<QListWidget*>(widget))
    {
        list->setGridSize(QSize(300, 60));
        list->setUniformItemSizes(true);
        list->setIconSize(QSize(24, 24));
        list->setSpacing(0);
        list->setViewMode(QListView::ListMode);
        list->setMovement(QListView::Static);
        list->setResizeMode(QListView::Fixed);
    }

    // Устанавливаем параметры для QLineEdit
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(widget))
    {
        lineEdit->setMinimumHeight(22);
    }

    // Для QSpinBox отключаем отрисовку внутреннего QLineEdit
    if (QSpinBox *spinBox = qobject_cast<QSpinBox*>(widget))
    {
        spinBox->setMinimumHeight(22);
        // Находим внутренний QLineEdit и отключаем его рамку
        QList<QLineEdit*> lineEdits = spinBox->findChildren<QLineEdit*>();
        for (QLineEdit *lineEdit : lineEdits)
        {
            lineEdit->setFrame(false);
            lineEdit->setAttribute(Qt::WA_StyledBackground, false);
        }
    }
}

void CategoryContentWidgetStyle::unpolish(QWidget *widget)
{
    QProxyStyle::unpolish(widget);
}

void CategoryContentWidgetStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                                               QPainter *painter, const QWidget *widget) const
{
    // Проверяем, является ли виджет QLineEdit
    bool isLineEdit = qobject_cast<const QLineEdit*>(widget) != nullptr;
    bool isSpinBox = qobject_cast<const QAbstractSpinBox*>(widget) != nullptr;

    // Для полей ввода (только QLineEdit, не QSpinBox) перехватываем примитивы
    if (isLineEdit && !isSpinBox &&
        (element == PE_FrameLineEdit ||
         element == PE_Frame ||
         element == PE_PanelLineEdit))
    {
        drawLineEdit(option, painter, widget);
        return;
    }

    // Для кнопок
    if (element == PE_PanelButtonCommand && qobject_cast<const QPushButton*>(widget))
    {
        drawPushButton(option, painter, widget);
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void CategoryContentWidgetStyle::drawControl(ControlElement element, const QStyleOption *option,
                                             QPainter *painter, const QWidget *widget) const
{
    switch (element)
    {
    case CE_PushButton:
        drawPushButton(option, painter, widget);
        return;

    case CE_ItemViewItem:
        if (const QStyleOptionViewItem *viewItem = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            drawListItem(*viewItem, painter, widget);
            return;
        }
        break;

    case CE_ComboBoxLabel:
        // Пропускаем, текст будет отрисован в drawComplexControl
        return;

    default:
        break;
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}

void CategoryContentWidgetStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                                                    QPainter *painter, const QWidget *widget) const
{
    switch (control)
    {
    case CC_ComboBox:
        drawComboBox(option, painter, widget);
        return;

    case CC_SpinBox:
        drawSpinBox(option, painter, widget);
        return;

    case CC_ScrollBar:
        drawScrollBar(option, painter, widget);
        return;

    case CC_GroupBox:
        drawGroupBox(option, painter, widget);
        return;

    default:
        break;
    }

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

int CategoryContentWidgetStyle::pixelMetric(PixelMetric metric, const QStyleOption *option,
                                            const QWidget *widget) const
{
    switch (metric)
    {
    case PM_ScrollBarExtent:
        return 12;
    case PM_ListViewIconSize:
        return 24;
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}

QSize CategoryContentWidgetStyle::sizeFromContents(ContentsType type, const QStyleOption *option,
                                                   const QSize &size, const QWidget *widget) const
{
    QSize newSize = QProxyStyle::sizeFromContents(type, option, size, widget);

    switch (type)
    {
    case CT_ComboBox:
        newSize.setHeight(40);
        break;

    case CT_LineEdit:
        newSize.setHeight(qMax(22, newSize.height()));
        break;

    case CT_PushButton:
        newSize.setHeight(qMax(22, newSize.height()));
        break;

    case CT_SpinBox:
        newSize.setHeight(qMax(23, newSize.height()));
        break;

    default:
        break;
    }

    return newSize;
}

void CategoryContentWidgetStyle::drawComboBox(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionComboBox *comboOption = qstyleoption_cast<const QStyleOptionComboBox*>(option);
    if (!comboOption)
    {
        QProxyStyle::drawComplexControl(CC_ComboBox, option, painter, widget);
        return;
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    bool isHover = (option->state & State_MouseOver);
    bool isPressed = (option->state & State_Sunken);
    bool isOpen = (option->state & State_On);
    bool isEnabled = (option->state & State_Enabled);

    // Определяем цвет фона
    QColor bgColor;
    if (!isEnabled)
        bgColor = m_backgroundColor;
    else if (isPressed || isOpen)
        bgColor = m_selectedColor;
    else if (isHover)
        bgColor = m_hoverColor;
    else
        bgColor = QColor(0xF5F5F5);

    // Рисуем фон с правильной границей
    painter->setBrush(bgColor);
    painter->setPen(QPen(isEnabled ? m_borderColor : QColor(0xE0, 0xE0, 0xE0), 1));
    painter->drawRect(rect);

    // Рисуем текст
    QRect textRect = rect.adjusted(8, 5, -25, -5);
    QString text = comboOption->currentText;

    painter->setPen(isEnabled ? m_textColor : m_disabledColor);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

    // Рисуем стрелку
    QRect arrowRect = rect.adjusted(rect.width() - 25, 0, -8, 0);
    QPoint center = arrowRect.center();

    painter->setPen(Qt::NoPen);
    painter->setBrush(isEnabled ? QColor(0x66, 0x66, 0x66) : m_disabledColor);

    QPolygon arrow;
    arrow << QPoint(center.x() - 4, center.y() - 2)
          << QPoint(center.x() + 4, center.y() - 2)
          << QPoint(center.x(), center.y() + 3);
    painter->drawPolygon(arrow);

    painter->restore();
}

void CategoryContentWidgetStyle::drawPushButton(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionButton *btnOption = qstyleoption_cast<const QStyleOptionButton*>(option);
    if (!btnOption)
    {
        QProxyStyle::drawControl(CE_PushButton, option, painter, widget);
        return;
    }

    Q_UNUSED(widget);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    bool isHover = (option->state & State_MouseOver);
    bool isPressed = (option->state & State_Sunken);
    bool isChecked = (option->state & State_On);
    bool isEnabled = (option->state & State_Enabled);

    // Определяем цвет фона в стиле SARibbon
    QColor bgColor;
    QColor borderColor = m_borderColor;

    if (!isEnabled)
    {
        bgColor = m_backgroundColor;
        borderColor = QColor(0xE0, 0xE0, 0xE0);
    }
    else if (isPressed || isChecked)
    {
        bgColor = m_selectedColor;
        borderColor = m_baseColor;
    }
    else if (isHover)
    {
        bgColor = m_hoverColor;
        borderColor = m_hoverColor.darker(110);
    }
    else
    {
        bgColor = Qt::transparent;
        borderColor = m_borderColor;
    }

    // Рисуем фон (прозрачный в обычном состоянии, как у QAction)
    if (bgColor != Qt::transparent)
    {
        painter->setBrush(bgColor);
        painter->setPen(QPen(borderColor, 1));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
    }
    else
    {
        // В обычном состоянии рисуем только серую рамку
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(borderColor, 1));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
    }

    // Рисуем текст
    QRect textRect = rect.adjusted(8, 0, -8, 0);
    painter->setPen(isEnabled ? m_textColor : m_disabledColor);

    // Если есть иконка, рисуем её
    if (!btnOption->icon.isNull())
    {
        int iconSize = 16;
        QRect iconRect = rect.adjusted(4, (rect.height() - iconSize) / 2, -rect.width() + iconSize + 8, -(rect.height() - iconSize) / 2);
        iconRect.setSize(QSize(iconSize, iconSize));
        btnOption->icon.paint(painter, iconRect, Qt::AlignCenter, isEnabled ? QIcon::Normal : QIcon::Disabled, QIcon::On);

        // Сдвигаем текст вправо, если есть иконка
        textRect = rect.adjusted(iconSize + 12, 0, -8, 0);
    }

    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, btnOption->text);

    painter->restore();
}

void CategoryContentWidgetStyle::drawLineEdit(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    Q_UNUSED(widget);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    bool isHover = (option->state & State_MouseOver);
    bool hasFocus = (option->state & State_HasFocus);
    bool isEnabled = (option->state & State_Enabled);

    // Рисуем фон (такой же как у комбобокса в обычном состоянии)
    painter->setBrush(isEnabled ? Qt::white : m_backgroundColor);

    // Рисуем границу
    if (hasFocus)
        painter->setPen(QPen(m_baseColor, 1));
    else if (isHover)
        painter->setPen(QPen(m_hoverColor, 1));
    else
        painter->setPen(QPen(m_borderColor, 1));

    painter->drawRect(rect.adjusted(1,1,-1,-1));

    painter->restore();
}

void CategoryContentWidgetStyle::drawSpinBox(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionSpinBox *spinOption = qstyleoption_cast<const QStyleOptionSpinBox*>(option);
    if (!spinOption)
    {
        QProxyStyle::drawComplexControl(CC_SpinBox, option, painter, widget);
        return;
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    bool isHover = (option->state & State_MouseOver);
    bool hasFocus = (option->state & State_HasFocus);
    bool isEnabled = (option->state & State_Enabled);

    // Определяем область для кнопок (правая часть)
    int buttonWidth = 16;
    QRect buttonsRect = rect.adjusted(rect.width() - buttonWidth, 0, 0, 0);
    buttonsRect.setWidth(buttonWidth);

    // Область для поля ввода (левая часть)
    QRect editRect = rect.adjusted(0, 0, 0, 0);

    // Рисуем внешнюю рамку вокруг всего виджета
    /*if (hasFocus)
        painter->setPen(QPen(m_baseColor, 1));
    else if (isHover)
        painter->setPen(QPen(m_hoverColor, 1));
    else
        painter->setPen(QPen(m_borderColor, 1));

    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
*/
    // Рисуем фон для поля ввода (белый)
    /*painter->setBrush(isEnabled ? Qt::white : m_backgroundColor);
    painter->setPen(Qt::NoPen);
    painter->drawRect(editRect.adjusted(1, 1, -1, -1));*/

    // Рисуем фон для области кнопок (серый)
    /*painter->setBrush(m_backgroundColor);
    painter->setPen(Qt::NoPen);
    painter->drawRect(buttonsRect.adjusted(1, 1, -1, -1));

    // Рисуем разделительную линию между полем и кнопками
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(buttonsRect.left(), rect.top() + 1, buttonsRect.left(), rect.bottom() - 1);*/

    // Верхняя кнопка
    QRect upRect = buttonsRect.adjusted(0, 0, 0, -buttonsRect.height() / 2);
    upRect.setHeight(buttonsRect.height() / 2);

    // Нижняя кнопка
    QRect downRect = buttonsRect.adjusted(0, buttonsRect.height() / 2, 0, 0);
    downRect.setHeight(buttonsRect.height() / 2);

    // Определяем состояния кнопок
    bool upHover = (spinOption->activeSubControls == SC_SpinBoxUp);
    bool upPressed = (spinOption->activeSubControls == SC_SpinBoxUp) && (option->state & State_Sunken);
    bool downHover = (spinOption->activeSubControls == SC_SpinBoxDown);
    bool downPressed = (spinOption->activeSubControls == SC_SpinBoxDown) && (option->state & State_Sunken);

    // Верхняя кнопка (плоская)
    QColor upBgColor;
    if (!isEnabled)
        upBgColor = Qt::transparent;
    else if (upPressed)
        upBgColor = m_selectedColor;
    else if (upHover)
        upBgColor = m_hoverColor;
    else
        upBgColor = Qt::transparent;

    if (upBgColor != Qt::transparent)
    {
        painter->setBrush(upBgColor);
        painter->setPen(Qt::NoPen);
        painter->drawRect(upRect.adjusted(1, 1, -1, -1));
    }

    // Нижняя кнопка (плоская)
    QColor downBgColor;
    if (!isEnabled)
        downBgColor = Qt::transparent;
    else if (downPressed)
        downBgColor = m_selectedColor;
    else if (downHover)
        downBgColor = m_hoverColor;
    else
        downBgColor = Qt::transparent;

    if (downBgColor != Qt::transparent)
    {
        painter->setBrush(downBgColor);
        painter->setPen(Qt::NoPen);
        painter->drawRect(downRect.adjusted(1, 1, -1, -1));
    }

    // Рисуем стрелки
    QPoint upCenter = upRect.center();
    QPoint downCenter = downRect.center();

    painter->setPen(Qt::NoPen);
    painter->setBrush(isEnabled ? m_textColor : m_disabledColor);

    // Стрелка вверх
    QPolygon upArrow;
    upArrow << QPoint(upCenter.x() - 3, upCenter.y() + 1)
            << QPoint(upCenter.x() + 3, upCenter.y() + 1)
            << QPoint(upCenter.x(), upCenter.y() - 2);
    painter->drawPolygon(upArrow);

    // Стрелка вниз
    QPolygon downArrow;
    downArrow << QPoint(downCenter.x() - 3, downCenter.y() - 1)
              << QPoint(downCenter.x() + 3, downCenter.y() - 1)
              << QPoint(downCenter.x(), downCenter.y() + 2);
    painter->drawPolygon(downArrow);

    painter->restore();
}

void CategoryContentWidgetStyle::drawCheckBox(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionButton *btnOption = qstyleoption_cast<const QStyleOptionButton*>(option);
    if (!btnOption)
    {
        QProxyStyle::drawControl(CE_CheckBox, option, painter, widget);
        return;
    }

    Q_UNUSED(widget);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    bool isChecked = (option->state & State_On);
    bool isHover = (option->state & State_MouseOver);
    bool isEnabled = (option->state & State_Enabled);

    QRect indicatorRect = rect.adjusted(0, 0, -rect.width() + 20, 0);
    indicatorRect.setSize(QSize(16, 16));

    // Рисуем индикатор
    painter->setBrush(isChecked ? m_baseColor : Qt::white);
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawRect(indicatorRect);

    if (isChecked)
    {
        // Рисуем галочку
        painter->setPen(QPen(Qt::white, 2));
        painter->drawLine(indicatorRect.x() + 3, indicatorRect.y() + 8,
                          indicatorRect.x() + 7, indicatorRect.y() + 12);
        painter->drawLine(indicatorRect.x() + 7, indicatorRect.y() + 12,
                          indicatorRect.x() + 13, indicatorRect.y() + 4);
    }

    // Рисуем текст
    QRect textRect = rect.adjusted(24, 0, 0, 0);
    painter->setPen(isEnabled ? m_textColor : m_disabledColor);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, btnOption->text);

    painter->restore();
}

void CategoryContentWidgetStyle::drawScrollBar(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    painter->save();

    QRect rect = option->rect;
    QRect handleRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget);

    // Фон
    painter->setBrush(m_backgroundColor);
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect);

    // Ползунок
    painter->setBrush(QColor(0xC0, 0xC0, 0xC0));
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(handleRect, 6, 6);

    painter->restore();
}

void CategoryContentWidgetStyle::drawListItem(const QStyleOptionViewItem &option, QPainter *painter, const QWidget *widget) const
{
    Q_UNUSED(widget);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option.rect;
    bool isSelected = option.state & QStyle::State_Selected;
    bool isHover = option.state & QStyle::State_MouseOver;
    bool isEnabled = option.state & QStyle::State_Enabled;

    // Определяем, является ли виджет QTreeView
    bool isTreeView = qobject_cast<const QTreeView*>(widget) != nullptr;

    // Для QTreeView определяем уровень вложенности
    int indent = 0;
    if (isTreeView)
    {
        // Получаем уровень вложенности из данных (если доступно)
        // В стандартном QStyleOptionViewItem нет прямого доступа к уровню,
        // поэтому используем стандартный отступ от стиля
        indent = option.rect.left() - rect.left();
        if (indent < 0) indent = 0;
    }

    // Определяем цвет фона
    QColor bgColor;
    if (!isEnabled)
        bgColor = Qt::transparent;
    else if (isSelected && isHover)
        bgColor = m_selectedColor.lighter(110);
    else if (isSelected)
        bgColor = m_selectedColor;
    else if (isHover)
        bgColor = m_hoverColor;
    else
        bgColor = Qt::transparent;

    // Рисуем фон элемента (без рамки)
    if (bgColor != Qt::transparent)
    {
        painter->setBrush(bgColor);
        painter->setPen(Qt::NoPen);
        painter->drawRect(rect);
    }

    // Отступы для иконки и текста
    int margin = 10;

    // Для QTreeView добавляем отступ для уровней вложенности
    int levelIndent = indent;

    // Рисуем иконку если есть
    if (!option.icon.isNull())
    {
        int iconLeft = margin + levelIndent;
        QRect iconRect = rect.adjusted(iconLeft, (rect.height() - 24) / 2,
                                       -rect.width() + 24 + iconLeft,
                                       -(rect.height() - 24) / 2);
        iconRect.setSize(QSize(24, 24));
        option.icon.paint(painter, iconRect, Qt::AlignCenter, QIcon::Normal, QIcon::On);
    }

    // Рисуем текст
    int textLeft = margin;
    if (!option.icon.isNull())
    {
        textLeft = margin + 24 + margin;
    }
    textLeft += levelIndent;

    QRect textRect = rect.adjusted(textLeft, 0, -margin, 0);
    painter->setPen(isEnabled ? m_textColor : m_disabledColor);

    QFont font = option.font;
    painter->setFont(font);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, option.text);

    painter->restore();
}

void CategoryContentWidgetStyle::drawGroupBox(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionGroupBox *groupOption = qstyleoption_cast<const QStyleOptionGroupBox*>(option);
    if (!groupOption)
    {
        QProxyStyle::drawComplexControl(CC_GroupBox, option, painter, widget);
        return;
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect titleRect = subControlRect(CC_GroupBox, option, SC_GroupBoxLabel, widget);
    QRect frameRect = subControlRect(CC_GroupBox, option, SC_GroupBoxFrame, widget);
    bool isEnabled = (option->state & State_Enabled);
    bool isFlat = m_groupBoxFlat;

    if (isFlat)
    {
        // Office 2013/2016 стиль - минималистичный
        painter->setBrush(Qt::NoBrush);

        // Рисуем только верхнюю и нижнюю линии (как в Excel)
        QPen linePen(QColor(0xE0, 0xE0, 0xE0), 1);
        painter->setPen(linePen);

        // Верхняя линия (с отступом для заголовка)
        int titleWidth = titleRect.width();
        if (!groupOption->text.isEmpty() && titleWidth > 0)
        {
            // Линия слева от заголовка
            painter->drawLine(frameRect.left(), frameRect.top() + 5,
                              titleRect.left() - 5, frameRect.top() + 5);
            // Линия справа от заголовка
            painter->drawLine(titleRect.right() + 5, frameRect.top() + 5,
                              frameRect.right(), frameRect.top() + 5);
        }
        else
        {
            // Сплошная линия
            painter->drawLine(frameRect.left(), frameRect.top() + 5,
                              frameRect.right(), frameRect.top() + 5);
        }

        // Нижняя линия
        painter->drawLine(frameRect.left(), frameRect.bottom() - 1,
                          frameRect.right(), frameRect.bottom() - 1);

        // Рисуем заголовок
        painter->setPen(QPen(m_baseColor, 1));
        QFont titleFont = painter->font();
        titleFont.setBold(false);
        painter->setFont(titleFont);
        painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, groupOption->text);

        // Если группа отключена
        if (!isEnabled)
        {
            painter->setPen(QPen(m_disabledColor, 1));
            painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, groupOption->text);
        }
    }
    else
    {
        // Классический стиль с рамкой
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QColor(0xE0, 0xE0, 0xE0), 1));
        painter->drawRoundedRect(frameRect.adjusted(0, 5, 0, 0), 2, 2);

        // Рисуем заголовок
        painter->setPen(m_baseColor);
        painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, groupOption->text);

        if (!isEnabled)
        {
            painter->setPen(QPen(m_disabledColor, 1));
            painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, groupOption->text);
        }
    }

    painter->restore();
}