// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "optionsdlg.h"
#include "ui_optionsdlg.h"
#include "styleoptionspage.h"
#include <QListWidgetItem>

class OptionsDlgPrivate
{
    Q_DECLARE_PUBLIC(OptionsDlg)
public:
    OptionsDlgPrivate(OptionsDlg *obj)
    {
        q_ptr = obj;
    }

    void itemClicked(QListWidgetItem *item, const int &row)
    {
        Q_Q(OptionsDlg);

        q->ui->scrollArea->setWidget(m_Pages[row]);
    }

    QList<OptionsPage*> m_Pages;
    OptionsDlg *q_ptr;
};

OptionsDlg::OptionsDlg(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::OptionsDlg),
    d_ptr(new OptionsDlgPrivate(this))
{
    Q_D(OptionsDlg);
    ui->setupUi(this);
    ui->listWidget->setItemAlignment(Qt::AlignCenter);

    connect(ui->listWidget, &QListWidget::itemClicked, [=](QListWidgetItem *item)
    {
        d->itemClicked(item, ui->listWidget->row(item));
    });
}

OptionsDlg::~OptionsDlg()
{
    delete ui;
    delete d_ptr;
}

void OptionsDlg::addStylePage()
{
    addPage(tr("Оформление"), QIcon(":/icons/themeui.dll_14_701-0.png"), new StyleOptionsPage());
}

void OptionsDlg::addPage(const QString &title, const QIcon &icon, OptionsPage *page)
{
    Q_D(OptionsDlg);

    QListWidgetItem *pageitem = new QListWidgetItem(title);
    pageitem->setIcon(icon);
    ui->listWidget->addItem(pageitem);

    d->m_Pages.append(page);
}
