#ifndef COMMANDSSTORAGE_H
#define COMMANDSSTORAGE_H

#include <QObject>
#include <QAction>
#include "ToolsRuntime_global.h"

typedef struct tagToolBarsAction
{
    tagToolBarsAction()
    {
        ref = nullptr;
    }

    QAction *ref;

    QString title, macrofile, icon;
}ToolBarsAction;

class QAbstractTableModel;
class TOOLSRUNTIME_EXPORT ToolBarsStorage
{
public:
    ToolBarsStorage();
    virtual ~ToolBarsStorage();

    const QString &name() const;
    void setName(const QString &name);

    void addAction(QAction *ref);
    void addAction(const QString &macro, const QString &title);
    void addAction(const ToolBarsAction &item);
    void removeAction(const QString &objectName);
    void removeAction(const int &index);

    void moveUp(const int &index);
    void moveDown(const int &index);
    int count() const;

    void setIcon(const int &index, const QString &icon);
    void setActionName(const int &index, const QString &name);
    ToolBarsAction *action(const int &index);

    QAbstractTableModel *model() const;

    QJsonArray save() const;
    void load(QJsonArray actions);

private:
    QString m_Name;
    QAbstractTableModel *m_model;
    QList<ToolBarsAction> m_toolBarsActions;
};

typedef QList<ToolBarsStorage*> ToolBarsStorageList;
class TOOLSRUNTIME_EXPORT CommandsStorage
{
public:
    CommandsStorage();
    virtual ~CommandsStorage();

    bool isExistsToolbar(const QString& name);
    void addToolbar(const QString& name);
    void removeToolbar(const QString& name);

    ToolBarsStorageList toolBarsStorages() const;
    ToolBarsStorage *toolBarsStorage(const QString& name) const;
    ToolBarsStorage *toolBarsStorage(const int& index) const;

    void removeToolBarsStorage(const int &index);

    bool isEmpty() const;
    QStringList toolbarNames() const;

    QByteArray save() const;
    void load(const QByteArray &data);

    int count() const;

private:
    ToolBarsStorageList m_toolBarsStorages;
};

#endif // COMMANDSSTORAGE_H
