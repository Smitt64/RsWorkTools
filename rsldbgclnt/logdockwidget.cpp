#include "logdockwidget.h"
#include "models/logeventmodel.h"
#include <QTreeView>
#include <QHeaderView>

LogDockWidget::LogDockWidget()
{
    m_View.reset(new QTreeView());
    m_View->setIndentation(0);
    m_View->setRootIsDecorated(false);
    setWidget(m_View.data());
}

LogDockWidget::~LogDockWidget()
{

}

void LogDockWidget::setModel(QAbstractItemModel *model)
{
    m_View->setModel(model);

    m_View->header()->resizeSection(LogEventModel::ColumnTime, 135);
    m_View->header()->resizeSection(LogEventModel::ColumnType, 30);
    m_View->header()->resizeSection(LogEventModel::ColumnContext, 50);
}
