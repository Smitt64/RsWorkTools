#ifndef CIRCULARMENU_H
#define CIRCULARMENU_H

#include <QWidget>
#include <QVector>
#include <QAction>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPen>
#include "ToolsRuntime_global.h"

class TOOLSRUNTIME_EXPORT CircularMenu : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QString title READ title WRITE setTitle)

public:
    explicit CircularMenu(QWidget *parent = nullptr);
    ~CircularMenu();

    void addAction(QAction *action);
    void addActions(const QList<QAction*> &actions);
    void clear();
    void showAt(const QPoint &globalPos);
    void hide();

    void setRadius(int radius);
    int radius() const { return m_radius; }

    QString title() const { return m_Title; }
    void setTitle(const QString &str);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    struct MenuItem
    {
        QAction *action = nullptr;
        double angleStart = 0;  // 0° = вверх, по часовой
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

signals:
    void triggered(QAction *action);
    void aboutToHide();
};

#endif // CIRCULARMENU_H