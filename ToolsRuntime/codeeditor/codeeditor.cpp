// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "codeeditor.h"
#include "codeeditor/codehighlighter.h"
#include "codeeditor/codesearchwidget.h"
#include "codeeditor/highlighterstyle.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>
#include <QStyle>
#include <QStyleOption>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }
    ~LineNumberArea()
    {
        codeEditor = NULL;
    }

    QSize sizeHint() const Q_DECL_OVERRIDE {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

class CodeEditorPrivate
{
    Q_DECLARE_PUBLIC(CodeEditor)
public:
    CodeEditorPrivate(CodeEditor *obj)
    {
        Q_Q(CodeEditor);
        q_ptr = obj;
        m_CurrentWordColor = QColor("#9BFF9B");

        m_CurrentLineColor = QColor(Qt::yellow).lighter(160);
        lineNumberArea = new LineNumberArea(obj);

        m_pCodeHighlighter = nullptr;
    }

    void lineNumberAreaPaintEvent(QPaintEvent *event)
    {
        Q_Q(CodeEditor);
        QPainter painter(lineNumberArea);

        QStyleOption opt;
        opt.init(lineNumberArea);
        opt.rect = event->rect();
        QStyle *s = qApp->style();
        QPalette palette = s->standardPalette();
        //m_pCodeHighlighter
        QColor linenumbersBackground = palette.color(QPalette::Normal, QPalette::Window);
        QColor linenumbersForeground = palette.color(QPalette::Normal, QPalette::Text);

        if (m_pCodeHighlighter && m_pCodeHighlighter->style())
        {
            if (m_pCodeHighlighter->style()->linenumbersBackground().isValid())
                linenumbersBackground = m_pCodeHighlighter->style()->linenumbersBackground();

            if (m_pCodeHighlighter->style()->linenumbersForeground().isValid())
                linenumbersForeground = m_pCodeHighlighter->style()->linenumbersForeground();
        }

        painter.fillRect(event->rect(), linenumbersBackground);

        QTextBlock block = q->firstVisibleBlock();
        int blockNumber = block.blockNumber();
        int top = (int) q->blockBoundingGeometry(block).translated(q->contentOffset()).top();
        int bottom = top + (int) q->blockBoundingRect(block).height();

        while (block.isValid() && top <= event->rect().bottom())
        {
            if (block.isVisible() && bottom >= event->rect().top())
            {
                QFont f = painter.font();
                f.setBold(false);
                QString number = QString::number(blockNumber + 1);
                painter.setPen(linenumbersForeground);

                if (block == q->textCursor().block())
                {
                    painter.setPen(linenumbersForeground);
                    f.setBold(true);
                }

                painter.setFont(f);
                painter.drawText(0, top, lineNumberArea->width() - 4, q->fontMetrics().height(),
                                 Qt::AlignRight, number);
            }

            block = block.next();
            top = bottom;
            bottom = top + (int) q->blockBoundingRect(block).height();
            ++blockNumber;
        }
    }

    void ApplyExtraSelections()
    {
        Q_Q(CodeEditor);
        QList<QTextEdit::ExtraSelection> selections;
        selections.append(selectionCurrentLine);
        selections.append(Selections);
        q->setExtraSelections(selections);
    }

    void onSelectionChanged()
    {
        Q_Q(CodeEditor);

        QTextCursor current = q->textCursor();

        if (lastLine != current.block().blockNumber())
        {
            emit q->currentLineChanged(lastLine, current.block().blockNumber());
            lastLine = current.block().blockNumber();
        }

        QString searchString = current.selectedText();
        searchString = searchString.simplified();
        Selections.clear();
        if (!searchString.isEmpty())
        {
            QBrush backBrush(m_CurrentWordColor);//"#9BFF9B"

            QTextCursor highlightCursor(q->document());
            while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
                highlightCursor = q->document()->find(searchString, highlightCursor, QTextDocument::FindWholeWords);

                if (!highlightCursor.isNull())
                {
                    QTextEdit::ExtraSelection selection;
                    selection.cursor = highlightCursor;
                    selection.format.setBackground(backBrush);
                    Selections.append(selection);
                }
            }
        }

        ApplyExtraSelections();
    }

    void updateLineNumberArea(const QRect &rect, int dy)
    {
        Q_Q(CodeEditor);

        if (dy)
            lineNumberArea->scroll(0, dy);
        else
            lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

        if (rect.contains(q->viewport()->rect()))
            q->setViewportMargins(q->lineNumberAreaWidth(), 0, 0, 0);
    }

    CodeSearchWidget *m_searchWidget;
    QWidget *lineNumberArea;
    QTextEdit::ExtraSelection selectionCurrentLine;
    QList<QTextEdit::ExtraSelection> Selections, applyExtraSelections;

    int tabStop, lastLine;

    QColor m_CurrentLineColor, m_CurrentWordColor;

    CodeHighlighter *m_pCodeHighlighter;
    CodeEditor *q_ptr;
};

