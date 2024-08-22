#include "windowactionsregistry.h"
#include "optionsdlg/commandsstorage.h"
#include "rslexecutor.h"
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QAbstractTableModel>
#include <QToolBar>
#include <QSettings>

Q_GLOBAL_STATIC(WindowActionsRegistry, actionRegisters)

typedef struct
{
    QAction *action;
    QString alias;
}ActionInfo;

class WindowActionsRegistryPrivate
{
    Q_DECLARE_PUBLIC(WindowActionsRegistry);

    WindowActionsRegistryPrivate(WindowActionsRegistry *obj)
    {
        m_pExec = nullptr;
        q_ptr = obj;
    }

    bool isExistsAction(const QAction *action) const
    {
        for (int i = 0; i < actions.size(); ++i)
        {
            if (actions[i].action->objectName() == action->objectName())
                return true;
        }

        return false;
    }

public: 
    WindowActionsRegistry *q_ptr;

    RslExecutor *m_pExec;
    QList<ActionInfo> actions;
};

WindowActionsRegistry::WindowActionsRegistry(QObject *parent)
    : QObject{parent},
    d_ptr(new WindowActionsRegistryPrivate(this))
{

}

WindowActionsRegistry::~WindowActionsRegistry()
{
    Q_D(WindowActionsRegistry);

    if (d->m_pExec)
        delete d->m_pExec;
}

void WindowActionsRegistry::registerAction(QAction *action, const QString &name)
{
    Q_D(WindowActionsRegistry);

    ActionInfo info;
    info.action = action;
    info.alias = name;

    d->actions.append(info);
}

int WindowActionsRegistry::getActionIndex(const QString &name) const
{
    Q_D(const WindowActionsRegistry);

    for (int i = 0; i < d->actions.size(); ++i)
    {
        if (d->actions[i].alias == name)
            return i;
    }

    return -1;
}

QAction *WindowActionsRegistry::getAction(const QString &name) const
{
    Q_D(const WindowActionsRegistry);

    for (int i = 0; i < d->actions.size(); ++i)
    {
        if (d->actions[i].alias == name)
            return d->actions[i].action;
    }

    return nullptr;
}

void WindowActionsRegistry::scanActions(QMenu *menu)
{
    Q_D(WindowActionsRegistry);

    for (int i = 0; i < menu->actions().size(); ++i)
    {
        QAction *action = menu->actions()[i];
        if (action->menu())
            scanActions(action->menu());

        ActionInfo info;
        info.action = action;
        info.alias = action->objectName();

        if (!info.alias.isEmpty() && !d->isExistsAction(action))
            d->actions.append(info);
    }
}

int WindowActionsRegistry::actionCount() const
{
    Q_D(const WindowActionsRegistry);
    return d->actions.size();
}

QAction *WindowActionsRegistry::action(int index) const
{
    Q_D(const WindowActionsRegistry);
    return d->actions[index].action;
}

QList<QToolBar*> WindowActionsRegistry::makeToolBars(const QByteArray &data, bool base64)
{
    QList<QToolBar*> toolbars;

    QByteArray decoded;
    if (base64)
        decoded = QByteArray::fromBase64(data);
    else
        decoded = data;

    CommandsStorage storage;
    storage.load(decoded);

    ToolBarsStorageList list = storage.toolBarsStorages();
    for (ToolBarsStorage *toolbarInfo : list)
    {
        QToolBar *toolbar = new QToolBar(toolbarInfo->name());
        toolbar->setObjectName(toolbarInfo->name());
        toolbar->setWindowTitle(toolbarInfo->name());

        for (int i = 0; i < toolbarInfo->count(); i++)
        {
            ToolBarsAction *info = toolbarInfo->action(i);

            if (info->ref)
                toolbar->addAction(info->ref);
            else
            {
                QAction *action = toolbar->addAction(QIcon(info->icon), info->title);
                action->setData(info->macrofile);
                action->setToolTip(info->title);

                connect(action, SIGNAL(triggered()), this, SLOT(ExecAction()));
            }
        }

        toolbars.append(toolbar);
    }

    return toolbars;
}

void WindowActionsRegistry::setRslExecutor(RslExecutor *executor)
{
    Q_D(WindowActionsRegistry);
    d->m_pExec = executor;
}

QList<QToolBar*> WindowActionsRegistry::makeToolBars(QSettings *settings, const QString &group, const QString &key)
{
    QList<QToolBar*> toolbars;

    settings->beginGroup(group);
    QByteArray data = settings->value(key).toByteArray();

    if (!data.isEmpty())
        toolbars = makeToolBars(data, true);

    settings->endGroup();

    return toolbars;
}

void WindowActionsRegistry::ExecAction()
{
    Q_D(WindowActionsRegistry);

    if (d->m_pExec)
    {
        QAction *action = qobject_cast<QAction*>(sender());
        if (action)
        {
            QVariant var = action->data();
            d->m_pExec->playRep(var.toString());
        }
    }
}

#ifdef _DEBUG
void WindowActionsRegistry::printActions()
{
    Q_D(WindowActionsRegistry);

    for (int i = 0; i < d->actions.size(); ++i)
        qDebug() << d->actions[i].action->objectName() << d->actions[i].alias;
}
#endif

WindowActionsRegistry *windowActionsRegistry()
{
    return actionRegisters;
}
