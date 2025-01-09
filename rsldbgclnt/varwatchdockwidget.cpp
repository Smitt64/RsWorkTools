#include "varwatchdockwidget.h"
#include "models/varwatchmodel.h"
#include <QTreeView>

VarWatchDockWidget::VarWatchDockWidget() :
    StdViewDockWidget()
{
    QTreeView *tree = view();
    connect(tree, &QTreeView::expanded, [=](const QModelIndex &index)
    {
        int indx = index.data(VarWatchModel::IndexRole).toInt();
        qint64 stack = index.data(VarWatchModel::IndexRole).value<qint64>();

        emit expandVariable(indx, stack);
    });
}

VarWatchDockWidget::~VarWatchDockWidget()
{

}

void VarWatchDockWidget::setModel(QAbstractItemModel *model)
{
    StdViewDockWidget::setModel(model);
}
