// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "markdownoptionspage.h"

#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QVBoxLayout>

namespace
{
    struct CssPreset
    {
        QString name;
        QString url;
    };

    const CssPreset CSS_PRESETS[] = {
        {QStringLiteral("GitHub Light (cdnjs)"),
         QStringLiteral("https://cdnjs.cloudflare.com/ajax/libs/github-markdown-css/5.2.0/github-markdown-light.min.css")},
        {QStringLiteral("GitHub Dark (cdnjs)"),
         QStringLiteral("https://cdnjs.cloudflare.com/ajax/libs/github-markdown-css/5.2.0/github-markdown-dark.min.css")},
        {QStringLiteral("GitHub Dark Dimmed (cdnjs)"),
         QStringLiteral("https://cdnjs.cloudflare.com/ajax/libs/github-markdown-css/5.2.0/github-markdown-dark-dimmed.min.css")},
        {QStringLiteral("Custom"), QString()}
    };

    const char *SETTING_RENDERER = "Markdown/renderer";
    const char *SETTING_CSS_URL = "Markdown/cssUrl";
    const char *SETTING_API_VERSION = "Markdown/githubApiVersion";
    const char *SETTING_MODE = "Markdown/githubMode";
}

MarkdownOptionsPage::MarkdownOptionsPage(QWidget *parent)
    : OptionsPage(parent)
    , m_rendererCombo(nullptr)
    , m_cssPresetCombo(nullptr)
    , m_cssUrlEdit(nullptr)
    , m_apiVersionEdit(nullptr)
    , m_modeEdit(nullptr)
    , m_loading(false)
{
    setupUi();
}

void MarkdownOptionsPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);

    // Группа "Рендеринг Markdown"
    QGroupBox *renderGroup = new QGroupBox(tr("Рендеринг Markdown"), this);
    renderGroup->setFlat(true);
    QVBoxLayout *renderLayout = new QVBoxLayout(renderGroup);
    renderLayout->setContentsMargins(12, 12, 12, 12);
    renderLayout->setSpacing(14);

    addRow(renderLayout, tr("Рендерер:"), m_rendererCombo = new QComboBox(this));
    m_rendererCombo->addItem(tr("Локальный Qt"), MarkdownSettings::LocalQt);
    m_rendererCombo->addItem(tr("GitHub API"), MarkdownSettings::GitHubApi);
    connect(m_rendererCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MarkdownOptionsPage::onRendererChanged);

    addRow(renderLayout, tr("GitHub API version:"), m_apiVersionEdit = new QLineEdit(this));
    m_apiVersionEdit->setPlaceholderText(QStringLiteral("2022-11-28"));
    m_apiVersionEdit->setMinimumWidth(250);

    addRow(renderLayout, tr("GitHub mode:"), m_modeEdit = new QLineEdit(this));
    m_modeEdit->setPlaceholderText(QStringLiteral("gfm / markdown"));
    m_modeEdit->setMinimumWidth(250);

    mainLayout->addWidget(renderGroup);

    // Группа "Оформление"
    QGroupBox *styleGroup = new QGroupBox(tr("Оформление"), this);
    styleGroup->setFlat(true);
    QVBoxLayout *styleLayout = new QVBoxLayout(styleGroup);
    styleLayout->setContentsMargins(12, 12, 12, 12);
    styleLayout->setSpacing(14);

    addRow(styleLayout, tr("CSS тема:"), m_cssPresetCombo = new QComboBox(this));
    fillCssPresets();
    connect(m_cssPresetCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MarkdownOptionsPage::onCssPresetChanged);

    addRow(styleLayout, tr("CSS URL:"), m_cssUrlEdit = new QLineEdit(this));
    m_cssUrlEdit->setPlaceholderText(tr("URL файла стилей"));
    m_cssUrlEdit->setMinimumWidth(400);

    mainLayout->addWidget(styleGroup);
    mainLayout->addStretch();
}

