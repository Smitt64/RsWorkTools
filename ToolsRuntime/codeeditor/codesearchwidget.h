#ifndef CODESEARCHWIDGET_H
#define CODESEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include "ToolsRuntime_global.h"
#include <QTextDocument>

class CodeEditor;

class TOOLSRUNTIME_EXPORT CodeSearchWidget : public QFrame
{
    Q_OBJECT
public:
    explicit CodeSearchWidget(CodeEditor *editor, QWidget *parent = nullptr);
    ~CodeSearchWidget();

    void show();
    void hide();
    void setFocusToSearch();
    void findNext();
    void findPrevious();
    void updatePosition();

signals:
    void visibilityChanged(bool visible);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onSearchTextChanged(const QString &text);
    void onFindNext();
    void onFindPrevious();
    void onToggleCaseSensitive(bool checked);
    void onToggleWholeWord(bool checked);
    void onToggleRegex(bool checked);
    void onClose();
    void onSearchTimeout();

private:
    void performSearch();
    bool findText(const QString &text, QTextDocument::FindFlags flags);
    void updateSearchStatus(int matches, int currentMatch);
    void updateButtonsState();

private:
    CodeEditor *m_editor;

    // UI Components - First row
    QLineEdit *m_searchLineEdit;
    QToolButton *m_findNextButton;
    QToolButton *m_findPreviousButton;
    QToolButton *m_closeButton;
    QHBoxLayout *m_firstRowLayout;

    // UI Components - Second row
    QToolButton *m_caseSensitiveButton;
    QToolButton *m_wholeWordButton;
    QToolButton *m_regexButton;
    QLabel *m_statusLabel;
    QHBoxLayout *m_secondRowLayout;

    // Main layout
    QVBoxLayout *m_mainLayout;

    QTimer *m_searchTimer;
    QString m_lastSearchText;
    QTextDocument::FindFlags m_currentFlags;
    int m_totalMatches;
    int m_currentMatchIndex;
};

#endif // CODESEARCHWIDGET_H