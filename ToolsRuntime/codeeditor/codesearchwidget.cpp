#include "codesearchwidget.h"
#include "codeeditor.h"
#include <QKeyEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QTextCursor>
#include <QTextDocument>
#include <QRegularExpression>
#include <QToolTip>
#include <QApplication>
#include <QDebug>
#include <QResizeEvent>

CodeSearchWidget::CodeSearchWidget(CodeEditor *editor, QWidget *parent)
    : QFrame(parent)
    , m_editor(editor)
    , m_totalMatches(0)
    , m_currentMatchIndex(0)
{
    setFrameShape(StyledPanel);
    setFrameShadow(Plain);

    setAttribute(Qt::WA_StyledBackground);
    setAutoFillBackground(true);

    // Устанавливаем тот же фон, что и у LineNumberArea
    QPalette pal = palette();
    QStyle *s = qApp->style();
    QPalette standardPalette = s->standardPalette();
    pal.setColor(QPalette::Window, standardPalette.color(QPalette::Normal, QPalette::Window));
    setPalette(pal);

    // Создаем UI компоненты - первый ряд
    m_searchLineEdit = new QLineEdit(this);
    m_searchLineEdit->setPlaceholderText(tr("Поиск..."));
    m_searchLineEdit->setClearButtonEnabled(true);
    m_searchLineEdit->installEventFilter(this);

    m_findNextButton = new QToolButton(this);
    m_findNextButton->setText(tr("↓"));
    m_findNextButton->setToolTip(tr("Найти далее (F3)"));

    if (QIcon::hasThemeIcon("FindNext"))
        m_findNextButton->setIcon(QIcon::fromTheme("FindNext"));
    else
        m_findNextButton->setIcon(QIcon("://img/FindNext.svg"));

    m_findPreviousButton = new QToolButton(this);
    m_findPreviousButton->setText(tr("↑"));
    m_findPreviousButton->setToolTip(tr("Найти предыдущее (Shift+F3)"));

    if (QIcon::hasThemeIcon("FindPrevious"))
        m_findPreviousButton->setIcon(QIcon::fromTheme("FindPrevious"));
    else
        m_findPreviousButton->setIcon(QIcon("://img/FindPrevious.svg"));

    m_closeButton = new QToolButton(this);
    m_closeButton->setToolTip(tr("Закрыть (Esc)"));
    if (QIcon::hasThemeIcon("Close"))
        m_closeButton->setIcon(QIcon::fromTheme("Close"));
    else
        m_closeButton->setIcon(QIcon("://img/Close.svg"));
    if (m_closeButton->icon().isNull())
        m_closeButton->setText(tr("✕"));

    // Создаем UI компоненты - второй ряд
    m_caseSensitiveButton = new QToolButton(this);
    if (QIcon::hasThemeIcon("CaseSensitive"))
        m_caseSensitiveButton->setIcon(QIcon::fromTheme("CaseSensitive"));
    else
        m_caseSensitiveButton->setIcon(QIcon("://img/CaseSensitive.svg"));
    if (m_caseSensitiveButton->icon().isNull())
        m_caseSensitiveButton->setText(tr("Aa"));
    m_caseSensitiveButton->setToolTip(tr("Учитывать регистр"));
    m_caseSensitiveButton->setCheckable(true);

    m_wholeWordButton = new QToolButton(this);
    m_wholeWordButton->setToolTip(tr("Слово целиком"));
    m_wholeWordButton->setCheckable(true);
    if (QIcon::hasThemeIcon("String"))
        m_wholeWordButton->setIcon(QIcon::fromTheme("String"));
    else
        m_wholeWordButton->setIcon(QIcon("://img/String.svg"));
    if (m_wholeWordButton->icon().isNull())
        m_wholeWordButton->setText(tr("ab|cd"));

    m_regexButton = new QToolButton(this);
    m_regexButton->setToolTip(tr("Использовать регулярное выражение"));
    m_regexButton->setCheckable(true);
    if (QIcon::hasThemeIcon("String"))
        m_regexButton->setIcon(QIcon::fromTheme("RegularExpression"));
    else
        m_regexButton->setIcon(QIcon("://img/RegularExpression.svg"));
    if (m_regexButton->icon().isNull())
        m_regexButton->setText(tr(".*"));

    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_statusLabel->setMinimumWidth(60);

    // Setup layouts
    m_firstRowLayout = new QHBoxLayout();
    m_firstRowLayout->setContentsMargins(4, 4, 4, 4);
    m_firstRowLayout->setSpacing(2);
    m_firstRowLayout->addWidget(m_searchLineEdit, 1);
    m_firstRowLayout->addWidget(m_findPreviousButton);
    m_firstRowLayout->addWidget(m_findNextButton);
    m_firstRowLayout->addWidget(m_closeButton);

    m_secondRowLayout = new QHBoxLayout();
    m_secondRowLayout->setContentsMargins(4, 4, 4, 4);
    m_secondRowLayout->setSpacing(2);
    m_secondRowLayout->addWidget(m_caseSensitiveButton);
    m_secondRowLayout->addWidget(m_wholeWordButton);
    m_secondRowLayout->addWidget(m_regexButton);
    m_secondRowLayout->addStretch();
    m_secondRowLayout->addWidget(m_statusLabel);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->addLayout(m_firstRowLayout);
    m_mainLayout->addLayout(m_secondRowLayout);

    // Setup timer for incremental search
    m_searchTimer = new QTimer(this);
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(300);

    // Connect signals
    connect(m_searchLineEdit, &QLineEdit::textChanged, this, &CodeSearchWidget::onSearchTextChanged);
    connect(m_findNextButton, &QToolButton::clicked, this, &CodeSearchWidget::onFindNext);
    connect(m_findPreviousButton, &QToolButton::clicked, this, &CodeSearchWidget::onFindPrevious);
    connect(m_closeButton, &QToolButton::clicked, this, &CodeSearchWidget::onClose);
    connect(m_caseSensitiveButton, &QToolButton::toggled, this, &CodeSearchWidget::onToggleCaseSensitive);
    connect(m_wholeWordButton, &QToolButton::toggled, this, &CodeSearchWidget::onToggleWholeWord);
    connect(m_regexButton, &QToolButton::toggled, this, &CodeSearchWidget::onToggleRegex);
    connect(m_searchTimer, &QTimer::timeout, this, &CodeSearchWidget::onSearchTimeout);

    // Initial state
    m_currentFlags = QTextDocument::FindFlags();
    hide();
    raise();
}

