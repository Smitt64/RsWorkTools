#include "updateoptionspage.h"
#include "ui_updateoptionspage.h"
#include "updatechecker.h"
#include <QStandardItemModel>
#include <QMessageBox>
#include <QHeaderView>
#include <QSettings>

UpdateOptionsPage::UpdateOptionsPage(QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::UpdateOptionsPage)
{
    ui->setupUi(this);

    CheckDataList updatedata;
    QStandardItemModel *model = nullptr;
    UpdateChecker::MakeUpdateModel(&model, updatedata, this);
    ui->treeView->setModel(model);
    ui->treeView->header()->resizeSection(0, 400);
}

UpdateOptionsPage::~UpdateOptionsPage()
{
    delete ui;
}

int UpdateOptionsPage::save()
{
    QSettings *ps = settings();
    ps->beginGroup("Update");
    ps->setValue("Autocheck", ui->autoCheck->isChecked());
    ps->setValue("Interval", ui->intervalEdit->time());
    ps->endGroup();
    return 0;
}

void UpdateOptionsPage::restore()
{
    QSettings *ps = settings();
    ps->beginGroup("Update");
    ui->autoCheck->setChecked(ps->value("Autocheck", true).toBool());
    ui->intervalEdit->setTime(ps->value("Interval", QTime(0, 15)).toTime());
    ps->endGroup();
}

void UpdateOptionsPage::on_checkButton_clicked()
{
    QScopedPointer<UpdateChecker> checker(new UpdateChecker());
    checker->setProgramName("RsWorkMaintenanceTool.exe");

    connect(checker.data(), &UpdateChecker::checkFinished, [=](bool hasUpdates, const CheckDataList &updatedata)
    {
        if (!hasUpdates)
        {
            if (updatedata.isEmpty() || (updatedata.size() == 1 && updatedata[0].name.contains("There are currently no updates available")))
            {
                QMessageBox::information(this, tr("Проверка обновлений"),
                                         tr("Обновлений не обнаружено"));
            }
            else
            {
                QMessageBox::critical(this, tr("Проверка обновлений"),
                                         tr("При проверке обновлений возникла ошибка"));
            }
        }
    });

    CheckDataList updatedata;
    checker->checkUpdate(&updatedata);

    QStandardItemModel *model = (QStandardItemModel*)ui->treeView->model();

    if (model)
        delete model;

    UpdateChecker::MakeUpdateModel(&model, updatedata, this);

    ui->treeView->setModel(model);
    ui->treeView->header()->resizeSection(0, 400);
}

