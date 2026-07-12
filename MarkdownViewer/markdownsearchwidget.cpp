#include "markdownsearchwidget.h"
#include "markdownview.h"

#include <QKeyEvent>
#include <QWebEngineView>
#include <QDebug>

MarkdownSearchWidget::MarkdownSearchWidget(MarkdownView *view, QWidget *parent)
    : QFrame(parent)
    , m_view(view)
    , m_caseSensitive(false)
{
    setFrameShape(StyledPanel);
    setFrameShadow(Plain);
    setAttribute(Qt::WA_StyledBackground);
    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#F0F0F0"));
    setPalette(pal);

    // Первый ряд
    m_searchLineEdit = new QLineEdit(this);
    m_searchLineEdit->setPlaceholderText(tr("Поиск..."));
    m_searchLineEdit->setClearButtonEnabled(true);
    m_searchLineEdit->installEventFilter(this);

    m_findPreviousButton = new QToolButton(this);
    m_findPreviousButton->setToolTip(tr("Найти предыдущее (Shift+Enter)"));
    m_findPreviousButton->setIcon(QIcon::fromTheme("FindPrevious"));
    if (m_findPreviousButton->icon().isNull())
        m_findPreviousButton->setText(tr("↑"));

    m_findNextButton = new QToolButton(this);
    m_findNextButton->setToolTip(tr("Найти далее (Enter)"));
    m_findNextButton->setIcon(QIcon::fromTheme("FindNext"));
    if (m_findNextButton->icon().isNull())
        m_findNextButton->setText(tr("↓"));

    m_closeButton = new QToolButton(this);
    m_closeButton->setToolTip(tr("Закрыть (Esc)"));
    m_closeButton->setIcon(QIcon::fromTheme("Close"));
    if (m_closeButton->icon().isNull())
        m_closeButton->setText(tr("✕"));

    // Второй ряд
    m_caseSensitiveButton = new QToolButton(this);
    m_caseSensitiveButton->setToolTip(tr("Учитывать регистр"));
    m_caseSensitiveButton->setIcon(QIcon::fromTheme("CaseSensitive"));
    if (m_caseSensitiveButton->icon().isNull())
        m_caseSensitiveButton->setText(tr("Aa"));
    m_caseSensitiveButton->setCheckable(true);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_statusLabel->setMinimumWidth(80);

    // Layouts
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
    m_secondRowLayout->addStretch();
    m_secondRowLayout->addWidget(m_statusLabel);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->addLayout(m_firstRowLayout);
    m_mainLayout->addLayout(m_secondRowLayout);

    m_searchTimer = new QTimer(this);
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(300);

    connect(m_searchLineEdit, &QLineEdit::textChanged, this, &MarkdownSearchWidget::onSearchTextChanged);
    connect(m_findNextButton, &QToolButton::clicked, this, &MarkdownSearchWidget::onFindNext);
    connect(m_findPreviousButton, &QToolButton::clicked, this, &MarkdownSearchWidget::onFindPrevious);
    connect(m_closeButton, &QToolButton::clicked, this, &MarkdownSearchWidget::onClose);
    connect(m_caseSensitiveButton, &QToolButton::toggled, this, &MarkdownSearchWidget::onToggleCaseSensitive);
    connect(m_searchTimer, &QTimer::timeout, this, &MarkdownSearchWidget::onSearchTimeout);

    hide();
    raise();
}

MarkdownSearchWidget::~MarkdownSearchWidget() = default;

void MarkdownSearchWidget::setSearchText(const QString &text)
{
    if (text.isEmpty())
        return;

    m_searchLineEdit->setText(text);
    m_searchLineEdit->selectAll();
    performSearch();
}

void MarkdownSearchWidget::updatePosition()
{
    if (!m_view || !isVisible())
        return;

    if (parent() != m_view)
        setParent(m_view);

    QRect viewRect = m_view->rect();
    QSize widgetSize = sizeHint();

    int x = viewRect.width() - widgetSize.width();
    int y = 0;

    setGeometry(x, y, widgetSize.width(), widgetSize.height());
    raise();
}

void MarkdownSearchWidget::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);
    updatePosition();
    raise();
}

