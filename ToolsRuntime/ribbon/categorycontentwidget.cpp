#include "CategoryContentWidget.h"
#include "CategoryContentWidgetStyle.h"
#include <QStyleOption>
#include <QToolButton>
#include <QPainter>
#include <QDebug>
#include <QFrame>
#include <QScrollBar>
#include <QEvent>
#include <QApplication>
#include <QGroupBox>

CategoryContentWidget::CategoryContentWidget(QWidget *parent)
    : QWidget(parent)
    , m_layoutType(LayoutGroups)
    , m_readOnly(false)
    , m_scrollingEnabled(true)
    , m_listContainer(nullptr)
    , m_listContainerLayout(nullptr)
    , m_listView(nullptr)
    , m_detailContainer(nullptr)
    , m_detailWidget(nullptr)
    , m_groupsContainer(nullptr)
    , m_groupsLayout(nullptr)
    , m_groupsColumns(2)
    , m_groupSpacing(20)
    , m_mainWidget(nullptr)
    , m_leftPanelWidget(nullptr)
    , m_leftPanelContainer(nullptr)
    , m_leftPanelWidth(300)
    , m_leftPanelVisible(true)
    , m_customStyle(nullptr)
    , m_showDetailHeader(true)
    , m_detailHeaderIconSize(24, 24)
    , m_detailHeaderWidget(nullptr)
    , m_detailHeaderIconLabel(nullptr)
    , m_detailHeaderTitleLabel(nullptr)
{
    setupUI();

    // Создаем кастомный стиль только для этого виджета
    m_customStyle = new CategoryContentWidgetStyle(QApplication::style());

    // Применяем стиль к этому виджету и всем его дочерним
    setStyle(m_customStyle);
    applyStyleRecursively(this);
}

CategoryContentWidget::~CategoryContentWidget()
{
    clearGroups();
    delete m_customStyle;
}

void CategoryContentWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setStyleSheet("QScrollArea { border: none; background-color: #F5F5F5; }");
    m_scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_contentWidget = new QWidget();
    m_contentWidget->setStyleSheet("background-color: #F5F5F5;");
    m_contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_contentLayout = new QVBoxLayout(m_contentWidget);
    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(0);

    m_scrollArea->setWidget(m_contentWidget);
    m_mainLayout->addWidget(m_scrollArea, 1);

    updateLayout();
}

void CategoryContentWidget::applyStyleToWidget(QWidget *widget)
{
    if (!widget)
        return;

    // Применяем стиль к текущему виджету
    widget->setStyle(m_customStyle);
    widget->setAttribute(Qt::WA_StyledBackground, true);
    widget->setMouseTracking(true);

    // Для кнопок дополнительно
    if (QPushButton *button = qobject_cast<QPushButton*>(widget))
    {
        button->setAttribute(Qt::WA_StyledBackground, true);
        button->setMouseTracking(true);
    }

    // Рекурсивно применяем ко всем дочерним виджетам
    for (QObject *child : widget->children())
    {
        if (QWidget *childWidget = qobject_cast<QWidget*>(child))
        {
            applyStyleToWidget(childWidget);
        }
    }
}

