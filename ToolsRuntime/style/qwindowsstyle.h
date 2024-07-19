// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef QWINDOWSSTYLE_H
#define QWINDOWSSTYLE_H

#include <QCommonStyle>
#include "ToolsRuntime_global.h"
class QWindowsStylePrivate;

class TOOLSRUNTIME_EXPORT QWindowsStyle : public QCommonStyle
{
    Q_OBJECT
public:
    QWindowsStyle();
    ~QWindowsStyle();

    void polish(QApplication*);
    void unpolish(QApplication*);

    void polish(QWidget*);
    void unpolish(QWidget*);

    void polish(QPalette &);

    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p,
                       const QWidget *w = 0) const;
    void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p,
                     const QWidget *w = 0) const;
    QRect subElementRect(SubElement r, const QStyleOption *opt, const QWidget *widget = 0) const;
    void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p,
                            const QWidget *w = 0) const;
    QSize sizeFromContents(ContentsType ct, const QStyleOption *opt,
                           const QSize &contentsSize, const QWidget *widget = 0) const;

    int pixelMetric(PixelMetric pm, const QStyleOption *option = 0, const QWidget *widget = 0) const;

    int styleHint(StyleHint hint, const QStyleOption *opt = 0, const QWidget *widget = 0,
                  QStyleHintReturn *returnData = 0) const;

    QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt,
                           const QWidget *widget = 0) const;

protected Q_SLOTS:
    QIcon standardIconImplementation(StandardPixmap standardIcon, const QStyleOption *option,
                                     const QWidget *widget = 0) const;

protected:
    bool eventFilter(QObject *o, QEvent *e);
    void timerEvent(QTimerEvent *event);
    QWindowsStyle(QWindowsStylePrivate &dd);

private:
    Q_DISABLE_COPY(QWindowsStyle)
    Q_DECLARE_PRIVATE(QWindowsStyle)
    void *reserved;
};

#endif // QWINDOWSSTYLE_H
