#ifndef LOGDOCKWIDGET_H
#define LOGDOCKWIDGET_H

#include <QDockWidget>

class QTreeView;
class QAbstractItemModel;
class LogDockWidget : public QDockWidget
{
public:
    LogDockWidget();
    virtual ~LogDockWidget();

    void setModel(QAbstractItemModel *model);

private:
    QScopedPointer<QTreeView> m_View;
};

#endif // LOGDOCKWIDGET_H
