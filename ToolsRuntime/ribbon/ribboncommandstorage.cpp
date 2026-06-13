// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "ribboncommandstorage.h"
#include <QUuid>
#include <QJsonObject>
#include <QAbstractTableModel>
#include <QIcon>

RibbonCommandItem::RibbonCommandItem()
{
    id = QUuid::createUuid().toString(QUuid::WithoutBraces);
}

class RibbonCommandModel : public QAbstractTableModel
{
public:
    RibbonCommandModel(RibbonCommandStorage *storage, QObject *parent = nullptr)
        : QAbstractTableModel(parent)
        , pStorage(storage)
    {
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return pStorage ? pStorage->count() : 0;
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return 1;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid() || !pStorage)
            return QVariant();

        const RibbonCommandItem *item = pStorage->command(index.row());
        if (!item)
            return QVariant();

        if (role == Qt::DisplayRole)
            return item->title;

        if (role == Qt::DecorationRole)
        {
            if (item->icon.isEmpty())
                return QVariant();
            return QIcon(item->icon);
        }

        if (role == Qt::UserRole)
            return item->id;

        return QVariant();
    }

    void reset()
    {
        beginResetModel();
        endResetModel();
    }

private:
    RibbonCommandStorage *pStorage;
};

RibbonCommandStorage::RibbonCommandStorage(QObject *parent)
    : QObject(parent)
    , m_model(new RibbonCommandModel(this, this))
{
}

RibbonCommandStorage::~RibbonCommandStorage()
{
}

int RibbonCommandStorage::count() const
{
    return m_commands.count();
}

RibbonCommandItem *RibbonCommandStorage::command(int index)
{
    if (index < 0 || index >= m_commands.count())
        return nullptr;
    return &m_commands[index];
}

const RibbonCommandItem *RibbonCommandStorage::command(int index) const
{
    if (index < 0 || index >= m_commands.count())
        return nullptr;
    return &m_commands[index];
}

void RibbonCommandStorage::addCommand(const RibbonCommandItem &item)
{
    m_commands.append(item);
    m_model->reset();
    emit commandListChanged();
}

void RibbonCommandStorage::addCommand(const QString &macrofile, const QString &title)
{
    RibbonCommandItem item;
    item.macrofile = macrofile;
    item.title = title;
    addCommand(item);
}

void RibbonCommandStorage::removeCommand(int index)
{
    if (index < 0 || index >= m_commands.count())
        return;

    m_commands.removeAt(index);
    m_model->reset();
    emit commandListChanged();
}

void RibbonCommandStorage::setTitle(int index, const QString &title)
{
    if (index < 0 || index >= m_commands.count())
        return;

    m_commands[index].title = title;
    m_model->reset();
    emit commandListChanged();
}

void RibbonCommandStorage::setIcon(int index, const QString &icon)
{
    if (index < 0 || index >= m_commands.count())
        return;

    m_commands[index].icon = icon;
    m_model->reset();
    emit commandListChanged();
}

QAbstractTableModel *RibbonCommandStorage::model() const
{
    return m_model;
}

QJsonArray RibbonCommandStorage::save() const
{
    QJsonArray result;
    for (const RibbonCommandItem &item : m_commands)
    {
        QJsonObject obj;
        obj.insert("id", item.id);
        obj.insert("macrofile", item.macrofile);
        obj.insert("title", item.title);
        if (!item.icon.isEmpty())
            obj.insert("icon", item.icon);

        result.append(obj);
    }
    return result;
}

void RibbonCommandStorage::load(const QJsonArray &data)
{
    m_commands.clear();
    for (const QJsonValue &value : data)
    {
        QJsonObject obj = value.toObject();
        RibbonCommandItem item;
        item.id = obj.value("id").toString();
        if (item.id.isEmpty())
            item.id = QUuid::createUuid().toString(QUuid::WithoutBraces);

        item.macrofile = obj.value("macrofile").toString();
        item.title = obj.value("title").toString();
        item.icon = obj.value("icon").toString();

        m_commands.append(item);
    }
    m_model->reset();
    emit commandListChanged();
}
