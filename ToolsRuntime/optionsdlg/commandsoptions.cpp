#include "commandsoptions.h"
#include "ui_commandsoptions.h"
#include "windowactionsregistry.h"
#include "commandsstorage.h"
#include "widgets\selectactiondlg.h"
#include "iconlibdlg.h"
#include "rsscript/registerobjlist.hpp"
#include <QInputDialog>
#include <QStyledItemDelegate>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QSettings>

class OptionsMenuStyledItemDelegate : public QStyledItemDelegate
{
public:
    OptionsMenuStyledItemDelegate(QObject *parent = nullptr) : 
        QStyledItemDelegate(parent) 
    {
        QStyle *style = QApplication::style();

        QMenu menu;
        QAction action;
        menu.addAction(&action);
        m_ActionSize = menu.actionGeometry(&action).size();
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyle *style = QApplication::style();

        QStyleOptionMenuItem opt;
        opt.rect = option.rect;
        opt.state = option.state;
        opt.text = index.data(Qt::DisplayRole).toString();
        opt.icon = index.data(Qt::DecorationRole).value<QIcon>();
        style->drawControl(QStyle::CE_MenuItem, &opt, painter);
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyle *style = QApplication::style();
        return style->sizeFromContents(QStyle::CT_MenuItem, &option, m_ActionSize);
    }

private:
    QSize m_ActionSize;
};

void GetMenuStyledItemDelegate(QStyledItemDelegate **delegate, QObject *parent)
{
    *delegate = new OptionsMenuStyledItemDelegate(parent);
}

CommandsOptions::CommandsOptions(QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::CommandsOptions)
{
    ui->setupUi(this);

    commandsStorage = new CommandsStorage();
    menuStyledItemDelegate = new OptionsMenuStyledItemDelegate(this);

    ui->listView->setItemDelegate(menuStyledItemDelegate);
    ui->comboBox->addItems(commandsStorage->toolbarNames());

    m_pIcomMenu = new QMenu(this);

    QAction *addFromLib = m_pIcomMenu->addAction(tr("Добавить из библиотеки"));
    QAction *addFromfile = m_pIcomMenu->addAction(tr("Добавить из файла"));
    ui->buttonIcon->setMenu(m_pIcomMenu);

    updateButtonsState();
    connect(ui->buttonAddMacro, SIGNAL(clicked()), this, SLOT(onAddMacro()));
    connect(ui->buttonDelAction, SIGNAL(clicked()), this, SLOT(onRemoveCommand()));
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
    connect(ui->buttonAddTool, SIGNAL(clicked()), this, SLOT(onAddToolBarButton()));
    connect(ui->buttonAddCmd, SIGNAL(clicked()), this, SLOT(onAddCommand()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onToolbarChanged(int)));
    connect(addFromLib, SIGNAL(triggered()), this, SLOT(onChangeLibIcon()));
    connect(ui->buttonUp, SIGNAL(clicked()), this, SLOT(onButtonUp()));
    connect(ui->buttonDown, SIGNAL(clicked()), this, SLOT(onButtonDown()));
    connect(addFromfile, SIGNAL(triggered()), this, SLOT(onChangeIconFromFile()));
    connect(ui->buttonName, SIGNAL(clicked()), this, SLOT(onChangeText()));
    connect(ui->buttonDelTool, SIGNAL(clicked()), this, SLOT(onDeleteToolBar()));
    connect(ui->buttonChangeName, SIGNAL(clicked()), this, SLOT(onRenameToolbar()));
}

CommandsOptions::~CommandsOptions()
{
    delete ui;
    delete commandsStorage;
}

void CommandsOptions::setUseRslcommands(const bool &v)
{
    ui->buttonAddMacro->setVisible(v);
    ui->buttonIcon->setVisible(v);
    ui->buttonName->setVisible(v);
    ui->line_2->setVisible(v);
}

void CommandsOptions::onAddToolBarButton()
{
    QString name = QInputDialog::getText(this, tr("Панель инструментов"), "Введите название панели инструментов:");
    if (name.isEmpty())
        return; 

    commandsStorage->addToolbar(name);

    ui->comboBox->clear();
    ui->comboBox->addItems(commandsStorage->toolbarNames());
    ui->comboBox->setCurrentText(name);

    updateButtonsState();
}

