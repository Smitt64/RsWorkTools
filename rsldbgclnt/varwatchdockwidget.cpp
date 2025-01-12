#include "varwatchdockwidget.h"
#include "models/varwatchmodel.h"
#include <QTreeView>

VarWatchDockWidget::VarWatchDockWidget() :
    StdViewDockWidget()
{
    QTreeView *tree = view();
    tree->setRootIsDecorated(true);
    tree->resetIndentation();
    //tree->setIndentation(10);
    connect(tree, &QTreeView::expanded, [=](const QModelIndex &index)
    {
        int indx = index.data(VarWatchModel::IndexRole).toInt();
        qint64 stack = index.data(VarWatchModel::IndexRole).value<qint64>();
        bool RealHasChild = index.data(VarWatchModel::RealHasChild).toBool();

        if (!RealHasChild)
            emit expandVariable(indx, stack);
    });
}

VarWatchDockWidget::~VarWatchDockWidget()
{

}

void VarWatchDockWidget::setModel(QAbstractItemModel *model)
{
    StdViewDockWidget::setModel(model);

    connect((VarWatchModel*)model, &VarWatchModel::expand, view(), &QTreeView::expand);
}
