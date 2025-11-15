#include "RTTextDocument.h"
#include "rttextcursor.h"
#include "rsscript/registerobjlist.hpp"

RTTextDocument::RTTextDocument(QObject *parent)
    : QObject(parent)
    , m_document(new QTextDocument(this))
{
    connect(m_document, &QTextDocument::contentsChanged, this, &RTTextDocument::documentChanged);
    connect(m_document, &QTextDocument::contentsChange, this, &RTTextDocument::contentsChange);
    connect(m_document, &QTextDocument::undoCommandAdded, this, &RTTextDocument::undoCommandAdded);
}

RTTextDocument::RTTextDocument(const QString &text, QObject *parent)
    : QObject(parent)
    , m_document(new QTextDocument(text, this))
{
    connect(m_document, &QTextDocument::contentsChanged, this, &RTTextDocument::documentChanged);
    connect(m_document, &QTextDocument::contentsChange, this, &RTTextDocument::contentsChange);
    connect(m_document, &QTextDocument::undoCommandAdded, this, &RTTextDocument::undoCommandAdded);
}

RTTextDocument::RTTextDocument(const RTTextDocument &other)
    : QObject(other.parent())
    , m_document(other.m_document->clone(this))
{
    connect(m_document, &QTextDocument::contentsChanged, this, &RTTextDocument::documentChanged);
    connect(m_document, &QTextDocument::contentsChange, this, &RTTextDocument::contentsChange);
    connect(m_document, &QTextDocument::undoCommandAdded, this, &RTTextDocument::undoCommandAdded);
}

RTTextDocument::~RTTextDocument()
{
}

RTTextDocument &RTTextDocument::operator=(const RTTextDocument &other)
{
    if (this != &other) {
        delete m_document;
        m_document = other.m_document->clone(this);
        connect(m_document, &QTextDocument::contentsChanged, this, &RTTextDocument::documentChanged);
        connect(m_document, &QTextDocument::contentsChange, this, &RTTextDocument::contentsChange);
        connect(m_document, &QTextDocument::undoCommandAdded, this, &RTTextDocument::undoCommandAdded);
        emit documentChanged();
    }
    return *this;
}

QTextDocument *RTTextDocument::textDocument() const
{
    return m_document;
}

void RTTextDocument::setTextDocument(QTextDocument *document)
{
    if (m_document != document) {
        delete m_document;
        m_document = document;
        if (m_document) {
            m_document->setParent(this);
        }
        emit documentChanged();
    }
}

// Геттеры
int RTTextDocument::availableRedoSteps() const { return m_document->availableRedoSteps(); }
int RTTextDocument::availableUndoSteps() const { return m_document->availableUndoSteps(); }
QUrl RTTextDocument::baseUrl() const { return m_document->baseUrl(); }
QTextBlock RTTextDocument::begin() const { return m_document->begin(); }
int RTTextDocument::blockCount() const { return m_document->blockCount(); }
QChar RTTextDocument::characterAt(int pos) const { return m_document->characterAt(pos); }
int RTTextDocument::characterCount() const { return m_document->characterCount(); }
int RTTextDocument::defaultCursorMoveStyle() const { return static_cast<int>(m_document->defaultCursorMoveStyle()); }
QFont RTTextDocument::defaultFont() const { return m_document->defaultFont(); }
QString RTTextDocument::defaultStyleSheet() const { return m_document->defaultStyleSheet(); }
QTextOption RTTextDocument::defaultTextOption() const { return m_document->defaultTextOption(); }
QAbstractTextDocumentLayout* RTTextDocument::documentLayout() const { return m_document->documentLayout(); }
qreal RTTextDocument::documentMargin() const { return m_document->documentMargin(); }
QTextBlock RTTextDocument::end() const { return m_document->end(); }
QTextBlock RTTextDocument::findBlock(int pos) const { return m_document->findBlock(pos); }
QTextBlock RTTextDocument::findBlockByLineNumber(int lineNumber) const { return m_document->findBlockByLineNumber(lineNumber); }
QTextBlock RTTextDocument::findBlockByNumber(int blockNumber) const { return m_document->findBlockByNumber(blockNumber); }
QTextBlock RTTextDocument::firstBlock() const { return m_document->firstBlock(); }
qreal RTTextDocument::idealWidth() const { return m_document->idealWidth(); }
qreal RTTextDocument::indentWidth() const { return m_document->indentWidth(); }
bool RTTextDocument::isEmpty() const { return m_document->isEmpty(); }
bool RTTextDocument::isModified() const { return m_document->isModified(); }
bool RTTextDocument::isRedoAvailable() const { return m_document->isRedoAvailable(); }
bool RTTextDocument::isUndoAvailable() const { return m_document->isUndoAvailable(); }
bool RTTextDocument::isUndoRedoEnabled() const { return m_document->isUndoRedoEnabled(); }
QTextBlock RTTextDocument::lastBlock() const { return m_document->lastBlock(); }
int RTTextDocument::lineCount() const { return m_document->lineCount(); }
int RTTextDocument::maximumBlockCount() const { return m_document->maximumBlockCount(); }
QString RTTextDocument::metaInformation(int info) const { return m_document->metaInformation(static_cast<QTextDocument::MetaInformation>(info)); }
QTextObject *RTTextDocument::object(int objectIndex) const { return m_document->object(objectIndex); }
QTextObject *RTTextDocument::objectForFormat(const QTextFormat &f) const { return m_document->objectForFormat(f); }
int RTTextDocument::pageCount() const { return m_document->pageCount(); }
QSizeF RTTextDocument::pageSize() const { return m_document->pageSize(); }
QVariant RTTextDocument::resource(int type, const QUrl &name) const { return m_document->resource(type, name); }
int RTTextDocument::revision() const { return m_document->revision(); }
QTextFrame *RTTextDocument::rootFrame() const { return m_document->rootFrame(); }
QSizeF RTTextDocument::size() const { return m_document->size(); }
qreal RTTextDocument::textWidth() const { return m_document->textWidth(); }
QString RTTextDocument::toHtml(const QByteArray &encoding) const { return m_document->toHtml(encoding); }
QString RTTextDocument::toMarkdown(int features) const { return m_document->toMarkdown(static_cast<QTextDocument::MarkdownFeatures>(features)); }
QString RTTextDocument::toPlainText() const { return m_document->toPlainText(); }
QString RTTextDocument::toRawText() const { return m_document->toRawText(); }
bool RTTextDocument::useDesignMetrics() const { return m_document->useDesignMetrics(); }

