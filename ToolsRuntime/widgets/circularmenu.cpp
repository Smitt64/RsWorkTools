// CircularMenu.cpp (обновленные методы)
#include "CircularMenu.h"
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QRadialGradient>
#include <QPropertyAnimation>
#include <QTimer>
#include <QScreen>
#include <QFontMetrics>
#include <QtMath>
#include <cmath>
#include <QDebug>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ---------------------------------------------------------------

class CircularMenuEventLoop : public QEventLoop
{
public:
    CircularMenuEventLoop(CircularMenu *menu, int timeoutMs = 0)
        : m_menu(menu), m_timeoutMs(timeoutMs)
    {
        // Подключаем сигналы для завершения цикла
        connect(menu, &CircularMenu::triggered, this, &CircularMenuEventLoop::onActionTriggered);
        connect(menu, &CircularMenu::aboutToHide, this, &CircularMenuEventLoop::onMenuHidden);

        if (m_timeoutMs > 0)
        {
            QTimer::singleShot(m_timeoutMs, this, [this]() {
                if (isRunning())
                {
                    m_selectedAction = nullptr;
                    quit();
                }
            });
        }
    }

    QAction *exec(const QPoint &globalPos)
    {
        if (!m_menu || m_menu->m_items.isEmpty())
            return nullptr;

        m_selectedAction = nullptr;
        m_menu->showAt(globalPos);

        // Запускаем блокирующий цикл событий
        QEventLoop::exec();

        // Скрываем меню если оно еще видимо
        if (m_menu && m_menu->isVisible())
            m_menu->hide();

        return m_selectedAction;
    }

private slots:
    void onActionTriggered(QAction *action)
    {
        m_selectedAction = action;
        quit();
    }

    void onMenuHidden()
    {
        if (m_selectedAction == nullptr)
            quit();
    }

private:
    CircularMenu *m_menu;
    QAction *m_selectedAction = nullptr;
    int m_timeoutMs = 0;
};

// ---------------------------------------------------------------

// Пользовательская: 0° = вверх (12 часов), ↻ по часовой
// Qt/экранная: 0° = вправо (3 часа), ↺ против часовой, единицы = 1/16°

double CircularMenu::userToQtAngle(double userAngle)
{
    // Конвертация: 90 - user, нормализация к [0, 360)
    return std::fmod(90.0 - userAngle + 360.0, 360.0);
}

QPointF CircularMenu::positionFromUserAngle(double userAngle, double distance, const QPoint &center)
{
    // Для 0°=вверх: x = sin(α), y = -cos(α) (Y вниз в экранной системе)
    double rad = userAngle * M_PI / 180.0;
    return center + QPointF(std::sin(rad) * distance, -std::cos(rad) * distance);
}

void CircularMenu::drawSector(QPainter &painter, double userAngleStart, double userAngleEnd, const QBrush &brush, const QPen &pen) const
{
    if (userAngleEnd <= userAngleStart)
        return;

    QRectF ellipseRect(m_center.x() - m_radius, m_center.y() - m_radius,
                       m_radius * 2, m_radius * 2);

    // Прямое преобразование: для Qt угол 0° = 3 часа
    // Нам нужно: 0° = 12 часов, поэтому вычитаем из 90
    double startAngle = 90.0 - userAngleEnd;  // Важно: end, потому что sweep будет положительным
    double spanAngle = userAngleEnd - userAngleStart;

    if (startAngle < 0)
        startAngle += 360.0;

    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawPie(ellipseRect, static_cast<int>(startAngle * 16), static_cast<int>(spanAngle * 16));
}

