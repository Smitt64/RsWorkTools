#ifndef RTTEXTCURSOR_H
#define RTTEXTCURSOR_H

#include <QTextCursor>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QObject>
#include "ToolsRuntime_global.h"

class QTextFrame;
class QTextList;
class QTextTable;
class RTTextBlockFormat;
class RTTextCharFormat;
class TOOLSRUNTIME_EXPORT RTTextCursor : public QObject
{
    Q_OBJECT

    // Свойства для скалярных типов без параметров
    Q_PROPERTY(int anchor READ anchor NOTIFY cursorChanged)
    Q_PROPERTY(bool atBlockEnd READ atBlockEnd NOTIFY cursorChanged)
    Q_PROPERTY(bool atBlockStart READ atBlockStart NOTIFY cursorChanged)
    Q_PROPERTY(bool atEnd READ atEnd NOTIFY cursorChanged)
    Q_PROPERTY(bool atStart READ atStart NOTIFY cursorChanged)
    /*Q_PROPERTY(QTextBlock block READ block NOTIFY cursorChanged)
    Q_PROPERTY(QTextCharFormat blockCharFormat READ blockCharFormat NOTIFY cursorChanged)
    Q_PROPERTY(QTextBlockFormat blockFormat READ blockFormat NOTIFY cursorChanged)*/
    Q_PROPERTY(int blockNumber READ blockNumber NOTIFY cursorChanged)
    //Q_PROPERTY(QTextCharFormat charFormat READ charFormat NOTIFY cursorChanged)
    Q_PROPERTY(int columnNumber READ columnNumber NOTIFY cursorChanged)
    //Q_PROPERTY(QTextFrame* currentFrame READ currentFrame NOTIFY cursorChanged)
    //Q_PROPERTY(QTextList* currentList READ currentList NOTIFY cursorChanged)
    //Q_PROPERTY(QTextTable* currentTable READ currentTable NOTIFY cursorChanged)
    //Q_PROPERTY(QTextDocument* document READ document NOTIFY cursorChanged)
    Q_PROPERTY(bool hasComplexSelection READ hasComplexSelection NOTIFY cursorChanged)
    Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY cursorChanged)
    Q_PROPERTY(bool isNull READ isNull NOTIFY cursorChanged)
    Q_PROPERTY(bool keepPositionOnInsert READ keepPositionOnInsert NOTIFY cursorChanged)
    Q_PROPERTY(int position READ position NOTIFY cursorChanged)
    Q_PROPERTY(int positionInBlock READ positionInBlock NOTIFY cursorChanged)
    Q_PROPERTY(QString selectedText READ selectedText NOTIFY cursorChanged)
    //Q_PROPERTY(QTextDocumentFragment selection READ selection NOTIFY cursorChanged)
    Q_PROPERTY(int selectionEnd READ selectionEnd NOTIFY cursorChanged)
    Q_PROPERTY(int selectionStart READ selectionStart NOTIFY cursorChanged)
    Q_PROPERTY(int verticalMovementX READ verticalMovementX NOTIFY cursorChanged)
    Q_PROPERTY(bool visualNavigation READ visualNavigation NOTIFY cursorChanged)

