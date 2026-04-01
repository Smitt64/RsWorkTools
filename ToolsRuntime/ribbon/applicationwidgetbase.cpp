#include "ApplicationWidgetBase.h"
#include "ribbon/appoptionscontentwidget.h"
#include "ribbon/categorycontentwidget.h"
#include "toolsruntime.h"
#include <QSvgRenderer>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <QFrame>
#include <QIcon>
#include <QEvent>

ApplicationWidgetBase::ApplicationWidgetBase(SARibbonMainWindow *parent)
    : SARibbonApplicationWidget(parent)
    , m_menuPanelColor(QColor(52, 73, 94))
    , m_currentIndex(-1)
    , m_pOptionsWidget(nullptr)
{
    setupUI();
    setupBackButton();

    setStyleSheet("ApplicationWidgetBase {background-color: #F5F5F5;}");
    setAutoFillBackground(true);
}

ApplicationWidgetBase::~ApplicationWidgetBase()
{
    m_tabContents.clear();
    m_tabTitles.clear();
}

void ApplicationWidgetBase::setupUI()
{
    // Основной горизонтальный layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Левая панель меню
    m_menuPanel = new QWidget(this);
    m_menuPanel->setFixedWidth(160);
    m_menuPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_menuPanel->setContentsMargins(0, 0, 0, 0);

    // Layout для панели меню
    m_menuLayout = new QVBoxLayout(m_menuPanel);
    m_menuLayout->setContentsMargins(0, 0, 0, 0);
    m_menuLayout->setSpacing(0);

    // Кнопка возврата будет создана в setupBackButton()
    m_backButton = nullptr;

    // Разделитель
    QFrame *separator = new QFrame(m_menuPanel);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Plain);
    separator->setFixedHeight(1);
    separator->setStyleSheet("background-color: rgba(255, 255, 255, 0.1);");
    m_menuLayout->addWidget(separator);

    // Список вкладок
    m_tabList = new QListWidget(m_menuPanel);
    m_tabList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tabList->setFocusPolicy(Qt::NoFocus);
    m_tabList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_tabList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabList->setFrameShape(QFrame::NoFrame);
    m_tabList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tabList->setAttribute(Qt::WA_StyledBackground, true);

    connect(m_tabList, &QListWidget::itemClicked, this, &ApplicationWidgetBase::onTabItemClicked);

    m_menuLayout->addWidget(m_tabList);

    // Добавляем растяжку внизу
    m_menuLayout->addStretch();

    mainLayout->addWidget(m_menuPanel);
    //mainLayout->addStretch(64);

    // Разделитель между панелью меню и контентом
    /*QFrame *mainSeparator = new QFrame(this);
    mainSeparator->setFrameShape(QFrame::VLine);
    mainSeparator->setFrameShadow(QFrame::Plain);
    mainSeparator->setFixedWidth(1);
    mainSeparator->setStyleSheet("background-color: rgba(0, 0, 0, 0.05);");
    mainLayout->addWidget(mainSeparator);*/

    // Правая область с контентом
    m_contentArea = new QWidget(this);
    m_contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *contentLayout = new QVBoxLayout(m_contentArea);
    contentLayout->setContentsMargins(32, 0, 32, 32);
    contentLayout->setSpacing(0);

    // Заголовок категории с верхним отступом для выравнивания
    m_categoryTitle = new QLabel(m_contentArea);
    m_categoryTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_categoryTitle->setStyleSheet(
        "QLabel {"
        "    font-size: 36px;"
        "    font-weight: normal;"
        "    color: #444444;"
        "    padding: 0px 20px 10px 0px;"
        "    background-color: #F5F5F5;"
        "    font-family: 'Segoe UI', 'Microsoft YaHei', sans-serif;"
        "}"
        );
    m_categoryTitle->setText("");
    // Добавляем верхний отступ равный высоте кнопки назад (64px)
    m_categoryTitle->setContentsMargins(0, 0, 0, 0);
    contentLayout->addSpacing(60);
    contentLayout->addWidget(m_categoryTitle);
    contentLayout->addSpacing(24);

    // Стек для контента
    m_stackedWidget = new QStackedWidget(m_contentArea);
    m_stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_stackedWidget->setContentsMargins(0, 0, 0, 0);
    m_stackedWidget->setStyleSheet(
        "QStackedWidget {"
        "    background-color: #F5F5F5;"
        "    border: none;"
        "}"
        );

    contentLayout->addWidget(m_stackedWidget);

    mainLayout->addWidget(m_contentArea);

    setLayout(mainLayout);

    // Применяем стили
    updateMenuPanelStyle();
}

