#ifndef WINDOWACTIONSREGISTRY_H
#define WINDOWACTIONSREGISTRY_H

#include <QObject>
#include "ToolsRuntime_global.h"

class QAction;
class QMenu;
class QToolBar;
class QSettings;
class RslExecutor;
class WindowActionsRegistryPrivate;
typedef QMap<QString, QList<QAction*>> ActionsRegistryMap;
class TOOLSRUNTIME_EXPORT WindowActionsRegistry : public QObject
{
    Q_OBJECT
public:
    WindowActionsRegistry(QObject *parent = nullptr);
    virtual ~WindowActionsRegistry();

    void registerAction(QAction *action, const QString &name);
    int getActionIndex(const QString &name) const;
    QAction *getAction(const QString &name) const;
    int actionCount() const;
    QAction *action(int index) const;
    void scanActions(QMenu *menu);

    void setRslExecutor(RslExecutor *executor);

    int menuCount() const;
    QStringList menuNames() const;
    QList<QAction*> menuActions(const QString &name) const;

    QList<QToolBar*> makeToolBars(const QByteArray &data, bool base64 = false);
    QList<QToolBar*> makeToolBars(QSettings *settings, const QString &group, const QString &key);

#ifdef _DEBUG
    void printActions();
#endif

private slots:
    void ExecAction();

private:
    WindowActionsRegistryPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(WindowActionsRegistry);
};

TOOLSRUNTIME_EXPORT WindowActionsRegistry *windowActionsRegistry();
#endif // WINDOWACTIONSREGISTRY_H
