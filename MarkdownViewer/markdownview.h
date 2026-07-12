// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef MARKDOWNVIEW_H
#define MARKDOWNVIEW_H

#include "markdownsettings.h"
#include <QWidget>

class QNetworkAccessManager;
class QNetworkReply;
class QWebEngineView;
class MarkdownSearchWidget;
class QResizeEvent;

class MarkdownView : public QWidget
{
    Q_OBJECT
public:
    explicit MarkdownView(QWidget *parent = nullptr);

    bool loadFromFile(const QString &filePath);
    bool saveToFile(const QString &filePath = QString());
    bool saveAsHtml(const QString &filePath);
    bool saveAsPdf(const QString &filePath);

    QString filePath() const;
    void setFilePath(const QString &filePath);

    QString documentTitle() const;
    bool isModified() const;

    QString markdownSource() const;

    void setSettings(const MarkdownSettings &settings);
    MarkdownSettings settings() const;

    void rerender();
    void scrollToLine(int lineNumber);

    QWebEngineView *webView() const;
    void showSearchWidget();

signals:
    void fileDropped(const QString &filePath);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onRenderFinished();

private:
    void setupUi();
    void handleDropEvent(QDropEvent *event);
    void renderMarkdown(const QString &text);
    void renderMarkdownLocal(const QString &text);
    void setHtmlContent(const QString &bodyHtml, const QString &title);
    void ensureHtmlRendered();
    QString renderingHtml() const;

    QWebEngineView *m_webView;
    MarkdownSearchWidget *m_searchWidget;
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_currentReply;
    QString m_filePath;
    QString m_markdownSource;
    QString m_lastHtml;
    MarkdownSettings m_settings;
};

#endif // MARKDOWNVIEW_H