void CategoryContentWidget::updateLayout()
{
    // Очищаем текущий контент
    clearLayout(m_contentLayout);

    switch (m_layoutType)
    {
    case LayoutListDetail:
    {
        // Создаем горизонтальный layout для списка и деталей
        QHBoxLayout *horizontalLayout = new QHBoxLayout();
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setSpacing(0);

        // Левая часть - список (фиксированная ширина 300px)
        m_listContainer = new QWidget();
        m_listContainer->setFixedWidth(300);
        m_listContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

        // Создаем вертикальный layout для левой панели
        m_listContainerLayout = new QVBoxLayout(m_listContainer);
        m_listContainerLayout->setContentsMargins(0, 0, 0, 0);
        m_listContainerLayout->setSpacing(0);

        // Добавляем виджеты, которые были добавлены перед списком
        for (QWidget *widget : m_widgetsBeforeList)
        {
            if (widget && widget->parent() != m_listContainer)
            {
                widget->setParent(m_listContainer);
            }
            // Проверяем, не добавлен ли уже виджет
            bool found = false;
            for (int i = 0; i < m_listContainerLayout->count(); ++i)
            {
                QLayoutItem *item = m_listContainerLayout->itemAt(i);
                if (item->widget() == widget)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                m_listContainerLayout->addWidget(widget);
            }
        }

        // Заголовок группы (если есть)
        if (!m_listGroups.isEmpty())
        {
            m_listHeaderLabel = new QLabel(m_listGroups.first(), m_listContainer);
            m_listHeaderLabel->setStyleSheet(
                "QLabel {"
                "    font-size: 14px;"
                "    font-weight: bold;"
                "    color: #333333;"
                "    padding: 12px 16px 4px 16px;"
                "    background-color: transparent;"
                "}"
                );
            m_listContainerLayout->addWidget(m_listHeaderLabel);
        }

        // Список
        m_listView = new QListWidget(m_listContainer);
        m_listView->setSelectionMode(QAbstractItemView::SingleSelection);
        m_listView->setFrameShape(QFrame::NoFrame);
        m_listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_listView->setAttribute(Qt::WA_StyledBackground, true);
        m_listView->setMouseTracking(true);
        m_listView->viewport()->setMouseTracking(true);
        m_listView->viewport()->installEventFilter(this);

        // Устанавливаем фиксированный размер элементов
        m_listView->setGridSize(QSize(300, 60));
        m_listView->setUniformItemSizes(true);
        m_listView->setIconSize(QSize(0, 0));
        m_listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // Применяем стиль списка
        updateListStyle();

        connect(m_listView, &QListWidget::itemClicked, this, &CategoryContentWidget::onListClicked);
        m_listContainerLayout->addWidget(m_listView);

        horizontalLayout->addWidget(m_listContainer);

        // ВЕРТИКАЛЬНАЯ РАЗДЕЛИТЕЛЬНАЯ ПОЛОСА
        QFrame *verticalLine = new QFrame();
        verticalLine->setFrameShape(QFrame::VLine);
        verticalLine->setFrameShadow(QFrame::Plain);
        verticalLine->setStyleSheet("QFrame { border: 1px solid #E1E1E1; }");
        verticalLine->setFixedWidth(1);
        horizontalLayout->addWidget(verticalLine);

        // Правая часть - детальная область (растягивается)
        m_detailContainer = new QWidget();
        m_detailContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout *detailLayout = new QVBoxLayout(m_detailContainer);
        detailLayout->setContentsMargins(20, 0, 20, 0);
        detailLayout->setSpacing(0);

        if (m_detailWidget)
        {
            m_detailWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            detailLayout->addWidget(m_detailWidget);
        }

        horizontalLayout->addWidget(m_detailContainer, 1);

        // Добавляем горизонтальный layout в контент
        m_contentLayout->addLayout(horizontalLayout, 1);

        // Настраиваем поведение курсора
        setupCursorBehavior();

        break;
    }

    case LayoutLeftPanel:
    {
        // Создаем горизонтальный layout для левой панели и деталей
        QHBoxLayout *horizontalLayout = new QHBoxLayout();
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setSpacing(0);

        // Левая часть - контейнер для произвольного виджета
        if (m_leftPanelVisible && m_leftPanelWidget)
        {
            m_leftPanelContainer = new QWidget();
            m_leftPanelContainer->setFixedWidth(m_leftPanelWidth);
            m_leftPanelContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

            QVBoxLayout *panelLayout = new QVBoxLayout(m_leftPanelContainer);
            panelLayout->setContentsMargins(0, 0, 0, 0);
            panelLayout->setSpacing(0);

            m_leftPanelWidget->setParent(m_leftPanelContainer);
            m_leftPanelWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            panelLayout->addWidget(m_leftPanelWidget);

            horizontalLayout->addWidget(m_leftPanelContainer);

            // ВЕРТИКАЛЬНАЯ РАЗДЕЛИТЕЛЬНАЯ ПОЛОСА
            QFrame *verticalLine = new QFrame();
            verticalLine->setFrameShape(QFrame::VLine);
            verticalLine->setFrameShadow(QFrame::Plain);
            verticalLine->setStyleSheet("QFrame { border: 1px solid #E1E1E1; }");
            verticalLine->setFixedWidth(1);
            horizontalLayout->addWidget(verticalLine);
        }

        // Правая часть - детальная область (растягивается)
        m_detailContainer = new QWidget();
        m_detailContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout *detailLayout = new QVBoxLayout(m_detailContainer);
        detailLayout->setContentsMargins(20, 0, 20, 0);
        detailLayout->setSpacing(0);

        if (m_detailWidget)
        {
            m_detailWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            detailLayout->addWidget(m_detailWidget);
        }

        horizontalLayout->addWidget(m_detailContainer, 1);

        // Добавляем горизонтальный layout в контент
        m_contentLayout->addLayout(horizontalLayout, 1);

        // Настраиваем поведение курсора
        setupCursorBehavior();

        break;
    }

    case LayoutGroups:
    {
        // Контейнер для групп
        m_groupsContainer = new QWidget();
        m_groupsLayout = new QGridLayout(m_groupsContainer);
        m_groupsLayout->setContentsMargins(20, 10, 20, 20);
        m_groupsLayout->setHorizontalSpacing(m_groupSpacing);
        m_groupsLayout->setVerticalSpacing(m_groupSpacing);

        updateGroupsLayout();

        // Добавляем контейнер с коэффициентом растяжения
        m_contentLayout->addWidget(m_groupsContainer, 1);
        break;
    }

    case LayoutFullWidget:
    {
        if (m_mainWidget)
        {
            // Устанавливаем политику размера для полноэкранного виджета
            m_mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            m_contentLayout->addWidget(m_mainWidget, 1);
        }
        break;
    }
    }

    updateStyle();
}

void CategoryContentWidget::setupCursorBehavior()
{
    // Проверяем и устанавливаем курсор для всех возможных компонентов
    if (m_listView)
    {
        m_listView->setCursor(Qt::ArrowCursor);
        if (m_listView->viewport())
            m_listView->viewport()->setCursor(Qt::ArrowCursor);
    }

    if (m_listContainer)
        m_listContainer->setCursor(Qt::ArrowCursor);

    if (m_leftPanelContainer)
        m_leftPanelContainer->setCursor(Qt::ArrowCursor);

    if (m_detailContainer)
        m_detailContainer->setCursor(Qt::ArrowCursor);

    // Устанавливаем глобальный курсор для всего виджета
    setCursor(Qt::ArrowCursor);
}

