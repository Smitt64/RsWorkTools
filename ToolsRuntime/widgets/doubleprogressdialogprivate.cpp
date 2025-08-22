#include "doubleprogressdialogprivate.h"
#include <QApplication>
#include <QStyle>
#include <QFontMetrics>
#include <QDebug>

DoubleProgressDialogPrivate::DoubleProgressDialogPrivate(DoubleProgressDialog *q)
    : q_ptr(q)
    , mainProgressBar(nullptr)
    , mainMinValue(0)
    , mainMaxValue(100)
    , mainCurrentValue(0)
    , mainLabelText("Main progress:")
    , secondaryProgressBar(nullptr)
    , secondaryMinValue(0)
    , secondaryMaxValue(100)
    , secondaryCurrentValue(0)
    , secondaryLabelText("Secondary progress:")
    , mainLabel(nullptr)
    , secondaryLabel(nullptr)
    , cancelButton(nullptr)
    , mainLayout(nullptr)
    , formLayout(nullptr)
    , buttonLayout(nullptr)
    , wasCanceled(false)
{
}

DoubleProgressDialogPrivate::~DoubleProgressDialogPrivate()
{
    // Все виджеты будут автоматически удалены родителем (q_ptr)
}

void DoubleProgressDialogPrivate::setupUi()
{
    Q_Q(DoubleProgressDialog);

    q->setWindowTitle(QApplication::translate("DoubleProgressDialog", "Progress"));
    q->setMinimumWidth(350);
    //q->setMinimumSize(450, 150);

    // Создаем основной layout
    mainLayout = new QVBoxLayout(q);
    //mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // Создаем form layout для выравнивания меток
    formLayout = new QFormLayout();
    formLayout->setSpacing(10);
    formLayout->setContentsMargins(0, 0, 0, 0);
    //formLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);

    // Основной прогресс бар
    mainLabel = new QLabel(q);
    mainLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mainLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    mainProgressBar = new QProgressBar(q);
    mainProgressBar->setRange(mainMinValue, mainMaxValue);
    mainProgressBar->setValue(mainCurrentValue);

    formLayout->addRow(mainLabel, mainProgressBar);

    // Дополнительный прогресс бар
    secondaryLabel = new QLabel(q);
    secondaryLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    secondaryLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    secondaryProgressBar = new QProgressBar(q);
    secondaryProgressBar->setRange(secondaryMinValue, secondaryMaxValue);
    secondaryProgressBar->setValue(secondaryCurrentValue);

    formLayout->addRow(secondaryLabel, secondaryProgressBar);

    mainLayout->addLayout(formLayout);

    // Кнопка отмены
    cancelButton = new QPushButton(QApplication::translate("DoubleProgressDialog", "Cancel"), q);
    QObject::connect(cancelButton, &QPushButton::clicked,
                     [this]() { onCancelClicked(); });

    buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // Обновляем тексты и выравниваем ширину
    updateMainLabel();
    updateSecondaryLabel();
    updateLabelWidths();

    qDebug() << formLayout->rowCount();
}

void DoubleProgressDialogPrivate::updateMainLabel()
{
    if (mainLabel) {
        QString text = mainLabelText.isEmpty() ?
                           QApplication::translate("DoubleProgressDialog", "Main progress:") : mainLabelText;
        text += QString(" %1/%2").arg(mainCurrentValue).arg(mainMaxValue);
        mainLabel->setText(text);
        updateLabelWidths();
    }
}

void DoubleProgressDialogPrivate::updateSecondaryLabel()
{
    if (secondaryLabel) {
        QString text = secondaryLabelText.isEmpty() ?
                           QApplication::translate("DoubleProgressDialog", "Secondary progress:") : secondaryLabelText;
        text += QString(" %1/%2").arg(secondaryCurrentValue).arg(secondaryMaxValue);
        secondaryLabel->setText(text);
        updateLabelWidths();
    }
}

void DoubleProgressDialogPrivate::updateLabelWidths()
{
    /*if (!mainLabel || !secondaryLabel) return;

    // Вычисляем максимальную ширину среди всех меток
    QFontMetrics metrics(mainLabel->font());
    int mainWidth = metrics.horizontalAdvance(mainLabel->text());
    int secondaryWidth = metrics.horizontalAdvance(secondaryLabel->text());
    int maxWidth = qMax(mainWidth, secondaryWidth);

    // Добавляем отступ для красоты
    maxWidth += metrics.horizontalAdvance("  ");

    // Устанавливаем фиксированную ширину для обеих меток
    mainLabel->setFixedWidth(maxWidth);
    secondaryLabel->setFixedWidth(maxWidth);*/
}

void DoubleProgressDialogPrivate::onCancelClicked()
{
    Q_Q(DoubleProgressDialog);
    wasCanceled = true;
    emit q->canceled();
    q->reject();
}
