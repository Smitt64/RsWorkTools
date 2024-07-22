#ifndef OPTIONSPAGE_H
#define OPTIONSPAGE_H

#include <QWidget>
#include "ToolsRuntime_global.h"

class OptionsDlg;
class OptionsPagePrivate;
class TOOLSRUNTIME_EXPORT OptionsPage : public QWidget
{
    Q_OBJECT
    friend class OptionsDlg;
public:
    OptionsPage(QWidget *parent);
    virtual ~OptionsPage();

    OptionsDlg *options();

private:
    void setOptionsDlg(OptionsDlg *dlg);

    OptionsPagePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(OptionsPage);
};

#endif // OPTIONSPAGE_H
