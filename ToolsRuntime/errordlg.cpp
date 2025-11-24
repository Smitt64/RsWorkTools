// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "errordlg.h"
#include "ui_errordlg.h"
#include "errorsmodel.h"
#include "errorfiltermodel_p.h"
#include <QPushButton>
#include <QStyle>

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

ErrorDlg::ErrorDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDlg),
    d_ptr(new ErrorDlgPrivate(this))
{
    ui->setupUi(this);
    setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
}

ErrorDlg::ErrorDlg(const qint16 &mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDlg),
    d_ptr(new ErrorDlgPrivate(this))
{
    ui->setupUi(this);

    setMode(mode);
    setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
}

ErrorDlg::~ErrorDlg()
{
    delete d_ptr;
    delete ui;
}

void ErrorDlg::setMode(const qint16 &mode)
{
    Q_D(ErrorDlg);

    d->m_Mode = mode;
    if (mode == ModeInformation)
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok);
    else if (mode == ModeMessageBox)
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Abort);
    else if (mode == ModeWidget)
        ui->buttonBox->setVisible(false);
}

void ErrorDlg::setErrors(ErrorsModel *e)
{
    Q_D(ErrorDlg);
    ui->widget->setErrors(e);

    if (d->m_Mode == ModeMessageBox)
    {
        if (e->hasErrors())
            ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    }
}

void ErrorDlg::setMessage(const QString &msg)
{
    ui->widget->setMessage(msg);
}

QDialogButtonBox *ErrorDlg::buttons()
{
    return ui->buttonBox;
}
