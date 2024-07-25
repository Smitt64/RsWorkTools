// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "styleoptionspage.h"
#include "ui_styleoptionspage.h"
#include <QStyleFactory>
#include <QMdiSubWindow>
#include <QUiLoader>
#include <QFile>
#include <QDebug>
#include <QMenuBar>
#include <QStyle>
#include <QSettings>

class StyleOptionsPagePrivate
{
    Q_DECLARE_PUBLIC(StyleOptionsPage)
public:
    StyleOptionsPagePrivate(StyleOptionsPage *parent)
    {
        q_ptr = parent;

        auto LoadPreview = [=]() -> QWidget*
        {
            QFile file(":/ui/StyleTest.ui");
            file.open(QFile::ReadOnly);

            QUiLoader loader;
            QWidget *widget = loader.load(&file);
            file.close();

            return widget;
        };

        w = LoadPreview();
    }

    void SetStyle(const QString &stylename)
    {
        Q_Q(StyleOptionsPage);
        QStyle *style = QStyleFactory::create(stylename);

        style->polish(w);
        qApp->setStyle(style);
        w->setStyle(style);
        q->ui->mdiArea->update();
        w->update();
    }

    QWidget *w;
    QString m_Group, m_Key;
    StyleOptionsPage *q_ptr;
};

StyleOptionsPage::StyleOptionsPage(QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::StyleOptionsPage)
    , d_ptr(new StyleOptionsPagePrivate(this))
{
    Q_D(StyleOptionsPage);
    ui->setupUi(this);

    ui->comboBox->addItems(QStyleFactory::keys());
    QMdiSubWindow *wnd = ui->mdiArea->addSubWindow(d->w);
    wnd->setMinimumSize(QSize(600, 500));

    wnd->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);
    wnd->show();

    auto SetStyle = [=](const QString &stylename)
    {
        d->SetStyle(stylename);
    };

    connect(ui->comboBox, &QComboBox::currentTextChanged, SetStyle);
}

StyleOptionsPage::~StyleOptionsPage()
{
    delete ui;
}

void StyleOptionsPage::setSettingsKey(const QString &group, const QString &key)
{
    Q_D(StyleOptionsPage);
    d->m_Group = group;
    d->m_Key = key;
}

int StyleOptionsPage::save()
{
    Q_D(StyleOptionsPage);
    QSettings *ini = settings();

    if (!d->m_Group.isEmpty())
        ini->beginGroup(d->m_Group);

    ini->setValue(d->m_Key, ui->comboBox->currentText());

    if (!d->m_Group.isEmpty())
        ini->endGroup();

    return 0;
}

QString StyleOptionsPage::currentStyle() const
{
    return ui->comboBox->currentText();
}

void StyleOptionsPage::restore()
{
    Q_D(StyleOptionsPage);
    QSettings *ini = settings();

    if (!d->m_Group.isEmpty())
        ini->beginGroup(d->m_Group);

    ui->comboBox->setCurrentText(ini->value(d->m_Key, "windowsvista").toString());
    d->SetStyle(ui->comboBox->currentText());

    if (!d->m_Group.isEmpty())
        ini->endGroup();
}