CodeEditor::CodeEditor(QWidget *parent) :
    QPlainTextEdit(parent),
    d_ptr(new CodeEditorPrivate(this))
{
    Q_D(CodeEditor);
    d->m_searchWidget = new CodeSearchWidget(this, viewport());

    QFont font("Courier New", 10);
    document()->setDefaultFont(font);

    connect(this, &CodeEditor::blockCountChanged, [=]()
    {
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    });

    connect(this, &CodeEditor::updateRequest, [=]()
    {
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    });

    connect(this, &CodeEditor::cursorPositionChanged, [=]()
    {
        d->selectionCurrentLine.format.setBackground(d->m_CurrentLineColor);
        d->selectionCurrentLine.format.setProperty(QTextFormat::FullWidthSelection, true);
        d->selectionCurrentLine.cursor = textCursor();
        d->selectionCurrentLine.cursor.clearSelection();

        d->ApplyExtraSelections();
    });

    connect(this, &CodeEditor::selectionChanged, [=]()
    {
        d->onSelectionChanged();
    });

    connect(this, &CodeEditor::updateRequest, [=](const QRect &rect, int dy)
    {
        d->updateLineNumberArea(rect, dy);
    });

    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

CodeEditor::~CodeEditor()
{
    Q_D(CodeEditor);

    if (d->m_pCodeHighlighter)
        delete d->m_pCodeHighlighter;

    if (d->m_searchWidget)
        delete d->m_searchWidget;

    delete d_ptr;
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    Q_D(CodeEditor);
    d->lineNumberAreaPaintEvent(event);
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(4, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 1 + fontMetrics().width(QLatin1Char('9')) * /*digits*/5;
    return space;
}

void CodeEditor::setTabStop(int w)
{
    Q_D(CodeEditor);
    d->tabStop = w;
}

void CodeEditor::setCurrentLineColor(const QColor &color)
{
    Q_D(CodeEditor);
    d->m_CurrentLineColor = color;
}

void CodeEditor::setCurrentWordColor(const QColor &color)
{
    Q_D(CodeEditor);
    d->m_CurrentWordColor = color;
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    Q_D(CodeEditor);
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    d->lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));

    // Обновляем позицию виджета поиска при изменении размера
    if (d->m_searchWidget && d->m_searchWidget->isVisible()) {
        d->m_searchWidget->updatePosition();
    }
}

void CodeEditor::setCodeHighlighter(CodeHighlighter *highlighter)
{
    Q_D(CodeEditor);
    d->m_pCodeHighlighter = highlighter;
}

void CodeEditor::rehighlight()
{
    Q_D(CodeEditor);
    if (!d->m_pCodeHighlighter)
        return;

    QSharedPointer<StyleItem> sstyle = d->m_pCodeHighlighter->style();
    QTextCharFormat def =  sstyle->format(FormatDefault);
    QColor background = sstyle->editorBackground();

    QFont defaultFont = def.font();
    defaultFont.setStyleHint(QFont::Courier);
    document()->setDefaultFont(defaultFont);

    setStyleSheet(QString("QPlainTextEdit { background-color: rgb(%1, %2, %3); color: rgb(%4, %5, %6) }")
                               .arg(background.red())
                               .arg(background.green())
                               .arg(background.blue())
                               .arg(def.foreground().color().red())
                               .arg(def.foreground().color().green())
                               .arg(def.foreground().color().blue()));

    setCurrentLineColor(sstyle->editorCurrentLine());
    setCurrentWordColor(sstyle->editorCurrentWord());

    setAutoFillBackground(true);

    d->m_pCodeHighlighter->rehighlight();
    d->lineNumberArea->update();

    update();
}

CodeHighlighter *CodeEditor::highlighter()
{
    Q_D(CodeEditor);
    return d->m_pCodeHighlighter;
}

void CodeEditor::setStyle(const QString &style)
{
    Q_D(CodeEditor);

    if (d->m_pCodeHighlighter)
        d->m_pCodeHighlighter->setStyle(style);
}

void CodeEditor::showSearchWidget()
{
    Q_D(CodeEditor);

    if (!d->m_searchWidget)
        d->m_searchWidget = new CodeSearchWidget(this, viewport());

    // Position widget in top-right corner
    QRect viewportRect = viewport()->rect();
    QSize widgetSize = d->m_searchWidget->sizeHint();
    int x = viewportRect.width() - widgetSize.width() - 10;
    int y = 10;

    d->m_searchWidget->setGeometry(x, y, widgetSize.width(), widgetSize.height());
    d->m_searchWidget->show();
}

void CodeEditor::hideSearchWidget()
{
    Q_D(CodeEditor);

    if (d->m_searchWidget)
        d->m_searchWidget->hide();
}

bool CodeEditor::isSearchWidgetVisible() const
{
    Q_D(const CodeEditor);

    return d->m_searchWidget && d->m_searchWidget->isVisible();
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    Q_D(CodeEditor);  // Уберите const, так как мы будем изменять состояние

    if (event->key() == Qt::Key_F && event->modifiers() == Qt::ControlModifier) {
        showSearchWidget();
    }
    else if (event->key() == Qt::Key_F3) {
        if (d->m_searchWidget && d->m_searchWidget->isVisible()) {
            if (event->modifiers() & Qt::ShiftModifier)
                d->m_searchWidget->findPrevious();
            else
                d->m_searchWidget->findNext();
        }
    }
    else if (event->key() == Qt::Key_Escape && d->m_searchWidget && d->m_searchWidget->isVisible()) {
        hideSearchWidget();
    }
    else {
        QPlainTextEdit::keyPressEvent(event);
    }
}