void MarkdownOptionsPage::addRow(QVBoxLayout *layout, const QString &labelText, QWidget *widget)
{
    QHBoxLayout *rowLayout = new QHBoxLayout();
    rowLayout->setSpacing(10);

    QLabel *label = new QLabel(labelText, this);
    label->setMinimumWidth(140);
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    widget->setMinimumHeight(28);
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    rowLayout->addWidget(label);
    rowLayout->addWidget(widget);
    rowLayout->addStretch();

    layout->addLayout(rowLayout);
}

void MarkdownOptionsPage::fillCssPresets()
{
    for (const CssPreset &preset : CSS_PRESETS)
        m_cssPresetCombo->addItem(preset.name, preset.url);
}

MarkdownSettings MarkdownOptionsPage::markdownSettings() const
{
    return m_currentSettings;
}

void MarkdownOptionsPage::setMarkdownSettings(const MarkdownSettings &settings)
{
    m_currentSettings = settings;
    writeToWidgets();
}

void MarkdownOptionsPage::readFromWidgets()
{
    m_currentSettings.renderer = m_rendererCombo->currentIndex() == 0
                                     ? MarkdownSettings::LocalQt
                                     : MarkdownSettings::GitHubApi;
    m_currentSettings.cssUrl = m_cssUrlEdit->text();
    m_currentSettings.githubApiVersion = m_apiVersionEdit->text();
    m_currentSettings.githubMode = m_modeEdit->text();
}

void MarkdownOptionsPage::writeToWidgets()
{
    m_loading = true;

    m_rendererCombo->setCurrentIndex(m_currentSettings.renderer == MarkdownSettings::LocalQt ? 0 : 1);
    m_apiVersionEdit->setText(m_currentSettings.githubApiVersion);
    m_modeEdit->setText(m_currentSettings.githubMode);

    bool presetFound = false;
    for (int i = 0; i < m_cssPresetCombo->count(); ++i)
    {
        if (m_cssPresetCombo->itemData(i).toString() == m_currentSettings.cssUrl)
        {
            m_cssPresetCombo->setCurrentIndex(i);
            presetFound = true;
            break;
        }
    }
    if (!presetFound)
        m_cssPresetCombo->setCurrentIndex(m_cssPresetCombo->count() - 1);

    m_cssUrlEdit->setText(m_currentSettings.cssUrl);

    m_loading = false;
    onRendererChanged(m_rendererCombo->currentIndex());
}

int MarkdownOptionsPage::save()
{
    readFromWidgets();

    QSettings *s = settings();
    if (s)
    {
        s->setValue(SETTING_RENDERER, static_cast<int>(m_currentSettings.renderer));
        s->setValue(SETTING_CSS_URL, m_currentSettings.cssUrl);
        s->setValue(SETTING_API_VERSION, m_currentSettings.githubApiVersion);
        s->setValue(SETTING_MODE, m_currentSettings.githubMode);
    }

    emit settingsChanged();
    return true;
}

void MarkdownOptionsPage::restore()
{
    QSettings *s = settings();
    if (s)
    {
        m_currentSettings.renderer = s->value(SETTING_RENDERER, static_cast<int>(MarkdownSettings::GitHubApi)).toInt()
                                     == static_cast<int>(MarkdownSettings::LocalQt)
                                     ? MarkdownSettings::LocalQt
                                     : MarkdownSettings::GitHubApi;
        m_currentSettings.cssUrl = s->value(SETTING_CSS_URL, m_currentSettings.cssUrl).toString();
        m_currentSettings.githubApiVersion = s->value(SETTING_API_VERSION, m_currentSettings.githubApiVersion).toString();
        m_currentSettings.githubMode = s->value(SETTING_MODE, m_currentSettings.githubMode).toString();
    }

    writeToWidgets();
}

void MarkdownOptionsPage::onRendererChanged(int index)
{
    const bool isGitHub = (index == 1);
    m_apiVersionEdit->setEnabled(isGitHub);
    m_modeEdit->setEnabled(isGitHub);
}

void MarkdownOptionsPage::onCssPresetChanged(int index)
{
    if (m_loading)
        return;

    const QString url = m_cssPresetCombo->itemData(index).toString();
    if (!url.isEmpty())
        m_cssUrlEdit->setText(url);
}
