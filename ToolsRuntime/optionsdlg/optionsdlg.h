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
    void setDefaultStyle(const QString &style);
    const QString &defaultStyle() const;

protected:
    void addPage(const QString &title, const QIcon &icon, OptionsPage *page);
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void done(int r) Q_DECL_OVERRIDE;

private:
    Ui::OptionsDlg *ui;

    OptionsDlgPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(OptionsDlg);
};

#endif // OPTIONSDLG_H
