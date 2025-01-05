#include "stdviewdockwidget.h"
#include "models/logeventmodel.h"
#include <QTreeView>
#include <QHeaderView>

StdViewDockWidget::StdViewDockWidget() :
    QDockWidget()
{
    m_View.reset(new QTreeView());
    m_View->setIndentation(0);
    m_View->setRootIsDecorated(false);
    setWidget(m_View.data());

    //setWindowTitle(tr("Debugger log"));
}

StdViewDockWidget::~StdViewDockWidget()
{

}

void StdViewDockWidget::setModel(QAbstractItemModel *model)
{
    m_View->setModel(model);

    connect(model, &QAbstractItemModel::rowsInserted, this, &StdViewDockWidget::rowsInserted);
}

void StdViewDockWidget::rowsInserted(const QModelIndex &parent, int first, int last)
{
    if (m_AutoScrollToBottom)
        m_View->scrollToBottom();
}

void StdViewDockWidget::setAutoScrollToBottom(bool value)
{
    m_AutoScrollToBottom = value;
}

QTreeView *StdViewDockWidget::view()
{
    return m_View.data();
}
