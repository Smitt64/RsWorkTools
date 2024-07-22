// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "OptionsPage.h"

class OptionsPagePrivate
{
    Q_DECLARE_PUBLIC(OptionsPage)
public:
    OptionsPagePrivate(OptionsPage *parent)
    {
        q_ptr = parent;
    }

    OptionsDlg *m_pDlg;
    OptionsPage *q_ptr;
};

OptionsPage::OptionsPage(QWidget *parent) :
    QWidget(parent),
    d_ptr(new OptionsPagePrivate(this))
{

}

OptionsPage::~OptionsPage()
{

}

void OptionsPage::setOptionsDlg(OptionsDlg *dlg)
{
    Q_D(OptionsPage);
    d->m_pDlg = dlg;
}

OptionsDlg *OptionsPage::options()
{
    Q_D(OptionsPage);
    return d->m_pDlg;
}