void CategoryContentWidget::updateGroupsLayout()
{
    if (!m_groupsLayout || m_layoutType != LayoutGroups)
        return;

    // Очищаем layout
    QLayoutItem *item;
    while ((item = m_groupsLayout->takeAt(0)) != nullptr)
    {
        if (item->widget())
        {
            item->widget()->hide();
        }
        delete item;
    }

    // Заново создаем все группы
    int row = 0;
    int col = 0;

    QList<GroupInfo> validGroups;

    for (int i = 0; i < m_groups.size(); ++i)
    {
        GroupInfo groupInfo = m_groups[i];

        if (!groupInfo.widget)
        {
            qDebug() << "Warning: null widget in group" << groupInfo.title;
            continue;
        }

        try {
            QString name = groupInfo.widget->objectName();
            validGroups.append(groupInfo);
        } catch (...) {
            qDebug() << "Error: widget for group" << groupInfo.title << "is corrupted";
            continue;
        }
    }

    for (const GroupInfo &groupInfo : validGroups)
    {
        QWidget *groupWidget = createGroupWidget(groupInfo);

        if (groupWidget)
        {
            groupWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            m_groupsLayout->addWidget(groupWidget, row, col, 1, groupInfo.columnSpan);

            col += groupInfo.columnSpan;
            if (col >= m_groupsColumns)
            {
                col = 0;
                row++;
            }
        }
    }

    if (row > 0)
    {
        m_groupsLayout->setRowStretch(row, 1);
    }
}

QWidget* CategoryContentWidget::createGroupWidget(const GroupInfo &group)
{
    if (!group.widget)
    {
        qDebug() << "Error: group.widget is null for group" << group.title;
        return nullptr;
    }

    QWidget *groupWidget = new QWidget();
    groupWidget->setObjectName("GroupWidget");
    groupWidget->setStyleSheet(
        "QWidget#GroupWidget {"
        "    background-color: white;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 4px;"
        "}"
        );

    groupWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QVBoxLayout *layout = new QVBoxLayout(groupWidget);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    QLabel *titleLabel = new QLabel(group.title, groupWidget);
    titleLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    color: #333333;"
        "    padding-bottom: 8px;"
        "    border-bottom: 1px solid #E0E0E0;"
        "}"
        );
    layout->addWidget(titleLabel);

    layout->addWidget(group.widget);

    return groupWidget;
}

void CategoryContentWidget::clearLayout(QLayout *layout)
{
    if (!layout)
        return;

    while (layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        if (item->widget())
            item->widget()->hide();
        delete item;
    }
}

void CategoryContentWidget::updateListStyle()
{
    if (!m_listView)
        return;

    QColor baseColor(0x21, 0x73, 0x46);

    QWidget *parent = parentWidget();
    while (parent)
    {
        if (parent->property("menuPanelColor").isValid())
        {
            baseColor = parent->property("menuPanelColor").value<QColor>();
            break;
        }
        parent = parent->parentWidget();
    }

    // Обновляем цвета в стиле
    if (m_customStyle)
    {
        m_customStyle->setBaseColor(baseColor);
    }
}

// Вспомогательная функция для смешивания с белым
QColor CategoryContentWidget::mixWithWhite(const QColor &base, int percent)
{
    percent = qBound(0, percent, 100);
    int r = base.red() + (255 - base.red()) * percent / 100;
    int g = base.green() + (255 - base.green()) * percent / 100;
    int b = base.blue() + (255 - base.blue()) * percent / 100;
    return QColor(r, g, b);
}

// Вспомогательная функция для смешивания с черным
QColor CategoryContentWidget::mixWithBlack(const QColor &base, int percent)
{
    percent = qBound(0, percent, 100);
    int r = base.red() * (100 - percent) / 100;
    int g = base.green() * (100 - percent) / 100;
    int b = base.blue() * (100 - percent) / 100;
    return QColor(r, g, b);
}

void CategoryContentWidget::updateOfficeStyle()
{
    QColor baseColor(0x21, 0x73, 0x46);

    QWidget *parent = parentWidget();
    while (parent)
    {
        if (parent->property("menuPanelColor").isValid())
        {
            baseColor = parent->property("menuPanelColor").value<QColor>();
            break;
        }
        parent = parent->parentWidget();
    }

    if (m_customStyle)
    {
        m_customStyle->setBaseColor(baseColor);
    }

    // Обновляем цвет заголовка
    if (m_detailHeaderTitleLabel)
    {
        m_detailHeaderTitleLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(baseColor.name()));
    }

    // Обновляем стили для всех кнопок
    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    for (QPushButton *button : buttons)
    {
        applyButtonStyle(button);
    }

    QList<QGroupBox*> groupBoxes = findChildren<QGroupBox*>();
    for (QGroupBox *groupBox : groupBoxes)
    {
        applyGroupBoxStyle(groupBox/*, groupBox->isFlat()*/);
    }

    QList<QToolButton*> toolbuttons = findChildren<QToolButton*>();
    for (QToolButton *button : toolbuttons)
    {
        applyButtonStyle(button);
    }

    // Обновляем стиль для всех виджетов
    applyStyleRecursively(this);
}

