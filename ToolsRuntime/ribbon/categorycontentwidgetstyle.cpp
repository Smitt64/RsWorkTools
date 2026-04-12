#include "CategoryContentWidgetStyle.h"
#include <QPainter>
#include <QPainterPath>
#include <QTreeView>
#include <QHeaderView>
#include <QGroupBox>
#include <QtWidgets>
#include <QDebug>

CategoryContentWidgetStyle::CategoryContentWidgetStyle(QStyle *style)
    : QProxyStyle(style)
    , m_baseColor(0x21, 0x73, 0x46)
    , m_borderColor(0xD5D5D5)//0xAB, 0xAB, 0xAB
    , m_textColor(0x33, 0x33, 0x33)
    , m_disabledColor(0xAB, 0xAB, 0xAB)
    , m_backgroundColor(0xF0, 0xF0, 0xF0)
    , m_alternateColor(0xFA, 0xFA, 0xFA)
    , m_groupBoxFlat(true)
    , m_cornerRadius(0)
{
    m_GroupBoxFont.setPointSize(12);
    updateColors();
}

void CategoryContentWidgetStyle::setBaseColor(const QColor &color)
{
    m_baseColor = color;
    updateColors();
}

void CategoryContentWidgetStyle::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
}

void CategoryContentWidgetStyle::setTextColor(const QColor &color)
{
    m_textColor = color;
}

void CategoryContentWidgetStyle::updateColors()
{
    // Цвет при наведении вычисляется относительно базового цвета
    // Для #217346 (33, 115, 70) получается #D3F0E0 (211, 240, 224)
    // Формула: baseColor + (255 - baseColor) * 0.85
    m_hoverColor = QColor(
        m_baseColor.red() + (255 - m_baseColor.red()) * 85 / 100,
        m_baseColor.green() + (255 - m_baseColor.green()) * 85 / 100,
        m_baseColor.blue() + (255 - m_baseColor.blue()) * 85 / 100
        );

    // Цвет при выделении/нажатии - на 15% светлее базового
    m_selectedColor = QColor(
        m_baseColor.red() + (255 - m_baseColor.red()) * 70 / 100,
        m_baseColor.green() + (255 - m_baseColor.green()) * 70 / 100,
        m_baseColor.blue() + (255 - m_baseColor.blue()) * 70 / 100
        );

    // Цвет при нажатии - на 15% темнее базового
    m_pressedColor = QColor(
        m_baseColor.red() * 85 / 100,
        m_baseColor.green() * 85 / 100,
        m_baseColor.blue() * 85 / 100
        );
}

