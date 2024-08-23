#ifndef COMMANDSOPTIONS_H
#define COMMANDSOPTIONS_H

#include "optionsdlg/OptionsPage.h"
#include <QWidget>

namespace Ui {
class CommandsOptions;
}

class QMenu;
class CommandsStorage;
class OptionsMenuStyledItemDelegate;
class CommandsOptions : public OptionsPage
{
    Q_OBJECT

public:
    CommandsOptions(QWidget *parent = nullptr);
    virtual ~CommandsOptions();

    void setUseRslcommands(const bool &v);
    virtual int save() Q_DECL_OVERRIDE;
    virtual void restore() Q_DECL_OVERRIDE;

private slots:
    void onAddToolBarButton();
    void onAddCommand();
    void onRemoveCommand();
    void onAddMacro();
    void onChangeLibIcon();
    void onChangeIconFromFile();
    void onToolbarChanged(const int &index);
    void onItemClicked(const QModelIndex &index);
    void onButtonUp();
    void onButtonDown();
    void onChangeText();
    void onDeleteToolBar();
    void onRenameToolbar();

private:
    void updateButtonsState(); 
    Ui::CommandsOptions *ui;

    QMenu *m_pIcomMenu;
    OptionsMenuStyledItemDelegate *menuStyledItemDelegate;
    CommandsStorage *commandsStorage;
};

#endif // COMMANDSOPTIONS_H
