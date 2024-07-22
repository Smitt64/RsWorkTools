// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "optionsdlg.h"
#include "ui_optionsdlg.h"
#include "styleoptionspage.h"
#include <QListWidgetItem>
#include <QDebug>
#include <QStyleFactory>

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

    void restoreAppStyle()
    {
        if (m_CurrentStyle.isEmpty())
            return;

        QStyle *style = QStyleFactory::create(m_CurrentStyle);
        qApp->setStyle(style);
    }

    QString m_CurrentStyle;
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
    Q_D(OptionsDlg);

    if (d->m_CurrentStyle.isEmpty())
        return;

    addPage(tr("Оформление"), QIcon(":/icons/themeui.dll_14_701-0.png"), new StyleOptionsPage());
}

void OptionsDlg::addPage(const QString &title, const QIcon &icon, OptionsPage *page)
{
    Q_D(OptionsDlg);

    QListWidgetItem *pageitem = new QListWidgetItem(title);
    pageitem->setIcon(icon);
    ui->listWidget->addItem(pageitem);

    page->setOptionsDlg(this);
    d->m_Pages.append(page);
}

void OptionsDlg::setDefaultStyle(const QString &style)
{
    Q_D(OptionsDlg);
    d->m_CurrentStyle = style;
}

void OptionsDlg::closeEvent(QCloseEvent *event)
{
    Q_D(OptionsDlg);
    d->restoreAppStyle();
    QDialog::closeEvent(event);
}

void OptionsDlg::done(int r)
{
    Q_D(OptionsDlg);
    QDialog::done(r);
    d->restoreAppStyle();
}

const QString &OptionsDlg::defaultStyle() const
{
    Q_D(const OptionsDlg);
    return d->m_CurrentStyle;
}

void OptionsDlg::showEvent(QShowEvent *event)
{
    Q_D(OptionsDlg);
    QDialog::showEvent(event);

    if (!ui->listWidget->count())
        return;

    ui->listWidget->setCurrentItem(0);
    d->itemClicked(ui->listWidget->item(0), 0);
}