void CategoryContentWidgetStyle::polish(QWidget *widget)
{
    QProxyStyle::polish(widget);

    if (widget->property("_style_applied").toBool())
        return;

    if (qobject_cast<QComboBox*>(widget) ||
        qobject_cast<QPushButton*>(widget) ||
        qobject_cast<QToolButton*>(widget) ||
        qobject_cast<QLineEdit*>(widget) ||
        qobject_cast<QSpinBox*>(widget) ||
        qobject_cast<QCheckBox*>(widget) ||
        qobject_cast<QRadioButton*>(widget) ||
        qobject_cast<QScrollBar*>(widget) ||
        qobject_cast<QGroupBox*>(widget) ||
        qobject_cast<QProgressBar*>(widget) ||
        qobject_cast<QHeaderView*>(widget))
    {
        widget->setAttribute(Qt::WA_StyledBackground, true);
        widget->setMouseTracking(true);

        if (QAbstractButton *button = qobject_cast<QAbstractButton*>(widget))
        {
            button->setAutoFillBackground(false);
        }

        widget->setProperty("_style_applied", true);
    }

    QGroupBox *group = qobject_cast<QGroupBox*>(widget);
    if (group)
        group->setFont(m_GroupBoxFont);

    if (QListWidget *list = qobject_cast<QListWidget*>(widget))
    {
        if (!list->property("_list_configured").toBool())
        {
            list->setAttribute(Qt::WA_StyledBackground, true);
            list->setGridSize(QSize(300, 60));
            list->setUniformItemSizes(true);
            list->setIconSize(QSize(24, 24));
            list->setSpacing(0);

            QPalette palette = list->palette();
            palette.setColor(QPalette::Text, Qt::black);
            palette.setColor(QPalette::Highlight, m_selectedColor);
            palette.setColor(QPalette::HighlightedText, Qt::black);
            list->setPalette(palette);

            list->setProperty("_list_configured", true);
        }
    }

    if (QTreeView *treeView = qobject_cast<QTreeView*>(widget))
    {
        if (!treeView->property("_tree_configured").toBool())
        {
            treeView->setAttribute(Qt::WA_StyledBackground, true);
            //treeView->setIndentation(20);
            //treeView->setRootIsDecorated(true);

            QPalette palette = treeView->palette();
            palette.setColor(QPalette::Text, Qt::black);
            palette.setColor(QPalette::Highlight, m_selectedColor);
            palette.setColor(QPalette::HighlightedText, Qt::black);
            palette.setColor(QPalette::Base, Qt::white);
            palette.setColor(QPalette::Window, Qt::white);
            treeView->setPalette(palette);

            treeView->setProperty("_tree_configured", true);
        }
    }

    if (QComboBox *combo = qobject_cast<QComboBox*>(widget))
    {
        if (!combo->property("_size_configured").toBool())
            combo->setProperty("_size_configured", true);
    }

    if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(widget))
    {
        if (!lineEdit->property("_size_configured").toBool())
        {
            lineEdit->setMinimumHeight(28);
            lineEdit->setMaximumHeight(28);
            lineEdit->setProperty("_size_configured", true);
        }
    }

    if (QSpinBox *spinBox = qobject_cast<QSpinBox*>(widget))
    {
        if (!spinBox->property("_size_configured").toBool())
        {
            spinBox->setMinimumHeight(28);
            spinBox->setMaximumHeight(28);
            spinBox->setProperty("_size_configured", true);
        }
    }

    if (QPushButton *button = qobject_cast<QPushButton*>(widget))
    {
        if (!button->property("_size_configured").toBool())
        {
            button->setMinimumHeight(28);
            button->setMaximumHeight(28);
            button->setAutoFillBackground(false);
            button->setProperty("_size_configured", true);
        }
    }

    if (QToolButton *toolButton = qobject_cast<QToolButton*>(widget))
    {
        if (!toolButton->property("_size_configured").toBool())
        {
            if (toolButton->minimumHeight() <= 0)
            {
                toolButton->setMinimumHeight(28);
            }
            toolButton->setAutoFillBackground(false);
            toolButton->setProperty("_size_configured", true);
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
    QWidget *parentWidget = widget->parentWidget();
    switch (element)
    {
    case PE_FrameLineEdit:
    case PE_PanelLineEdit:
        // Проверяем, находится ли LineEdit внутри редактируемого комбобокса
        if (qobject_cast<const QLineEdit*>(widget))
        {
            // Проверяем, является ли родитель QComboBox и редактируемым
            if (QComboBox *combo = qobject_cast<QComboBox*>(parentWidget))
            {
                if (combo->isEditable())
                {
                    // Не рисуем рамку для LineEdit внутри редактируемого комбобокса
                    return;
                }
            }
        }

        if (qobject_cast<const QLineEdit*>(widget) && !qobject_cast<const QAbstractSpinBox*>(parentWidget))
            drawLineEdit(option, painter, widget);
        return;

    case PE_PanelButtonCommand:
        if (qobject_cast<const QPushButton*>(widget))
        {
            drawPushButton(option, painter, widget);
            return;
        }
        break;

    case PE_FrameTabWidget:
        if (qobject_cast<const QTabWidget*>(widget))
        {
            return;
        }
        break;

    case PE_FrameGroupBox:
        if (qobject_cast<const QGroupBox*>(widget))
        {
            return;
        }
        break;

    case PE_Frame:
        if (qobject_cast<const QScrollArea*>(widget))
        {
            return;
        }
        break;

    default:
        break;
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

    case CE_PushButtonBevel:
        drawPushButton(option, painter, widget);
        return;

    case CE_ItemViewItem:
        if (const QStyleOptionViewItem *viewItem = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            if (widget && qobject_cast<const QTreeView*>(widget))
            {
                drawTreeViewItem(viewItem, painter, widget);
                return;
            }
            drawListItem(*viewItem, painter, widget);
            return;
        }
        break;

    case CE_CheckBox:
        drawCheckBox(option, painter, widget);
        return;

    case CE_RadioButton:
        drawCheckBox(option, painter, widget);
        return;

    case CE_ProgressBar:
        if (const QStyleOptionProgressBar *progressOption = qstyleoption_cast<const QStyleOptionProgressBar*>(option))
        {
            drawProgressBar(progressOption, painter, widget);
            return;
        }
        break;

    case CE_ComboBoxLabel:
        if (const QStyleOptionComboBox *comboOption = qstyleoption_cast<const QStyleOptionComboBox*>(option))
        {
            painter->save();

            QRect rect = comboOption->rect;
            bool isEnabled = (option->state & State_Enabled);
            bool isEditable = comboOption->editable;

            // Отступ для текста (слева 8px, справа с учетом стрелки)
            QRect textRect;
            if (isEditable)
            {
                // Для редактируемого комбобокса текст рисуется внутри QLineEdit
                // Поэтому здесь ничего не рисуем
                painter->restore();
                return;
            }
            else
            {
                textRect = rect.adjusted(8, 0, -28, 0);
            }

            painter->setPen(isEnabled ? m_textColor : m_disabledColor);

            QString text = comboOption->currentText;
            if (text.isEmpty() && !comboOption->currentIcon.isNull())
            {
                // Если есть иконка, рисуем её
                int iconSize = 16;
                QRect iconRect = rect.adjusted(8, (rect.height() - iconSize) / 2,
                                               -rect.width() + iconSize + 16,
                                               -(rect.height() - iconSize) / 2);
                iconRect.setSize(QSize(iconSize, iconSize));
                comboOption->currentIcon.paint(painter, iconRect, Qt::AlignCenter,
                                               isEnabled ? QIcon::Normal : QIcon::Disabled);
            }
            else
            {
                // Рисуем текст с многоточием
                QFontMetrics fm(painter->font());
                QString elidedText = fm.elidedText(text, Qt::ElideRight, textRect.width());
                painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, elidedText);
            }

            painter->restore();
            return;
        }
        break;

    case CE_HeaderSection:
        if (const QStyleOptionHeader *headerOption = qstyleoption_cast<const QStyleOptionHeader*>(option))
        {
            drawHeaderSection(headerOption, painter, widget);
            return;
        }
        break;

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

    case CC_ToolButton:
        if (const QStyleOptionToolButton *toolOption = qstyleoption_cast<const QStyleOptionToolButton*>(option))
        {
            drawToolButton(toolOption, painter, widget);
            return;
        }
        break;

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
    case PM_DefaultFrameWidth:
        return 1;
    case PM_ButtonMargin:
        return 6;
    case PM_ButtonIconSize:
        return 16;
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}

QSize CategoryContentWidgetStyle::sizeFromContents(ContentsType type, const QStyleOption *option,
                                                   const QSize &size, const QWidget *widget) const
{
    QWidget *parentWidget = widget->parentWidget();
    QSize newSize = QProxyStyle::sizeFromContents(type, option, size, widget);

    switch (type)
    {
    case CT_ComboBox:
        if (!parentWidget || !qobject_cast<const QTreeView*>(parentWidget))
            newSize.setHeight(32);
        break;

    case CT_LineEdit:
        if (!parentWidget || !qobject_cast<const QAbstractSpinBox*>(parentWidget))
            newSize.setHeight(qMax(28, newSize.height()));
        break;

    case CT_ItemViewItem:
    case CT_PushButton:
        newSize.setHeight(qMax(28, newSize.height()));
        break;

    case CT_ToolButton:
        newSize.setWidth(qMax(28, newSize.width()));
        newSize.setHeight(qMax(28, newSize.height()));
        break;

    case CT_SpinBox:
        newSize.setHeight(qMax(28, newSize.height()));
        break;

    case CT_CheckBox:
        newSize.setHeight(qMax(24, newSize.height()));
        break;

    case CT_HeaderSection:
        newSize.setHeight(qMax(24, newSize.height()));
        break;

    default:
        break;
    }

    return newSize;
}

QRect CategoryContentWidgetStyle::subElementRect(SubElement element, const QStyleOption *option,
                                                 const QWidget *widget) const
{
    Q_ASSERT(widget);

    QWidget *parentWidget = widget->parentWidget();
    QRect rect = QProxyStyle::subElementRect(element, option, widget);

    switch (element)
    {
    case SE_LineEditContents:
        if (!qobject_cast<const QAbstractSpinBox*>(parentWidget))
            rect.adjust(4, 0, -4, 0);
        break;

    case SE_PushButtonContents:
        rect.adjust(8, 0, -8, 0);
        break;

    case SE_ItemViewItemCheckIndicator:
        // Уменьшаем размер чекбокса, но оставляем его в том же месте
        rect.setSize(QSize(16, 16));
        rect.moveTop(rect.top() + (rect.height() - 16) / 2);
        break;

    default:
        break;
    }

    return rect;
}

QRect CategoryContentWidgetStyle::subControlRect(ComplexControl control, const QStyleOptionComplex *option,
                                                 SubControl subControl, const QWidget *widget) const
{
    QRect rect = QProxyStyle::subControlRect(control, option, subControl, widget);

    if (control == CC_SpinBox && subControl == SC_SpinBoxEditField)
    {
        rect.adjust(4, 0, -20, 0);
    }
    else if (control == CC_ComboBox && subControl == SC_ComboBoxEditField)
    {
        // Увеличиваем область для текста
        rect.adjust(4, 0, -28, 0);
    }
    else if (control == CC_GroupBox && subControl == SC_GroupBoxLabel)
    {
        const QStyleOptionGroupBox *groupOption = qstyleoption_cast<const QStyleOptionGroupBox*>(option);
        QFontMetrics metrics(m_GroupBoxFont);
        QRect metrics_rect = metrics.boundingRect(groupOption->text);

        rect.setWidth(metrics_rect.width());
        rect.setHeight(metrics_rect.height());
    }
    /*else if (control == CC_GroupBox && subControl == SC_GroupBoxContents)
    {
        rect = QProxyStyle::subControlRect(control, option, subControl, widget);
        // Сдвигаем содержимое вниз на ту же величину, на которую увеличили заголовок
        rect.setTop(rect.top() + 10);
    }*/

    return rect;
}

// ==================== Реализация методов отрисовки ====================

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
    bool isOpen = (option->state & State_On);
    bool isEnabled = (option->state & State_Enabled);
    bool hasFocus = (option->state & State_HasFocus);
    bool isEditable = comboOption->editable;

    // Стиль как у кнопки
    QColor bgColor = Qt::transparent;
    QColor borderColor = m_borderColor;

    if (!isEnabled)
    {
        borderColor = QColor(0xE0, 0xE0, 0xE0);
        bgColor = m_backgroundColor;
    }
    else if (isOpen)
    {
        bgColor = m_selectedColor;
        borderColor = m_baseColor;
    }
    else if (isHover)
    {
        bgColor = m_hoverColor;
        borderColor = m_hoverColor.darker(110);
    }
    else if (hasFocus)
    {
        borderColor = m_baseColor;
    }

    // Для редактируемого комбобокса всегда белый фон
    if (isEditable && isEnabled && bgColor == Qt::transparent)
    {
        bgColor = Qt::white;
    }

    // Рисуем фон и рамку
    if (bgColor != Qt::transparent)
    {
        painter->setBrush(bgColor);
    }
    else
    {
        painter->setBrush(Qt::NoBrush);
    }
    painter->setPen(QPen(borderColor, 1));
    painter->drawRect(rect);

    // Рисуем стрелку справа ВСЕГДА (и для редактируемого, и для нередактируемого)
    int arrowWidth = 28;
    QRect arrowRect = rect.adjusted(rect.width() - arrowWidth, 0, 0, 0);

    // Для редактируемого комбобокса рисуем разделительную линию
    if (isEditable && isEnabled)
    {
        painter->setPen(QPen(m_borderColor, 1));
        painter->drawLine(arrowRect.left(), rect.top() + 1,
                          arrowRect.left(), rect.bottom() - 1);
    }

    // Рисуем стрелку
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

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    bool isHover = (option->state & State_MouseOver);
    bool isPressed = (option->state & State_Sunken);
    bool isChecked = (option->state & State_On);
    bool isEnabled = (option->state & State_Enabled);
    bool isDefault = btnOption->features & QStyleOptionButton::DefaultButton;

    QColor bgColor = Qt::transparent;
    QColor borderColor = m_borderColor;

    if (!isEnabled)
    {
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
    else if (isDefault)
    {
        borderColor = m_baseColor;
    }

    if (bgColor != Qt::transparent)
    {
        painter->setBrush(bgColor);
    }
    else
    {
        painter->setBrush(Qt::NoBrush);
    }
    painter->setPen(QPen(borderColor, 1));
    painter->drawRect(rect);

    QRect textRect = rect.adjusted(8, 0, -8, 0);

    if (!btnOption->icon.isNull())
    {
        int iconSize = 16;
        QRect iconRect = rect.adjusted(4, (rect.height() - iconSize) / 2,
                                       -rect.width() + iconSize + 8,
                                       -(rect.height() - iconSize) / 2);
        iconRect.setSize(QSize(iconSize, iconSize));
        btnOption->icon.paint(painter, iconRect, Qt::AlignCenter,
                              isEnabled ? QIcon::Normal : QIcon::Disabled);

        textRect = rect.adjusted(iconSize + 12, 0, -8, 0);
    }

    painter->setPen(isEnabled ? m_textColor : m_disabledColor);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, btnOption->text);

    painter->restore();
}

