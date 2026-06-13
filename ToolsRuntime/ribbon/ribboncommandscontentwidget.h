// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef RIBBONCOMMANDSCONTENTWIDGET_H
#define RIBBONCOMMANDSCONTENTWIDGET_H

#include "categorycontentwidget.h"
#include "ToolsRuntime_global.h"
#include <QMap>

class RibbonCommandStorage;
class RibbonCommandListWidget;
class SARibbonActionsManager;
class SARibbonBar;
class SARibbonCategory;
class SARibbonPannel;
class SARibbonToolButton;
class SARibbonCustomizeWidget;
class QSettings;
class QStackedWidget;
class QAction;
class QHideEvent;

class TOOLSRUNTIME_EXPORT RibbonCommandsContentWidget : public CategoryContentWidget
{
    Q_OBJECT
public:
    explicit RibbonCommandsContentWidget(QWidget *parent = nullptr);
    virtual ~RibbonCommandsContentWidget();

    void setRibbonBar(SARibbonBar *ribbonBar);
    void setActionsManager(SARibbonActionsManager *manager);

    void save(QSettings *settings);
    void load(QSettings *settings);

    QList<QAction*> macroActions() const;

signals:
    void macroActionAdded(QAction *action);

protected:
    void onListItemSelected(const QVariant &data) override;
    void hideEvent(QHideEvent *event) override;

private slots:
    void onCommandListChanged();
    void onCommandRemoveRequested(int index);

private:
    void setupUI();
    void syncMacroActions();
    void updateCustomizeWidget();
    bool isActionOnRibbon(QAction *action) const;
    bool removeActionFromRibbon(QAction *action);

    RibbonCommandStorage *m_storage;
    RibbonCommandListWidget *m_commandListWidget;
    SARibbonCustomizeWidget *m_customizeWidget;
    SARibbonActionsManager *m_actionsManager;
    SARibbonBar *m_ribbonBar;
    QStackedWidget *m_stackedWidget;

    QMap<QString, QAction*> m_macroActions;
};

#endif // RIBBONCOMMANDSCONTENTWIDGET_H
