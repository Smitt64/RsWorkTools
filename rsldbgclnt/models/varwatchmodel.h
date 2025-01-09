#ifndef VARWATCHMODEL_H
#define VARWATCHMODEL_H

#include <QAbstractItemModel>
#include <QObject>

class TreeItem;
class VarWatchModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Column
    {
        ColumnName = 0,
        ColumnType,
        ColumnValue,

        ColumnCount
    };

    enum CustomRoles
    {
        IndexRole = Qt::UserRole + 1,
        StackRole
    };

    VarWatchModel(QObject *parent = nullptr);
    virtual ~VarWatchModel();

    void clear();
    void append(Qt::HANDLE var, const QString &val);

    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent = {}) const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

private:
    std::unique_ptr<TreeItem> rootItem;
};

#endif // VARWATCHMODEL_H
