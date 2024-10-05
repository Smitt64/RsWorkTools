#include "spellstringsdlg.h"
#include "spelling/spellchecker.h"
#include "spelling/spellcheckerdlg.h"
#include "ui_spellstringsdlg.h"
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QDebug>

#define USERDATA_PTR (Qt::UserRole + 1)

//typedef void (*_scanTextProc)(QTextCursor &cursor);
typedef std::function<int(QTextCursor &cursor,const QString &word)> _scanTextProc;
typedef QList<QTextEdit::ExtraSelection> ExtraSelectionList;

class TextDelegate : public QStyledItemDelegate
{
public:
    TextDelegate(QObject *parent = nullptr) :
        QStyledItemDelegate(parent)
    {

    }

    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        //ExtraSelectionList lst =
        //    *reinterpret_cast<ExtraSelectionList*>(index.data(Qt::UserRole + 1).toInt());

        QTextEdit doc;
        doc.setPlainText(index.data().toString());
        //doc.setExtraSelections(lst);

        doc.render(painter);
    }
};

class SpellStringsDlgPrivate
{
    Q_DECLARE_PUBLIC(SpellStringsDlg)
public:
    SpellStringsDlgPrivate(SpellStringsDlg *obj) :
        q_ptr(obj)
    {
        m_pModel = new QStandardItemModel();
        m_HighlightFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
        m_HighlightFormat.setUnderlineColor(Qt::darkBlue);
        m_HighlightFormat.setBackground(QBrush(QColor(0xff, 0x60, 0x60)));

        //m_pDelegate = new TextDelegate();
    }

    virtual ~SpellStringsDlgPrivate()
    {
        delete m_pModel;
    }

    int scanText(QTextDocument *document, ExtraSelectionList &selections, _scanTextProc proc = nullptr)
    {
        int result = 0;
        Q_Q(SpellStringsDlg);
        QTextCursor cursor(document);
        while (!cursor.atEnd())
        {
            QCoreApplication::processEvents();

            cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor, 1);
            QString word = cursor.selectedText();

            while (!word.isEmpty() &&
                   !word.at(0).isLetter() &&
                   cursor.anchor() < cursor.position())
            {
                int cursorPos = cursor.position();
                cursor.setPosition(cursor.anchor() + 1, QTextCursor::MoveAnchor);
                cursor.setPosition(cursorPos, QTextCursor::KeepAnchor);
                word = cursor.selectedText();
            }

            if (!word.isEmpty() && !q->skipWord(word) && !m_pChecker->spell(word))
            {
                QTextEdit::ExtraSelection es;
                es.cursor = cursor;
                es.format = m_HighlightFormat;
                selections.append(es);

                if (proc)
                {
                    result = proc(cursor, word);

                    if (result < 0)
                        break;
                }
            }

            cursor.movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor, 1);
        }

        return result;
    }

    void replaceAll(int nPos, const QString &sOld, const QString &sNew)
    {
        Q_Q(SpellStringsDlg);
        QTextCursor cursor(q->ui->textEdit->document());
        cursor.setPosition(nPos - sOld.length(), QTextCursor::MoveAnchor);

        while (!cursor.atEnd())
        {
            QTextCursor cursor(q->ui->textEdit->document());
            cursor.setPosition(nPos-sOld.length(), QTextCursor::MoveAnchor);

            QCoreApplication::processEvents();
            cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor, 1);
            QString word = cursor.selectedText();

            while (!word.isEmpty() &&
                   !word.at(0).isLetter() &&
                   cursor.anchor() < cursor.position())
            {
                int cursorPos = cursor.position();
                cursor.setPosition(cursor.anchor() + 1, QTextCursor::MoveAnchor);
                cursor.setPosition(cursorPos, QTextCursor::KeepAnchor);
                word = cursor.selectedText();
            }

            qDebug() << "replaceAll" << word;
            if (word == sOld)
            {
                cursor.insertText(sNew);
                QCoreApplication::processEvents();
            }

            cursor.movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor, 1);
        }
    }

    int checkSpellMode(QTextCursor &cursor, const QString &word)
    {
        Q_Q(SpellStringsDlg);
        QTextCursor tmpCursor(cursor);
        tmpCursor.setPosition(cursor.anchor());

        ExtraSelectionList selections;
        q->ui->textEdit->setTextCursor(tmpCursor);
        q->ui->textEdit->ensureCursorVisible();

        QTextEdit::ExtraSelection es;
        es.cursor = cursor;
        es.format = m_HighlightFormat;

        selections << es;
        q->ui->textEdit->setExtraSelections(selections);
        QCoreApplication::processEvents();

        SpellCheckerDlg dlg(m_pChecker, q->ui->textEdit);
        SpellCheckerDlg::SpellCheckAction result = (SpellCheckerDlg::SpellCheckAction)dlg.checkWord(word);
        selections.clear();
        q->ui->textEdit->setExtraSelections(selections);
        QCoreApplication::processEvents();

        if (result == SpellCheckerDlg::AbortCheck)
            return result;

        switch (result)
        {
        case SpellCheckerDlg::ReplaceOnce:
            cursor.insertText(dlg.replacement());
            break;
        case SpellCheckerDlg::ReplaceAll:
            replaceAll(cursor.position(), word, dlg.replacement());
            break;
        case SpellCheckerDlg::AbortCheck:
        case SpellCheckerDlg::AddToDict:
        case SpellCheckerDlg::IgnoreOnce:
        case SpellCheckerDlg::IgnoreAll:
            break;
        }

        return 0;
    }

    QTextCharFormat m_HighlightFormat;
    QStandardItemModel *m_pModel;
    SpellChecker *m_pChecker;
    SpellStringsDlg *q_ptr;
};