bool CircularMenu::pointInSector(const QPoint &pos, double userAngleStart, double userAngleEnd) const
{
    QPointF rel = pos - m_center;
    double dist = std::hypot(rel.x(), rel.y());

    if (dist < m_centerRadius || dist > m_radius)
        return false;

    double screenAngle = std::atan2(rel.y(), rel.x()) * 180.0 / M_PI;
    if (screenAngle < 0)
        screenAngle += 360.0;

    double userAngle = std::fmod(screenAngle + 90.0, 360.0);
    if (userAngle < 0) userAngle += 360.0;

    // Проверка диапазона угла
    if (userAngleStart <= userAngleEnd)
        return userAngle >= userAngleStart && userAngle < userAngleEnd;
    else
    {
        // Переход через 360° (не должен случаться при равномерном делении)
        return userAngle >= userAngleStart || userAngle < userAngleEnd;
    }
}

CircularMenu::CircularMenu(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setStyleSheet("background: transparent;");

    m_Title = tr("Menu");
    m_animation = new QPropertyAnimation(this, "opacity", this);
    m_animation->setDuration(150);
    m_animation->setEasingCurve(QEasingCurve::OutQuad);

    m_hoverTimer = new QTimer(this);
    m_hoverTimer->setSingleShot(true);
    m_hoverTimer->setInterval(25);
    connect(m_hoverTimer, &QTimer::timeout, this, [this]() { update(); });
}

CircularMenu::~CircularMenu()
{
    clear();
}

// QSS сеттеры
void CircularMenu::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    update();
}

void CircularMenu::setHoverColor(const QColor &color)
{
    m_hoverColor = color;
    update();
}

void CircularMenu::setBorderColor(const QColor &color)
{
    m_borderColor = color;
    update();
}

void CircularMenu::setTextColor(const QColor &color)
{
    m_textColor = color;
    update();
}

void CircularMenu::setTextHoverColor(const QColor &color)
{
    m_textHoverColor = color;
    update();
}

void CircularMenu::setCenterBgColor(const QColor &color)
{
    m_centerBgColor = color;
    update();
}

void CircularMenu::setCenterBorderColor(const QColor &color)
{
    m_centerBorderColor = color;
    update();
}

void CircularMenu::setCenterTextColor(const QColor &color)
{
    m_centerTextColor = color;
    update();
}

void CircularMenu::setSeparatorColor(const QColor &color)
{
    m_separatorColor = color;
    update();
}

void CircularMenu::setShadowColor(const QColor &color)
{
    m_shadowColor = color;
    update();
}

void CircularMenu::setCenterRadius(int radius)
{
    m_centerRadius = qBound(20, radius, 60);
    updateGeometry();
}

void CircularMenu::setIconSize(int size)
{
    m_iconSize = qBound(16, size, 48);
    updateGeometry();
}

void CircularMenu::setTextDistance(double distance)
{
    m_textDistance = qBound(0.3, distance, 0.95);
    updateGeometry();
}

void CircularMenu::setIconDistance(double distance)
{
    m_iconDistance = qBound(0.3, distance, 0.8);
    updateGeometry();
}

void CircularMenu::setTextFont(const QFont &font)
{
    m_textFont = font;
    updateGeometry();
}

void CircularMenu::setCenterFont(const QFont &font)
{
    m_centerFont = font;
    update();
}

void CircularMenu::addAction(QAction *action)
{
    if (!action) return;

    MenuItem item;
    item.action = action;
    m_items.append(item);

    connect(action, &QAction::destroyed, this, [this, action]()
    {
        for (int i = m_items.size() - 1; i >= 0; --i)
        {
            if (m_items[i].action == action)
            {
                m_items.remove(i);
                break;
            }
        }

        updateGeometry();
    });

    updateGeometry();
}

void CircularMenu::addActions(const QList<QAction *> &actions)
{
    for (QAction *a : qAsConst(actions))
        addAction(a);
}

void CircularMenu::clear()
{
    for (const auto &item : qAsConst(m_items))
    {
        if (item.action && item.action->parent() == this)
        {
            disconnect(item.action, &QAction::destroyed, this, nullptr);
            delete item.action;
        }
    }

    m_items.clear();
    m_hoveredIndex = -1;
    updateGeometry();
}

void CircularMenu::setRadius(int radius)
{
    m_radius = qBound(80, radius, 250);
    updateGeometry();
}

