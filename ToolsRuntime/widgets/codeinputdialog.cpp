#include "codeinputdialog.h"
#include "toolsruntime.h"
#include "codeeditor/codeeditor.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>

CodeInputDialog::CodeInputDialog(QWidget *parent,
                                 const QString &title,
                                 const QString &label,
                                 const QString &text,
                                 HighlighterSyntax syntax)
    : QDialog(parent),
    m_editor(nullptr)
{
    setWindowTitle(title);
    setMinimumSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    // Метка
    if (!label.isEmpty()) {
        QLabel *labelWidget = new QLabel(label, this);
        layout->addWidget(labelWidget);
    }

    // Редактор кода
    m_editor = new CodeEditor(this);
    m_editor->setPlainText(text);
    ToolApplyHighlighter(m_editor, syntax);
    layout->addWidget(m_editor);

    // Кнопки
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Фокус на редактор и выделение всего текста
    m_editor->setFocus();
    m_editor->selectAll();
}

QString CodeInputDialog::getCodeText(QWidget *parent,
                                     const QString &title,
                                     const QString &label,
                                     const QString &text,
                                     HighlighterSyntax syntax,
                                     bool *ok)
{
    CodeInputDialog dialog(parent, title, label, text, syntax);
    bool result = dialog.exec() == QDialog::Accepted;

    if (ok) {
        *ok = result;
    }

    return result ? dialog.text() : text;
}

QString CodeInputDialog::text() const
{
    return m_editor ? m_editor->toPlainText() : QString();
}