void CategoryContentWidget::updateStyle()
{
    updateListStyle();
    updateOfficeStyle();
}

// ==================== Базовые методы ====================

CategoryContentWidget::LayoutType CategoryContentWidget::layoutType() const
{
    return m_layoutType;
}

void CategoryContentWidget::setLayoutType(LayoutType type)
{
    if (m_layoutType == type)
        return;

    m_layoutType = type;
    updateLayout();
    emit layoutTypeChanged(type);
    emit dataChanged();
}

bool CategoryContentWidget::isReadOnly() const
{
    return m_readOnly;
}

void CategoryContentWidget::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
    emit dataChanged();
}

// ==================== Для LayoutListDetail ====================

void CategoryContentWidget::addListItem(const QString &text, const QVariant &data)
{
    if (m_listView)
    {
        QListWidgetItem *item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, data);
        item->setSizeHint(QSize(m_listView->gridSize().width(), 60));
        m_listView->addItem(item);

        if (m_listView->count() == 1 && m_listView->currentItem() == nullptr)
        {
            m_listView->setCurrentItem(item);
            m_currentDetailTitle = text;
            m_currentDetailIcon = QIcon();
            updateDetailHeader();
        }
    }
}

void CategoryContentWidget::addListItem(const QString &text, const QIcon &icon, const QVariant &data)
{
    if (m_listView)
    {
        QListWidgetItem *item = new QListWidgetItem(text);
        item->setIcon(icon);
        item->setData(Qt::UserRole, data);
        item->setSizeHint(QSize(m_listView->gridSize().width(), 60));
        m_listView->addItem(item);

        if (m_listView->count() == 1 && m_listView->currentItem() == nullptr)
        {
            m_listView->setCurrentItem(item);
            m_currentDetailTitle = text;
            m_currentDetailIcon = icon;
            updateDetailHeader();
        }
    }
}

void CategoryContentWidget::addListGroup(const QString &groupTitle)
{
    if (m_listView && !groupTitle.isEmpty())
    {
        QListWidgetItem *item = new QListWidgetItem(groupTitle);
        item->setFlags(Qt::NoItemFlags);
        item->setBackground(QColor(240, 240, 240));
        item->setSizeHint(QSize(m_listView->gridSize().width(), 60));
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
        m_listView->addItem(item);

        if (m_listView->count() == 1 && m_listView->currentItem() == nullptr)
        {
            m_listView->setCurrentItem(item);
            m_currentDetailTitle = groupTitle;
            m_currentDetailIcon = QIcon();
            updateDetailHeader();
        }
    }
}

void CategoryContentWidget::clearList()
{
    if (m_listView)
    {
        m_listView->clear();
    }
}

void CategoryContentWidget::setDetailWidget(QWidget *detailWidget)
{
    m_detailWidget = detailWidget;

    if (detailWidget)
    {
        // Применяем стиль к новому виджету
        applyStyleToWidget(detailWidget);
    }

    if ((m_layoutType == LayoutListDetail || m_layoutType == LayoutLeftPanel) && m_detailContainer)
    {
        QLayout *layout = m_detailContainer->layout();
        if (layout)
        {
            while (layout->count() > 0)
            {
                QLayoutItem *item = layout->takeAt(0);
                delete item;
            }

            // Создаем заголовок
            createDetailHeader();

            // Добавляем заголовок
            if (m_showDetailHeader && m_detailHeaderWidget)
            {
                QVBoxLayout *detailLayout = qobject_cast<QVBoxLayout*>(layout);
                if (detailLayout)
                {
                    detailLayout->addWidget(m_detailHeaderWidget);
                }
            }

            // Добавляем основной виджет
            layout->addWidget(detailWidget);
        }
    }

    updateStyle();
}

void CategoryContentWidget::setListHeaderVisible(bool visible)
{
    if (m_listHeaderLabel)
    {
        m_listHeaderLabel->setVisible(visible);
    }
}

void CategoryContentWidget::setListGroups(const QStringList &groups)
{
    m_listGroups = groups;
}

QWidget* CategoryContentWidget::detailWidget() const
{
    return m_detailWidget;
}

QListWidget* CategoryContentWidget::listWidget() const
{
    return m_listView;
}

// ==================== Новые методы для добавления виджетов перед списком ====================

void CategoryContentWidget::addWidgetBeforeList(QWidget *widget, int stretch, Qt::Alignment alignment)
{
    if (!widget)
        return;

    // Сохраняем виджет в список
    m_widgetsBeforeList.append(widget);

    // Если уже есть контейнер, добавляем виджет в layout
    if (m_listContainerLayout && m_layoutType == LayoutListDetail)
    {
        widget->setParent(m_listContainer);

        // Определяем позицию для вставки (перед заголовком)
        int insertIndex = 0;

        // Если есть заголовок, вставляем перед ним
        if (m_listHeaderLabel && m_listContainerLayout->indexOf(m_listHeaderLabel) >= 0)
        {
            insertIndex = m_listContainerLayout->indexOf(m_listHeaderLabel);
        }
        // Если нет заголовка, но есть список, вставляем перед списком
        else if (m_listView && m_listContainerLayout->indexOf(m_listView) >= 0)
        {
            insertIndex = m_listContainerLayout->indexOf(m_listView);
        }

        m_listContainerLayout->insertWidget(insertIndex, widget, stretch, alignment);
        updateStyle();
    }
}

