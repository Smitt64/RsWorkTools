#ifndef VARWATCHDOCKWIDGET_H
#define VARWATCHDOCKWIDGET_H

#include "stdviewdockwidget.h"

class VarWatchDockWidget : public StdViewDockWidget
{
    Q_OBJECT
public:
    VarWatchDockWidget();
    virtual ~VarWatchDockWidget();

    virtual void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

signals:
    void expandVariable(int, qint64);
};

#endif // VARWATCHDOCKWIDGET_H
