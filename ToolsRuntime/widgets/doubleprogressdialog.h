#ifndef DOUBLEPROGRESSDIALOG_H
#define DOUBLEPROGRESSDIALOG_H

#include "ToolsRuntime_global.h"
#include <QProgressDialog>

class DoubleProgressDialogPrivate;
class TOOLSRUNTIME_EXPORT DoubleProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DoubleProgressDialog(QWidget *parent = nullptr,
                                  Qt::WindowFlags flags = Qt::WindowFlags());
    ~DoubleProgressDialog();

    // Основной прогресс бар
    void setMainValue(int value);
    int mainValue() const;
    void setMainRange(int minimum, int maximum);
    void setMainMinimum(int minimum);
    void setMainMaximum(int maximum);
    int mainMinimum() const;
    int mainMaximum() const;

    // Дополнительный прогресс бар
    void setSecondaryValue(int value);
    int secondaryValue() const;
    void setSecondaryRange(int minimum, int maximum);
    void setSecondaryMinimum(int minimum);
    void setSecondaryMaximum(int maximum);
    int secondaryMinimum() const;
    int secondaryMaximum() const;

    // Тексты
    void setMainLabelText(const QString &text);
    QString mainLabelText() const;
    void setSecondaryLabelText(const QString &text);
    QString secondaryLabelText() const;

    // Кнопка отмены
    void setCancelButtonText(const QString &text);
    QString cancelButtonText() const;
    void setCancelButtonVisible(bool visible);
    bool isCancelButtonVisible() const;

    // Было ли отменено
    bool wasCanceled() const;

public slots:
    void cancel();
    void reset();
    void setValue(int mainValue, int secondaryValue);

signals:
    void canceled();
    void mainValueChanged(int value);
    void secondaryValueChanged(int value);

private:
    std::unique_ptr<DoubleProgressDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE(DoubleProgressDialog)
    Q_DISABLE_COPY(DoubleProgressDialog)
};

#endif // DOUBLEPROGRESSDIALOG_H
