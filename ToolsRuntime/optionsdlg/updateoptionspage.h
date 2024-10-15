#ifndef UPDATEOPTIONSPAGE_H
#define UPDATEOPTIONSPAGE_H

#include "optionsdlg/OptionsPage.h"
#include <QWidget>

namespace Ui {
class UpdateOptionsPage;
}

class UpdateOptionsPage : public OptionsPage
{
    Q_OBJECT

public:
    UpdateOptionsPage(QWidget *parent = nullptr);
    ~UpdateOptionsPage();

    virtual int save() Q_DECL_OVERRIDE;
    virtual void restore() Q_DECL_OVERRIDE;

private slots:
    void on_checkButton_clicked();

private:
    Ui::UpdateOptionsPage *ui;
};

#endif // UPDATEOPTIONSPAGE_H
