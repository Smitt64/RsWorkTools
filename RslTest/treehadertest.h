#ifndef TREEHADERTEST_H
#define TREEHADERTEST_H

#include <QDialog>

namespace Ui {
class TreeHaderTest;
}

class TreeHaderTest : public QDialog
{
    Q_OBJECT

public:
    explicit TreeHaderTest(QWidget *parent = nullptr);
    ~TreeHaderTest();

private:
    Ui::TreeHaderTest *ui;
};

#endif // TREEHADERTEST_H
