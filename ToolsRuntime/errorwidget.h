#ifndef ERRORWIDGET_H
#define ERRORWIDGET_H

#include <QWidget>
#include "ToolsRuntime_global.h"

namespace Ui {
class ErrorWidget;
}

class ErrorsModel;
class ErrorWidgetPrivate;
class TOOLSRUNTIME_EXPORT ErrorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ErrorWidget(QWidget *parent = nullptr);
    ~ErrorWidget();

    /**
     * @brief Устанавливает информационное сообщение
     * @param сообщение
     */
    Q_INVOKABLE void setMessage(const QString &msg);

    /**
     * @brief Устанавливает модель ошибок
     * @param ошибки
     */
    Q_INVOKABLE void setErrors(ErrorsModel *e);

private slots:
    void onActivated(const QModelIndex &index);
    void errorsCountChanged(const quint32 &value);
    void warningsCountChanged(const quint32 &value);
    void infoCountChanged(const quint32 &value);

private:
    Ui::ErrorWidget *ui;

    ErrorWidgetPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ErrorWidget);
};

#endif // ERRORWIDGET_H
