#include "styleoptionspage.h"
#include "ui_styleoptionspage.h"
#include <QStyleFactory>
#include <QMdiSubWindow>
#include <QUiLoader>
#include <QFile>

StyleOptionsPage::StyleOptionsPage(QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::StyleOptionsPage)
{
    ui->setupUi(this);

    ui->comboBox->addItems(QStyleFactory::keys());

    QFile file(":/ui/StyleTest.ui");
    file.open(QFile::ReadOnly);

    QUiLoader loader;
    QWidget *widget = loader.load(&file);
    file.close();
    QMdiSubWindow *wnd = ui->mdiArea->addSubWindow(widget);

    wnd->setMinimumSize(QSize(600, 500));
}

StyleOptionsPage::~StyleOptionsPage()
{
    delete ui;
}
