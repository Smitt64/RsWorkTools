// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "ribboncommandlistwidget.h"
#include "ribboncommandstorage.h"
#include "iconlibdlg.h"
#include "rslexecutor.h"
#include "rsscript/registerobjlist.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QToolButton>
#include <QMenu>
#include <QFileDialog>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QAbstractItemModel>

RibbonCommandListWidget::RibbonCommandListWidget(QWidget *parent)
    : QWidget(parent)
    , m_storage(nullptr)
    , m_tableView(nullptr)
    , m_actionAdd(nullptr)
    , m_actionRemove(nullptr)
    , m_actionEditTitle(nullptr)
    , m_actionChangeIcon(nullptr)
{
    setupUI();
    updateButtonsState();
}

RibbonCommandListWidget::~RibbonCommandListWidget()
{
}

void RibbonCommandListWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(6);

    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    toolbarLayout->setSpacing(6);
    toolbarLayout->addStretch();

    m_actionAdd = new QAction(tr("Добавить"), this);
    m_actionRemove = new QAction(tr("Удалить"), this);
    m_actionEditTitle = new QAction(tr("Изменить название"), this);
    m_actionChangeIcon = new QAction(tr("Изменить иконку"), this);

    QToolButton *btnAdd = new QToolButton(this);
    btnAdd->setDefaultAction(m_actionAdd);

    QToolButton *btnRemove = new QToolButton(this);
    btnRemove->setDefaultAction(m_actionRemove);

    QToolButton *btnEdit = new QToolButton(this);
    btnEdit->setDefaultAction(m_actionEditTitle);

    QMenu *iconMenu = new QMenu(this);
    QAction *actIconLib = iconMenu->addAction(tr("Из библиотеки"));
    QAction *actIconFile = iconMenu->addAction(tr("Из файла"));

    QToolButton *btnIcon = new QToolButton(this);
    btnIcon->setDefaultAction(m_actionChangeIcon);
    btnIcon->setPopupMode(QToolButton::InstantPopup);
    btnIcon->setMenu(iconMenu);

    toolbarLayout->addWidget(btnAdd);
    toolbarLayout->addWidget(btnRemove);
    toolbarLayout->addWidget(btnEdit);
    toolbarLayout->addWidget(btnIcon);

    mainLayout->addLayout(toolbarLayout);

    m_tableView = new QTableView(this);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->horizontalHeader()->hide();
    m_tableView->verticalHeader()->hide();
    mainLayout->addWidget(m_tableView);

    connect(m_actionAdd, &QAction::triggered, this, &RibbonCommandListWidget::onAddMacro);
    connect(m_actionRemove, &QAction::triggered, this, &RibbonCommandListWidget::onRemoveCommand);
    connect(m_actionEditTitle, &QAction::triggered, this, &RibbonCommandListWidget::onEditTitle);
    connect(actIconLib, &QAction::triggered, this, &RibbonCommandListWidget::onChangeIconFromLib);
    connect(actIconFile, &QAction::triggered, this, &RibbonCommandListWidget::onChangeIconFromFile);
    connect(m_tableView, &QTableView::doubleClicked,
            this, &RibbonCommandListWidget::onDoubleClicked);
}

void RibbonCommandListWidget::setStorage(RibbonCommandStorage *storage)
{
    if (m_storage)
        disconnect(m_tableView->selectionModel(), nullptr, this, nullptr);

    m_storage = storage;
    if (m_storage)
        m_tableView->setModel(m_storage->model());
    else
        m_tableView->setModel(nullptr);

    if (m_tableView->selectionModel())
    {
        connect(m_tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &RibbonCommandListWidget::onSelectionChanged);
    }

    updateButtonsState();
}

RibbonCommandStorage *RibbonCommandListWidget::storage() const
{
    return m_storage;
}

void RibbonCommandListWidget::onSelectionChanged()
{
    updateButtonsState();
}

