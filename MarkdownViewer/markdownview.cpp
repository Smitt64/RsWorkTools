// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "markdownview.h"
#include "markdownsearchwidget.h"

#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeData>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslSocket>
#include <QTextDocument>
#include <QTextStream>
#include <QDateTime>
#include <QUrl>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QPageLayout>

namespace
{
    const char *GITHUB_MARKDOWN_API_URL = "https://api.github.com/markdown";

    QString wrapHtml(const QString &body, const QString &title, const QString &cssUrl)
    {
        QString cssLink;
        if (!cssUrl.isEmpty())
        {
            cssLink = QStringLiteral("<link rel=\"stylesheet\" href=\"%1\"/>").arg(cssUrl);
        }

        QString html;
        html.reserve(1024 + body.length() + cssLink.length());
        html += QStringLiteral("<!DOCTYPE html><html><head><meta charset=\"utf-8\"/><title>");
        html += title.toHtmlEscaped();
        html += QStringLiteral("</title>");
        html += cssLink;
        html += QStringLiteral("<style>"
                               "body { margin: 0; background-color: #ffffff; }"
                               ".markdown-body { box-sizing: border-box; min-width: 200px; max-width: 980px; margin: 0 auto; padding: 45px; }"
                               "@media (max-width: 767px) { .markdown-body { padding: 15px; } }"
                               "</style>"
                               "</head>"
                               "<body>"
                               "<div class=\"markdown-body\">");
        html += body;
        html += QStringLiteral("</div></body></html>");
        return html;
    }

    QString loadingHtml(const QString &message)
    {
        return QStringLiteral(
            "<!DOCTYPE html>"
            "<html>"
            "<head><meta charset=\"utf-8\"/>"
            "<style>"
            "html, body { margin: 0; height: 100%; background-color: #ffffff;"
            "  display: flex; align-items: center; justify-content: center;"
            "  font-family: Segoe UI, Arial, sans-serif; font-size: 16px; color: #5B5FC7; }"
            ".container { text-align: center; }"
            ".spinner { width: 48px; height: 48px; margin: 0 auto 16px;"
            "  border: 5px solid #e6e6fa; border-top-color: #5B5FC7; border-radius: 50%;"
            "  animation: spin 1s linear infinite; }"
            "@keyframes spin { to { transform: rotate(360deg); } }"
            "</style>"
            "</head>"
            "<body>"
            "<div class=\"container\">"
            "<div class=\"spinner\"></div>"
            "<div>%1</div>"
            "</div>"
            "</body>"
            "</html>").arg(message.toHtmlEscaped());
    }
}

MarkdownView::MarkdownView(QWidget *parent)
    : QWidget(parent)
    , m_webView(nullptr)
    , m_searchWidget(nullptr)
    , m_networkManager(nullptr)
    , m_currentReply(nullptr)
{
    setupUi();
}

void MarkdownView::setupUi()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_webView = new QWebEngineView(this);
    m_webView->installEventFilter(this);
    layout->addWidget(m_webView);
    setLayout(layout);

    m_searchWidget = new MarkdownSearchWidget(this, this);

    m_networkManager = new QNetworkAccessManager(this);
}

QString MarkdownView::renderingHtml() const
{
    return loadingHtml(tr("Рендеринг..."));
}

QWebEngineView *MarkdownView::webView() const
{
    return m_webView;
}

void MarkdownView::showSearchWidget()
{
    if (!m_searchWidget)
        return;

    m_searchWidget->show();
}

void MarkdownView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (m_searchWidget)
        m_searchWidget->updatePosition();
}

bool MarkdownView::loadFromFile(const QString &filePath)
{
    if (filePath.isEmpty())
        return false;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Cannot open markdown file:" << filePath << file.errorString();
        return false;
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");

    QString text = stream.readAll();
    m_filePath = filePath;
    renderMarkdown(text);
    return true;
}