void CategoryContentWidget::insertWidgetBeforeList(int index, QWidget *widget, int stretch, Qt::Alignment alignment)
{
    if (!widget)
        return;

    index = qBound(0, index, m_widgetsBeforeList.size());

    // Вставляем в список
    m_widgetsBeforeList.insert(index, widget);

    // Если уже есть контейнер, обновляем layout
    if (m_listContainerLayout && m_layoutType == LayoutListDetail)
    {
        // Перестраиваем layout левой панели
        updateListContainerLayout();
    }
}

void CategoryContentWidget::removeWidgetBeforeList(QWidget *widget)
{
    if (!widget)
        return;

    int index = m_widgetsBeforeList.indexOf(widget);
    if (index >= 0)
    {
        m_widgetsBeforeList.removeAt(index);

        // Если уже есть контейнер, обновляем layout
        if (m_listContainerLayout && m_layoutType == LayoutListDetail)
        {
            updateListContainerLayout();
        }

        widget->setParent(nullptr);
    }
}

void CategoryContentWidget::clearWidgetsBeforeList()
{
    for (QWidget *widget : m_widgetsBeforeList)
    {
        widget->setParent(nullptr);
    }
    m_widgetsBeforeList.clear();

    // Если уже есть контейнер, обновляем layout
    if (m_listContainerLayout && m_layoutType == LayoutListDetail)
    {
        updateListContainerLayout();
    }
}

void CategoryContentWidget::updateListContainerLayout()
{
    if (!m_listContainerLayout || m_layoutType != LayoutListDetail)
        return;

    // Сохраняем указатели на важные виджеты
    QLabel *headerLabel = m_listHeaderLabel;
    QListWidget *listView = m_listView;

    // Очищаем layout, но сохраняем ссылки на виджеты
    while (m_listContainerLayout->count() > 0)
    {
        QLayoutItem *item = m_listContainerLayout->takeAt(0);
        // Не удаляем виджеты, только убираем из layout
    }

    // Добавляем виджеты из списка в правильном порядке
    for (QWidget *widget : m_widgetsBeforeList)
    {
        if (widget)
        {
            widget->setParent(m_listContainer);
            m_listContainerLayout->addWidget(widget);
        }
    }

    // Добавляем заголовок группы
    if (headerLabel)
    {
        m_listContainerLayout->addWidget(headerLabel);
    }

    // Добавляем список
    if (listView)
    {
        m_listContainerLayout->addWidget(listView);
    }
}

// ==================== Для LayoutGroups ====================

void CategoryContentWidget::addGroup(const QString &groupTitle, QWidget *groupWidget, int columnSpan)
{
    insertGroup(m_groups.size(), groupTitle, groupWidget, columnSpan);
}

void CategoryContentWidget::insertGroup(int index, const QString &groupTitle, QWidget *groupWidget, int columnSpan)
{
    if (!groupWidget)
        return;

    index = qBound(0, index, m_groups.size());

    GroupInfo info;
    info.title = groupTitle;
    info.widget = groupWidget;
    info.columnSpan = qBound(1, columnSpan, m_groupsColumns);

    m_groups.insert(index, info);

    groupWidget->setAttribute(Qt::WA_DeleteOnClose, false);

    if (m_layoutType == LayoutGroups)
    {
        updateGroupsLayout();
    }

    onGroupAdded(index);
    emit dataChanged();
}

void CategoryContentWidget::removeGroup(int index)
{
    if (index < 0 || index >= m_groups.size())
        return;

    GroupInfo info = m_groups.takeAt(index);
    if (info.widget)
    {
        info.widget->setParent(nullptr);
    }

    if (m_layoutType == LayoutGroups)
    {
        updateGroupsLayout();
    }

    onGroupRemoved(index);
    emit dataChanged();
}

void CategoryContentWidget::removeGroup(const QString &groupTitle)
{
    for (int i = 0; i < m_groups.size(); ++i)
    {
        if (m_groups[i].title == groupTitle)
        {
            removeGroup(i);
            break;
        }
    }
}

void CategoryContentWidget::clearGroups()
{
    for (const GroupInfo &info : m_groups)
    {
        if (info.widget)
        {
            info.widget->setParent(nullptr);
        }
    }
    m_groups.clear();

    if (m_layoutType == LayoutGroups)
    {
        updateGroupsLayout();
    }

    emit dataChanged();
}

void CategoryContentWidget::setGroupsColumns(int columns)
{
    if (m_groupsColumns == columns)
        return;

    m_groupsColumns = qBound(1, columns, 4);

    if (m_layoutType == LayoutGroups)
    {
        updateGroupsLayout();
    }

    emit dataChanged();
}

void CategoryContentWidget::setGroupSpacing(int spacing)
{
    if (m_groupSpacing == spacing)
        return;

    m_groupSpacing = spacing;

    if (m_groupsLayout)
    {
        m_groupsLayout->setHorizontalSpacing(spacing);
        m_groupsLayout->setVerticalSpacing(spacing);
    }
}

int CategoryContentWidget::groupCount() const
{
    return m_groups.size();
}

