// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
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
