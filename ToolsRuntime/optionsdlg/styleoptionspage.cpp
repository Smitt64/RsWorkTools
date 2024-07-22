#include "styleoptionspage.h"
#include "ui_styleoptionspage.h"
#include <QStyleFactory>
#include <QMdiSubWindow>
#include <QUiLoader>
#include <QFile>
#include <QDebug>
#include <QMenuBar>
#include <QStyle>

StyleOptionsPage::StyleOptionsPage(QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::StyleOptionsPage)
{
    ui->setupUi(this);

    ui->comboBox->addItems(QStyleFactory::keys());

    auto LoadPreview = [=]() -> QWidget*
    {
        QFile file(":/ui/StyleTest.ui");
        file.open(QFile::ReadOnly);

        QUiLoader loader;
        QWidget *widget = loader.load(&file);
        file.close();

        return widget;
    };

    QWidget *w = LoadPreview();
    QMdiSubWindow *wnd = ui->mdiArea->addSubWindow(w);
    wnd->setMinimumSize(QSize(600, 500));

    wnd->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);
    wnd->show();

    auto SetStyle = [=](const QString &stylename)
    {
        QStyle *style = QStyleFactory::create(stylename);

        style->polish(w);
        qApp->setStyle(style);
        w->setStyle(style);
        ui->mdiArea->update();
        w->update();
    };

    connect(ui->comboBox, &QComboBox::currentTextChanged, SetStyle);
}

StyleOptionsPage::~StyleOptionsPage()
{
    delete ui;
}
