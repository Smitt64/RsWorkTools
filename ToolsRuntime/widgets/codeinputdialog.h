#ifndef CODEINPUTDIALOG_H
#define CODEINPUTDIALOG_H

#include "ToolsRuntime_global.h"
#include "codeeditor/codehighlighter.h"
#include <QDialog>

class CodeEditor;
class TOOLSRUNTIME_EXPORT CodeInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CodeInputDialog(QWidget *parent = nullptr,
                             const QString &title = tr("Ввод текста"),
                             const QString &label = tr("Текст:"),
                             const QString &text = QString(),
                             HighlighterSyntax syntax = HighlighterPlain);

    static QString getCodeText(QWidget *parent = nullptr,
                               const QString &title = tr("Ввод текста"),
                               const QString &label = tr("Текст:"),
                               const QString &text = QString(),
                               HighlighterSyntax syntax = HighlighterPlain,
                               bool *ok = nullptr);

    QString text() const;

private:
    CodeEditor *m_editor;
};

#endif // CODEINPUTDIALOG_H
