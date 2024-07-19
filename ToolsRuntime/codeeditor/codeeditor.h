#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include "ToolsRuntime_global.h"

class CodeEditorPrivate;
class TOOLSRUNTIME_EXPORT CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    friend class LineNumberArea;
public:
    CodeEditor(QWidget *parent = nullptr);
    virtual ~CodeEditor();

    void setTabStop(int w);

protected:
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
