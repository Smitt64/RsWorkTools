// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtWidgets module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWINDOWSVISTASTYLE_P_H
#define QWINDOWSVISTASTYLE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtWidgets/private/qtwidgetsglobal_p.h>
#include "qwindowsxpstyle_p.h"

QT_BEGIN_NAMESPACE

class QWindowsVistaStylePrivate;
class QWindowsVistaStyle : public QWindowsXPStyle
{
    Q_OBJECT
public:
    QWindowsVistaStyle();
    virtual ~QWindowsVistaStyle() override;

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter,
                       const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const override;

    QRect subElementRect(SubElement element, const QStyleOption *option,
                         const QWidget *widget) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget) const override;

    SubControl hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                                     const QPoint &pos, const QWidget *widget = nullptr) const override;

    QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption *option = nullptr,
                       const QWidget *widget = nullptr) const override;
    QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt,
                           const QWidget *widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr,
                    const QWidget *widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption *opt = nullptr,
                  const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const override;

    void polish(QWidget *widget) override;
    void unpolish(QWidget *widget) override;
    void polish(QPalette &pal) override;
    void polish(QApplication *app) override;
    void unpolish(QApplication *app) override;
    QPalette standardPalette() const override;

private:
    Q_DISABLE_COPY_MOVE(QWindowsVistaStyle)
    Q_DECLARE_PRIVATE(QWindowsVistaStyle)
    friend class QStyleFactory;
};

QT_END_NAMESPACE

#endif // QWINDOWSVISTASTYLE_P_H