void ApplicationWidgetBase::setupBackButton()
{
    m_backButton = new QPushButton(m_menuPanel);
    m_backButton->setFixedSize(64, 64);  // Увеличено до 64x64
    m_backButton->setCursor(Qt::PointingHandCursor);
    m_backButton->setFocusPolicy(Qt::NoFocus);
    m_backButton->setFlat(true);

    // Читаем SVG файл
    QByteArray svgData = toolReadFileContent("://img/back-button.svg");

    if (!svgData.isEmpty())
    {
        // Создаем иконки для разных состояний
        m_backButtonNormalPixmap = createColoredPixmap(svgData, Qt::white);
        QColor hoverColor = m_menuPanelColor.lighter(130);
        m_backButtonHoverPixmap = createColoredPixmap(svgData, hoverColor);

        // Устанавливаем нормальную иконку
        m_backButton->setIcon(QIcon(m_backButtonNormalPixmap));
        m_backButton->setIconSize(QSize(36, 36));  // Увеличено до 36x36
    }

    // Стиль кнопки
    m_backButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    background-color: transparent;"
        "}"
        );

    // Устанавливаем фильтр событий для отслеживания наведения
    m_backButton->installEventFilter(this);

    connect(m_backButton, &QPushButton::clicked, this, &ApplicationWidgetBase::onBackButtonClicked);
    connect(m_backButton, &QPushButton::clicked, this, &QWidget::hide);

    // Вставляем кнопку в начало layout
    m_menuLayout->insertWidget(0, m_backButton);
}

QPixmap ApplicationWidgetBase::createColoredPixmap(const QByteArray &svgData, const QColor &color)
{
    // Загружаем SVG из QByteArray
    QSvgRenderer renderer;
    renderer.load(svgData);

    // Создаем pixmap увеличенного размера
    QPixmap pixmap(36, 36);  // Увеличено до 36x36
    pixmap.fill(Qt::transparent);

    // Рендерим SVG
    QPainter painter(&pixmap);
    renderer.render(&painter);
    painter.end();

    // Меняем цвет
    QImage image = pixmap.toImage();
    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            QColor pixelColor = image.pixelColor(x, y);
            if (pixelColor.alpha() > 0)
            {
                pixelColor.setRed(color.red());
                pixelColor.setGreen(color.green());
                pixelColor.setBlue(color.blue());
                image.setPixelColor(x, y, pixelColor);
            }
        }
    }

    return QPixmap::fromImage(image);
}

bool ApplicationWidgetBase::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_backButton)
    {
        if (event->type() == QEvent::Enter)
        {
            if (!m_backButtonHoverPixmap.isNull())
            {
                m_backButton->setIcon(QIcon(m_backButtonHoverPixmap));
            }
        }
        else if (event->type() == QEvent::Leave)
        {
            if (!m_backButtonNormalPixmap.isNull())
            {
                m_backButton->setIcon(QIcon(m_backButtonNormalPixmap));
            }
        }
    }

    return SARibbonApplicationWidget::eventFilter(obj, event);
}