void MarkdownView::renderMarkdown(const QString &text)
{
    m_markdownSource = text;

    // Показываем страницу ожидания до завершения рендера
    if (m_webView)
        m_webView->setHtml(renderingHtml(), QUrl::fromLocalFile(m_filePath));

    qDebug() << "SSL diagnostics:"
             << "  supportsSsl()      =" << QSslSocket::supportsSsl()
             << "  buildVersionString =" << QSslSocket::sslLibraryBuildVersionString()
             << "  runtimeVersion     =" << QSslSocket::sslLibraryVersionString();

    if (m_settings.renderer == MarkdownSettings::LocalQt)
    {
        renderMarkdownLocal(text);
        return;
    }

    // Отменяем предыдущий запрос, если он ещё выполняется
    if (m_currentReply)
    {
        disconnect(m_currentReply, &QNetworkReply::finished, this, &MarkdownView::onRenderFinished);
        m_currentReply->abort();
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
    }

    QUrl url(QString::fromLatin1(GITHUB_MARKDOWN_API_URL));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/vnd.github+json");
    request.setRawHeader("X-GitHub-Api-Version", m_settings.githubApiVersion.toUtf8());
    request.setRawHeader("User-Agent", "MarkdownViewer-Qt-App");

    QJsonObject body;
    body[QStringLiteral("text")] = text;
    body[QStringLiteral("mode")] = m_settings.githubMode;

    const QByteArray payload = QJsonDocument(body).toJson(QJsonDocument::Compact);
    qDebug() << "Sending markdown to GitHub API, size:" << payload.size();

    m_currentReply = m_networkManager->post(request, payload);
    connect(m_currentReply, &QNetworkReply::finished, this, &MarkdownView::onRenderFinished);
}

void MarkdownView::onRenderFinished()
{
    if (!m_currentReply)
        return;

    QNetworkReply *reply = m_currentReply;
    m_currentReply = nullptr;

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "GitHub API request failed:" << reply->errorString();
        renderMarkdownLocal(m_markdownSource);
        return;
    }

    const QByteArray htmlData = reply->readAll();
    const QString html = QString::fromUtf8(htmlData);

    if (html.isEmpty())
    {
        qWarning() << "GitHub API returned empty HTML, falling back to local renderer";
        renderMarkdownLocal(m_markdownSource);
        return;
    }

    setHtmlContent(html, QFileInfo(m_filePath).fileName());
}

void MarkdownView::renderMarkdownLocal(const QString &text)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QTextDocument doc;
    doc.setMarkdown(text, QTextDocument::MarkdownDialectGitHub);
    setHtmlContent(doc.toHtml(), QFileInfo(m_filePath).fileName());
#else
    setHtmlContent(QStringLiteral("<pre>%1</pre>").arg(text.toHtmlEscaped()), QFileInfo(m_filePath).fileName());
#endif
}

void MarkdownView::setHtmlContent(const QString &bodyHtml, const QString &title)
{
    // Сохраняем полученный HTML в промежуточный файл для отладки
    QDir debugDir(QDir::temp().filePath("MarkdownViewerDebug"));
    if (!debugDir.exists())
        debugDir.mkpath(".");

    const QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
    const QString baseName = title.isEmpty() ? QStringLiteral("untitled") : QFileInfo(title).baseName();
    const QString debugFileName = QStringLiteral("%1_%2.html").arg(baseName, timestamp);
    const QString debugFilePath = debugDir.absoluteFilePath(debugFileName);

    QFile debugFile(debugFilePath);
    if (debugFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&debugFile);
        stream.setCodec("UTF-8");
        stream << bodyHtml;
        debugFile.close();
        qDebug() << "Debug HTML saved to:" << debugFilePath;
    }
    else
    {
        qWarning() << "Cannot save debug HTML to:" << debugFilePath;
    }

    m_lastHtml = wrapHtml(bodyHtml, title.isEmpty() ? tr("Markdown Viewer") : title, m_settings.cssUrl);
    m_webView->setHtml(m_lastHtml, QUrl::fromLocalFile(m_filePath));
}