// Сеттеры
void RTTextDocument::setBaseUrl(const QUrl &url) { m_document->setBaseUrl(url); emit documentChanged(); }
void RTTextDocument::setDefaultCursorMoveStyle(int style) { m_document->setDefaultCursorMoveStyle(static_cast<Qt::CursorMoveStyle>(style)); emit documentChanged(); }
void RTTextDocument::setDefaultFont(const QFont &font) { m_document->setDefaultFont(font); emit documentChanged(); }
void RTTextDocument::setDefaultStyleSheet(const QString &sheet) { m_document->setDefaultStyleSheet(sheet); emit documentChanged(); }
void RTTextDocument::setDefaultTextOption(const QTextOption &option) { m_document->setDefaultTextOption(option); emit documentChanged(); }
void RTTextDocument::setDocumentLayout(QAbstractTextDocumentLayout *layout) { m_document->setDocumentLayout(layout); emit documentChanged(); }
void RTTextDocument::setDocumentMargin(qreal margin) { m_document->setDocumentMargin(margin); emit documentChanged(); }
void RTTextDocument::setHtml(const QString &html) { m_document->setHtml(html); emit documentChanged(); }
void RTTextDocument::setIndentWidth(qreal width) { m_document->setIndentWidth(width); emit documentChanged(); }
void RTTextDocument::setMarkdown(const QString &markdown, int features) { m_document->setMarkdown(markdown, static_cast<QTextDocument::MarkdownFeatures>(features)); emit documentChanged(); }
void RTTextDocument::setMaximumBlockCount(int maximum) { m_document->setMaximumBlockCount(maximum); emit documentChanged(); }
void RTTextDocument::setMetaInformation(int info, const QString &string) { m_document->setMetaInformation(static_cast<QTextDocument::MetaInformation>(info), string); emit documentChanged(); }
void RTTextDocument::setPageSize(const QSizeF &size) { m_document->setPageSize(size); emit documentChanged(); }
void RTTextDocument::setPlainText(const QString &text) { m_document->setPlainText(text); emit documentChanged(); }
void RTTextDocument::setTextWidth(qreal width) { m_document->setTextWidth(width); emit documentChanged(); }
void RTTextDocument::setUndoRedoEnabled(bool enable) { m_document->setUndoRedoEnabled(enable); emit documentChanged(); }
void RTTextDocument::setUseDesignMetrics(bool b) { m_document->setUseDesignMetrics(b); emit documentChanged(); }

// Методы с параметрами
void RTTextDocument::addResource(int type, const QUrl &name, const QVariant &resource) { m_document->addResource(type, name, resource); emit documentChanged(); }
void RTTextDocument::adjustSize() { m_document->adjustSize(); emit documentChanged(); }
QVector<QTextFormat> RTTextDocument::allFormats() const { return m_document->allFormats(); }
void RTTextDocument::clear() { m_document->clear(); emit documentChanged(); }
void RTTextDocument::clearUndoRedoStacks(int stacksToClear) { m_document->clearUndoRedoStacks(static_cast<QTextDocument::Stacks>(stacksToClear)); emit documentChanged(); }
RTTextDocument *RTTextDocument::clone(QObject *parent) const { return new RTTextDocument(m_document->clone(parent)); }
void RTTextDocument::drawContents(QPainter *p, const QRectF &rect) { m_document->drawContents(p, rect); }
RTTextCursor *RTTextDocument::find(const QString &subString, RTTextCursor *cursor, int options) const
{
    QTextCursor result = m_document->find(subString, cursor->textCursor(), static_cast<QTextDocument::FindFlags>(options));
    RTTextCursor *pCur = new RTTextCursor(result);
    rslSetOwnerRsl(pCur);
    return pCur;
}
RTTextCursor *RTTextDocument::find(const QString &subString, int position, int options) const
{
    QTextCursor result = m_document->find(subString, position, static_cast<QTextDocument::FindFlags>(options));
    RTTextCursor *pCur = new RTTextCursor(result);
    rslSetOwnerRsl(pCur);
    return pCur;
}
void RTTextDocument::markContentsDirty(int position, int length) { m_document->markContentsDirty(position, length); emit documentChanged(); }
void RTTextDocument::print(QPagedPaintDevice *printer) const { m_document->print(printer); }
void RTTextDocument::redo(QTextCursor *cursor) { m_document->redo(cursor); emit documentChanged(); }
void RTTextDocument::undo(QTextCursor *cursor) { m_document->undo(cursor); emit documentChanged(); }
