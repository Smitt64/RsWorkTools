// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "optionsdlg.h"
#include "optionsdlg/updateoptionspage.h"
#include "ui_optionsdlg.h"
#include "styleoptionspage.h"
#include "codeeditoroptionspage.h"
#include "rsloptionspage.h"
#include "commandsoptions.h"
#include "logoptionspage.h"
#include <QListWidgetItem>
#include <QDebug>
#include <QStyleFactory>
#include <QSettings>
#include <QPushButton>

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
        (void)q->ui->scrollArea->takeWidget();
        q->ui->scrollArea->setWidget(m_Pages[row]);
    }

    void restoreAppStyle()
    {
        if (m_CurrentStyle.isEmpty())
            return;

        QStyle *style = QStyleFactory::create(m_CurrentStyle);
        qApp->setStyle(style);
    }

    void applySettings()
    {
        for (OptionsPage *page : m_Pages)
        {
            (void)page->save();
            m_pSettings->sync();

            StyleOptionsPage *stylepage = qobject_cast<StyleOptionsPage*>(page);

            if (stylepage)
                m_CurrentStyle = stylepage->currentStyle();
        }
    }

    QSettings *m_pSettings;
    QString m_CurrentStyle;
    QList<OptionsPage*> m_Pages;
    OptionsDlg *q_ptr;
};

OptionsDlg::OptionsDlg(QSettings *settings, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::OptionsDlg),
    d_ptr(new OptionsDlgPrivate(this))
{
    Q_D(OptionsDlg);
    ui->setupUi(this);
    ui->listWidget->setItemAlignment(Qt::AlignCenter);

    d->m_pSettings = settings;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setAutoDefault(false);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setAutoDefault(false);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(false);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setDefault(false);

    connect(ui->listWidget, &QListWidget::itemClicked, [=](QListWidgetItem *item)
    {
        d->itemClicked(item, ui->listWidget->row(item));
    });

    QAbstractButton *Apply = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(Apply, &QAbstractButton::clicked, [=]()
    {
        d->applySettings();
    });
}

OptionsDlg::~OptionsDlg()
{
    delete ui;
    delete d_ptr;
}

void OptionsDlg::addStylePage(const QString &group, const QString &key)
{
    Q_D(OptionsDlg);

    if (d->m_CurrentStyle.isEmpty())
        return;

    StyleOptionsPage *stylepage = new StyleOptionsPage();
    stylepage->setSettingsKey(group, key);
    addPage(tr("Оформление"), QIcon(":/icons/themeui.dll_14_701-0.png"), stylepage);
}

void OptionsDlg::addCodeEditorPage(const QString &group, const QString &key)
{
    Q_D(OptionsDlg);

    CodeEditorOptionsPage *page = new CodeEditorOptionsPage();
    page->setSettingsKey(group, key);
    addPage(tr("Текстовый редактор"), QIcon(":/icons/wordpad.exe_14_128-5.png"), page);
}

void OptionsDlg::addRslPage()
{
    Q_D(OptionsDlg);

    RslOptionsPage *page = new RslOptionsPage();
    addPage(tr("Rsl интерпретатор"), QIcon(":/icons/shell32.dll_14_151-3.png"), page);
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

void OptionsDlg::addCommandsPage(bool UseRslCommands)
{
    CommandsOptions *page = new CommandsOptions();
    page->setUseRslcommands(UseRslCommands);
    addPage(tr("Команды"), QIcon(":/icons/osk.exe_14_APP_OSK-2.png"), page);
}

void OptionsDlg::addCommandsPage(CommandsOptions *page)
{
    addPage(tr("Команды"), QIcon(":/icons/osk.exe_14_APP_OSK-2.png"), page);
}

void OptionsDlg::addLogPage(const QString &prefix)
{
    LogOptionsPage *page = new LogOptionsPage();
    page->setPrefix(prefix);
    addPage(tr("Логирование"), QIcon(":/icons/fxscover.exe_14_108-2.png"), page);
}

void OptionsDlg::addUpdatePage()
{
    UpdateOptionsPage *page = new UpdateOptionsPage();
    page->setWindowIcon(QIcon(":/icons/inetcpl.cpl_14_4487-3.png"));
    addPage(tr("Обновления"), QIcon(":/icons/inetcpl.cpl_14_4487-3.png"), page);
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
}

void OptionsDlg::accept()
{
    if (!ui->buttonBox->button(QDialogButtonBox::Ok)->hasFocus())
        return;

    QDialog::accept();
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

QSettings *OptionsDlg::settings()
{
    Q_D(OptionsDlg);
    return d->m_pSettings;
}

int OptionsDlg::exec()
{
    Q_D(OptionsDlg);
    for (OptionsPage *page : qAsConst(d->m_Pages))
    {
        d->m_pSettings->sync();
        page->restore();
        d->m_pSettings->sync();

        StyleOptionsPage *stylepage = qobject_cast<StyleOptionsPage*>(page);

        if (stylepage)
            d->m_CurrentStyle = stylepage->currentStyle();
    }

    d->m_pSettings->sync();
    int result = QDialog::exec();

    if (result == QDialog::Accepted)
        d->applySettings();

    d->restoreAppStyle();

    return result;
}
