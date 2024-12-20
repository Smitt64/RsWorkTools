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

#ifndef QWINDOWSXPSTYLE_P_H
#define QWINDOWSXPSTYLE_P_H

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
#include <QtWidgets/private/qwindowsstyle_p.h>

QT_BEGIN_NAMESPACE

class QWindowsXPStylePrivate;
class QWindowsXPStyle : public QWindowsStyle
{
    Q_OBJECT
public:
    QWindowsXPStyle();
    QWindowsXPStyle(QWindowsXPStylePrivate &dd);
    virtual ~QWindowsXPStyle() override;

    void unpolish(QApplication*) override;
    void polish(QApplication*) override;
    void polish(QWidget*) override;
    void polish(QPalette&) override;
    void unpolish(QWidget*) override;

    void drawPrimitive(PrimitiveElement pe, const QStyleOption *option, QPainter *p,
                       const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *p,
                     const QWidget *wwidget = nullptr) const override;
    QRect subElementRect(SubElement r, const QStyleOption *option,
                         const QWidget *widget = nullptr) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *option, SubControl sc,
                         const QWidget *widget = nullptr) const override;
    void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *option, QPainter *p,
                            const QWidget *widget = nullptr) const override;
    QSize sizeFromContents(ContentsType ct, const QStyleOption *option, const QSize &contentsSize,
                           const QWidget *widget = nullptr) const override;
    int pixelMetric(PixelMetric pm, const QStyleOption *option = nullptr,
                    const QWidget *widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption *option = nullptr,
                  const QWidget *widget = nullptr,
                  QStyleHintReturn *returnData = nullptr) const override;

    QPalette standardPalette() const override;
    QPixmap standardPixmap(StandardPixmap standardIcon, const QStyleOption *option,
                           const QWidget *widget = nullptr) const override;
    QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption *option = nullptr,
                       const QWidget *widget = nullptr) const override;

private:
    Q_DISABLE_COPY_MOVE(QWindowsXPStyle)
    Q_DECLARE_PRIVATE(QWindowsXPStyle)
    friend class QStyleFactory;
};

QT_END_NAMESPACE

#endif // QWINDOWSXPSTYLE_P_H