void MarkdownView::setSettings(const MarkdownSettings &settings)
{
    m_settings = settings;
}

MarkdownSettings MarkdownView::settings() const
{
    return m_settings;
}

void MarkdownView::rerender()
{
    if (!m_markdownSource.isEmpty())
        renderMarkdown(m_markdownSource);
}

void MarkdownView::scrollToLine(int lineNumber)
{
    if (!m_webView || !m_webView->page())
        return;

    const QStringList lines = m_markdownSource.split('\n');
    const int total = lines.size();
    if (total <= 1)
        return;

    const int clampedLine = qBound(0, lineNumber, total - 1);
    const double ratio = static_cast<double>(clampedLine) / (total - 1);

    const QString script = QStringLiteral("window.scrollTo(0, document.body.scrollHeight * %1);")
                               .arg(ratio, 0, 'f', 6);
    m_webView->page()->runJavaScript(script);
}

bool MarkdownView::saveToFile(const QString &filePath)
{
    const QString targetPath = filePath.isEmpty() ? m_filePath : filePath;
    if (targetPath.isEmpty())
        return false;

    QFile file(targetPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Cannot save markdown file:" << targetPath << file.errorString();
        return false;
    }

    file.write(m_markdownSource.toUtf8());
    file.close();

    m_filePath = targetPath;
    return true;
}

bool MarkdownView::saveAsHtml(const QString &filePath)
{
    ensureHtmlRendered();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Cannot save HTML file:" << filePath << file.errorString();
        return false;
    }

    file.write(m_lastHtml.toUtf8());
    file.close();
    return true;
}

bool MarkdownView::saveAsPdf(const QString &filePath)
{
    ensureHtmlRendered();

    if (!m_webView || !m_webView->page())
        return false;

    m_webView->page()->printToPdf(filePath);
    return true;
}

void MarkdownView::ensureHtmlRendered()
{
    if (!m_lastHtml.isEmpty())
        return;

    if (m_markdownSource.isEmpty())
        setHtmlContent(QString(), QFileInfo(m_filePath).fileName());
    else
        renderMarkdownLocal(m_markdownSource);
}

QString MarkdownView::filePath() const
{
    return m_filePath;
}

void MarkdownView::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}

QString MarkdownView::documentTitle() const
{
    const QStringList lines = m_markdownSource.split('\n');
    for (const QString &line : lines)
    {
        const QString trimmed = line.trimmed();
        if (trimmed.startsWith("# "))
            return trimmed.mid(2).trimmed();
    }

    if (!m_filePath.isEmpty())
        return QFileInfo(m_filePath).fileName();

    return QString();
}

bool MarkdownView::isModified() const
{
    return false;
}

QString MarkdownView::markdownSource() const
{
    return m_markdownSource;
}

bool MarkdownView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_webView)
    {
        switch (event->type())
        {
        case QEvent::DragEnter:
        {
            QDragEnterEvent *dragEvent = static_cast<QDragEnterEvent *>(event);
            if (dragEvent->mimeData()->hasUrls())
            {
                dragEvent->acceptProposedAction();
                return true;
            }
            break;
        }
        case QEvent::DragMove:
        {
            QDragMoveEvent *moveEvent = static_cast<QDragMoveEvent *>(event);
            if (moveEvent->mimeData()->hasUrls())
            {
                moveEvent->acceptProposedAction();
                return true;
            }
            break;
        }
        case QEvent::Drop:
        {
            QDropEvent *dropEvent = static_cast<QDropEvent *>(event);
            handleDropEvent(dropEvent);
            return true;
        }
        default:
            break;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void MarkdownView::handleDropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (!mimeData->hasUrls())
    {
        event->ignore();
        return;
    }

    event->acceptProposedAction();

    const QList<QUrl> urls = mimeData->urls();
    for (const QUrl &url : urls)
    {
        if (!url.isLocalFile())
            continue;

        const QString filePath = url.toLocalFile();
        if (filePath.isEmpty())
            continue;

        emit fileDropped(filePath);
    }
}
