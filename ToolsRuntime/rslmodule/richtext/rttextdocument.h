#ifndef RTTEXTDOCUMENT_H
#define RTTEXTDOCUMENT_H

#include <QTextDocument>
#include <QObject>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextFrame>
#include <QTextOption>
#include <QFont>
#include <QUrl>
#include <QSizeF>
#include <QPagedPaintDevice>

class RTTextCursor;
class RTTextDocument : public QObject
{
    Q_OBJECT

    // Свойства для скалярных типов без параметров
    Q_PROPERTY(int availableRedoSteps READ availableRedoSteps NOTIFY documentChanged)
    Q_PROPERTY(int availableUndoSteps READ availableUndoSteps NOTIFY documentChanged)
    //Q_PROPERTY(QUrl baseUrl READ baseUrl WRITE setBaseUrl NOTIFY documentChanged)
    Q_PROPERTY(int blockCount READ blockCount NOTIFY documentChanged)
    Q_PROPERTY(int characterCount READ characterCount NOTIFY documentChanged)
    Q_PROPERTY(int defaultCursorMoveStyle READ defaultCursorMoveStyle WRITE setDefaultCursorMoveStyle NOTIFY documentChanged)
    //Q_PROPERTY(QFont defaultFont READ defaultFont WRITE setDefaultFont NOTIFY documentChanged)
    Q_PROPERTY(QString defaultStyleSheet READ defaultStyleSheet WRITE setDefaultStyleSheet NOTIFY documentChanged)
    //Q_PROPERTY(QTextOption defaultTextOption READ defaultTextOption WRITE setDefaultTextOption NOTIFY documentChanged)
    //Q_PROPERTY(QAbstractTextDocumentLayout* documentLayout READ documentLayout WRITE setDocumentLayout NOTIFY documentChanged)
    Q_PROPERTY(qreal documentMargin READ documentMargin WRITE setDocumentMargin NOTIFY documentChanged)
    Q_PROPERTY(qreal idealWidth READ idealWidth NOTIFY documentChanged)
    Q_PROPERTY(qreal indentWidth READ indentWidth WRITE setIndentWidth NOTIFY documentChanged)
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY documentChanged)
    Q_PROPERTY(bool isModified READ isModified NOTIFY documentChanged)
    Q_PROPERTY(bool isRedoAvailable READ isRedoAvailable NOTIFY documentChanged)
    Q_PROPERTY(bool isUndoAvailable READ isUndoAvailable NOTIFY documentChanged)
    Q_PROPERTY(bool isUndoRedoEnabled READ isUndoRedoEnabled WRITE setUndoRedoEnabled NOTIFY documentChanged)
    Q_PROPERTY(int lineCount READ lineCount NOTIFY documentChanged)
    Q_PROPERTY(int maximumBlockCount READ maximumBlockCount WRITE setMaximumBlockCount NOTIFY documentChanged)
    Q_PROPERTY(int pageCount READ pageCount NOTIFY documentChanged)
    //Q_PROPERTY(QSizeF pageSize READ pageSize WRITE setPageSize NOTIFY documentChanged)
    Q_PROPERTY(int revision READ revision NOTIFY documentChanged)
    //Q_PROPERTY(QTextFrame* rootFrame READ rootFrame NOTIFY documentChanged)
    //Q_PROPERTY(QSizeF size READ size NOTIFY documentChanged)
    Q_PROPERTY(qreal textWidth READ textWidth WRITE setTextWidth NOTIFY documentChanged)
    Q_PROPERTY(bool useDesignMetrics READ useDesignMetrics WRITE setUseDesignMetrics NOTIFY documentChanged)

