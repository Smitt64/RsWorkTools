#ifndef ICONLIBDLG_H
#define ICONLIBDLG_H

#include <QDialog>
#include "ToolsRuntime_global.h"

namespace Ui {
class IconLibDlg;
}

class IconListModel;
class TOOLSRUNTIME_EXPORT IconLibDlg : public QDialog
{
    Q_OBJECT

public:
    IconLibDlg(QWidget *parent = nullptr);
    ~IconLibDlg();

    QString getIconPath() const;

private:
    Ui::IconLibDlg *ui;

    IconListModel *m_pModel;
};

#endif // ICONLIBDLG_H
