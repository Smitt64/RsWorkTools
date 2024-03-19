#include "updatecheckermessagebox.h"
#include <QStandardItemModel>
#include <QListView>
#include <QLayout>

UpdateCheckerMessageBox::UpdateCheckerMessageBox(QWidget *parent) :
    QMessageBox(parent)
{
    setIcon(QMessageBox::Information);
    setWindowTitle(tr("Проверка обновлений"));
    setInformativeText(tr("Найдена новая версия"));
    setStandardButtons(QMessageBox::Ok);
}

UpdateCheckerMessageBox::~UpdateCheckerMessageBox()
{

}

void UpdateCheckerMessageBox::setList(const CheckDataList &lst)
{
    QListView *view = new QListView(this);
    layout()->addWidget(view);

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Наименование"),
                                      tr("Версия"),
                                      tr("Размер")});

    for(const CheckUpdateData &item : lst)
    {
        QStandardItem *name = new QStandardItem();
        name->setText(item.name);

        QStandardItem *version = new QStandardItem();
        version->setText(item.name);

        QStandardItem *size = new QStandardItem();
        size->setText(item.name);

        model->appendRow({name, version, size});
    }
}