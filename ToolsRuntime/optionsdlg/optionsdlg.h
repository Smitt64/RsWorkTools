// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef OPTIONSDLG_H
#define OPTIONSDLG_H

#include <QDialog>

namespace Ui {
class OptionsDlg;
}

class OptionsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDlg(QWidget *parent = nullptr);
    ~OptionsDlg();

private:
    Ui::OptionsDlg *ui;
};

#endif // OPTIONSDLG_H
