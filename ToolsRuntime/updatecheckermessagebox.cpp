// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "updatecheckermessagebox.h"
#include "updatechecker.h"
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

    QStandardItemModel *model = nullptr;
    UpdateChecker::MakeUpdateModel(&model, lst, this);
    view->setModel(model);

    QGridLayout *main = (QGridLayout*)layout();
    main->addWidget(view, main->rowCount(), 0, 1, main->columnCount());
}
