// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef OPTIONSDLG_H
#define OPTIONSDLG_H

#include <QDialog>
#include "ToolsRuntime_global.h"

namespace Ui {
class OptionsDlg;
}

class OptionsPage;
class OptionsDlgPrivate;
class TOOLSRUNTIME_EXPORT OptionsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDlg(QWidget *parent = nullptr);
    ~OptionsDlg();

    void addStylePage();

protected:
    void addPage(const QString &title, const QIcon &icon, OptionsPage *page);

private:
    Ui::OptionsDlg *ui;

    OptionsDlgPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(OptionsDlg);
};

#endif // OPTIONSDLG_H