CodeSearchWidget::~CodeSearchWidget()
{
}

void CodeSearchWidget::setSearchText(const QString &text)
{
    if (text.isEmpty())
        return;

    m_searchLineEdit->setText(text);
    m_searchLineEdit->selectAll();

    performSearch();
}

void CodeSearchWidget::updatePosition()
{
    if (!m_editor || !isVisible())
        return;

    QWidget *viewport = m_editor->viewport();
    if (!viewport)
        return;

    // Обновляем родителя если нужно
    if (parent() != viewport) {
        setParent(viewport);
    }

    QRect viewportRect = viewport->rect();
    QSize widgetSize = sizeHint();

    // Прижимаем к правому верхнему углу
    int x = viewportRect.width() - widgetSize.width();
    int y = 0;

    setGeometry(x, y, widgetSize.width(), widgetSize.height());
    raise();
}

void CodeSearchWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updatePosition();
    raise();
}

void CodeSearchWidget::show()
{
    updatePosition();
    QWidget::show();
    setFocusToSearch();
    emit visibilityChanged(true);
    raise();
}

void CodeSearchWidget::hide()
{
    QWidget::hide();
    m_searchLineEdit->clear();
    m_statusLabel->clear();
    emit visibilityChanged(false);
}

void CodeSearchWidget::setFocusToSearch()
{
    m_searchLineEdit->setFocus();
    m_searchLineEdit->selectAll();
}