void ApplicationWidgetBase::updateCategoryTitle()
{
    if (m_categoryTitle && m_currentIndex >= 0 && m_currentIndex < m_tabTitles.size())
    {
        //qDebug() << m_tabTitles;
        m_categoryTitle->setText(m_tabTitles[m_currentIndex]);
        // Сохраняем верхний отступ
        m_categoryTitle->setContentsMargins(0, 0, 0, 0);
    }
    else if (m_categoryTitle)
    {
        m_categoryTitle->setText("");
        m_categoryTitle->setContentsMargins(0, 0, 0, 0);
    }
}

void ApplicationWidgetBase::updateMenuPanelStyle()
{
    if (m_menuPanel)
    {
        m_menuPanel->setStyleSheet(
            QString("QWidget { background-color: %1; border: none; }")
                .arg(m_menuPanelColor.name())
            );

        updateTabListStyle();

        // Обновляем цвет иконки при наведении
        if (m_backButton && !m_backButtonNormalPixmap.isNull())
        {
            QByteArray svgData = toolReadFileContent("://img/back-button.svg");
            if (!svgData.isEmpty())
            {
                QColor hoverColor = m_menuPanelColor.lighter(130);
                m_backButtonHoverPixmap = createColoredPixmap(svgData, hoverColor);

                if (m_backButton->underMouse())
                {
                    m_backButton->setIcon(QIcon(m_backButtonHoverPixmap));
                }
            }
        }
    }
}

void ApplicationWidgetBase::updateTabListStyle()
{
    if (!m_tabList)
        return;

    QColor baseColor = m_menuPanelColor;
    QColor selectedColor = baseColor.lighter(120);
    QColor hoverColor = baseColor.lighter(110);

    QString styleSheet = QString(
                             "QListWidget {"
                             "    border: none;"
                             "    background-color: %1;"
                             "    outline: none;"
                             "    color: white;"
                             "    font-size: 13px;"
                             "    padding: 0px;"
                             "    margin: 0px;"
                             "}"
                             "QListWidget::item {"
                             "    padding: 10px 12px;"
                             "    border: none;"
                             "    color: white;"
                             "    background-color: transparent;"
                             "    margin: 0px;"
                             "}"
                             "QListWidget::item:hover {"
                             "    background-color: %2;"
                             "    color: white;"
                             "}"
                             "QListWidget::item:selected {"
                             "    background-color: %3;"
                             "    color: white;"
                             "}"
                             "QListWidget::item:selected:hover {"
                             "    background-color: %3;"
                             "    color: white;"
                             "}"
                             "QListWidget::item:focus {"
                             "    outline: none;"
                             "}"
                             "QListWidget::item:!selected {"
                             "    background-color: transparent;"
                             "}"
                             ).arg(baseColor.name())
                             .arg(hoverColor.name())
                             .arg(selectedColor.name());

    m_tabList->setStyleSheet(styleSheet);
}

void ApplicationWidgetBase::addTab(const QString &title, QWidget *content)
{
    insertTab(m_tabTitles.size(), title, content);
}

void ApplicationWidgetBase::insertTab(int index, const QString &title, QWidget *content)
{
    if (!content)
        return;

    index = qBound(0, index, m_tabTitles.size());

    QMap<int, QString> newTitles;
    QMap<int, QWidget*> newContents;

    for (int i = 0; i < m_tabTitles.size(); ++i)
    {
        int newIndex = (i < index) ? i : i + 1;
        newTitles[newIndex] = m_tabTitles[i];
        newContents[newIndex] = m_tabContents[i];
    }

    newTitles[index] = title;
    newContents[index] = content;

    m_tabTitles = newTitles;
    m_tabContents = newContents;

    QListWidgetItem *item = new QListWidgetItem(title);
    item->setData(Qt::UserRole, index);
    item->setForeground(QBrush(Qt::white));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_tabList->insertItem(index, item);

    m_stackedWidget->insertWidget(index, content);

    CategoryContentWidget *pCateg = qobject_cast<CategoryContentWidget*>(content);
    if (pCateg)
        pCateg->updateListStyle();

    if (m_tabTitles.size() == 1)
    {
        setCurrentTabIndex(0);
    }
}

