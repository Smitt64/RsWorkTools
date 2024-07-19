// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "updatecheckermessagebox.h"
#include <QStandardItemModel>
#include <QTreeView>
#include <QLayout>
#include <QGridLayout>

UpdateCheckerMessageBox::UpdateCheckerMessageBox(QWidget *parent) :
    QMessageBox(parent)
{
    setIcon(QMessageBox::Information);
    setWindowTitle(tr("Проверка обновлений"));
    setText(tr("Найдена новая версия"));
    setStandardButtons(QMessageBox::Ok);
}

UpdateCheckerMessageBox::~UpdateCheckerMessageBox()
{

}

void UpdateCheckerMessageBox::setList(const CheckDataList &lst)
{
    QTreeView *view = new QTreeView(this);
    view->setRootIsDecorated(false);
    view->setMinimumWidth(320);
    view->setMinimumHeight(240);

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Наименование"),
                                      tr("Версия"),
                                      tr("Размер")});

    for(const CheckUpdateData &item : lst)
    {
        QStandardItem *name = new QStandardItem();
        name->setText(item.name);

        QStandardItem *version = new QStandardItem();
        version->setText(item.version);

        QStandardItem *size = new QStandardItem();
        size->setText(item.sizeString);

        model->appendRow({name, version, size});
    }

    view->setModel(model);

    QGridLayout *main = (QGridLayout*)layout();
    main->addWidget(view, main->rowCount(), 0, 1, main->columnCount());
}
