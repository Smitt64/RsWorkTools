#ifndef MARKDOWNSEARCHWIDGET_H
#define MARKDOWNSEARCHWIDGET_H

#include <QFrame>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QWebEnginePage>

class MarkdownView;

class MarkdownSearchWidget : public QFrame
{
    Q_OBJECT
public:
    explicit MarkdownSearchWidget(MarkdownView *view, QWidget *parent = nullptr);
    ~MarkdownSearchWidget();

    void setSearchText(const QString &text);
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
    void onClose();
    void onSearchTimeout();

private:
    void performSearch();
    void updateSearchStatus(bool found);
    void updateButtonsState();
    QWebEnginePage::FindFlags currentFlags() const;

private:
    MarkdownView *m_view;

    QLineEdit *m_searchLineEdit;
    QToolButton *m_findNextButton;
    QToolButton *m_findPreviousButton;
    QToolButton *m_closeButton;
    QToolButton *m_caseSensitiveButton;
    QLabel *m_statusLabel;

    QHBoxLayout *m_firstRowLayout;
    QHBoxLayout *m_secondRowLayout;
    QVBoxLayout *m_mainLayout;

    QTimer *m_searchTimer;
    QString m_lastSearchText;
    bool m_caseSensitive;
};

#endif // MARKDOWNSEARCHWIDGET_H
