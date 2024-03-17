#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>
#include "ToolsRuntime_global.h"

namespace Ui {
class AboutDlg;
}

class AboutDlgPrivate;
class TOOLSRUNTIME_EXPORT AboutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDlg(const QString &config, QWidget *parent = nullptr);
    ~AboutDlg();

private slots:
    void urlChanged(const QUrl &url);

private:
    Ui::AboutDlg *ui;
    AboutDlgPrivate * const d_ptr;

    Q_DECLARE_PRIVATE(AboutDlg);
};

#endif // ABOUTDLG_H