void CategoryContentWidgetStyle::drawLineEdit(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    bool isHover = (option->state & State_MouseOver);
    bool hasFocus = (option->state & State_HasFocus);
    bool isEnabled = (option->state & State_Enabled);

    painter->setBrush(isEnabled ? Qt::white : m_backgroundColor);

    QColor borderColor = m_borderColor;
    if (hasFocus)
        borderColor = m_baseColor;
    else if (isHover)
        borderColor = m_hoverColor.darker(120);

    painter->setPen(QPen(borderColor, 1));
    painter->drawRect(rect);

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

    int buttonWidth = 20;
    QRect buttonsRect = rect.adjusted(rect.width() - buttonWidth, 0, 0, 0);
    QRect editRect = rect.adjusted(0, 0, -buttonWidth, 0);

    QColor borderColor = m_borderColor;
    if (hasFocus)
        borderColor = m_baseColor;
    else if (isHover)
        borderColor = m_hoverColor.darker(120);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(borderColor, 1));
    painter->drawRect(rect);

    painter->setBrush(isEnabled ? Qt::white : m_backgroundColor);
    painter->setPen(Qt::NoPen);
    painter->drawRect(editRect.adjusted(1, 1, -1, -1));

    QRect upRect = buttonsRect.adjusted(0, 0, 0, -buttonsRect.height() / 2);
    QRect downRect = buttonsRect.adjusted(0, buttonsRect.height() / 2, 0, 0);

    bool upHover = (spinOption->activeSubControls == SC_SpinBoxUp);
    bool upPressed = upHover && (option->state & State_Sunken);
    bool downHover = (spinOption->activeSubControls == SC_SpinBoxDown);
    bool downPressed = downHover && (option->state & State_Sunken);

    QColor upBgColor = Qt::transparent;
    if (upPressed)
        upBgColor = m_selectedColor;
    else if (upHover)
        upBgColor = m_hoverColor;

    if (upBgColor != Qt::transparent)
    {
        painter->setBrush(upBgColor);
        painter->setPen(QPen(m_borderColor, 1));
        painter->drawRect(upRect);
    }

    QColor downBgColor = Qt::transparent;
    if (downPressed)
        downBgColor = m_selectedColor;
    else if (downHover)
        downBgColor = m_hoverColor;

    if (downBgColor != Qt::transparent)
    {
        painter->setBrush(downBgColor);
        painter->setPen(QPen(m_borderColor, 1));
        painter->drawRect(downRect);
    }

    QPoint upCenter = upRect.center();
    QPoint downCenter = downRect.center();

    painter->setPen(Qt::NoPen);
    painter->setBrush(isEnabled ? m_textColor : m_disabledColor);

    QPolygon upArrow;
    upArrow << QPoint(upCenter.x() - 3, upCenter.y() + 1)
            << QPoint(upCenter.x() + 3, upCenter.y() + 1)
            << QPoint(upCenter.x(), upCenter.y() - 2);
    painter->drawPolygon(upArrow);

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

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    bool isChecked = (option->state & State_On);
    bool isHover = (option->state & State_MouseOver);
    bool isEnabled = (option->state & State_Enabled);

    QRect indicatorRect = rect.adjusted(0, (rect.height() - 18) / 2, -rect.width() + 20, -(rect.height() - 18) / 2);
    indicatorRect.setSize(QSize(18, 18));

    QColor bgColor = isChecked ? m_baseColor : Qt::white;
    QColor borderColor = m_borderColor;

    if (isHover && !isChecked)
        borderColor = m_baseColor;

    painter->setBrush(bgColor);

    QRectF rectF(indicatorRect.x() + 0.5, indicatorRect.y() + 0.5,
                 indicatorRect.width() - 1, indicatorRect.height() - 1);
    painter->setPen(QPen(borderColor, 1));
    painter->drawRect(rectF);

    if (isChecked)
    {
        painter->setPen(QPen(Qt::white, 2));
        // Уменьшенная галочка
        painter->drawLine(indicatorRect.x() + 5, indicatorRect.y() + 9,
                          indicatorRect.x() + 8, indicatorRect.y() + 12);
        painter->drawLine(indicatorRect.x() + 8, indicatorRect.y() + 12,
                          indicatorRect.x() + 13, indicatorRect.y() + 5);
    }

    QRect textRect = rect.adjusted(24, 0, 0, 0);
    painter->setPen(isEnabled ? m_textColor : m_disabledColor);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, btnOption->text);

    painter->restore();
}