public:
    // Конструкторы
    Q_INVOKABLE RTTextCursor(QObject *parent = nullptr);
    explicit RTTextCursor(const QTextCursor &cursor, QObject *parent = nullptr);
    explicit RTTextCursor(const QTextBlock &block, QObject *parent = nullptr);
    explicit RTTextCursor(QTextFrame *frame, QObject *parent = nullptr);
    explicit RTTextCursor(QTextDocument *document, QObject *parent = nullptr);
    RTTextCursor(const RTTextCursor &other);

    // Оператор присваивания
    RTTextCursor &operator=(const RTTextCursor &other);
    ~RTTextCursor();

    // Методы-свойства (только геттеры)
    int anchor() const;
    bool atBlockEnd() const;
    bool atBlockStart() const;
    bool atEnd() const;
    bool atStart() const;
    /*QTextBlock block() const;
    QTextCharFormat blockCharFormat() const;
    QTextBlockFormat blockFormat() const;*/
    int blockNumber() const;
    //QTextCharFormat charFormat() const;
    int columnNumber() const;
    /*QTextFrame* currentFrame() const;
    QTextList* currentList() const;
    QTextTable* currentTable() const;
    QTextDocument* document() const;*/
    bool hasComplexSelection() const;
    bool hasSelection() const;
    bool isNull() const;
    bool keepPositionOnInsert() const;
    int position() const;
    int positionInBlock() const;
    QString selectedText() const;
    //QTextDocumentFragment selection() const;
    int selectionEnd() const;
    int selectionStart() const;
    int verticalMovementX() const;
    bool visualNavigation() const;

    // Методы с параметрами
    Q_INVOKABLE void beginEditBlock();
    Q_INVOKABLE void clearSelection();
    //QTextList* createList(const QTextListFormat &format);
    //QTextList* createList(QTextListFormat::Style style);
    Q_INVOKABLE void deleteChar();
    Q_INVOKABLE void deletePreviousChar();
    Q_INVOKABLE void endEditBlock();
    Q_INVOKABLE void insertBlock();
    Q_INVOKABLE void insertBlock(RTTextBlockFormat *format);
    Q_INVOKABLE void insertBlock(RTTextBlockFormat *format, RTTextCharFormat *charFormat);
    void insertFragment(const QTextDocumentFragment &fragment);
    QTextFrame* insertFrame(const QTextFrameFormat &format);
    Q_INVOKABLE void insertHtml(const QString &html);
    void insertImage(const QTextImageFormat &format);
    void insertImage(const QTextImageFormat &format, QTextFrameFormat::Position alignment);
    Q_INVOKABLE void insertImage(const QString &name);
    void insertImage(const QImage &image, const QString &name = QString());
    QTextList* insertList(const QTextListFormat &format);
    QTextList* insertList(QTextListFormat::Style style);
    QTextTable* insertTable(int rows, int columns, const QTextTableFormat &format);
    QTextTable* insertTable(int rows, int columns);
    Q_INVOKABLE void insertText(const QString &text);
    Q_INVOKABLE void insertText(const QString &text, RTTextCharFormat *format);
    bool isCopyOf(const QTextCursor &other) const;
    Q_INVOKABLE bool isCopyOf(const RTTextCursor &other) const;
    Q_INVOKABLE void joinPreviousEditBlock();
    Q_INVOKABLE void mergeBlockCharFormat(RTTextCharFormat *format);
    void mergeBlockFormat(const QTextBlockFormat &modifier);
    Q_INVOKABLE void mergeCharFormat(RTTextCharFormat *format);
    Q_INVOKABLE bool movePosition(/*QTextCursor::MoveOperation*/int operation, /*QTextCursor::MoveMode*/int mode = QTextCursor::MoveAnchor, int n = 1);
    Q_INVOKABLE void removeSelectedText();
    Q_INVOKABLE void select(int selection);
    Q_INVOKABLE void selectedTableCells(int *firstRow, int *numRows, int *firstColumn, int *numColumns) const;
    Q_INVOKABLE void setBlockCharFormat(RTTextCharFormat *format);
    void setBlockFormat(const QTextBlockFormat &format);
    Q_INVOKABLE void setCharFormat(RTTextCharFormat *format);
    void setKeepPositionOnInsert(bool b);
    Q_INVOKABLE void setPosition(int pos, int m = QTextCursor::MoveAnchor);
    void setVerticalMovementX(int x);
    void setVisualNavigation(bool b);
    void swap(QTextCursor &other);
    Q_INVOKABLE void swap(RTTextCursor &other);

    // Доступ к внутреннему курсору
    QTextCursor textCursor() const;
    void setTextCursor(const QTextCursor &cursor);

signals:
    void cursorChanged();

private:
    QTextCursor m_cursor;
};

#endif // RTTEXTCURSOR_H
