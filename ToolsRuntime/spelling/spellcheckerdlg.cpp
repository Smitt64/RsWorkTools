#include "spellcheckerdlg.h"
#include "spelling/spellchecker.h"
#include "ui_spellcheckerdlg.h"
#include <QPushButton>

class SpellCheckerDlgPrivate
{
    Q_DECLARE_PUBLIC(SpellCheckerDlg)
public:
    SpellCheckerDlgPrivate(SpellCheckerDlg *obj):
        q_ptr(obj)
    {

    }

    SpellChecker *m_pChecker;
    SpellCheckerDlg *q_ptr;
};

SpellCheckerDlg::SpellCheckerDlg(QObject *checker, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SpellCheckerDlg)
    , d_ptr(new SpellCheckerDlgPrivate(this))
{
    Q_D(SpellCheckerDlg);
    ui->setupUi(this);
    d->m_pChecker = qobject_cast<SpellChecker*>(checker);

    connect(ui->listWidget, &QListWidget::currentTextChanged,
            ui->ledtReplaceWith, &QLineEdit::setText);

    connect(ui->btnAddToDict, &QPushButton::clicked, [=]()
    {
        if (!d->m_pChecker)
            setResult(AbortCheck);

        d->m_pChecker->addToUserWordlist(ui->lblUnknownWord->text());
        setResult(AddToDict);
        accept();
    });

    connect(ui->btnReplaceOnce, &QPushButton::clicked, [=]()
    {
        if (!d->m_pChecker)
            setResult(AbortCheck);

        setResult(ReplaceOnce);
        accept();
    });

    connect(ui->btnReplaceAll, &QPushButton::clicked, [=]()
    {
        if (!d->m_pChecker)
            setResult(AbortCheck);

        setResult(ReplaceAll);
        accept();
    });

    connect(ui->btnIgnoreOnce, &QPushButton::clicked, [=]()
    {
        if (!d->m_pChecker)
            setResult(AbortCheck);

        setResult(IgnoreOnce);
        accept();
    });

    connect(ui->btnIgnoreAll, &QPushButton::clicked, [=]()
    {
        if (!d->m_pChecker)
            setResult(AbortCheck);

        setResult(IgnoreAll);
        accept();
    });
}

SpellCheckerDlg::~SpellCheckerDlg()
{
    delete d_ptr;
    delete ui;
}

int SpellCheckerDlg::checkWord(const QString &word)
{
    Q_D(SpellCheckerDlg);

    if (!d->m_pChecker)
        return AbortCheck;

    QStringList suggestions = d->m_pChecker->suggest(word);

    ui->lblUnknownWord->setText(word);

    ui->listWidget->clear();
    ui->listWidget->addItems(suggestions);

    if (suggestions.count() > 0)
        ui->listWidget->setCurrentRow(0, QItemSelectionModel::Select);

    QDialog::exec();
    return QDialog::result();
}