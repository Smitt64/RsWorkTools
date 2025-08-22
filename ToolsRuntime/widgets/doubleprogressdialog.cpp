#include "doubleprogressdialog.h"
#include "doubleprogressdialogprivate.h"
#include <QApplication>

DoubleProgressDialog::DoubleProgressDialog(QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
    , d_ptr(new DoubleProgressDialogPrivate(this))
{
    Q_D(DoubleProgressDialog);
    d->setupUi();
}

DoubleProgressDialog::~DoubleProgressDialog() = default;

// Основной прогресс бар
void DoubleProgressDialog::setMainValue(int value)
{
    Q_D(DoubleProgressDialog);
    d->mainCurrentValue = qBound(d->mainMinValue, value, d->mainMaxValue);
    d->mainProgressBar->setValue(d->mainCurrentValue);
    d->updateMainLabel();
    emit mainValueChanged(d->mainCurrentValue);
}

int DoubleProgressDialog::mainValue() const
{
    Q_D(const DoubleProgressDialog);
    return d->mainCurrentValue;
}

void DoubleProgressDialog::setMainRange(int minimum, int maximum)
{
    Q_D(DoubleProgressDialog);
    d->mainMinValue = minimum;
    d->mainMaxValue = maximum;
    d->mainProgressBar->setRange(minimum, maximum);
    d->updateMainLabel();
}

void DoubleProgressDialog::setMainMinimum(int minimum)
{
    Q_D(DoubleProgressDialog);
    d->mainMinValue = minimum;
    d->mainProgressBar->setMinimum(minimum);
    d->updateMainLabel();
}

void DoubleProgressDialog::setMainMaximum(int maximum)
{
    Q_D(DoubleProgressDialog);
    d->mainMaxValue = maximum;
    d->mainProgressBar->setMaximum(maximum);
    d->updateMainLabel();
}

int DoubleProgressDialog::mainMinimum() const
{
    Q_D(const DoubleProgressDialog);
    return d->mainMinValue;
}

int DoubleProgressDialog::mainMaximum() const
{
    Q_D(const DoubleProgressDialog);
    return d->mainMaxValue;
}

// Дополнительный прогресс бар
void DoubleProgressDialog::setSecondaryValue(int value)
{
    Q_D(DoubleProgressDialog);
    d->secondaryCurrentValue = qBound(d->secondaryMinValue, value, d->secondaryMaxValue);
    d->secondaryProgressBar->setValue(d->secondaryCurrentValue);
    d->updateSecondaryLabel();
    emit secondaryValueChanged(d->secondaryCurrentValue);
}

int DoubleProgressDialog::secondaryValue() const
{
    Q_D(const DoubleProgressDialog);
    return d->secondaryCurrentValue;
}

void DoubleProgressDialog::setSecondaryRange(int minimum, int maximum)
{
    Q_D(DoubleProgressDialog);
    d->secondaryMinValue = minimum;
    d->secondaryMaxValue = maximum;
    d->secondaryProgressBar->setRange(minimum, maximum);
    d->updateSecondaryLabel();
}

void DoubleProgressDialog::setSecondaryMinimum(int minimum)
{
    Q_D(DoubleProgressDialog);
    d->secondaryMinValue = minimum;
    d->secondaryProgressBar->setMinimum(minimum);
    d->updateSecondaryLabel();
}

void DoubleProgressDialog::setSecondaryMaximum(int maximum)
{
    Q_D(DoubleProgressDialog);
    d->secondaryMaxValue = maximum;
    d->secondaryProgressBar->setMaximum(maximum);
    d->updateSecondaryLabel();
}

int DoubleProgressDialog::secondaryMinimum() const
{
    Q_D(const DoubleProgressDialog);
    return d->secondaryMinValue;
}

int DoubleProgressDialog::secondaryMaximum() const
{
    Q_D(const DoubleProgressDialog);
    return d->secondaryMaxValue;
}

// Тексты
void DoubleProgressDialog::setMainLabelText(const QString &text)
{
    Q_D(DoubleProgressDialog);
    d->mainLabelText = text;
    d->updateMainLabel();
}

QString DoubleProgressDialog::mainLabelText() const
{
    Q_D(const DoubleProgressDialog);
    return d->mainLabelText;
}

void DoubleProgressDialog::setSecondaryLabelText(const QString &text)
{
    Q_D(DoubleProgressDialog);
    d->secondaryLabelText = text;
    d->updateSecondaryLabel();
}

QString DoubleProgressDialog::secondaryLabelText() const
{
    Q_D(const DoubleProgressDialog);
    return d->secondaryLabelText;
}

// Кнопка отмены
void DoubleProgressDialog::setCancelButtonText(const QString &text)
{
    Q_D(DoubleProgressDialog);
    if (d->cancelButton) {
        d->cancelButton->setText(text);
    }
}

QString DoubleProgressDialog::cancelButtonText() const
{
    Q_D(const DoubleProgressDialog);
    return d->cancelButton ? d->cancelButton->text() : QString();
}

void DoubleProgressDialog::setCancelButtonVisible(bool visible)
{
    Q_D(DoubleProgressDialog);
    if (d->cancelButton) {
        d->cancelButton->setVisible(visible);
    }
}

bool DoubleProgressDialog::isCancelButtonVisible() const
{
    Q_D(const DoubleProgressDialog);
    return d->cancelButton ? d->cancelButton->isVisible() : false;
}

bool DoubleProgressDialog::wasCanceled() const
{
    Q_D(const DoubleProgressDialog);
    return d->wasCanceled;
}

// Слоты
void DoubleProgressDialog::cancel()
{
    Q_D(DoubleProgressDialog);
    d->wasCanceled = true;
    emit canceled();
    reject();
}

void DoubleProgressDialog::reset()
{
    Q_D(DoubleProgressDialog);
    d->wasCanceled = false;
    setMainValue(d->mainMinValue);
    setSecondaryValue(d->secondaryMinValue);
}

void DoubleProgressDialog::setValue(int mainValue, int secondaryValue)
{
    setMainValue(mainValue);
    setSecondaryValue(secondaryValue);
}