void CategoryContentWidgetStyle::drawScrollBar(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    bool isHorizontal = (option->state & State_Horizontal);

    QRect handleRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget);

    painter->setBrush(m_backgroundColor);
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect);

    painter->setBrush(QColor(0xC0, 0xC0, 0xC0));
    painter->setPen(Qt::NoPen);
    painter->drawRect(handleRect);

    painter->restore();
}

void CategoryContentWidgetStyle::drawListItem(const QStyleOptionViewItem &option, QPainter *painter, const QWidget *widget) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option.rect;
    bool isSelected = option.state & QStyle::State_Selected;
    bool isHover = option.state & QStyle::State_MouseOver;
    bool isEnabled = option.state & QStyle::State_Enabled;

    QColor bgColor;
    if (!isEnabled)
        bgColor = Qt::transparent;
    else if (isSelected && isHover)
        bgColor = m_selectedColor.lighter(105);
    else if (isSelected)
        bgColor = m_selectedColor;
    else if (isHover)
        bgColor = m_hoverColor;
    else
        bgColor = Qt::transparent;

    if (bgColor != Qt::transparent)
    {
        painter->setBrush(bgColor);
        painter->setPen(Qt::NoPen);
        painter->drawRect(rect);
    }

    int margin = 12;
    int iconSize = option.widget ? option.widget->style()->pixelMetric(QStyle::PM_ListViewIconSize, &option, option.widget) : 24;

    // Рисуем чекбокс если есть
    if (option.features & QStyleOptionViewItem::HasCheckIndicator)
    {
        QRect checkRect = subElementRect(SE_ItemViewItemCheckIndicator, &option, widget);
        bool isChecked = (option.checkState == Qt::Checked);
        bool isHoverCheck = (option.state & State_MouseOver);

        QColor bgColorCheck;
        QColor borderColor;

        if (!isEnabled)
        {
            // Неактивное состояние - серые тона
            bgColorCheck = isChecked ? QColor(0xCC, 0xCC, 0xCC) : QColor(0xF0, 0xF0, 0xF0);
            borderColor = QColor(0xCC, 0xCC, 0xCC);
        }
        else
        {
            // Активное состояние
            bgColorCheck = isChecked ? m_baseColor : Qt::white;
            borderColor = m_borderColor;

            if (isHoverCheck && !isChecked)
                borderColor = m_baseColor;
        }

        painter->setBrush(bgColorCheck);
        painter->setPen(QPen(borderColor, 1));

        QRectF rectF(checkRect.x() + 0.5, checkRect.y() + 0.5,
                     checkRect.width() - 1, checkRect.height() - 1);
        painter->drawRect(rectF);

        if (isChecked)
        {
            if (!isEnabled)
            {
                // Неактивная галочка - серая
                painter->setPen(QPen(QColor(0x99, 0x99, 0x99), 2));
            }
            else
            {
                // Активная галочка - белая
                painter->setPen(QPen(Qt::white, 2));
            }
            painter->drawLine(checkRect.x() + 4, checkRect.y() + 8,
                              checkRect.x() + 7, checkRect.y() + 11);
            painter->drawLine(checkRect.x() + 7, checkRect.y() + 11,
                              checkRect.x() + 12, checkRect.y() + 5);
        }

        margin = checkRect.right() + 8;
    }

    if (!option.icon.isNull())
    {
        QRect iconRect = rect.adjusted(margin, (rect.height() - iconSize) / 2,
                                       -rect.width() + iconSize + margin,
                                       -(rect.height() - iconSize) / 2);
        iconRect.setSize(QSize(iconSize, iconSize));

        QIcon::Mode mode = isEnabled ? QIcon::Normal : QIcon::Disabled;
        QIcon::State state = isSelected ? QIcon::On : QIcon::Off;
        option.icon.paint(painter, iconRect, Qt::AlignCenter, mode, state);

        margin = iconRect.right() + 8;
    }

    QRect textRect = rect.adjusted(margin, 0, -margin, 0);
    painter->setPen(isEnabled ? m_textColor : m_disabledColor);

    QFont font = option.font;
    painter->setFont(font);

    QFontMetrics fm(font);
    QString elidedText = fm.elidedText(option.text, Qt::ElideRight, textRect.width());
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, elidedText);

    painter->restore();
}

