#ifndef STYLEOPTIONSPAGE_H
#define STYLEOPTIONSPAGE_H

#include "optionsdlg/OptionsPage.h"
#include <QWidget>

namespace Ui {
class StyleOptionsPage;
}

class StyleOptionsPage : public OptionsPage
{
    Q_OBJECT

public:
    explicit StyleOptionsPage(QWidget *parent = nullptr);
    ~StyleOptionsPage();

private:
    Ui::StyleOptionsPage *ui;
};

#endif // STYLEOPTIONSPAGE_H