bool CodeSearchWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_searchLineEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Escape) {
            hide();
            m_editor->setFocus();
            return true;
        } else if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            if (keyEvent->modifiers() & Qt::ShiftModifier)
                onFindPrevious();
            else
                onFindNext();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void CodeSearchWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        hide();
        m_editor->setFocus();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void CodeSearchWidget::onSearchTextChanged(const QString &text)
{
    m_lastSearchText = text;
    if (text.isEmpty()) {
        m_statusLabel->clear();
        m_totalMatches = 0;
        m_currentMatchIndex = 0;
        updateButtonsState();
        return;
    }
    m_searchTimer->start();
}

void CodeSearchWidget::onSearchTimeout()
{
    performSearch();
}

void CodeSearchWidget::performSearch()
{
    if (m_lastSearchText.isEmpty()) {
        return;
    }

    // Clear previous selections
    QList<QTextEdit::ExtraSelection> currentSelections = m_editor->extraSelections();
    QList<QTextEdit::ExtraSelection> lineSelections;

    for (const QTextEdit::ExtraSelection &sel : currentSelections) {
        if (sel.format.hasProperty(QTextFormat::FullWidthSelection)) {
            lineSelections.append(sel);
        }
    }

    // Find all matches
    QTextDocument *document = m_editor->document();
    QTextCursor cursor(document);
    QList<QTextCursor> matches;

    QTextDocument::FindFlags flags = m_currentFlags;

    if (m_regexButton->isChecked()) {
        QRegularExpression regex(m_lastSearchText);
        if (!m_caseSensitiveButton->isChecked())
            regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

        QString plainText = document->toPlainText();
        QRegularExpressionMatchIterator i = regex.globalMatch(plainText);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QTextCursor matchCursor(document);
            matchCursor.setPosition(match.capturedStart());
            matchCursor.setPosition(match.capturedEnd(), QTextCursor::KeepAnchor);
            matches.append(matchCursor);
        }
    } else {
        while (!cursor.isNull() && !cursor.atEnd()) {
            cursor = document->find(m_lastSearchText, cursor, flags);
            if (!cursor.isNull()) {
                matches.append(cursor);
                cursor.setPosition(cursor.position());
            }
        }
    }

    m_totalMatches = matches.size();

    if (m_totalMatches > 0) {
        QList<QTextEdit::ExtraSelection> extraSelections;
        QTextEdit::ExtraSelection matchSelection;
        matchSelection.format.setBackground(QColor("#264F78"));
        matchSelection.format.setForeground(QColor("#FFFFFF"));

        for (const QTextCursor &matchCursor : matches) {
            matchSelection.cursor = matchCursor;
            extraSelections.append(matchSelection);
        }

        extraSelections.append(lineSelections);
        m_editor->setExtraSelections(extraSelections);

        m_currentMatchIndex = 0;
        m_editor->setTextCursor(matches.first());
        updateSearchStatus(m_totalMatches, m_currentMatchIndex + 1);
    } else {
        m_statusLabel->setText(tr("Нет результатов"));
        m_statusLabel->setStyleSheet("color: #F48771;");
        m_currentMatchIndex = 0;
        m_editor->setExtraSelections(lineSelections);
    }

    updateButtonsState();
}

bool CodeSearchWidget::findText(const QString &text, QTextDocument::FindFlags flags)
{
    QTextCursor cursor = m_editor->textCursor();
    QTextCursor newCursor = m_editor->document()->find(text, cursor, flags);

    if (newCursor.isNull()) {
        if (flags & QTextDocument::FindBackward) {
            QTextCursor endCursor(m_editor->document());
            endCursor.movePosition(QTextCursor::End);
            newCursor = m_editor->document()->find(text, endCursor, flags);
        } else {
            QTextCursor startCursor(m_editor->document());
            startCursor.movePosition(QTextCursor::Start);
            newCursor = m_editor->document()->find(text, startCursor, flags);
        }

        if (!newCursor.isNull()) {
            m_statusLabel->setText(tr("Поиск продолжен"));
            QTimer::singleShot(1000, [this]() {
                if (m_totalMatches > 0)
                    updateSearchStatus(m_totalMatches, m_currentMatchIndex + 1);
            });
        }
    }

    if (!newCursor.isNull()) {
        m_editor->setTextCursor(newCursor);
        return true;
    }

    return false;
}

