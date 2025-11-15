#include "RTTextCursor.h"
#include "rttextblockformat.h"

RTTextCursor::RTTextCursor(QObject *parent)
    : QObject(parent)
    , m_cursor()
{
}

RTTextCursor::RTTextCursor(const QTextCursor &cursor, QObject *parent)
    : QObject(parent)
    , m_cursor(cursor)
{
}

RTTextCursor::RTTextCursor(const QTextBlock &block, QObject *parent)
    : QObject(parent)
    , m_cursor(block)
{
}

RTTextCursor::RTTextCursor(QTextFrame *frame, QObject *parent)
    : QObject(parent)
    , m_cursor(frame)
{
}

RTTextCursor::RTTextCursor(QTextDocument *document, QObject *parent)
    : QObject(parent)
    , m_cursor(document)
{
}

RTTextCursor::RTTextCursor(const RTTextCursor &other)
    : QObject(other.parent())
    , m_cursor(other.m_cursor)
{
}

RTTextCursor &RTTextCursor::operator=(const RTTextCursor &other)
{
    if (this != &other) {
        m_cursor = other.m_cursor;
        emit cursorChanged();
    }
    return *this;
}

RTTextCursor::~RTTextCursor()
{
}

// Реализация методов-свойств
int RTTextCursor::anchor() const { return m_cursor.anchor(); }
bool RTTextCursor::atBlockEnd() const { return m_cursor.atBlockEnd(); }
bool RTTextCursor::atBlockStart() const { return m_cursor.atBlockStart(); }
bool RTTextCursor::atEnd() const { return m_cursor.atEnd(); }
bool RTTextCursor::atStart() const { return m_cursor.atStart(); }
/*QTextBlock RTTextCursor::block() const { return m_cursor.block(); }
QTextCharFormat RTTextCursor::blockCharFormat() const { return m_cursor.blockCharFormat(); }
QTextBlockFormat RTTextCursor::blockFormat() const { return m_cursor.blockFormat(); }*/
int RTTextCursor::blockNumber() const { return m_cursor.blockNumber(); }
//QTextCharFormat RTTextCursor::charFormat() const { return m_cursor.charFormat(); }
int RTTextCursor::columnNumber() const { return m_cursor.columnNumber(); }
/*QTextFrame* RTTextCursor::currentFrame() const { return m_cursor.currentFrame(); }
QTextList* RTTextCursor::currentList() const { return m_cursor.currentList(); }
QTextTable* RTTextCursor::currentTable() const { return m_cursor.currentTable(); }
QTextDocument* RTTextCursor::document() const { return m_cursor.document(); }*/
bool RTTextCursor::hasComplexSelection() const { return m_cursor.hasComplexSelection(); }
bool RTTextCursor::hasSelection() const { return m_cursor.hasSelection(); }
bool RTTextCursor::isNull() const { return m_cursor.isNull(); }
bool RTTextCursor::keepPositionOnInsert() const { return m_cursor.keepPositionOnInsert(); }
int RTTextCursor::position() const { return m_cursor.position(); }
int RTTextCursor::positionInBlock() const { return m_cursor.positionInBlock(); }
QString RTTextCursor::selectedText() const { return m_cursor.selectedText(); }
//QTextDocumentFragment RTTextCursor::selection() const { return m_cursor.selection(); }
int RTTextCursor::selectionEnd() const { return m_cursor.selectionEnd(); }
int RTTextCursor::selectionStart() const { return m_cursor.selectionStart(); }
int RTTextCursor::verticalMovementX() const { return m_cursor.verticalMovementX(); }
bool RTTextCursor::visualNavigation() const { return m_cursor.visualNavigation(); }

