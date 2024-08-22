#ifndef SELECTACTIONDLG_H
#define SELECTACTIONDLG_H

#include <QDialog>

namespace Ui {
class SelectActionDlg;
}

class QAction;
class AcctionTableModel;
class SelectActionDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectActionDlg(QWidget *parent = nullptr);
    ~SelectActionDlg();

    QAction *getSelectedAction();

public slots:
    void onItemClicked(const QModelIndex &index);
    void onItemDbClicked(const QModelIndex &index);

private:
    Ui::SelectActionDlg *ui;
    AcctionTableModel *m_pModel;
};

#endif // SELECTACTIONDLG_H
