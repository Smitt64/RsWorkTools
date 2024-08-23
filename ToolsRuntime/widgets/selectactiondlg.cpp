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

typedef struct tagAcctionTableModelItem
{
    tagAcctionTableModelItem()
    {
        action = nullptr;
    }

    QString name;
    QAction *action;
}AcctionTableModelItem;

#define IS_ACTION_ROLE (Qt::UserRole + 1)

class AcctionTableModel : public QAbstractTableModel
{
    QVector<AcctionTableModelItem> m_Items;

public:
    AcctionTableModel(QObject *parent = 0) : 
        QAbstractTableModel(parent)
    {
        int count = windowActionsRegistry()->menuCount();
        QStringList menus = windowActionsRegistry()->menuNames();

        for (int i = 0; i < count; ++i)
        {
            int pos = menus[i].indexOf('_');
            AcctionTableModelItem item;
            item.name = menus[i].mid(pos + 1);
            m_Items.push_back(item);

            QList<QAction*> actions = windowActionsRegistry()->menuActions(menus[i]);
            for (int j = 0; j < actions.size(); ++j)
            {
                item.action = actions[j];
                m_Items.push_back(item);
            }
        }
    }

    AcctionTableModelItem item(const QModelIndex &index) const
    {
        return m_Items[index.row()];
    }

    int rowCount(const QModelIndex &parent) const
    {
        return m_Items.size();
    }

    int columnCount(const QModelIndex &parent) const
    {
        return 1;
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (!m_Items[index.row()].action)
            return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;

        return QAbstractTableModel::flags(index) | Qt::ItemNeverHasChildren;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole)
        {
            if (m_Items[index.row()].action != nullptr)
                return m_Items[index.row()].action->text();
            else
                return m_Items[index.row()].name;
        }
        else if (role == Qt::DecorationRole)
        {
            if (m_Items[index.row()].action != nullptr)
                return m_Items[index.row()].action->icon();
        }
        else if (role == IS_ACTION_ROLE)
            return m_Items[index.row()].action != nullptr;

        return QVariant();
    }
};

class MenuStyledItemDelegate : public QStyledItemDelegate
{
    AcctionTableModel *m_pModel;
public:
    MenuStyledItemDelegate(QObject *parent = nullptr) : 
        QStyledItemDelegate(parent) 
    {
        m_pModel = dynamic_cast<AcctionTableModel*>(parent);
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyle *style = QApplication::style();
        QAction *action = m_pModel->item(index).action;

        if (action)
        {
            QStyleOptionMenuItem opt;
            opt.rect = option.rect;
            opt.state = option.state;
            opt.text = action->text();
            opt.icon = action->icon();
            style->drawControl(QStyle::CE_MenuItem, &opt, painter);
        }
        else
        {
            QStyleOptionToolBox opt;
            (*((QStyleOption*)&opt)) = option;
            opt.text = index.data().toString();

            style->drawControl(QStyle::CE_ToolBoxTab, &opt, painter);
        }
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyle *style = QApplication::style();

        QMenu menu;
        QAction *action = menu.addAction("test");

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
    ui->listView->setItemDelegate(new MenuStyledItemDelegate(m_pModel));

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

    if (index.data(IS_ACTION_ROLE).toBool())
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void SelectActionDlg::onItemDbClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    if (!index.data(IS_ACTION_ROLE).toBool())
        return;

    accept();
}

QAction *SelectActionDlg::getSelectedAction()
{
    QModelIndex index = ui->listView->currentIndex();
    if (!index.isValid())
        return nullptr;

    return m_pModel->item(index).action;
}
