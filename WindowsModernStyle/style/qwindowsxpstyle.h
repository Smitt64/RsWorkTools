/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWINDOWSXPSTYLE_H
#define QWINDOWSXPSTYLE_H

#include <style/qwindowsstyle.h>

class QWindowsXPStylePrivate;
class QWindowsXPStyle : public QWindowsStyle
{
    Q_OBJECT
public:
    QWindowsXPStyle();
    QWindowsXPStyle(QWindowsXPStylePrivate &dd);
    ~QWindowsXPStyle();

    void unpolish(QApplication*);
    void polish(QApplication*);
    void polish(QWidget*);
    void polish(QPalette&);
    void unpolish(QWidget*);

    void drawPrimitive(PrimitiveElement pe, const QStyleOption *option, QPainter *p,
                       const QWidget *widget = 0) const;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *p,
                     const QWidget *wwidget = 0) const;
    QRect subElementRect(SubElement r, const QStyleOption *option, const QWidget *widget = 0) const;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *option, SubControl sc,
                         const QWidget *widget = 0) const;
    void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *option, QPainter *p,
                            const QWidget *widget = 0) const;
    QSize sizeFromContents(ContentsType ct, const QStyleOption *option, const QSize &contentsSize,
                           const QWidget *widget = 0) const;
    int pixelMetric(PixelMetric pm, const QStyleOption *option = 0,
                    const QWidget *widget = 0) const;
    int styleHint(StyleHint hint, const QStyleOption *option = 0, const QWidget *widget = 0,
                  QStyleHintReturn *returnData = 0) const;

    QPalette standardPalette() const;
    QPixmap standardPixmap(StandardPixmap standardIcon, const QStyleOption *option,
                           const QWidget *widget = 0) const;

protected Q_SLOTS:
    QIcon standardIconImplementation(StandardPixmap standardIcon, const QStyleOption *option,
                                     const QWidget *widget = 0) const;

private:
    Q_DISABLE_COPY(QWindowsXPStyle)
    Q_DECLARE_PRIVATE(QWindowsXPStyle)
    friend class QStyleFactory;
    void *reserved;
};

#endif // QWINDOWSXPSTYLE_H