void RibbonCommandListWidget::updateButtonsState()
{
    bool hasSelection = m_tableView->selectionModel() && m_tableView->selectionModel()->hasSelection();
    m_actionAdd->setEnabled(m_storage != nullptr);
    m_actionRemove->setEnabled(hasSelection);
    m_actionEditTitle->setEnabled(hasSelection);
    m_actionChangeIcon->setEnabled(hasSelection);
}

void RibbonCommandListWidget::onAddMacro()
{
    if (!m_storage)
        return;

    QFileDialog dlg(this);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setOptions(QFileDialog::DontUseNativeDialog | QFileDialog::ReadOnly);
    dlg.setNameFilter(tr("Макросы (*.mac)"));
    dlg.setWindowTitle(tr("Выберите макрос"));

    RegisterObjList *objList = rslObjList();
    if (objList)
    {
        QList<QUrl> urls;
        QStringList macrodirs = objList->incDirs().split(";", Qt::SkipEmptyParts);
        for (const QString &dir : macrodirs)
        {
            QDir d(dir);
            if (d.exists())
                urls.append(QUrl::fromLocalFile(d.absolutePath()));
        }
        if (!urls.isEmpty())
            dlg.setSidebarUrls(urls);
    }

    if (dlg.exec() != QDialog::Accepted)
        return;

    QList<QUrl> selectedUrls = dlg.selectedUrls();
    if (selectedUrls.isEmpty())
        return;

    QString macro = selectedUrls.first().toLocalFile();
    if (macro.isEmpty() || !QFile::exists(macro))
        return;

    if (!IsMacroExistsFunction(macro, "ExecAction"))
    {
        QFileInfo fi(macro);
        QMessageBox::critical(this, tr("Ошибка"),
                              tr("Макрос <b>%1</b> не содержит макрофункции <b>ExecAction</b>")
                                  .arg(fi.fileName()));
        return;
    }

    QMap<QString, QString> macInfo = rslGetMacroInfo(macro);
    QString name;
    if (macInfo.contains("Title"))
        name = macInfo.value("Title");
    else if (macInfo.contains("Description"))
        name = macInfo.value("Description");
    else
    {
        QFileInfo fi(macro);
        name = fi.baseName();
    }

    m_storage->addCommand(macro, name);
}

void RibbonCommandListWidget::onRemoveCommand()
{
    QModelIndex index = m_tableView->currentIndex();
    if (!index.isValid())
        return;

    emit commandRemoveRequested(index.row());
}

void RibbonCommandListWidget::onEditTitle()
{
    if (!m_storage)
        return;

    QModelIndex index = m_tableView->currentIndex();
    if (!index.isValid())
        return;

    RibbonCommandItem *item = m_storage->command(index.row());
    if (!item)
        return;

    bool ok = false;
    QString newTitle = QInputDialog::getText(this, tr("Изменить название"),
                                             tr("Название команды:"), QLineEdit::Normal,
                                             item->title, &ok);
    if (ok && !newTitle.isEmpty())
        m_storage->setTitle(index.row(), newTitle);
}

void RibbonCommandListWidget::onChangeIconFromLib()
{
    if (!m_storage)
        return;

    QModelIndex index = m_tableView->currentIndex();
    if (!index.isValid())
        return;

    IconLibDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted)
        m_storage->setIcon(index.row(), dlg.getIconPath());
}

void RibbonCommandListWidget::onChangeIconFromFile()
{
    if (!m_storage)
        return;

    QModelIndex index = m_tableView->currentIndex();
    if (!index.isValid())
        return;

    QString path = QFileDialog::getOpenFileName(this, tr("Открыть файл"), QString(),
                                                tr("Изображения (*.png *.jpeg *.bmp *.svg *.ico)"));
    if (!path.isEmpty())
        m_storage->setIcon(index.row(), path);
}

void RibbonCommandListWidget::onDoubleClicked(const QModelIndex &index)
{
    if (index.isValid())
        onEditTitle();
}