CategoryContentWidget::GroupInfo CategoryContentWidget::groupAt(int index) const
{
    if (index < 0 || index >= m_groups.size())
        return GroupInfo();

    return m_groups[index];
}

QString CategoryContentWidget::groupTitle(int index) const
{
    if (index < 0 || index >= m_groups.size())
        return QString();

    return m_groups[index].title;
}

QWidget* CategoryContentWidget::groupWidget(int index) const
{
    if (index < 0 || index >= m_groups.size())
        return nullptr;

    return m_groups[index].widget;
}

// ==================== Для LayoutFullWidget ====================

void CategoryContentWidget::setMainWidget(QWidget *widget)
{
    m_mainWidget = widget;

    if (m_layoutType == LayoutFullWidget)
    {
        clearLayout(m_contentLayout);
        if (widget)
        {
            widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            m_contentLayout->addWidget(widget, 1);
        }
    }

    emit dataChanged();
}

QWidget* CategoryContentWidget::mainWidget() const
{
    return m_mainWidget;
}

// ==================== Для LayoutLeftPanel ====================

void CategoryContentWidget::setLeftPanelWidget(QWidget *widget, int fixedWidth)
{
    m_leftPanelWidget = widget;
    m_leftPanelWidth = fixedWidth;

    if (widget)
    {
        widget->setParent(this);
        applyStyleToWidget(widget);
    }

    if (m_layoutType == LayoutLeftPanel)
    {
        updateLayout();
    }

    updateStyle();
    emit dataChanged();
}

void CategoryContentWidget::setLeftPanelWidth(int width)
{
    if (m_leftPanelWidth == width)
        return;

    m_leftPanelWidth = qBound(150, width, 500);

    if (m_layoutType == LayoutLeftPanel && m_leftPanelContainer)
    {
        m_leftPanelContainer->setFixedWidth(m_leftPanelWidth);
    }

    emit dataChanged();
}

QWidget* CategoryContentWidget::leftPanelWidget() const
{
    return m_leftPanelWidget;
}

void CategoryContentWidget::setLeftPanelVisible(bool visible)
{
    if (m_leftPanelVisible == visible)
        return;

    m_leftPanelVisible = visible;

    if (m_layoutType == LayoutLeftPanel)
    {
        updateLayout();
    }

    emit dataChanged();
}

bool CategoryContentWidget::isLeftPanelVisible() const
{
    return m_leftPanelVisible;
}

// ==================== Скроллинг ====================

void CategoryContentWidget::enableScrolling(bool enable)
{
    m_scrollingEnabled = enable;
    m_scrollArea->setVerticalScrollBarPolicy(enable ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
}

void CategoryContentWidget::scrollToTop()
{
    if (m_scrollArea && m_scrollArea->verticalScrollBar())
    {
        m_scrollArea->verticalScrollBar()->setValue(0);
    }
}

// ==================== Слоты ====================

void CategoryContentWidget::onListClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    // Сохраняем информацию о выбранном элементе
    m_currentDetailTitle = item->text();
    m_currentDetailIcon = item->icon();

    // Обновляем заголовок детальной области
    updateDetailHeader();

    QVariant data = item->data(Qt::UserRole);
    onListItemSelected(data);
    emit listItemSelected(data);
}

// ==================== Виртуальные методы ====================

void CategoryContentWidget::onListItemSelected(const QVariant &data)
{
    Q_UNUSED(data);
}

void CategoryContentWidget::onGroupAdded(int index)
{
    Q_UNUSED(index);
}

void CategoryContentWidget::onGroupRemoved(int index)
{
    Q_UNUSED(index);
}

void CategoryContentWidget::onLayoutChanged()
{
}

bool CategoryContentWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (m_listView && obj == m_listView->viewport() && event->type() == QEvent::MouseMove) {
        m_listView->viewport()->update();
    }
    return QWidget::eventFilter(obj, event);
}

void CategoryContentWidget::applyStyleRecursively(QWidget *widget)
{
    if (!widget)
        return;

    // Применяем кастомный стиль
    widget->setStyle(m_customStyle);

    // Устанавливаем атрибуты
    widget->setAttribute(Qt::WA_StyledBackground, true);
    widget->setMouseTracking(true);

    // Для кнопок применяем специальный QSS стиль
    if (QPushButton *button = qobject_cast<QPushButton*>(widget))
        applyButtonStyle(button);

    if (QToolButton *button = qobject_cast<QToolButton*>(widget))
        applyButtonStyle(button);

    if (QGroupBox *groupBox = qobject_cast<QGroupBox*>(widget))
        applyGroupBoxStyle(groupBox/*, groupBox->isFlat()*/);

    // Рекурсивно применяем ко всем дочерним виджетам
    for (QObject *child : widget->children())
    {
        if (QWidget *childWidget = qobject_cast<QWidget*>(child))
        {
            applyStyleRecursively(childWidget);
        }
    }
}

