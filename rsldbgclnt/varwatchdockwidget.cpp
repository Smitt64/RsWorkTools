#include "varwatchdockwidget.h"
#include "models/varwatchmodel.h"
#include <QTreeView>
#include <QHeaderView>
#include <QPushButton>
#include <QDebug>
#include <QScrollBar>
#include <QStyleOptionButton>
#include <QStyle>
#include <QMouseEvent>

// https://stackoverflow.com/questions/59202334/python-pyqt5-is-it-possible-to-add-a-button-to-press-inside-qtreeview
TreeButtonDelegate::TreeButtonDelegate() :
    QStyledItemDelegate()
{
    m_pressed = false;
}

TreeButtonDelegate::~TreeButtonDelegate()
{

}

QStyleOptionToolButton TreeButtonDelegate::getOption(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionToolButton btnOption;
    btnOption.initFrom(option.widget);
    btnOption.toolButtonStyle = Qt::ToolButtonIconOnly;
    btnOption.features = QStyleOptionToolButton::None;
    btnOption.arrowType = Qt::NoArrow;
    btnOption.text = "";

    btnOption.rect = QRect(option.rect);
    //btnOption.rect.setWidth(btnOption.rect.height());

    btnOption.iconSize = QSize(16, 16);
    btnOption.icon = QIcon::fromTheme("ViewVarValue");
    btnOption.subControls = QStyle::SC_ToolButton;

    //QStyle *style = option.widget->style();
    //int margin = style->pixelMetric(QStyle::PM_ButtonMargin, &btnOption) * 2;
    btnOption.rect.setLeft(option.rect.right() - (btnOption.iconSize.width() + 2));

    //if (textRect.width() < textWidth + margin)

    return btnOption;
}

void TreeButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data(VarWatchModel::ShowWatchButtonRole).toBool())
    {
        /*QStyleOptionViewItem opt = option;
        opt.rect.setX(opt.rect.x() + 18);
        opt.rect.setWidth(opt.rect.width() - 18);

        QStyledItemDelegate::paint(painter, opt, index);*/

        QStyledItemDelegate::paint(painter, option, index);
        QStyleOptionToolButton btnOption = getOption(option, index);
        btnOption.state &= ~QStyle::State_HasFocus;
        btnOption.state &= ~QStyle::State_DownArrow;
        btnOption.state &= ~QStyle::State_UpArrow;

        if (!m_MousePos.isNull() && btnOption.rect.contains(m_MousePos))
        {
            btnOption.state |= QStyle::State_MouseOver;

            if (m_pressed)
                btnOption.state |= QStyle::State_On;
        }
        else
            btnOption.state &= ~QStyle::State_MouseOver;

        option.widget->style()->drawComplexControl(QStyle::CC_ToolButton, &btnOption, painter);
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

bool TreeButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!index.data(VarWatchModel::ShowWatchButtonRole).toBool())
        return QStyledItemDelegate::editorEvent(event, model, option, index);

    if (event->type() == QEvent::Enter || event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouse = dynamic_cast<QMouseEvent*>(event);

        if (mouse)
        {
            m_MousePos = mouse->pos();

            QWidget *widget = const_cast<QWidget*>(option.widget);
            widget->update();
        }
    }
    else if (event->type() == QEvent::Leave)
    {
        m_MousePos = QPoint();

        QWidget *widget = const_cast<QWidget*>(option.widget);
        widget->update();
    }
    else if ((event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick))
    {
        QMouseEvent *mouse = dynamic_cast<QMouseEvent*>(event);

        if (mouse && mouse->button() == Qt::LeftButton)
        {
            QRect rect = getOption(option, index).rect;
            if (rect.contains(mouse->pos()))
            {
                m_pressed = true;

                QWidget *widget = const_cast<QWidget*>(option.widget);
                widget->update();
            }

            if (event->type() == QEvent::MouseButtonDblClick)
                return true;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouse = dynamic_cast<QMouseEvent*>(event);

        if (m_pressed && mouse && mouse->button() == Qt::LeftButton)
        {
            QRect rect = getOption(option, index).rect;
            if (rect.contains(mouse->pos()))
                emit clicked(index);
        }

        m_pressed = false;
        QWidget *widget = const_cast<QWidget*>(option.widget);
        widget->update();
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

// -------------------------------------------------------------------------

VarWatchDockWidget::VarWatchDockWidget() :
    StdViewDockWidget()
{
    QTreeView *tree = view();
    tree->setRootIsDecorated(true);
    tree->resetIndentation();
    tree->setMouseTracking(true);
    tree->setUniformRowHeights(true);

    m_pDelegate.reset(new TreeButtonDelegate());
    tree->setItemDelegateForColumn(VarWatchModel::ColumnValue, m_pDelegate.data());

    //tree->setIndentation(10);
    connect(tree, &QTreeView::expanded, [=](const QModelIndex &index)
    {
        int indx = index.data(VarWatchModel::IndexRole).toInt();
        qint64 stack = index.data(VarWatchModel::IndexRole).value<qint64>();
        bool RealHasChild = index.data(VarWatchModel::RealHasChild).toBool();

        if (!RealHasChild)
        {
            m_LastScroll = view()->verticalScrollBar()->value();
            emit expandVariable(indx, stack);
        }
    });

    connect(m_pDelegate.data(), &TreeButtonDelegate::clicked, [=](const QModelIndex &index)
    {
        qint64 value = index.data(VarWatchModel::ValueRole).value<qint64>();
        qint64 info = index.data(VarWatchModel::ValueInfoRole).value<qint64>();

        emit showVarValue(value, info);
    });

    setDrawBranches(true);
}

VarWatchDockWidget::~VarWatchDockWidget()
{

}

void VarWatchDockWidget::setModel(QAbstractItemModel *model)
{
    StdViewDockWidget::setModel(model);

    view()->header()->setSectionResizeMode(VarWatchModel::ColumnName, QHeaderView::ResizeToContents);
    connect((VarWatchModel*)model, &VarWatchModel::expand, view(), &QTreeView::expand);

    connect((VarWatchModel*)model, &VarWatchModel::modelAboutToBeReset, [=]()
    {
        m_LastScroll = view()->verticalScrollBar()->value();
    });
    connect((VarWatchModel*)model, &VarWatchModel::modelReset, [=]()
    {
        qDebug() << m_LastScroll;

        if (m_LastScroll.isValid())
            view()->verticalScrollBar()->setValue(m_LastScroll.toInt());
    });

    connect((VarWatchModel*)model, &VarWatchModel::expand, [=](const QModelIndex &index)
    {
        //QPushButton *btn = new QPushButton();
        //view()->setIndexWidget(index, btn);
        //qDebug() << index << view()->itemDelegate(index);
    });
}
