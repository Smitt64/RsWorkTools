#include "codeeditoroptionspage.h"
#include "ui_codeeditoroptionspage.h"
#include "codeeditor/codeeditor.h"
#include "codeeditor/highlighterstyle.h"
#include "codeeditor/codehighlighter.h"
#include "toolsruntime.h"

class CodeEditorOptionsPagePrivate
{
    Q_DECLARE_PUBLIC(CodeEditorOptionsPage)
public:
    CodeEditorOptionsPagePrivate(CodeEditorOptionsPage *parent)
    {
        q_ptr = parent;

        pEditor = new CodeEditor(parent);
    }

    CodeEditor *pEditor;
    CodeEditorOptionsPage *q_ptr;
};

CodeEditorOptionsPage::CodeEditorOptionsPage(QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::CodeEditorOptionsPage)
    , d_ptr(new CodeEditorOptionsPagePrivate(this))
{
    Q_D(CodeEditorOptionsPage);
    ui->setupUi(this);

    ui->comboBox->addItems(HighlighterStyle::inst()->themes());
    QVBoxLayout *pLayout = new QVBoxLayout();
    ui->groupBox->setLayout(pLayout);

    pLayout->addWidget(d->pEditor);
    d->pEditor->setReadOnly(true);
    d->pEditor->setPlainText(toolReadTextFileContent(":/codeeditoroptions/example.cpp"));

    ToolApplyHighlighter(d->pEditor, HighlighterCpp, ui->comboBox->currentText());

    connect(ui->comboBox, &QComboBox::currentTextChanged, [=](const QString &style)
    {
        d->pEditor->setStyle(style);
        d->pEditor->rehighlight();
    });
}

CodeEditorOptionsPage::~CodeEditorOptionsPage()
{
    delete ui;
}
