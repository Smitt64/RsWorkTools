// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef RIBBONCOMMANDLISTWIDGET_H
#define RIBBONCOMMANDLISTWIDGET_H

#include "ToolsRuntime_global.h"
#include <QWidget>

class RibbonCommandStorage;
class QTableView;
class QToolButton;
class QAction;

class TOOLSRUNTIME_EXPORT RibbonCommandListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RibbonCommandListWidget(QWidget *parent = nullptr);
    virtual ~RibbonCommandListWidget();

    void setStorage(RibbonCommandStorage *storage);
    RibbonCommandStorage *storage() const;

signals:
    void commandRemoveRequested(int index);

private slots:
    void onAddMacro();
    void onRemoveCommand();
    void onEditTitle();
    void onChangeIconFromLib();
    void onChangeIconFromFile();
    void onSelectionChanged();
    void onDoubleClicked(const QModelIndex &index);

private:
    void setupUI();
    void updateButtonsState();

    RibbonCommandStorage *m_storage;
    QTableView *m_tableView;

    QAction *m_actionAdd;
    QAction *m_actionRemove;
    QAction *m_actionEditTitle;
    QAction *m_actionChangeIcon;
};

#endif // RIBBONCOMMANDLISTWIDGET_H
