#include "errorwidget.h"
#include "ui_errorwidget.h"
#include "errorfiltermodel_p.h"
#include "errorsmodel.h"
#include <QStyle>
#include <QAbstractItemView>

ErrorFilterModel::ErrorFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    m_ShowInformation = true;
    m_ShowWarnings = true;
    m_ShowErrors = true;
    pErrors = NULL;
}

ErrorFilterModel::~ErrorFilterModel()
{

}

void ErrorFilterModel::setShowInformation(bool value)
{
    if (m_ShowInformation == value)
        return;

    m_ShowInformation = value;
    invalidateFilter();
}

void ErrorFilterModel::setShowWarnings(bool value)
{
    if (m_ShowWarnings == value)
        return;

    m_ShowWarnings = value;
    invalidateFilter();
}

void ErrorFilterModel::setShowErrors(bool value)
{
    if (m_ShowErrors == value)
        return;

    m_ShowErrors = value;
    invalidateFilter();
}

void ErrorFilterModel::setFmtErrors(ErrorsModel *e)
{
    pErrors = e;
    setSourceModel(pErrors);
}

bool ErrorFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent);
    if (!pErrors)
        return false;

    bool result = false;
    qint16 type = pErrors->type(sourceRow);

    if (type == ErrorsModel::TypeError && m_ShowErrors)
        result = true;

    if (type == ErrorsModel::TypeWarning && m_ShowWarnings)
        result = true;

    if (type == ErrorsModel::TypeInfo && m_ShowInformation)
        result = true;

    return result;
}

// -----------------------------------------------------------------

class ErrorWidgetPrivate
{
    Q_DECLARE_PUBLIC(ErrorWidget)
public:
    ErrorWidgetPrivate(ErrorWidget *obj)
    {
        q_ptr = obj;
    }

    ErrorFilterModel *pFilterModel;
    ErrorsModel *pErrors;
    qint16 m_Mode;

    ErrorWidget *q_ptr;
};

ErrorWidget::ErrorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ErrorWidget)
    ,d_ptr(new ErrorWidgetPrivate(this))
{
    Q_D(ErrorWidget);
    d->pErrors = Q_NULLPTR;
    ui->setupUi(this);

    d->pFilterModel = new ErrorFilterModel(this);
    ui->tableView->verticalHeader()->setDefaultSectionSize(30);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));

    ui->toolButtonMsg->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
    ui->toolButtonWarning->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
    ui->toolButtonError->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), SLOT(onActivated(QModelIndex)));
    connect(ui->tableView, SIGNAL(activated(QModelIndex)), SLOT(onActivated(QModelIndex)));

    connect(ui->toolButtonMsg, SIGNAL(toggled(bool)), d->pFilterModel, SLOT(setShowInformation(bool)));
    connect(ui->toolButtonError, SIGNAL(toggled(bool)), d->pFilterModel, SLOT(setShowErrors(bool)));
    connect(ui->toolButtonWarning, SIGNAL(toggled(bool)), d->pFilterModel, SLOT(setShowWarnings(bool)));
}

ErrorWidget::~ErrorWidget()
{
    delete ui;
}

void ErrorWidget::onActivated(const QModelIndex &index)
{
    Q_D(ErrorWidget);
    ui->textEdit->setText(d->pErrors->textAt(index.row()));
}

void ErrorWidget::setMessage(const QString &msg)
{
    ui->msgLabel->setText(msg);
}

void ErrorWidget::errorsCountChanged(const quint32 &value)
{
    ui->toolButtonError->setText(tr("Ошибки: %1").arg(value));
    ui->tableView->scrollToBottom();
}

void ErrorWidget::warningsCountChanged(const quint32 &value)
{
    ui->toolButtonWarning->setText(tr("Предупреждения: %1").arg(value));
    ui->tableView->scrollToBottom();
}

void ErrorWidget::infoCountChanged(const quint32 &value)
{
    ui->toolButtonMsg->setText(tr("Сообщения: %1").arg(value));
    ui->tableView->scrollToBottom();
}

void ErrorWidget::setErrors(ErrorsModel *e)
{
    Q_D(ErrorWidget);

    d->pErrors = e;
    d->pFilterModel->setFmtErrors(e);
    ui->tableView->setModel(d->pFilterModel);
    ui->tableView->setColumnWidth(0, 150);
    ui->tableView->setColumnWidth(1, 320);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    ui->tableView->selectRow(0);
    onActivated(e->index(0, 1));

    ui->toolButtonMsg->setText(tr("Сообщения: %1")
                                   .arg(e->infoCount()));
    ui->toolButtonWarning->setText(tr("Предупреждения: %1")
                                       .arg(e->warningsCount()));
    ui->toolButtonError->setText(tr("Ошибки: %1")
                                     .arg(e->errorsCount()));

    connect(e, SIGNAL(errorsCountChanged(quint32)), SLOT(errorsCountChanged(quint32)));
    connect(e, SIGNAL(warningsCountChanged(quint32)), SLOT(warningsCountChanged(quint32)));
    connect(e, SIGNAL(infoCountChanged(quint32)), SLOT(infoCountChanged(quint32)));
}