void CircularMenu::updateGeometry()
{
    if (m_items.isEmpty())
    {
        resize(0, 0);
        return;
    }

    m_radius = qBound(100, 90 + static_cast<int>(m_items.size() * 6), 220);
    const int margin = 15;
    resize((m_radius + margin) * 2, (m_radius + margin) * 2);
    m_center = QPoint(width() / 2, height() / 2);

    double step = 360.0 / m_items.size();

    for (int i = 0; i < m_items.size(); ++i)
    {
        MenuItem &item = m_items[i];
        item.angleStart = i * step;
        item.angleEnd = item.angleStart + step;

        // Позиции контента
        double midAngle = item.angleStart + step * 0.5;

        // Иконка с использованием m_iconDistance и m_iconSize
        QPointF iconPos = positionFromUserAngle(midAngle, m_radius * m_iconDistance, m_center);
        item.iconRect = QRectF(iconPos.x() - m_iconSize / 2,
                               iconPos.y() - m_iconSize / 2,
                               m_iconSize, m_iconSize);

        // Текст с использованием m_textDistance
        QPointF textPos = positionFromUserAngle(midAngle, m_radius * m_textDistance, m_center);
        QString txt = item.action->text().remove('&');
        QFontMetrics fm(m_textFont);
        int textWidth = fm.horizontalAdvance(txt);
        int textHeight = fm.height();

        item.textRect = QRectF(textPos.x() - textWidth / 2.0 - 6,
                               textPos.y() - textHeight / 2.0 - 3,
                               textWidth + 12, textHeight + 6);
    }
}

int CircularMenu::getItemIndex(const QPoint &pos) const
{
    if (m_items.isEmpty())
        return -1;

    for (int i = 0; i < m_items.size(); ++i)
    {
        if (pointInSector(pos, m_items[i].angleStart, m_items[i].angleEnd))
            return i;
    }
    return -1;
}

void CircularMenu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setClipRect(event->rect());
    painter.setOpacity(m_opacity);

    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    if (m_items.isEmpty())
    {
        painter.setBrush(m_centerBgColor);
        painter.setPen(QPen(m_centerBorderColor, 2));
        painter.drawEllipse(m_center, m_centerRadius, m_centerRadius);
        painter.setPen(m_centerTextColor);
        painter.setFont(m_centerFont);
        painter.drawText(QRect(m_center.x() - 25, m_center.y() - 12, 50, 24),
                         Qt::AlignCenter, "Menu");
        return;
    }

    // Тень
    QPainterPath shadowPath;
    shadowPath.addEllipse(m_center, m_radius + 5, m_radius + 5);
    painter.fillPath(shadowPath, m_shadowColor);

    // Внешнее кольцо (рамка)
    QPainterPath outerPath;
    outerPath.addEllipse(m_center, m_radius + 2, m_radius + 2);
    painter.fillPath(outerPath, m_backgroundColor.darker(120));

    // Основное кольцо с градиентом
    QPainterPath mainPath;
    mainPath.addEllipse(m_center, m_radius, m_radius);

    QRadialGradient gradient(m_center, m_radius);
    gradient.setColorAt(0.0, m_backgroundColor.lighter(110));
    gradient.setColorAt(0.7, m_backgroundColor);
    gradient.setColorAt(1.0, m_backgroundColor.darker(110));
    painter.fillPath(mainPath, gradient);

    painter.setPen(QPen(m_borderColor, 2));
    painter.drawPath(mainPath);

    if (m_hoveredIndex >= 0 && m_hoveredIndex < m_items.size())
    {
        const auto &item = m_items[m_hoveredIndex];
        drawSector(painter, item.angleStart, item.angleEnd, QBrush(m_hoverColor), Qt::NoPen);
    }

    // Разделительные линии
    painter.setPen(QPen(m_separatorColor, 1.5));
    for (const auto &item : qAsConst(m_items))
    {
        QPointF end = positionFromUserAngle(item.angleStart, m_radius - 2, m_center);
        painter.drawLine(m_center, end);
    }

    // Контент (иконки + текст)
    for (int i = 0; i < m_items.size(); ++i)
    {
        const auto &item = m_items[i];
        bool isHovered = (i == m_hoveredIndex);

        if (!item.action->icon().isNull())
        {
            QPixmap pix = item.action->icon().pixmap(m_iconSize, m_iconSize);
            if (!pix.isNull())
                painter.drawPixmap(item.iconRect.toRect(), pix);
        }

        painter.setFont(m_textFont);
        if (isHovered)
        {
            QFont boldFont = m_textFont;
            boldFont.setBold(true);
            painter.setFont(boldFont);
            painter.setPen(m_textHoverColor);
        }
        else
        {
            painter.setPen(m_textColor);
        }

        painter.drawText(item.textRect.toRect(), Qt::AlignCenter,
                         item.action->text().remove('&'));
    }

    // Центральная кнопка
    painter.setBrush(m_centerBgColor);
    painter.setPen(QPen(m_centerBorderColor, 2));
    painter.drawEllipse(m_center, m_centerRadius, m_centerRadius);

    painter.setPen(m_centerTextColor);
    painter.setFont(m_centerFont);
    painter.drawText(QRect(m_center.x() - m_centerRadius,
                           m_center.y() - m_centerRadius / 2,
                           m_centerRadius * 2, m_centerRadius),
                     Qt::AlignCenter, m_Title);
}

