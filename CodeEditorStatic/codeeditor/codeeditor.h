// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include "ToolsRuntime_global.h"

class CodeHighlighter;
class CodeEditorPrivate;
class TOOLSRUNTIME_EXPORT CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    friend class LineNumberArea;
    friend class CodeEditorOptionsPage;
    friend TOOLSRUNTIME_EXPORT void ToolApplyHighlighter(QPlainTextEdit *pEditor,
                                     const int &syntax,
                                     const QString &style);
public:
    CodeEditor(QWidget *parent = nullptr);
    virtual ~CodeEditor();

    void setTabStop(int w);
    void setCurrentLineColor(const QColor &color);
    void setCurrentWordColor(const QColor &color);

    CodeHighlighter *highlighter();
    void setStyle(const QString &style);
    void rehighlight();

protected:
    void setCodeHighlighter(CodeHighlighter *highlighter);
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

signals:
    void currentLineChanged(const int &last, const int &cur);

private:
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    CodeEditorPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(CodeEditor);
};

#endif // CODEEDITOR_H
