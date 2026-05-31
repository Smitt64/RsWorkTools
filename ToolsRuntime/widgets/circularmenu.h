// CircularMenu.h
#ifndef CIRCULARMENU_H
#define CIRCULARMENU_H

#include <QWidget>
#include <QVector>
#include <QAction>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPen>
#include "ToolsRuntime_global.h"

class QEventLoop;
class TOOLSRUNTIME_EXPORT CircularMenu : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QString title READ title WRITE setTitle)

    // QSS свойства для кастомизации
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor hoverColor READ hoverColor WRITE setHoverColor)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor textHoverColor READ textHoverColor WRITE setTextHoverColor)
    Q_PROPERTY(QColor centerBgColor READ centerBgColor WRITE setCenterBgColor)
    Q_PROPERTY(QColor centerBorderColor READ centerBorderColor WRITE setCenterBorderColor)
    Q_PROPERTY(QColor centerTextColor READ centerTextColor WRITE setCenterTextColor)
    Q_PROPERTY(QColor separatorColor READ separatorColor WRITE setSeparatorColor)
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor)

    Q_PROPERTY(int centerRadius READ centerRadius WRITE setCenterRadius)
    Q_PROPERTY(int iconSize READ iconSize WRITE setIconSize)
    Q_PROPERTY(double textDistance READ textDistance WRITE setTextDistance)
    Q_PROPERTY(double iconDistance READ iconDistance WRITE setIconDistance)

    Q_PROPERTY(QFont textFont READ textFont WRITE setTextFont)
    Q_PROPERTY(QFont centerFont READ centerFont WRITE setCenterFont)

    friend class CircularMenuEventLoop;
public:
    explicit CircularMenu(QWidget *parent = nullptr);
    ~CircularMenu();

    QAction *exec(const QPoint &globalPos);
    QAction *exec(const QPoint &globalPos, int timeoutMs);
    static QAction *exec(const QPoint &globalPos, const QList<QAction*> &actions, QWidget *parent = nullptr);

    void addAction(QAction *action);
    void addActions(const QList<QAction*> &actions);
    void clear();
    void showAt(const QPoint &globalPos);
    void hide();

    void setRadius(int radius);
    int radius() const { return m_radius; }

    QString title() const { return m_Title; }
    void setTitle(const QString &str);

    // QSS свойства - геттеры/сеттеры
    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor &color);

    QColor hoverColor() const { return m_hoverColor; }
    void setHoverColor(const QColor &color);

    QColor borderColor() const { return m_borderColor; }
    void setBorderColor(const QColor &color);

    QColor textColor() const { return m_textColor; }
    void setTextColor(const QColor &color);

    QColor textHoverColor() const { return m_textHoverColor; }
    void setTextHoverColor(const QColor &color);

    QColor centerBgColor() const { return m_centerBgColor; }
    void setCenterBgColor(const QColor &color);

    QColor centerBorderColor() const { return m_centerBorderColor; }
    void setCenterBorderColor(const QColor &color);

    QColor centerTextColor() const { return m_centerTextColor; }
    void setCenterTextColor(const QColor &color);

    QColor separatorColor() const { return m_separatorColor; }
    void setSeparatorColor(const QColor &color);

    QColor shadowColor() const { return m_shadowColor; }
    void setShadowColor(const QColor &color);

    int centerRadius() const { return m_centerRadius; }
    void setCenterRadius(int radius);

    int iconSize() const { return m_iconSize; }
    void setIconSize(int size);

    double textDistance() const { return m_textDistance; }
    void setTextDistance(double distance);

    double iconDistance() const { return m_iconDistance; }
    void setIconDistance(double distance);

    QFont textFont() const { return m_textFont; }
    void setTextFont(const QFont &font);

    QFont centerFont() const { return m_centerFont; }
    void setCenterFont(const QFont &font);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    struct MenuItem
    {
        QAction *action = nullptr;
        double angleStart = 0;
        double angleEnd = 0;
        QRectF iconRect;
        QRectF textRect;
    };

    void drawSector(QPainter &painter, double userAngleStart, double userAngleEnd, const QBrush &brush, const QPen &pen = QPen()) const;
    bool pointInSector(const QPoint &pos, double userAngleStart, double userAngleEnd) const;

    static double userToQtAngle(double userAngle);
    static QPointF positionFromUserAngle(double userAngle, double distance, const QPoint &center);

    void updateGeometry();
    int getItemIndex(const QPoint &pos) const;
    void startHideAnimation();

    QVector<MenuItem> m_items;
    int m_radius = 120;
    int m_hoveredIndex = -1;
    qreal m_opacity = 0.0;
    QPoint m_center;
    bool m_isShowing = false;

    qreal opacity() const { return m_opacity; }
    void setOpacity(qreal opacity)
    {
        if (m_opacity != opacity)
        {
            m_opacity = opacity;
            update();
        }
    }

    QPropertyAnimation *m_animation = nullptr;
    QTimer *m_hoverTimer = nullptr;
    QString m_Title;

    // QSS настройки с значениями по умолчанию
    QColor m_backgroundColor = QColor(60, 60, 60, 240);
    QColor m_hoverColor = QColor(60, 130, 220, 180);
    QColor m_borderColor = QColor(150, 150, 150, 200);
    QColor m_textColor = QColor(200, 200, 205);
    QColor m_textHoverColor = Qt::white;
    QColor m_centerBgColor = QColor(50, 50, 50, 240);
    QColor m_centerBorderColor = QColor(150, 150, 150);
    QColor m_centerTextColor = Qt::white;
    QColor m_separatorColor = QColor(200, 200, 200, 120);
    QColor m_shadowColor = QColor(0, 0, 0, 60);

    int m_centerRadius = 30;
    int m_iconSize = 28;
    double m_textDistance = 0.75;
    double m_iconDistance = 0.45;

    QFont m_textFont = QFont("Segoe UI", 9);
    QFont m_centerFont = QFont("Arial", 10, QFont::Bold);

    QEventLoop *m_pCurrentEventLoop = nullptr;

signals:
    void triggered(QAction *action);
    void aboutToHide();
};

#endif // CIRCULARMENU_H