// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef OPTIONSPAGE_H
#define OPTIONSPAGE_H

#include <QWidget>
#include "ToolsRuntime_global.h"

class QSettings;
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
    QSettings *settings();

    virtual int save();
    virtual void restore();

private:
    void setOptionsDlg(OptionsDlg *dlg);

    OptionsPagePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(OptionsPage);
};

#endif // OPTIONSPAGE_H