void CommandsOptions::updateButtonsState()
{
    if (ui->comboBox->count())
    { 
        ui->buttonAddMacro->setEnabled(true);
        ui->buttonAddCmd->setEnabled(true);
        ui->buttonDelTool->setEnabled(true);

        QModelIndex index = ui->listView->currentIndex();
        if (index.isValid())
        {
            ui->buttonDelAction->setEnabled(true);

            if (ui->listView->currentIndex().row() == 0)
                ui->buttonUp->setEnabled(false);
            else
            {
                if (ui->listView->model()->rowCount() != 1)
                    ui->buttonUp->setEnabled(true);
                else
                    ui->buttonUp->setEnabled(false);
            }

            if (ui->listView->currentIndex().row() == ui->listView->model()->rowCount() - 1)
                ui->buttonDown->setEnabled(false);
            else
            {
                if (ui->listView->model()->rowCount() != 1)
                    ui->buttonDown->setEnabled(true);
                else
                    ui->buttonDown->setEnabled(false);
            }

            ToolBarsStorage *toolbar = commandsStorage->toolBarsStorage(ui->comboBox->currentIndex());

            if (toolbar->action(index.row())->ref)
            {
                ui->buttonIcon->setEnabled(false);
                ui->buttonName->setEnabled(false);
            }
            else
            {
                ui->buttonIcon->setEnabled(true);
                ui->buttonName->setEnabled(true);
            }
        }
        else
        {
            ui->buttonDelAction->setEnabled(false);
            ui->buttonUp->setEnabled(false);
            ui->buttonDown->setEnabled(false);
        }
    }
    else
    {
        ui->buttonAddMacro->setEnabled(false);
        ui->buttonAddCmd->setEnabled(false);
        ui->buttonDelTool->setEnabled(false);
        ui->buttonDelAction->setEnabled(false);
        ui->buttonUp->setEnabled(false);
        ui->buttonDown->setEnabled(false);
        ui->buttonIcon->setEnabled(false);
        ui->buttonName->setEnabled(false);
    }
}