void CategoryContentWidgetStyle::drawTreeViewItem(const QStyleOptionViewItem *option, QPainter *painter, const QWidget *widget) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    bool isSelected = option->state & QStyle::State_Selected;
    bool isHover = option->state & QStyle::State_MouseOver;
    bool isEnabled = option->state & QStyle::State_Enabled;

    QColor bgColor;
    if (!isEnabled)
        bgColor = Qt::transparent;
    else if (isSelected && isHover)
        bgColor = m_selectedColor.lighter(105);
    else if (isSelected)
        bgColor = m_selectedColor;
    else if (isHover)
        bgColor = m_hoverColor;
    else
        bgColor = Qt::transparent;

    if (bgColor != Qt::transparent)
    {
        painter->setBrush(bgColor);
        painter->setPen(Qt::NoPen);
        painter->drawRect(rect);
    }

    // Проверяем, есть ли чекбокс или иконка
    bool hasCheckBox = (option->features & QStyleOptionViewItem::HasCheckIndicator);
    bool hasIcon = !option->icon.isNull();

    int textLeft = rect.x() + 4;  // Базовый отступ 4 пикселя

    // Чекбокс и иконка только если они есть
    if (hasCheckBox || hasIcon)
    {
        if (hasCheckBox)
        {
            QRect checkRect = subElementRect(SE_ItemViewItemCheckIndicator, option, widget);
            bool isChecked = (option->checkState == Qt::Checked);
            bool isHoverCheck = (option->state & State_MouseOver);

            QColor bgColorCheck;
            QColor borderColor;

            if (!isEnabled)
            {
                bgColorCheck = isChecked ? QColor(0xCC, 0xCC, 0xCC) : QColor(0xF0, 0xF0, 0xF0);
                borderColor = QColor(0xCC, 0xCC, 0xCC);
            }
            else
            {
                bgColorCheck = isChecked ? m_baseColor : Qt::white;
                borderColor = m_borderColor;

                if (isHoverCheck && !isChecked)
                    borderColor = m_baseColor;
            }

            painter->setBrush(bgColorCheck);
            painter->setPen(QPen(borderColor, 1));

            QRectF rectF(checkRect.x() + 0.5, checkRect.y() + 0.5,
                         checkRect.width() - 1, checkRect.height() - 1);
            painter->drawRect(rectF);

            if (isChecked)
            {
                if (!isEnabled)
                {
                    painter->setPen(QPen(QColor(0x99, 0x99, 0x99), 2));
                }
                else
                {
                    painter->setPen(QPen(Qt::white, 2));
                }
                painter->drawLine(checkRect.x() + 4, checkRect.y() + 8,
                                  checkRect.x() + 7, checkRect.y() + 11);
                painter->drawLine(checkRect.x() + 7, checkRect.y() + 11,
                                  checkRect.x() + 12, checkRect.y() + 5);
            }

            textLeft = checkRect.right() + 4;
        }

        if (hasIcon)
        {
            int iconSize = 24;
            QRect iconRect(textLeft, rect.y() + (rect.height() - iconSize) / 2, iconSize, iconSize);

            QIcon::Mode mode = isEnabled ? QIcon::Normal : QIcon::Disabled;
            QIcon::State state = isSelected ? QIcon::On : QIcon::Off;
            option->icon.paint(painter, iconRect, Qt::AlignCenter, mode, state);

            textLeft = iconRect.right() + 4;
        }
    }

    // Рисуем текст (если есть текст)
    if (!option->text.isEmpty())
    {
        bool drawText = true;
        const QTreeView *pView = qobject_cast<const QTreeView*>(widget);

        if (pView)
            drawText = !pView->isPersistentEditorOpen(option->index);

        if (drawText)
        {
            QRect textRect(textLeft, rect.y(), rect.right() - textLeft - 4, rect.height());
            painter->setPen(isEnabled ? m_textColor : m_disabledColor);

            QFont font = option->font;
            painter->setFont(font);

            QFontMetrics fm(font);
            QString elidedText = fm.elidedText(option->text, Qt::ElideRight, textRect.width());
            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, elidedText);
        }
    }

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
    //QRect frameRect = subControlRect(CC_GroupBox, option, SC_GroupBoxFrame, widget);
    bool isEnabled = (option->state & State_Enabled);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(0xE0, 0xE0, 0xE0), 1));

    painter->setPen(isEnabled ? m_baseColor : m_disabledColor);
    painter->setFont(m_GroupBoxFont);
    painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, groupOption->text);

    painter->restore();
}

