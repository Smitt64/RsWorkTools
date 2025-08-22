#ifndef DOUBLEPROGRESSDIALOGPRIVATE_H
#define DOUBLEPROGRESSDIALOGPRIVATE_H

#include "doubleprogressdialog.h"
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

class DoubleProgressDialogPrivate
{
    Q_DECLARE_PUBLIC(DoubleProgressDialog)

public:
    explicit DoubleProgressDialogPrivate(DoubleProgressDialog *q);
    ~DoubleProgressDialogPrivate();

    void setupUi();
    void updateMainLabel();
    void updateSecondaryLabel();
    void updateLabelWidths();
    void onCancelClicked();

    DoubleProgressDialog *q_ptr;

    // Основной прогресс бар
    QProgressBar *mainProgressBar;
    int mainMinValue;
    int mainMaxValue;
    int mainCurrentValue;
    QString mainLabelText;

    // Дополнительный прогресс бар
    QProgressBar *secondaryProgressBar;
    int secondaryMinValue;
    int secondaryMaxValue;
    int secondaryCurrentValue;
    QString secondaryLabelText;

    // Элементы UI
    QLabel *mainLabel;
    QLabel *secondaryLabel;
    QPushButton *cancelButton;
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QHBoxLayout *buttonLayout;

    bool wasCanceled;
};

#endif // DOUBLEPROGRESSDIALOGPRIVATE_H
