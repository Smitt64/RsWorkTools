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