void CategoryContentWidget::applyButtonStyle(QPushButton *button)
{
    if (!button || !m_customStyle)
        return;

    // Получаем текущий основной цвет
    QColor baseColor = m_customStyle->baseColor();

    // Рассчитываем цвета
    QColor hoverColor = mixWithWhite(baseColor, 85);
    QColor selectedColor = mixWithWhite(baseColor, 70);
    QColor borderColor = QColor(0xAB, 0xAB, 0xAB);

    QString styleSheet = QString(
                             "QPushButton {"
                             "    background-color: transparent;"
                             "    border: 1px solid #ABABAB;"
                             "    border-radius: 0px;"
                             "    padding: 5px 12px;"
                             "    min-height: 22px;"
                             "    color: #333333;"
                             "    text-align: center;"
                             "}"
                             "QPushButton:hover {"
                             "    background-color: %1;"
                             "    border: 1px solid %2;"
                             "}"
                             "QPushButton:pressed {"
                             "    background-color: %3;"
                             "    border: 1px solid %4;"
                             "}"
                             "QPushButton:checked {"
                             "    background-color: %3;"
                             "    border: 1px solid %4;"
                             "}"
                             "QPushButton:focus {"
                             "    outline: none;"
                             "    border: 1px solid %5;"
                             "}"
                             "QPushButton:disabled {"
                             "    background-color: #F5F5F5;"
                             "    color: #ABABAB;"
                             "    border: 1px solid #E0E0E0;"
                             "}"
                             // Добавляем стили для иконки
                             "QPushButton::icon {"
                             "    margin-right: 6px;"
                             "    width: 16px;"
                             "    height: 16px;"
                             "}"
                             "QPushButton::icon:disabled {"
                             "    opacity: 0.5;"
                             "}"
                             )
                             .arg(hoverColor.name())
                             .arg(hoverColor.darker(110).name())
                             .arg(selectedColor.name())
                             .arg(baseColor.name())
                             .arg(baseColor.name());

    button->setStyleSheet(styleSheet);
    button->setAttribute(Qt::WA_StyledBackground, true);
    button->setMouseTracking(true);
}

void CategoryContentWidget::applyButtonStyle(QToolButton *button)
{
    if (!button || !m_customStyle)
        return;

    // Получаем текущий основной цвет
    QColor baseColor = m_customStyle->baseColor();

    // Рассчитываем цвета
    QColor hoverColor = mixWithWhite(baseColor, 85);
    QColor selectedColor = mixWithWhite(baseColor, 70);
    QColor borderColor = QColor(0xAB, 0xAB, 0xAB);

    QString styleSheet = QString(
                             "QToolButton {"
                             "    background-color: transparent;"
                             "    border: 1px solid #ABABAB;"
                             "    border-radius: 0px;"
                             "    padding: 5px 12px;"
                             "    min-height: 22px;"
                             "    color: #333333;"
                             "    text-align: center;"
                             "}"
                             "QToolButton:hover {"
                             "    background-color: %1;"
                             "    border: 1px solid %2;"
                             "}"
                             "QToolButton:pressed {"
                             "    background-color: %3;"
                             "    border: 1px solid %4;"
                             "}"
                             "QToolButton:checked {"
                             "    background-color: %3;"
                             "    border: 1px solid %4;"
                             "}"
                             "QToolButton:focus {"
                             "    outline: none;"
                             "    border: 1px solid %5;"
                             "}"
                             "QToolButton:disabled {"
                             "    background-color: #F5F5F5;"
                             "    color: #ABABAB;"
                             "    border: 1px solid #E0E0E0;"
                             "}"
                             // Добавляем стили для иконки
                             "QToolButton::icon {"
                             "    margin-right: 6px;"
                             "    width: 16px;"
                             "    height: 16px;"
                             "}"
                             "QToolButton::icon:disabled {"
                             "    opacity: 0.5;"
                             "}"
                             )
                             .arg(hoverColor.name())
                             .arg(hoverColor.darker(110).name())
                             .arg(selectedColor.name())
                             .arg(baseColor.name())
                             .arg(baseColor.name());

    button->setStyleSheet(styleSheet);
    button->setAttribute(Qt::WA_StyledBackground, true);
    button->setMouseTracking(true);
}

void CategoryContentWidget::setShowDetailHeader(bool show)
{
    m_showDetailHeader = show;
    if (m_layoutType == LayoutListDetail || m_layoutType == LayoutLeftPanel)
    {
        updateDetailHeader();
    }
}

bool CategoryContentWidget::showDetailHeader() const
{
    return m_showDetailHeader;
}

void CategoryContentWidget::setDetailHeaderIconSize(const QSize &size)
{
    m_detailHeaderIconSize = size;
    if (m_detailHeaderIconLabel)
    {
        m_detailHeaderIconLabel->setFixedSize(size);
        m_detailHeaderIconLabel->setPixmap(m_currentDetailIcon.pixmap(size));
    }
}

QSize CategoryContentWidget::detailHeaderIconSize() const
{
    return m_detailHeaderIconSize;
}