void CommandsOptions::onAddCommand()
{
    QString toolbarName = ui->comboBox->currentText();
    if (toolbarName.isEmpty())
        return;

    SelectActionDlg dlg(this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    ToolBarsStorage *toolbar = commandsStorage->toolBarsStorage(ui->comboBox->currentIndex());
    toolbar->addAction(dlg.getSelectedAction());

    updateButtonsState();
}

void CommandsOptions::onToolbarChanged(const int &index)
{
    if (index >= 0)
    {
        ui->listView->setModel(commandsStorage->toolBarsStorage(index)->model());
        ui->listView->setModelColumn(0);
    }
    else
    {
        ui->listView->setModel(nullptr);
    }
}

void CommandsOptions::onItemClicked(const QModelIndex &index)
{
    updateButtonsState();
}

void CommandsOptions::onRemoveCommand()
{
    QModelIndex index = ui->listView->currentIndex();
    if (!index.isValid())
        return;

    if (QMessageBox::question(this, tr("Удалить команду"),
        tr("Вы действительно хотите удалить команду <b>&#8220;%1&#8220;</b>?")
            .arg(index.data().toString())) == QMessageBox::Yes)
    {
        ToolBarsStorage *toolbar = commandsStorage->toolBarsStorage(ui->comboBox->currentIndex());
        toolbar->removeAction(index.row());

        updateButtonsState();
    }
}

void CommandsOnAddMacro(ToolBarsStorage *toolbar, QWidget *parent)
{
    QFileDialog dlg(parent);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setOptions(QFileDialog::DontUseNativeDialog | QFileDialog::ReadOnly | QFileDialog::DontUseSheet);
    dlg.setNameFilter(QObject::tr("Макросы (*.mac)"));
    dlg.setWindowTitle(QObject::tr("Выберите макрос"));

    QList<QUrl> urls;
    QStringList macrodirs = rslObjList()->incDirs().split(";");
    for (const QString &dir : macrodirs)
        urls.append(QUrl::fromLocalFile(dir));

    dlg.setSupportedSchemes(macrodirs);
    dlg.setSidebarUrls(urls);

    if (dlg.exec() == QDialog::Accepted)
    {
        QString macro = dlg.selectedUrls().first().toLocalFile();
        QMap<QString, QString> macInfo = rslGetMacroInfo(macro);

        QString name;
        if (macInfo.contains("Title"))
            name = macInfo.value("Title");
        else if (macInfo.contains("Description"))
            name = macInfo.value("Description");
        else
        {
            QFileInfo info(dlg.selectedUrls().first().toLocalFile());
            name = info.baseName();
        }

        //ToolBarsStorage *toolbar = commandsStorage->toolBarsStorage(ui->comboBox->currentIndex());
        toolbar->addAction(macro, name);
    }
}

void CommandsOptions::onAddMacro()
{
    ToolBarsStorage *toolbar = commandsStorage->toolBarsStorage(ui->comboBox->currentIndex());
    CommandsOnAddMacro(toolbar, this);
}

void CommandsOptions::onChangeLibIcon()
{
    IconLibDlg dlg(this);
    QModelIndex index = ui->listView->currentIndex();
    if (dlg.exec() == QDialog::Accepted)
    {
        ToolBarsStorage *toolbar = commandsStorage->toolBarsStorage(ui->comboBox->currentIndex());
        toolbar->setIcon(ui->listView->currentIndex().row(), dlg.getIconPath());
    }
    ui->listView->setCurrentIndex(index);
}

void CommandsOptions::onButtonUp()
{
    ToolBarsStorage *toolbar = commandsStorage->toolBarsStorage(ui->comboBox->currentIndex());

    QModelIndex index = ui->listView->currentIndex();
    toolbar->moveUp(index.row());

    index = ui->listView->model()->index(index.row() - 1, 0);
    ui->listView->setCurrentIndex(index);

    updateButtonsState();
}

void CommandsOptions::onButtonDown()
{
    ToolBarsStorage *toolbar = commandsStorage->toolBarsStorage(ui->comboBox->currentIndex());

    QModelIndex index = ui->listView->currentIndex();
    toolbar->moveDown(index.row());

    index = ui->listView->model()->index(index.row() + 1, 0);
    ui->listView->setCurrentIndex(index);

    updateButtonsState();
}

int CommandsOptions::save()
{
    QSettings *setting = settings();
    QByteArray data = commandsStorage->save();

    setting->beginGroup("UserCommands");
    setting->setValue("ToolBars", QString::fromLocal8Bit(data.toBase64()));
    setting->endGroup();

    return 0;
}

void CommandsOptions::restore()
{
    QSettings *setting = settings();
    setting->beginGroup("UserCommands");

    QByteArray data = QByteArray::fromBase64(setting->value("ToolBars").toByteArray());
    setting->endGroup();

    if (data.isEmpty())
        return;

    commandsStorage->load(data);
    ui->comboBox->clear();

    if (!commandsStorage->isEmpty())
    {
        ui->comboBox->addItems(commandsStorage->toolbarNames());
        ui->comboBox->setCurrentText(commandsStorage->toolbarNames().first());
    }

    updateButtonsState();
}

void CommandsOptions::onChangeIconFromFile()
{
    QModelIndex index = ui->listView->currentIndex();
    QString path = QFileDialog::getOpenFileName(this, tr("Открыть файл"), QString(), 
        tr("Изображения (*.png *.jpeg *.bmp)"));

    if (path.isEmpty())
        return;

    ToolBarsStorage *toolbar = commandsStorage->toolBarsStorage(ui->comboBox->currentIndex());
    toolbar->setIcon(ui->listView->currentIndex().row(), path);
    ui->listView->setCurrentIndex(index);
}

void CommandsOptions::onChangeText()
{
    QModelIndex index = ui->listView->currentIndex();
    ToolBarsStorage *toolbar = commandsStorage->toolBarsStorage(ui->comboBox->currentIndex());

    QString oldname = toolbar->action(ui->listView->currentIndex().row())->title;
    QString newname = QInputDialog::getText(this, tr("Изменить название"), tr("Новое название: "), QLineEdit::Normal, oldname);
    
    if (newname.isEmpty())
        return;

    toolbar->setActionName(index.row(), newname);
    ui->listView->setCurrentIndex(index);
}

void CommandsOptions::onDeleteToolBar()
{
    if (QMessageBox::question(this, tr("Удалить панель инструментов"), 
        tr("Вы действительно хотите удалить панель инструментов <b>&#8220;%1&#8220;</b>?")
        .arg(ui->comboBox->currentText())) == QMessageBox::Yes)
    {
        int index = ui->comboBox->currentIndex();
        commandsStorage->removeToolBarsStorage(index);
        ui->comboBox->removeItem(index);

        if (ui->comboBox->count() > 0)
            ui->comboBox->setCurrentIndex(0);

        updateButtonsState();
    }
}

void CommandsOptions::onRenameToolbar()
{
    int index = ui->comboBox->currentIndex();
    QString newname = QInputDialog::getText(this, tr("Изменить название"), tr("Новое название: "), QLineEdit::Normal, ui->comboBox->currentText());

    if (newname.isEmpty())
        return;

    ToolBarsStorage *toolbar = commandsStorage->toolBarsStorage(index);
    toolbar->setName(newname);
    ui->comboBox->setItemText(index, newname);
}