void CodeSearchWidget::onFindNext()
{
    if (m_lastSearchText.isEmpty())
        return;

    QTextDocument::FindFlags flags = m_currentFlags;
    flags &= ~QTextDocument::FindBackward;

    if (findText(m_lastSearchText, flags)) {
        m_currentMatchIndex = (m_currentMatchIndex + 1) % m_totalMatches;
        updateSearchStatus(m_totalMatches, m_currentMatchIndex + 1);
    }
}

void CodeSearchWidget::findNext()
{
    onFindNext();
}

void CodeSearchWidget::findPrevious()
{
    onFindPrevious();
}

void CodeSearchWidget::onFindPrevious()
{
    if (m_lastSearchText.isEmpty())
        return;

    QTextDocument::FindFlags flags = m_currentFlags;
    flags |= QTextDocument::FindBackward;

    if (findText(m_lastSearchText, flags)) {
        m_currentMatchIndex = (m_currentMatchIndex - 1 + m_totalMatches) % m_totalMatches;
        updateSearchStatus(m_totalMatches, m_currentMatchIndex + 1);
    }
}

void CodeSearchWidget::onToggleCaseSensitive(bool checked)
{
    if (checked)
        m_currentFlags |= QTextDocument::FindCaseSensitively;
    else
        m_currentFlags &= ~QTextDocument::FindCaseSensitively;

    if (!m_lastSearchText.isEmpty())
        performSearch();
}

void CodeSearchWidget::onToggleWholeWord(bool checked)
{
    if (checked)
        m_currentFlags |= QTextDocument::FindWholeWords;
    else
        m_currentFlags &= ~QTextDocument::FindWholeWords;

    if (!m_lastSearchText.isEmpty())
        performSearch();
}

void CodeSearchWidget::onToggleRegex(bool checked)
{
    if (checked) {
        m_wholeWordButton->setEnabled(false);
        m_currentFlags &= ~QTextDocument::FindWholeWords;
        m_wholeWordButton->setChecked(false);
    } else {
        m_wholeWordButton->setEnabled(true);
    }

    if (!m_lastSearchText.isEmpty())
        performSearch();
}

void CodeSearchWidget::onClose()
{
    hide();
    m_editor->setFocus();

    QList<QTextEdit::ExtraSelection> selections = m_editor->extraSelections();
    QList<QTextEdit::ExtraSelection> lineSelections;

    for (const QTextEdit::ExtraSelection &sel : selections) {
        if (sel.format.hasProperty(QTextFormat::FullWidthSelection)) {
            lineSelections.append(sel);
        }
    }

    m_editor->setExtraSelections(lineSelections);
}

void CodeSearchWidget::updateSearchStatus(int matches, int currentMatch)
{
    if (matches > 0) {
        m_statusLabel->setText(QString("%1/%2").arg(currentMatch).arg(matches));
        m_statusLabel->setStyleSheet(QString("color: %1;").arg(palette().color(QPalette::Text).name()));
    } else {
        m_statusLabel->setText(tr("Нет результатов"));
        m_statusLabel->setStyleSheet("color: #F48771;");
    }
}

void CodeSearchWidget::updateButtonsState()
{
    bool hasSearchText = !m_lastSearchText.isEmpty();
    bool hasMatches = m_totalMatches > 0;

    m_findNextButton->setEnabled(hasSearchText && hasMatches);
    m_findPreviousButton->setEnabled(hasSearchText && hasMatches);
}