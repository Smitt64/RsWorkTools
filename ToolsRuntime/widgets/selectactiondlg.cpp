#include "selectactiondlg.h"
#include "ui_selectactiondlg.h"
#include "windowactionsregistry.h"
#include <QAction>
#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QDebug>
#include <QMenu>
#include <QDialogButtonBox>
#include <QPushButton>

class AcctionTableModel : public QAbstractTableModel
{
public:
    AcctionTableModel(QObject *parent = 0) : 
        QAbstractTableModel(parent)
    {
    }

    int rowCount(const QModelIndex &parent) const
    {
        return windowActionsRegistry()->actionCount();
    }

    int columnCount(const QModelIndex &parent) const
    {
        return 1;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole)
            return windowActionsRegistry()->action(index.row())->text();
        else if (role == Qt::DecorationRole)
            return windowActionsRegistry()->action(index.row())->icon();

        return QVariant();
    }
};

class MenuStyledItemDelegate : public QStyledItemDelegate
{
public:
    MenuStyledItemDelegate(QObject *parent = nullptr) : 
        QStyledItemDelegate(parent) 
    {

    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyle *style = QApplication::style();
        QAction *action = windowActionsRegistry()->action(index.row());

        QStyleOptionMenuItem opt;
        opt.rect = option.rect;
        opt.state = option.state;
        opt.text = action->text();
        opt.icon = action->icon();
        style->drawControl(QStyle::CE_MenuItem, &opt, painter);
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyle *style = QApplication::style();

        QMenu menu;
        QAction *action = windowActionsRegistry()->action(index.row());
        menu.addAction(action);

        QSize sz = menu.actionGeometry(action).size();
        return style->sizeFromContents(QStyle::CT_MenuItem, &option, sz);
    }
};

SelectActionDlg::SelectActionDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectActionDlg)
{
    ui->setupUi(this);

    m_pModel = new AcctionTableModel(this);
    ui->listView->setModel(m_pModel);
    ui->listView->setItemDelegate(new MenuStyledItemDelegate(this));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->listView, &QAbstractItemView::clicked, this, &SelectActionDlg::onItemClicked);
    connect(ui->listView, &QAbstractItemView::doubleClicked, this, &SelectActionDlg::onItemDbClicked);
}

SelectActionDlg::~SelectActionDlg()
{
    delete ui;
}

void SelectActionDlg::onItemClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void SelectActionDlg::onItemDbClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    accept();
}

QAction *SelectActionDlg::getSelectedAction()
{
    QModelIndex index = ui->listView->currentIndex();
    if (!index.isValid())
        return nullptr;

    return windowActionsRegistry()->action(index.row());
}