#include "callstackmodel.h"
#include <dbgserverproto.h>
#include <QFileInfo>
#include <QIcon>
#include <QTextCodec>

CallStackModel::CallStackModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

CallStackModel::~CallStackModel()
{

}

void CallStackModel::clear()
{
    beginResetModel();
    m_Items.clear();
    endResetModel();
}

int CallStackModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_Items.size();
}

int CallStackModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant CallStackModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == ColumnLevel)
            return index.row() + 1;
        if (index.column() == ColumnFunction)
            return m_Items[index.row()].func;
        if (index.column() == ColumnFile)
            return m_Items[index.row()].file;
        if (index.column() == ColumnLine)
            return m_Items[index.row()].line;
    }
    else if (role == Qt::DecorationRole && index.row() == 0 && index.column() == ColumnLevel)
        return QIcon::fromTheme("CurrentLine");
    else if (role == Qt::TextAlignmentRole && (index.column() == ColumnLevel || index.column() == ColumnLine))
        return (int)(Qt::AlignRight | Qt::AlignVCenter);

    return QVariant();
}

void CallStackModel::append(Qt::HANDLE stack)
{
    QTextCodec *oem866 = QTextCodec::codecForName("IBM 866");

    beginInsertRows(QModelIndex(), m_Items.size(), m_Items.size());

    DBG_UPDATSTACK *upd = (DBG_UPDATSTACK*)stack;

    StackItem item;
    item.fullfilename = oem866->toUnicode(upd->fullfilename);
    item.func = oem866->toUnicode(upd->func);

    item.len = upd->len;
    item.line = upd->line;
    item.offs = upd->line;

    QFileInfo fi(item.fullfilename);
    item.file = fi.fileName();

    m_Items.append(item);
    endInsertRows();
}

QVariant CallStackModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QVector<QString> Headers = {
        tr("Level"),
        tr("Function"),
        tr("File"),
        tr("Line")
    };

    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
            return Headers[section];
    }

    return QVariant();
}