void ApplicationWidgetBase::removeTab(int index)
{
    if (index < 0 || index >= m_tabTitles.size())
        return;

    QListWidgetItem *item = m_tabList->takeItem(index);
    delete item;

    QWidget *content = m_stackedWidget->widget(index);
    m_stackedWidget->removeWidget(content);

    QMap<int, QString> newTitles;
    QMap<int, QWidget*> newContents;

    int newIndex = 0;
    for (int i = 0; i < m_tabTitles.size(); ++i)
    {
        if (i != index)
        {
            newTitles[newIndex] = m_tabTitles[i];
            newContents[newIndex] = m_tabContents[i];

            if (newIndex < m_tabList->count())
            {
                m_tabList->item(newIndex)->setData(Qt::UserRole, newIndex);
            }

            newIndex++;
        }
    }

    m_tabTitles = newTitles;
    m_tabContents = newContents;

    if (m_currentIndex == index)
    {
        if (m_tabTitles.isEmpty())
        {
            m_currentIndex = -1;
        }
        else
        {
            setCurrentTabIndex(qMin(m_currentIndex, m_tabTitles.size() - 1));
        }
    }
    else if (m_currentIndex > index)
    {
        m_currentIndex--;
    }
}

void ApplicationWidgetBase::clearTabs()
{
    m_tabList->clear();

    while (m_stackedWidget->count() > 0)
    {
        QWidget *widget = m_stackedWidget->widget(0);
        m_stackedWidget->removeWidget(widget);
    }

    m_tabTitles.clear();
    m_tabContents.clear();
    m_currentIndex = -1;
    updateCategoryTitle();
}

int ApplicationWidgetBase::currentTabIndex() const
{
    return m_currentIndex;
}

void ApplicationWidgetBase::setCurrentTabIndex(int index)
{
    if (index < 0 || index >= m_tabTitles.size())
        return;

    if (m_currentIndex == index)
        return;

    m_currentIndex = index;
    m_tabList->setCurrentRow(index);
    m_stackedWidget->setCurrentIndex(index);
    updateCategoryTitle();

    emit currentTabChanged(index);
}

void ApplicationWidgetBase::setCurrentTab(const QString &title)
{
    for (int i = 0; i < m_tabTitles.size(); ++i)
    {
        if (m_tabTitles[i] == title)
        {
            setCurrentTabIndex(i);
            break;
        }
    }
}

QString ApplicationWidgetBase::tabTitle(int index) const
{
    if (index < 0 || index >= m_tabTitles.size())
        return QString();

    return m_tabTitles[index];
}

QWidget* ApplicationWidgetBase::tabContent(int index) const
{
    if (index < 0 || index >= m_tabContents.size())
        return nullptr;

    return m_tabContents[index];
}

int ApplicationWidgetBase::tabCount() const
{
    return m_tabTitles.size();
}

QColor ApplicationWidgetBase::menuPanelColor() const
{
    return m_menuPanelColor;
}

void ApplicationWidgetBase::setMenuPanelColor(const QColor &color)
{
    m_menuPanelColor = color;
    updateMenuPanelStyle();

    // Обновляем стили всех дочерних CategoryContentWidget
    for (int i = 0; i < m_stackedWidget->count(); ++i)
    {
        CategoryContentWidget *widget = qobject_cast<CategoryContentWidget*>(m_stackedWidget->widget(i));
        if (widget)
            widget->updateStyle();
    }
}

void ApplicationWidgetBase::onTabItemClicked(QListWidgetItem *item)
{
    int index = item->data(Qt::UserRole).toInt();
    setCurrentTabIndex(index);
}

void ApplicationWidgetBase::onBackButtonClicked()
{
    emit backButtonClicked();
}

AppOptionsContentWidget *ApplicationWidgetBase::optionsWidget()
{
    if (!m_pOptionsWidget)
        m_pOptionsWidget = new AppOptionsContentWidget();

    return m_pOptionsWidget;
}