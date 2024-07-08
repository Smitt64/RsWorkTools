#include "errordlg.h"
#include "ui_errordlg.h"
#include "errorsmodel.h"
#include "errorfiltermodel_p.h"
#include <QPushButton>

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
class ErrorDlgPrivate
{
    Q_DECLARE_PUBLIC(ErrorDlg)
public:
    ErrorDlgPrivate(ErrorDlg *obj)
    {
        q_ptr = obj;
    }

    ErrorFilterModel *pFilterModel;
    ErrorsModel *pErrors;
    qint16 m_Mode;

    ErrorDlg *q_ptr;
};

ErrorDlg::ErrorDlg(const qint16 &mode, QWidget *parent) :
    QDialog(parent),
    d_ptr(new ErrorDlgPrivate(this)),
    ui(new Ui::ErrorDlg)
{
    Q_D(ErrorDlg);
    d->pErrors = Q_NULLPTR;
    ui->setupUi(this);

    d->pFilterModel = new ErrorFilterModel(this);
    ui->tableView->verticalHeader()->setDefaultSectionSize(30);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    d->m_Mode = mode;
    if (mode == ModeInformation)
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok);
    else if (mode == ModeMessageBox)
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Abort);
    else if (mode == ModeWidget)
        ui->buttonBox->setVisible(false);

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

ErrorDlg::~ErrorDlg()
{
    delete d_ptr;
    delete ui;
}

void ErrorDlg::setErrors(ErrorsModel *e)
{
    Q_D(ErrorDlg);

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

    if (d->m_Mode == ModeMessageBox)
    {
        if (e->hasErrors())
            ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    }

    connect(e, SIGNAL(errorsCountChanged(quint32)), SLOT(errorsCountChanged(quint32)));
    connect(e, SIGNAL(warningsCountChanged(quint32)), SLOT(warningsCountChanged(quint32)));
    connect(e, SIGNAL(infoCountChanged(quint32)), SLOT(infoCountChanged(quint32)));
}

void ErrorDlg::onActivated(const QModelIndex &index)
{
    Q_D(ErrorDlg);
    ui->textEdit->setText(d->pErrors->textAt(index.row()));
}

void ErrorDlg::setMessage(const QString &msg)
{
    ui->msgLabel->setText(msg);
}

QDialogButtonBox *ErrorDlg::buttons()
{
    return ui->buttonBox;
}

void ErrorDlg::errorsCountChanged(const quint32 &value)
{
    ui->toolButtonError->setText(tr("Ошибки: %1").arg(value));
    ui->tableView->scrollToBottom();
}

void ErrorDlg::warningsCountChanged(const quint32 &value)
{
    ui->toolButtonWarning->setText(tr("Предупреждения: %1").arg(value));
    ui->tableView->scrollToBottom();
}

void ErrorDlg::infoCountChanged(const quint32 &value)
{
    ui->toolButtonMsg->setText(tr("Сообщения: %1").arg(value));
    ui->tableView->scrollToBottom();
}