public:
    Q_INVOKABLE RTTextDocument(QObject *parent = nullptr);
    Q_INVOKABLE RTTextDocument(const QString &text, QObject *parent = nullptr);
    RTTextDocument(const RTTextDocument &other);
    ~RTTextDocument();

    // Оператор присваивания
    RTTextDocument &operator=(const RTTextDocument &other);

    // Методы доступа к внутреннему документу
    QTextDocument *textDocument() const;
    void setTextDocument(QTextDocument *document);

    // Свойства-геттеры
    int availableRedoSteps() const;
    int availableUndoSteps() const;
    QUrl baseUrl() const;
    QTextBlock begin() const;
    int blockCount() const;
    QChar characterAt(int pos) const;
    int characterCount() const;
    int defaultCursorMoveStyle() const;
    QFont defaultFont() const;
    QString defaultStyleSheet() const;
    QTextOption defaultTextOption() const;
    QAbstractTextDocumentLayout* documentLayout() const;
    qreal documentMargin() const;
    QTextBlock end() const;
    QTextBlock findBlock(int pos) const;
    QTextBlock findBlockByLineNumber(int lineNumber) const;
    QTextBlock findBlockByNumber(int blockNumber) const;
    QTextBlock firstBlock() const;
    qreal idealWidth() const;
    qreal indentWidth() const;
    bool isEmpty() const;
    bool isModified() const;
    bool isRedoAvailable() const;
    bool isUndoAvailable() const;
    bool isUndoRedoEnabled() const;
    QTextBlock lastBlock() const;
    int lineCount() const;
    int maximumBlockCount() const;
    Q_INVOKABLE QString metaInformation(int info) const;
    QTextObject *object(int objectIndex) const;
    QTextObject *objectForFormat(const QTextFormat &f) const;
    int pageCount() const;
    QSizeF pageSize() const;
    QVariant resource(int type, const QUrl &name) const;
    int revision() const;
    QTextFrame *rootFrame() const;
    QSizeF size() const;
    qreal textWidth() const;
    Q_INVOKABLE QString toHtml(const QByteArray &encoding = QByteArray()) const;
    Q_INVOKABLE QString toMarkdown(int features = 0) const;
    Q_INVOKABLE QString toPlainText() const;
    Q_INVOKABLE QString toRawText() const;
    bool useDesignMetrics() const;

    // Свойства-сеттеры
    void setBaseUrl(const QUrl &url);
    void setDefaultCursorMoveStyle(int style);
    void setDefaultFont(const QFont &font);
    void setDefaultStyleSheet(const QString &sheet);
    void setDefaultTextOption(const QTextOption &option);
    void setDocumentLayout(QAbstractTextDocumentLayout *layout);
    void setDocumentMargin(qreal margin);
    void setHtml(const QString &html);
    void setIndentWidth(qreal width);
    void setMarkdown(const QString &markdown, int features = 0);
    void setMaximumBlockCount(int maximum);
    void setMetaInformation(int info, const QString &string);
    void setPageSize(const QSizeF &size);
    void setPlainText(const QString &text);
    void setTextWidth(qreal width);
    void setUndoRedoEnabled(bool enable);
    void setUseDesignMetrics(bool b);

    // Методы с параметрами
    void addResource(int type, const QUrl &name, const QVariant &resource);
    void adjustSize();
    QVector<QTextFormat> allFormats() const;
    Q_INVOKABLE void clear();
    Q_INVOKABLE void clearUndoRedoStacks(int stacksToClear = 0);
    RTTextDocument *clone(QObject *parent = nullptr) const;
    void drawContents(QPainter *p, const QRectF &rect = QRectF());
    RTTextCursor *find(const QString &subString, RTTextCursor *cursor, int options = 0) const;
    RTTextCursor *find(const QString &subString, int position = 0, int options = 0) const;
    Q_INVOKABLE void markContentsDirty(int position, int length);
    void print(QPagedPaintDevice *printer) const;
    void redo(QTextCursor *cursor);
    void undo(QTextCursor *cursor);

signals:
    void documentChanged();
    void contentsChanged();
    void contentsChange(int position, int charsRemoved, int charsAdded);
    void undoCommandAdded();

private:
    QTextDocument *m_document;
};

#endif // RTTEXTDOCUMENT_H
