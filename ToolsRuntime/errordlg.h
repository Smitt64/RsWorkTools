// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
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
/**
 * @brief Диалоговое окно для отображения ошибок и предупреждений
 */
/**
 * @brief Диалоговое окно для отображения ошибок и предупреждений
 */
class TOOLSRUNTIME_EXPORT ErrorDlg : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Режимы отображения диалогового окна
     */
    enum
    {
        ModeInformation,
        ModeMessageBox,
        ModeWidget
    };
    Q_INVOKABLE ErrorDlg(const qint16 &mode, QWidget *parent = Q_NULLPTR);

    /**
     * @brief Деструктор ErrorDlg
     */
    virtual ~ErrorDlg();

    /**
     * @brief Устанавливает модель ошибок
     * @param ошибки
     */
    Q_INVOKABLE void setErrors(ErrorsModel *e);

    /**
     * @brief Устанавливает информационное сообщение
     * @param сообщение
     */
    Q_INVOKABLE void setMessage(const QString &msg);

    /**
     * @brief Возвращает кнопки диалогового окна
     * @return
     */
    QDialogButtonBox *buttons();

private slots:
    void onActivated(const QModelIndex &index);
    void errorsCountChanged(const quint32 &value);
    void warningsCountChanged(const quint32 &value);
    void infoCountChanged(const quint32 &value);

private:
    Ui::ErrorDlg *ui; //-V122

    ErrorDlgPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ErrorDlg);
};

#endif // ERRORDLG_H
