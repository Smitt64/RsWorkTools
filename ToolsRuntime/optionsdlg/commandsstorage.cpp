#include "commandsstorage.h"
#include "windowactionsregistry.h"
#include <QAbstractTableModel>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class ToolBarsActionModel : public QAbstractTableModel
{
public:
    ToolBarsActionModel(ToolBarsStorage *storage, QObject *parent = nullptr) : 
        QAbstractTableModel(parent)
    {
        pStorage = storage;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return pStorage->count();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override\
    {
        return 1;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role == Qt::DisplayRole)
        {
            if (pStorage->action(index.row())->ref)
                return pStorage->action(index.row())->ref->text();
            else
                return pStorage->action(index.row())->title;
        }
        else if (role == Qt::DecorationRole)
        {
            if (pStorage->action(index.row())->ref)
                return pStorage->action(index.row())->ref->icon();
            else
            {
                if (pStorage->action(index.row())->icon.isEmpty())
                    return QVariant();

                return QIcon(pStorage->action(index.row())->icon);
            }
        }

        return QVariant();
    }

    void reset()
    {
        beginResetModel();
        endResetModel();
    }
    
private:
    ToolBarsStorage *pStorage;
};

ToolBarsStorage::ToolBarsStorage()
{
    m_model = new ToolBarsActionModel(this);
}

ToolBarsStorage::~ToolBarsStorage()
{
    delete m_model;
}

const QString &ToolBarsStorage::name() const
{
    return m_Name;
}

void ToolBarsStorage::setName(const QString &name)
{
    m_Name = name;
}

void ToolBarsStorage::setIcon(const int &index, const QString &icon)
{
    m_toolBarsActions[index].icon = icon;
    ((ToolBarsActionModel*)m_model)->reset();
}

void ToolBarsStorage::setActionName(const int &index, const QString &name)
{
    m_toolBarsActions[index].title = name;
    ((ToolBarsActionModel*)m_model)->reset();
}

void ToolBarsStorage::addAction(QAction *ref)
{
    ToolBarsAction item;
    item.ref = ref;
    m_toolBarsActions.append(item);

    ((ToolBarsActionModel*)m_model)->reset();
}

void ToolBarsStorage::addAction(const QString &macro, const QString &title)
{
    ToolBarsAction item;
    item.macrofile = macro;
    item.title = title;
    m_toolBarsActions.append(item);

    ((ToolBarsActionModel*)m_model)->reset();
}

void ToolBarsStorage::addAction(const ToolBarsAction &item)
{
    m_toolBarsActions.append(item);

    ((ToolBarsActionModel*)m_model)->reset();
}

int ToolBarsStorage::count() const
{
    return m_toolBarsActions.count();
}

ToolBarsAction *ToolBarsStorage::action(const int &index)
{
    return &m_toolBarsActions[index];
}

void ToolBarsStorage::removeAction(const QString &objectName)
{
    auto it = std::find_if(m_toolBarsActions.begin(), m_toolBarsActions.end(), [=](const ToolBarsAction &item)
    {
        if (item.ref)
            return item.ref->objectName() == objectName;

        return false;
    });

    if (it != m_toolBarsActions.end())
        m_toolBarsActions.erase(it);

    ((ToolBarsActionModel*)m_model)->reset();
}

void ToolBarsStorage::removeAction(const int &index)
{
    m_toolBarsActions.removeAt(index);
    ((ToolBarsActionModel*)m_model)->reset();
}

void ToolBarsStorage::moveUp(const int &index)
{
    if (index > 0)
    {
        ToolBarsAction item = m_toolBarsActions.at(index - 1);
        m_toolBarsActions.replace(index - 1, m_toolBarsActions.at(index));
        m_toolBarsActions.replace(index, item);
    }

    ((ToolBarsActionModel*)m_model)->reset();
}

void ToolBarsStorage::moveDown(const int &index)
{
    if (index < m_toolBarsActions.count() - 1)
    {
        ToolBarsAction item = m_toolBarsActions.at(index + 1);
        m_toolBarsActions.replace(index + 1, m_toolBarsActions.at(index));
        m_toolBarsActions.replace(index, item);
    }

    ((ToolBarsActionModel*)m_model)->reset();
}

