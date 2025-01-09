#include "varwatchmodel.h"
#include "dbgserverproto.h"
#include "mainwindow.h"
#include <QDebug>

class TreeItem
{
public:
    explicit TreeItem(VarWatchModel *model, TreeItem *parentItem = nullptr) :
        pModel(model),
        m_parentItem(parentItem)
    {

    }

    void appendChild(std::unique_ptr<TreeItem> &&child)
    {
        m_childItems.push_back(std::move(child));
    }

    TreeItem *child(int row)
    {
        return row >= 0 && row < childCount() ? m_childItems.at(row).get() : nullptr;
    }

    TreeItem *lastChild()
    {
        return !m_childItems.empty() ? m_childItems.back().get() : nullptr;
    }

    int childCount() const
    {
        return int(m_childItems.size());
    }

    QVariant data(int column, int role) const
    {
        if (role == Qt::DisplayRole)
        {
            if (column == VarWatchModel::ColumnName)
                return m_VarInfo.str_name;
            else if (column == VarWatchModel::ColumnType)
                return m_VarInfo.str_type;
            else if (column == VarWatchModel::ColumnValue)
                return value;
        }
        else if (role == Qt::DecorationRole)
        {
            if (column == VarWatchModel::ColumnName)
            {
                if (m_VarInfo.is_object)
                    return QIcon::fromTheme("Class");
                else
                {
                    if (!qstrcmp(m_VarInfo.str_type, "UNDEFINED"))
                        return QIcon::fromTheme("Undefined");

                    return QIcon::fromTheme("Variable");
                }
            }
        }
        else if (role == VarWatchModel::IndexRole)
            return m_VarInfo.index;
        else if (role == VarWatchModel::StackRole)
            return m_VarInfo.st;

        return QVariant();
    }

    int row() const
    {
        if (m_parentItem == nullptr)
            return 0;
        const auto it = std::find_if(m_parentItem->m_childItems.cbegin(), m_parentItem->m_childItems.cend(),
                                     [this](const std::unique_ptr<TreeItem> &treeItem) {
                                         return treeItem.get() == this;
                                     });

        if (it != m_parentItem->m_childItems.cend())
            return std::distance(m_parentItem->m_childItems.cbegin(), it);

        Q_ASSERT(false); // should not happen
        return -1;
    }

    TreeItem *parentItem()
    {
        return m_parentItem;
    }

    DBG_VARIABLEDATA &var()
    {
        return m_VarInfo;
    }

    const DBG_VARIABLEDATA &var() const
    {
        return m_VarInfo;
    }

    void setValue(const QString &val)
    {
        value = val;
    }

private:
    VarWatchModel *pModel;
    std::vector<std::unique_ptr<TreeItem>> m_childItems;
    //QVariantList m_itemData;
    TreeItem *m_parentItem;

    DBG_VARIABLEDATA m_VarInfo;
    QString value;
};

//-------------------------------------------------------------

VarWatchModel::VarWatchModel(QObject *parent)
    : QAbstractItemModel{parent},
    rootItem(std::make_unique<TreeItem>(this))
{

}

VarWatchModel::~VarWatchModel()
{

}

void VarWatchModel::clear()
{
    beginResetModel();
    rootItem.reset(new TreeItem(this));
    endResetModel();
}

void VarWatchModel::append(Qt::HANDLE var, const QString &val)
{
    beginInsertRows(QModelIndex(), rootItem->childCount(), rootItem->childCount());
    rootItem->appendChild(std::make_unique<TreeItem>(this, rootItem.get()));

    TreeItem *item = rootItem->lastChild();
    memcpy(&item->var(), var, sizeof(DBG_VARIABLEDATA));
    item->setValue(val);
    endInsertRows();
}

int VarWatchModel::columnCount(const QModelIndex &parent) const
{
    return ColumnCount;
}

QVariant VarWatchModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const auto *item = static_cast<const TreeItem*>(index.internalPointer());
    return item->data(index.column(), role);
}

QModelIndex VarWatchModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};

    TreeItem *parentItem = parent.isValid() ? static_cast<TreeItem*>(parent.internalPointer()) : rootItem.get();
    if (auto *childItem = parentItem->child(row))
        return createIndex(row, column, childItem);

    return {};
}

QModelIndex VarWatchModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    auto *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    return parentItem != rootItem.get() ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex{};
}

int VarWatchModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const TreeItem *parentItem = parent.isValid()
                                     ? static_cast<const TreeItem*>(parent.internalPointer())
                                     : rootItem.get();


    /*if (!parentItem->childCount() && parentItem->var().is_object)
        return 1;*/

    return parentItem->childCount();
}

Qt::ItemFlags VarWatchModel::flags(const QModelIndex &index) const
{
    return index.isValid() ? QAbstractItemModel::flags(index) : Qt::ItemFlags(Qt::NoItemFlags);
}

bool VarWatchModel::hasChildren(const QModelIndex &parent) const
{
    TreeItem *parentItem = parent.isValid() ? static_cast<TreeItem*>(parent.internalPointer()) : rootItem.get();
    return parentItem->var().is_object;
}

QVariant VarWatchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QStringList Headers =
    {
        tr("Variable"),
        tr("Type"),
        tr("Value")
    };

    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
            return Headers[section];
    }

    return QVariant();
}
