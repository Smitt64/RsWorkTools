#ifndef ERRORDLG_H
#define ERRORDLG_H

#include <QDialog>
#include <QStatusBar>
#include "ToolsRuntime_global.h"

namespace Ui {
class ErrorDlg;
}

class QDialogButtonBox;
class ErrorsModel;
class ErrorFilterModel;
class ErrorDlgPrivate;
class TOOLSRUNTIME_EXPORT ErrorDlg : public QDialog
{
    Q_OBJECT

public:
    enum
    {
        ModeInformation,
        ModeMessageBox,
        ModeWidget
    };
    Q_INVOKABLE ErrorDlg(const qint16 &mode, QWidget *parent = Q_NULLPTR);
    virtual ~ErrorDlg();

    Q_INVOKABLE void setErrors(ErrorsModel *e);
    Q_INVOKABLE void setMessage(const QString &msg);
    QDialogButtonBox *buttons();

private slots:
    void onActivated(const QModelIndex &index);
    void errorsCountChanged(const quint32 &value);
    void warningsCountChanged(const quint32 &value);
    void infoCountChanged(const quint32 &value);

private:
    Ui::ErrorDlg *ui;

    ErrorDlgPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ErrorDlg);
};

#endif // ERRORDLG_H
