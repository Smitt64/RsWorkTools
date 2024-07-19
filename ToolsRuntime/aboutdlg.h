// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
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