QJsonArray ToolBarsStorage::save() const
{
    QJsonArray actionsArray;
    for (auto &item : m_toolBarsActions)
    {
        QJsonObject actionObj;
        if (item.ref)
        {
            actionObj.insert("ref", item.ref->objectName());
        }
        else
        {
            actionObj.insert("macro", item.macrofile);
            actionObj.insert("title", item.title);
        }

        if (!item.icon.isEmpty())
            actionObj.insert("icon", item.icon);

        actionsArray.append(actionObj);
    }

    return actionsArray;
}

void ToolBarsStorage::load(QJsonArray actions)
{
    for (QJsonValueRef action : actions)
    {
        QJsonObject actionObj = action.toObject();
        QString ref = actionObj["ref"].toString();
        QString macro = actionObj["macro"].toString();
        QString title = actionObj["title"].toString();
        QString icon = actionObj["icon"].toString();

        ToolBarsAction item;
        item.ref = windowActionsRegistry()->getAction(ref);
        item.macrofile = macro;
        item.title = title;
        item.icon = icon;

        addAction(item);
    }
}

QAbstractTableModel *ToolBarsStorage::model() const
{
    return m_model;
}

// ------------------------------------------------------------------

CommandsStorage::CommandsStorage()
{

}

CommandsStorage::~CommandsStorage()
{
    qDeleteAll(m_toolBarsStorages);
    m_toolBarsStorages.clear();
}

bool CommandsStorage::isExistsToolbar(const QString& name)
{
    return std::find_if(m_toolBarsStorages.begin(), m_toolBarsStorages.end(), [=](ToolBarsStorage *item)
    {
        return item->name() == name;
    }) != m_toolBarsStorages.end();
}

void CommandsStorage::addToolbar(const QString& name)
{
    if (!isExistsToolbar(name))
    {
        ToolBarsStorage *storage = new ToolBarsStorage();
        storage->setName(name);
        m_toolBarsStorages.append(storage);
    }
}

void CommandsStorage::removeToolbar(const QString& name)
{
    auto it = std::find_if(m_toolBarsStorages.begin(), m_toolBarsStorages.end(), [=](ToolBarsStorage *item)
    {
        return item->name() == name;
    });

    if (it != m_toolBarsStorages.end())
    {
        delete *it;
        m_toolBarsStorages.erase(it);
    }
}

ToolBarsStorageList CommandsStorage::toolBarsStorages() const
{
    return m_toolBarsStorages;
}

ToolBarsStorage *CommandsStorage::toolBarsStorage(const QString& name) const
{
    ToolBarsStorageList::const_iterator iter = std::find_if(m_toolBarsStorages.cbegin(), m_toolBarsStorages.cend(), [=](ToolBarsStorage *item)
    {
        return item->name() == name;
    });

    if (iter != m_toolBarsStorages.end())
        return *iter;

    return nullptr;
}

ToolBarsStorage *CommandsStorage::toolBarsStorage(const int& index) const
{
    return m_toolBarsStorages.at(index);
}

int CommandsStorage::count() const
{
    return m_toolBarsStorages.count();
}

bool CommandsStorage::isEmpty() const
{
    return m_toolBarsStorages.isEmpty();
}

QStringList CommandsStorage::toolbarNames() const
{
    QStringList names;

    for (ToolBarsStorage *storage : m_toolBarsStorages)
        names.append(storage->name());

    return names;
}

QByteArray CommandsStorage::save() const
{
    QJsonDocument doc;
    QJsonObject root;
    QJsonArray toolbars;
    for (ToolBarsStorage *storage : m_toolBarsStorages)
    {
        QJsonObject toolbar;
        toolbar["name"] = storage->name();
        toolbar["actions"] = storage->save();
        toolbars.append(toolbar);
    }

    root["toolbars"] = toolbars;
    doc.setObject(root);

    return doc.toJson(QJsonDocument::Compact);
}

void CommandsStorage::load(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();

    QJsonArray toolbars = root["toolbars"].toArray();
    for (QJsonValueRef toolbar : toolbars)
    {
        QJsonObject toolbarObj = toolbar.toObject();
        QString name = toolbarObj["name"].toString();
        QJsonArray actions = toolbarObj["actions"].toArray();

        ToolBarsStorage *storage = new ToolBarsStorage();
        storage->setName(name);
        storage->load(actions);

        m_toolBarsStorages.append(storage);
    }
}

void CommandsStorage::removeToolBarsStorage(const int &index)
{
    ToolBarsStorage *item = m_toolBarsStorages[index];
    m_toolBarsStorages.removeAt(index);
    delete item;
}