void CircularMenu::showAt(const QPoint &globalPos)
{
    if (m_items.isEmpty()) return;

    //m_opacity = 0.0;
    m_isShowing = true;
    updateGeometry();

    QRect screenGeo;
    if (QScreen *scr = QGuiApplication::screenAt(globalPos))
        screenGeo = scr->availableGeometry();
    else
        screenGeo = QGuiApplication::primaryScreen()->availableGeometry();

    int x = qBound(screenGeo.left(), globalPos.x() - width() / 2, screenGeo.right() - width());
    int y = qBound(screenGeo.top(),  globalPos.y() - height() / 2, screenGeo.bottom() - height());

    move(x, y);
    m_center = QPoint(width() / 2, height() / 2);
    m_hoveredIndex = -1;
    m_opacity = 1.0;

    show();
    raise();
    setFocus(Qt::MouseFocusReason);

    //m_animation->disconnect();
    /*m_animation->stop();
    m_animation->setStartValue(0.0);
    m_animation->setEndValue(1.0);
    m_animation->start();*/
    //setOpacity()

    QTimer::singleShot(150, this, [this]() { m_isShowing = false; });
}

void CircularMenu::hide()
{
    qDebug() << "hide()";
    if (!isVisible())
        return;

    startHideAnimation();
}

void CircularMenu::startHideAnimation()
{
    /*if (m_animation->state() == QPropertyAnimation::Running)
        m_animation->stop();

    m_animation->setStartValue(m_opacity);
    m_animation->setEndValue(0.0);

    connect(m_animation, &QPropertyAnimation::finished, this, [this]()
    {
        QWidget::hide();
        emit aboutToHide();
        m_hoveredIndex = -1;
        m_opacity = 0.0;
        m_animation->stop();
    }, Qt::UniqueConnection);

    m_animation->start();*/
    emit aboutToHide();
    QWidget::hide();
}

void CircularMenu::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        int idx = getItemIndex(event->pos());
        if (idx != -1 && m_items[idx].action && m_items[idx].action->isEnabled())
        {
            emit triggered(m_items[idx].action);
            m_items[idx].action->trigger();
        }

        startHideAnimation();
    }
    else if (event->button() == Qt::RightButton)
        startHideAnimation();

    QWidget::mousePressEvent(event);
}

void CircularMenu::mouseMoveEvent(QMouseEvent *event)
{
    int idx = getItemIndex(event->pos());
    idx = (idx == -1) ? -1 : qBound(0, idx, m_items.size() - 1);

    if (idx != m_hoveredIndex)
    {
        m_hoveredIndex = idx;
        setCursor(idx != -1 ? Qt::PointingHandCursor : Qt::ArrowCursor);

        m_hoverTimer->start();
    }

    QWidget::mouseMoveEvent(event);
}