void MarkdownSearchWidget::show()
{
    updatePosition();
    QFrame::show();
    setFocusToSearch();
    emit visibilityChanged(true);
    raise();
}

void MarkdownSearchWidget::hide()
{
    QFrame::hide();
    m_searchLineEdit->clear();
    m_statusLabel->clear();

    if (m_view)
    {
        QWebEngineView *webView = m_view->webView();
        if (webView && webView->page())
            webView->page()->findText(QString());
    }

    emit visibilityChanged(false);
}

void MarkdownSearchWidget::setFocusToSearch()
{
    m_searchLineEdit->setFocus();
    m_searchLineEdit->selectAll();
}

bool MarkdownSearchWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_searchLineEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Escape)
        {
            hide();
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
        {
            if (keyEvent->modifiers() & Qt::ShiftModifier)
                onFindPrevious();
            else
                onFindNext();
            return true;
        }
    }
    return QFrame::eventFilter(obj, event);
}

void MarkdownSearchWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        hide();
    }
    else
    {
        QFrame::keyPressEvent(event);
    }
}

void MarkdownSearchWidget::onSearchTextChanged(const QString &text)
{
    m_lastSearchText = text;
    if (text.isEmpty())
    {
        m_statusLabel->clear();
        if (m_view)
        {
            QWebEngineView *webView = m_view->webView();
            if (webView && webView->page())
                webView->page()->findText(QString());
        }
        updateButtonsState();
        return;
    }
    m_searchTimer->start();
}

void MarkdownSearchWidget::onSearchTimeout()
{
    performSearch();
}

QWebEnginePage::FindFlags MarkdownSearchWidget::currentFlags() const
{
    return m_caseSensitive ? QWebEnginePage::FindCaseSensitively : QWebEnginePage::FindFlags();
}

void MarkdownSearchWidget::performSearch()
{
    QWebEngineView *webView = m_view ? m_view->webView() : nullptr;
    if (!webView || !webView->page())
        return;

    if (m_lastSearchText.isEmpty())
    {
        webView->page()->findText(QString());
        m_statusLabel->clear();
        updateButtonsState();
        return;
    }

    QWebEnginePage::FindFlags flags = currentFlags();
    webView->page()->findText(m_lastSearchText, flags,
        [this](bool found) {
            updateSearchStatus(found);
            updateButtonsState();
        });
}

void MarkdownSearchWidget::onFindNext()
{
    QWebEngineView *webView = m_view ? m_view->webView() : nullptr;
    if (!webView || !webView->page() || m_lastSearchText.isEmpty())
        return;

    webView->page()->findText(m_lastSearchText, currentFlags(),
        [this](bool found) {
            updateSearchStatus(found);
        });
}

void MarkdownSearchWidget::onFindPrevious()
{
    QWebEngineView *webView = m_view ? m_view->webView() : nullptr;
    if (!webView || !webView->page() || m_lastSearchText.isEmpty())
        return;

    QWebEnginePage::FindFlags flags = currentFlags() | QWebEnginePage::FindBackward;
    webView->page()->findText(m_lastSearchText, flags,
        [this](bool found) {
            updateSearchStatus(found);
        });
}

void MarkdownSearchWidget::findNext()
{
    onFindNext();
}

void MarkdownSearchWidget::findPrevious()
{
    onFindPrevious();
}

void MarkdownSearchWidget::onToggleCaseSensitive(bool checked)
{
    m_caseSensitive = checked;
    if (!m_lastSearchText.isEmpty())
        performSearch();
}

void MarkdownSearchWidget::onClose()
{
    hide();
}

void MarkdownSearchWidget::updateSearchStatus(bool found)
{
    if (found)
    {
        m_statusLabel->setText(tr("Найдено"));
        m_statusLabel->setStyleSheet(QString("color: %1;").arg(palette().color(QPalette::Text).name()));
    }
    else
    {
        m_statusLabel->setText(tr("Нет результатов"));
        m_statusLabel->setStyleSheet("color: #F48771;");
    }
}

void MarkdownSearchWidget::updateButtonsState()
{
    bool hasSearchText = !m_lastSearchText.isEmpty();
    m_findNextButton->setEnabled(hasSearchText);
    m_findPreviousButton->setEnabled(hasSearchText);
}