void CategoryContentWidgetStyle::drawProgressBar(const QStyleOptionProgressBar *option, QPainter *painter, const QWidget *widget) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    int progress = option->progress;
    int maximum = option->maximum;

    if (maximum <= 0)
        maximum = 100;

    int progressWidth = (rect.width() * progress) / maximum;

    painter->setBrush(m_backgroundColor);
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawRect(rect);

    if (progress > 0)
    {
        QRect progressRect = rect.adjusted(1, 1, -1, -1);
        progressRect.setWidth(progressWidth - 2);

        painter->setBrush(m_baseColor);
        painter->setPen(Qt::NoPen);
        painter->drawRect(progressRect);
    }

    if (option->textVisible && !option->text.isEmpty())
    {
        painter->setPen(Qt::black);
        painter->drawText(rect, Qt::AlignCenter, option->text);
    }

    painter->restore();
}

void CategoryContentWidgetStyle::drawHeaderSection(const QStyleOptionHeader *option, QPainter *painter, const QWidget *widget) const
{
    painter->save();

    QRect rect = option->rect;
    bool isLastSection = (option->position == QStyleOptionHeader::End);
    bool isOnlyOneSection = (option->position == QStyleOptionHeader::OnlyOneSection);
    bool isFirstSection = (option->position == QStyleOptionHeader::Beginning);

    // Фон заголовка
    painter->setBrush(m_backgroundColor);
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect);

    // Нижняя граница (сплошная)
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(rect.left(), rect.bottom() - 1, rect.right(), rect.bottom() - 1);

    // Вертикальный разделитель справа с градиентом (между секциями)
    if (!isLastSection && !isOnlyOneSection)
    {
        int height = rect.height();

        // Создаем градиент от прозрачного сверху до непрозрачного снизу
        QLinearGradient gradient(rect.right(), rect.top(),
                                 rect.right(), rect.bottom());
        gradient.setColorAt(0.0, QColor(0, 0, 0, 0));      // Прозрачный сверху
        gradient.setColorAt(0.3, QColor(0, 0, 0, 40));     // Легкое появление
        gradient.setColorAt(0.7, QColor(0, 0, 0, 80));     // Средняя прозрачность
        gradient.setColorAt(1.0, m_borderColor);           // Полностью непрозрачный снизу

        painter->setPen(QPen(QBrush(gradient), 1));
        painter->drawLine(rect.right(), rect.top(), rect.right() - 1, rect.bottom() - 2);
    }

    painter->restore();
}