// Реализация методов с параметрами
void RTTextCursor::beginEditBlock() { m_cursor.beginEditBlock(); }
void RTTextCursor::clearSelection() { m_cursor.clearSelection(); emit cursorChanged(); }
/*QTextList* RTTextCursor::createList(const QTextListFormat &format) { return m_cursor.createList(format); }
QTextList* RTTextCursor::createList(QTextListFormat::Style style) { return m_cursor.createList(style); }*/
void RTTextCursor::deleteChar() { m_cursor.deleteChar(); emit cursorChanged(); }
void RTTextCursor::deletePreviousChar() { m_cursor.deletePreviousChar(); emit cursorChanged(); }
void RTTextCursor::endEditBlock() { m_cursor.endEditBlock(); }
void RTTextCursor::insertBlock() { m_cursor.insertBlock(); emit cursorChanged(); }
void RTTextCursor::insertBlock(RTTextBlockFormat *format) { m_cursor.insertBlock(format->textBlockFormat()); emit cursorChanged(); }
void RTTextCursor::insertBlock(RTTextBlockFormat *format, RTTextCharFormat *charFormat) { m_cursor.insertBlock(format->textBlockFormat(), charFormat->textCharFormat()); emit cursorChanged(); }
void RTTextCursor::insertFragment(const QTextDocumentFragment &fragment) { m_cursor.insertFragment(fragment); emit cursorChanged(); }
//QTextFrame* RTTextCursor::insertFrame(const QTextFrameFormat &format) { QTextFrame* frame = m_cursor.insertFrame(format); emit cursorChanged(); return frame; }
void RTTextCursor::insertHtml(const QString &html) { m_cursor.insertHtml(html); emit cursorChanged(); }
void RTTextCursor::insertImage(const QTextImageFormat &format) { m_cursor.insertImage(format); emit cursorChanged(); }
void RTTextCursor::insertImage(const QTextImageFormat &format, QTextFrameFormat::Position alignment) { m_cursor.insertImage(format, alignment); emit cursorChanged(); }
void RTTextCursor::insertImage(const QString &name) { m_cursor.insertImage(name); emit cursorChanged(); }
void RTTextCursor::insertImage(const QImage &image, const QString &name) { m_cursor.insertImage(image, name); emit cursorChanged(); }
/*QTextList* RTTextCursor::insertList(const QTextListFormat &format) { QTextList* list = m_cursor.insertList(format); emit cursorChanged(); return list; }
QTextList* RTTextCursor::insertList(QTextListFormat::Style style) { QTextList* list = m_cursor.insertList(style); emit cursorChanged(); return list; }
QTextTable* RTTextCursor::insertTable(int rows, int columns, const QTextTableFormat &format) { QTextTable* table = m_cursor.insertTable(rows, columns, format); emit cursorChanged(); return table; }
QTextTable* RTTextCursor::insertTable(int rows, int columns) { QTextTable* table = m_cursor.insertTable(rows, columns); emit cursorChanged(); return table; }*/
void RTTextCursor::insertText(const QString &text) { m_cursor.insertText(text); emit cursorChanged(); }
void RTTextCursor::insertText(const QString &text, RTTextCharFormat *format) { m_cursor.insertText(text, format->textCharFormat()); emit cursorChanged(); }
bool RTTextCursor::isCopyOf(const QTextCursor &other) const { return m_cursor.isCopyOf(other); }
bool RTTextCursor::isCopyOf(const RTTextCursor &other) const { return m_cursor.isCopyOf(other.m_cursor); }
void RTTextCursor::joinPreviousEditBlock() { m_cursor.joinPreviousEditBlock(); }
void RTTextCursor::mergeBlockCharFormat(RTTextCharFormat *format) { m_cursor.mergeBlockCharFormat(format->textCharFormat()); emit cursorChanged(); }
void RTTextCursor::mergeBlockFormat(const QTextBlockFormat &modifier) { m_cursor.mergeBlockFormat(modifier); emit cursorChanged(); }
void RTTextCursor::mergeCharFormat(RTTextCharFormat *format) { m_cursor.mergeCharFormat(format->textCharFormat()); emit cursorChanged(); }

bool RTTextCursor::movePosition(int operation, int mode, int n)
{
    bool result = m_cursor.movePosition((QTextCursor::MoveOperation)operation, (QTextCursor::MoveMode)mode, n);
    if (result)
        emit cursorChanged();
    return result;
}

void RTTextCursor::removeSelectedText() { m_cursor.removeSelectedText(); emit cursorChanged(); }
void RTTextCursor::select(int selection)
{
    m_cursor.select((QTextCursor::SelectionType)selection);
    emit cursorChanged();
}
void RTTextCursor::selectedTableCells(int *firstRow, int *numRows, int *firstColumn, int *numColumns) const { m_cursor.selectedTableCells(firstRow, numRows, firstColumn, numColumns); }
void RTTextCursor::setBlockCharFormat(RTTextCharFormat *format) { m_cursor.setBlockCharFormat(format->textCharFormat()); emit cursorChanged(); }
void RTTextCursor::setBlockFormat(const QTextBlockFormat &format) { m_cursor.setBlockFormat(format); emit cursorChanged(); }
void RTTextCursor::setCharFormat(RTTextCharFormat *format) { m_cursor.setCharFormat(format->textCharFormat()); emit cursorChanged(); }
void RTTextCursor::setKeepPositionOnInsert(bool b) { m_cursor.setKeepPositionOnInsert(b); emit cursorChanged(); }
void RTTextCursor::setPosition(int pos, int m)
{ m_cursor.setPosition(pos, (QTextCursor::MoveMode)m); emit cursorChanged(); }
void RTTextCursor::setVerticalMovementX(int x) { m_cursor.setVerticalMovementX(x); emit cursorChanged(); }
void RTTextCursor::setVisualNavigation(bool b) { m_cursor.setVisualNavigation(b); emit cursorChanged(); }
void RTTextCursor::swap(QTextCursor &other) { m_cursor.swap(other); emit cursorChanged(); }
void RTTextCursor::swap(RTTextCursor &other) { m_cursor.swap(other.m_cursor); emit cursorChanged(); other.emit cursorChanged(); }

// Доступ к внутреннему курсору
QTextCursor RTTextCursor::textCursor() const { return m_cursor; }
void RTTextCursor::setTextCursor(const QTextCursor &cursor) { m_cursor = cursor; emit cursorChanged(); }