SpellStringsDlg::SpellStringsDlg(SpellChecker *spell, QWidget *parent)
    : QDialog(parent)
    , d_ptr(new SpellStringsDlgPrivate(this))
    ,ui(new Ui::SpellStringsDlg)
{
    Q_D(SpellStringsDlg);
    ui->setupUi(this);

    ui->listView->setModel(d->m_pModel);

    d->m_pChecker = spell;

    connect(ui->listView->selectionModel(), &QItemSelectionModel::currentChanged,
            [=](const QModelIndex &current, const QModelIndex &previous)
    {
        ui->textEdit->setPlainText(current.data().toString());

        ExtraSelectionList selections;
        d->scanText(ui->textEdit->document(), selections);
        ui->textEdit->setExtraSelections(selections);
    });

    connect(ui->checkSpell, &QPushButton::clicked, [=]()
    {
        ExtraSelectionList selections;
        QModelIndex index = ui->listView->selectionModel()->currentIndex();

        int result = 0;
        while (index.isValid() && result >= 0)
        {
            ui->listView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
            result = d->scanText(ui->textEdit->document(), selections,
                                     [=](QTextCursor &cursor, const QString &word) -> int
            {
                return d->checkSpellMode(cursor, word);
            });

            d->m_pModel->setData(index, ui->textEdit->toPlainText());

            if (result >= 0)
            {
                if (index.row() + 1 < d->m_pModel->rowCount())
                {
                    ui->listView->selectionModel()->clearSelection();
                    index = d->m_pModel->index(index.row() + 1, 0);
                    ui->listView->selectionModel()->select(index, QItemSelectionModel::Select);
                    QCoreApplication::processEvents();
                }
                else
                    index = QModelIndex();
            }
        }
    });
}

SpellStringsDlg::~SpellStringsDlg()
{
    delete d_ptr;
    delete ui;
}

void SpellStringsDlg::appendString(const QString &str, void *userData)
{
    Q_D(SpellStringsDlg);

    QTextDocument doc;
    doc.setPlainText(str);

    ExtraSelectionList selections;
    d->scanText(&doc, selections, nullptr);

    if (!selections.isEmpty())
    {
        QStandardItem *item = new QStandardItem(str);
        item->setData((int)userData, USERDATA_PTR);
        d->m_pModel->appendRow(item);
    }
}

bool SpellStringsDlg::skipWord(const QString &word)
{
    return false;
}

void SpellStringsDlg::setHighlightFormat(const QTextCharFormat &format)
{
    Q_D(SpellStringsDlg);
    d->m_HighlightFormat = format;
}

int SpellStringsDlg::count() const
{
    Q_D(const SpellStringsDlg);
    return d->m_pModel->rowCount();
}

QString SpellStringsDlg::value(const int &index) const
{
    Q_D(const SpellStringsDlg);
    return d->m_pModel->data(d->m_pModel->index(index, 0)).toString();
}

void *SpellStringsDlg::userData(const int &index) const
{
    Q_D(const SpellStringsDlg);
    int val = d->m_pModel->data(d->m_pModel->index(index, 0), USERDATA_PTR).toInt();

    return reinterpret_cast<void*>(val);
}