void CategoryContentWidgetStyle::drawToolButton(const QStyleOptionToolButton *option, QPainter *painter, const QWidget *widget) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option->rect;
    bool isHover = (option->state & State_MouseOver);
    bool isPressed = (option->state & State_Sunken);
    bool isChecked = (option->state & State_On);
    bool isEnabled = (option->state & State_Enabled);

    QColor bgColor = Qt::transparent;
    QColor borderColor = Qt::transparent;

    if (!isEnabled)
    {
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
        borderColor = m_borderColor;
    }

    if (bgColor != Qt::transparent)
    {
        painter->setBrush(bgColor);
    }
    else
    {
        painter->setBrush(Qt::NoBrush);
    }
    painter->setPen(QPen(borderColor, 1));
    painter->drawRect(rect);

    int iconSize = option->iconSize.width();
    if (iconSize <= 0)
        iconSize = 32;

    int spacing = 8;
    QFontMetrics fm(painter->font());
    int textHeight = fm.height();

    if (!option->icon.isNull())
    {
        QRect iconRect;

        if (option->toolButtonStyle == Qt::ToolButtonTextUnderIcon)
        {
            int totalHeight = iconSize + spacing + textHeight;
            int yOffset = (rect.height() - totalHeight) / 2;

            iconRect = QRect(
                rect.x() + (rect.width() - iconSize) / 2,
                rect.y() + yOffset,
                iconSize,
                iconSize
                );
        }
        else if (option->toolButtonStyle == Qt::ToolButtonTextBesideIcon)
        {
            iconRect = rect.adjusted(8, (rect.height() - iconSize) / 2,
                                     -rect.width() + iconSize + 16,
                                     -(rect.height() - iconSize) / 2);
            iconRect.setSize(QSize(iconSize, iconSize));
        }
        else
        {
            iconRect = QRect(
                rect.x() + (rect.width() - iconSize) / 2,
                rect.y() + (rect.height() - iconSize) / 2,
                iconSize,
                iconSize
                );
        }

        option->icon.paint(painter, iconRect, Qt::AlignCenter,
                           isEnabled ? QIcon::Normal : QIcon::Disabled,
                           isChecked ? QIcon::On : QIcon::Off);
    }

    if (option->toolButtonStyle != Qt::ToolButtonIconOnly && !option->text.isEmpty())
    {
        QRect textRect;

        if (option->toolButtonStyle == Qt::ToolButtonTextUnderIcon)
        {
            int totalHeight = iconSize + spacing + textHeight;
            int yOffset = (rect.height() - totalHeight) / 2;

            textRect = QRect(
                rect.x() + 4,
                rect.y() + yOffset + iconSize + spacing,
                rect.width() - 8,
                textHeight
                );

            painter->setPen(isEnabled ? m_textColor : m_disabledColor);
            QString elidedText = fm.elidedText(option->text, Qt::ElideRight, textRect.width());
            painter->drawText(textRect, Qt::AlignCenter, elidedText);
        }
        else if (option->toolButtonStyle == Qt::ToolButtonTextBesideIcon)
        {
            textRect = rect.adjusted(iconSize + 16, 0, -8, 0);
            painter->setPen(isEnabled ? m_textColor : m_disabledColor);
            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, option->text);
        }
        else if (option->toolButtonStyle == Qt::ToolButtonTextOnly)
        {
            textRect = rect.adjusted(8, 0, -8, 0);
            painter->setPen(isEnabled ? m_textColor : m_disabledColor);
            painter->drawText(textRect, Qt::AlignCenter, option->text);
        }
    }

    painter->restore();
}