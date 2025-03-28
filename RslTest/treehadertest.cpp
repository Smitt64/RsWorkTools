#include "treehadertest.h"
#include "ui_treehadertest.h"
#include "widgets/treeheadervew/gridtableheaderview.h"
#include <QStandardItemModel>

TreeHaderTest::TreeHaderTest(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TreeHaderTest)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(this);
    for (int i = 0; i < 9; i++)
    {
        QList<QStandardItem *> items;
        for (int j = 0; j < 5; j++)
            items.append(new QStandardItem(QString("item(%1,%2)").arg(i).arg(j)));

        model->appendRow(items);
    }

    ui->treeView->setModel(model);

    int levels = 2;

    GridTableHeaderView *header = new GridTableHeaderView(Qt::Horizontal, levels, 5);
    ui->treeView->setHeader(header);

    header->setCellLabel(1, 0, "cell1");
    header->setCellLabel(1, 1, "cell2");
    header->setCellLabel(1, 2, "cell3");
    header->setCellLabel(1, 3, "cell4");
    header->setCellLabel(1, 4, "cell5");

    header->setSpan(0, 0, 2, 0);
    header->setSpan(0, 1, 0, 2);

    header->setSpan(0, 3, 0, 2);

    //header->setSpan(1, 0, 0, 3);
    //header->setSpan(1, 3, 0, 3);

    //header->setSpan(2, 0, 3, 0);
    //header->setSpan(1, 0, 3, 0);

    //header->setSpan(0, 2, 0, 2);
    //header->setSpan(1, 2, 2, 0);

    //header->setSpan(0, 0, 0, 2);
    //header->setSpan(0, 2, 0, 2);

    header->setCellLabel(0, 0, "cell11");
    header->setCellLabel(0, 1, "cell21");
    header->setCellLabel(0, 3, "cell21");
    //header->setCellLabel(1, 0, "cell12");
    //header->setCellLabel(1, 2, "cell22");
    /*header->setSpan(0, 0, 2, 0);
    header->setSpan(0, 1, 2, 0);
    header->setSpan(0, 2, 2, 0);
    header->setSpan(0, 3, 1, 2);
    header->setSpan(0, 5, 2, 0);
    header->setSpan(0, 6);

    header->setCellLabel(0, 0, "cell1");
    header->setCellLabel(0, 1, "cell2");
    header->setCellLabel(0, 2, "cell3");
    header->setCellLabel(0, 3, "cell4");
    header->setCellLabel(1, 3, "cell5");


    header->setCellBackgroundColor(0, 0, Qt::cyan);
    header->setCellForegroundColor(0, 0, Qt::blue);*/
}

TreeHaderTest::~TreeHaderTest()
{
    delete ui;
}