void CategoryContentWidget::createDetailHeader()
{
    if (m_detailHeaderWidget)
        return;

    m_detailHeaderWidget = new QWidget();
    m_detailHeaderWidget->setObjectName("DetailHeaderWidget");
    m_detailHeaderWidget->setStyleSheet(
        "QWidget#DetailHeaderWidget {"
        "    background-color: transparent;"
        "    margin-bottom: 10px;"
        "}"
        );

    QHBoxLayout *headerLayout = new QHBoxLayout(m_detailHeaderWidget);
    headerLayout->setContentsMargins(0, 0, 0, 10);
    headerLayout->setSpacing(12);

    // Иконка
    m_detailHeaderIconLabel = new QLabel();
    m_detailHeaderIconLabel->setFixedSize(m_detailHeaderIconSize);
    m_detailHeaderIconLabel->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(m_detailHeaderIconLabel);

    // Заголовок
    m_detailHeaderTitleLabel = new QLabel();
    QFont titleFont = m_detailHeaderTitleLabel->font();
    titleFont.setPointSize(14);
    m_detailHeaderTitleLabel->setFont(titleFont);

    // Получаем основной цвет для заголовка
    QColor baseColor = m_customStyle ? m_customStyle->baseColor() : QColor(0x21, 0x73, 0x46);
    m_detailHeaderTitleLabel->setStyleSheet(QString("color: %1;").arg(baseColor.name()));

    headerLayout->addWidget(m_detailHeaderTitleLabel);
    headerLayout->addStretch();

    // Добавляем в детальную область
    if (m_detailContainer && m_detailContainer->layout())
    {
        QVBoxLayout *detailLayout = qobject_cast<QVBoxLayout*>(m_detailContainer->layout());
        if (detailLayout)
        {
            // Вставляем заголовок в начало
            detailLayout->insertWidget(0, m_detailHeaderWidget);
        }
    }
}

void CategoryContentWidget::updateDetailHeader()
{
    if (!m_showDetailHeader)
    {
        if (m_detailHeaderWidget)
            m_detailHeaderWidget->hide();
        return;
    }

    if (!m_detailHeaderWidget)
        createDetailHeader();

    if (m_detailHeaderWidget)
    {
        m_detailHeaderWidget->show();

        // Обновляем иконку
        if (m_detailHeaderIconLabel && !m_currentDetailIcon.isNull())
        {
            m_detailHeaderIconLabel->setPixmap(m_currentDetailIcon.pixmap(m_detailHeaderIconSize));
        }

        // Обновляем текст
        if (m_detailHeaderTitleLabel && !m_currentDetailTitle.isEmpty())
        {
            m_detailHeaderTitleLabel->setText(m_currentDetailTitle);

            // Обновляем цвет заголовка
            QColor baseColor = m_customStyle ? m_customStyle->baseColor() : QColor(0x21, 0x73, 0x46);
            m_detailHeaderTitleLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(baseColor.name()));
        }
    }
}

void CategoryContentWidget::applyGroupBoxStyle(QGroupBox *groupBox)
{
    if (!groupBox || !m_customStyle)
        return;

    // Получаем текущий основной цвет
    QColor baseColor = m_customStyle->baseColor();

    QString styleSheet;

    if (groupBox->isCheckable())
    {
        // Стиль для QGroupBox с checkbox
        styleSheet = QString(
                         "QGroupBox {"
                         "    background-color: transparent;"
                         "    border: none;"
                         "    margin-top: 4px;"
                         "    margin-bottom: 4px;"
                         "    font-weight: normal;"
                         "    font-size: 14pt;"
                         "    color: #262626;"
                         "}"
                         "QGroupBox::title {"
                         "    subcontrol-origin: margin;"
                         "    subcontrol-position: top left;"
                         "    left: 0px;"
                         "    padding: 0 8px 0 20px;"
                         "    color: #262626;"
                         "    background-color: transparent;"
                         "}"
                         // Стиль для индикатора чекбокса
                         "QGroupBox::indicator {"
                         "    width: 16px;"
                         "    height: 16px;"
                         "    border: 1px solid #ABABAB;"
                         "    border-radius: 2px;"
                         "    background-color: white;"
                         "    margin-top: 2px;"
                         "}"
                         "QGroupBox::indicator:hover {"
                         "    border: 1px solid %1;"
                         "    background-color: %2;"
                         "}"
                         "QGroupBox::indicator:checked {"
                         "    background-color: %1;"
                         "    border: 1px solid %1;"
                         "}"
                         // Линия после заголовка
                         "QGroupBox::indicator:checked + QGroupBox::title {"
                         "    color: %1;"
                         "}"
                         // Верхняя линия (отображается только если группа не отключена)
                         "QGroupBox::indicator:enabled {"
                         "    border-top: 1px solid #E0E0E0;"
                         "    margin-top: 8px;"
                         "}"
                         )
                         .arg(baseColor.name())
                         .arg(mixWithWhite(baseColor, 85).name());
    }
    else
    {
        // Стиль для обычного QGroupBox
        styleSheet = QString(
            "QGroupBox {"
            "    background-color: transparent;"
            "    border: none;"
            "    margin-top: 4px;"
            "    margin-bottom: 4px;"
            "    font-weight: normal;"
            "    font-size: 14pt;"
            "    color: #262626;"
            "}"
            "QGroupBox::title {"
            "    subcontrol-origin: margin;"
            "    subcontrol-position: top left;"
            "    left: 0px;"
            "    padding: 0 8px 0 0px;"
            "    color: #262626;"
            "    background-color: transparent;"
            "}"
            // Верхняя линия
            "QGroupBox::indicator {"
            "    border-top: 1px solid #E0E0E0;"
            "    margin-top: 8px;"
            "}"
            );
    }

    groupBox->setStyleSheet(styleSheet);
    groupBox->setFlat(true);
    groupBox->setAttribute(Qt::WA_StyledBackground, true);
}