void CircularMenu::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape: startHideAnimation(); break;
    case Qt::Key_Return: case Qt::Key_Enter: case Qt::Key_Space:
        if (m_hoveredIndex != -1 && m_hoveredIndex < m_items.size() && m_items[m_hoveredIndex].action)
        {
            emit triggered(m_items[m_hoveredIndex].action);
            m_items[m_hoveredIndex].action->trigger();
        }

        startHideAnimation();
        break;

    case Qt::Key_Up: case Qt::Key_Left:
        if (!m_items.isEmpty())
        {
            m_hoveredIndex = (m_hoveredIndex - 1 + m_items.size()) % m_items.size();
            m_hoverTimer->start(); update();
        }
        break;

    case Qt::Key_Down: case Qt::Key_Right:
        if (!m_items.isEmpty())
        {
            m_hoveredIndex = (m_hoveredIndex + 1) % m_items.size();
            m_hoverTimer->start(); update();
        }
        break;

    default: QWidget::keyPressEvent(event);
    }
}

void CircularMenu::showEvent(QShowEvent *event)
{
    qDebug() << "CircularMenu::showEvent";
    QWidget::showEvent(event);
    m_center = QPoint(width() / 2, height() / 2);
}

void CircularMenu::hideEvent(QHideEvent *event)
{
    emit aboutToHide();
    qDebug() << "CircularMenu::hideEvent";
    m_pCurrentEventLoop->quit();
    QWidget::hideEvent(event);
}

void CircularMenu::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_center = QPoint(width() / 2, height() / 2);
    if (!m_items.isEmpty())
        updateGeometry();
}

void CircularMenu::setTitle(const QString &str)
{
    m_Title = str;
    update();
}

QAction *CircularMenu::exec(const QPoint &globalPos)
{
    if (m_items.isEmpty())
        return nullptr;

    //QEventLoop loop;
    m_pCurrentEventLoop = new QEventLoop(this);
    QAction *selectedAction = nullptr;

    auto triggeredConnection = connect(this, &CircularMenu::triggered, [&](QAction *action)
    {
        selectedAction = action;
        qDebug() << "1 m_pCurrentEventLoop->quit();";
        m_pCurrentEventLoop->quit();
    });

    auto aboutToHideConnection = connect(this, &CircularMenu::aboutToHide, [&]()
    {
        qDebug() << "2 m_pCurrentEventLoop->quit();";
        m_pCurrentEventLoop->quit();
    });

    showAt(globalPos);
    m_pCurrentEventLoop->exec();

    disconnect(triggeredConnection);
    disconnect(aboutToHideConnection);

    if (isVisible())
        hide();

    return selectedAction;
}

QAction *CircularMenu::exec(const QPoint &globalPos, int timeoutMs)
{
    if (m_items.isEmpty())
        return nullptr;

    QEventLoop loop;
    QAction *selectedAction = nullptr;

    QTimer timeoutTimer;
    if (timeoutMs > 0)
    {
        timeoutTimer.setSingleShot(true);
        connect(&timeoutTimer, &QTimer::timeout, &loop, &QEventLoop::quit);
        timeoutTimer.start(timeoutMs);
    }

    auto triggeredConnection = connect(this, &CircularMenu::triggered, [&](QAction *action)
    {
       selectedAction = action;
       loop.quit();
    });

    auto aboutToHideConnection = connect(this, &CircularMenu::aboutToHide, [&]()
    {
        loop.quit();
    });

    showAt(globalPos);
    loop.exec();

    disconnect(triggeredConnection);
    disconnect(aboutToHideConnection);

    if (isVisible())
        hide();

    return selectedAction;
}

QAction *CircularMenu::exec(const QPoint &globalPos, const QList<QAction *> &actions, QWidget *parent)
{
    CircularMenu menu(parent);
    menu.addActions(actions);
    return menu.exec(globalPos);
}