#include "optionsdlg.h"
#include "ui_optionsdlg.h"

OptionsDlg::OptionsDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionsDlg)
{
    ui->setupUi(this);
}

OptionsDlg::~OptionsDlg()
{
    delete ui;
}
