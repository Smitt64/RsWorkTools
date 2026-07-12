// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef MARKDOWNOPTIONSPAGE_H
#define MARKDOWNOPTIONSPAGE_H

#include "optionsdlg/OptionsPage.h"
#include "markdownsettings.h"

class QComboBox;
class QLineEdit;
class QVBoxLayout;

class MarkdownOptionsPage : public OptionsPage
{
    Q_OBJECT
public:
    explicit MarkdownOptionsPage(QWidget *parent = nullptr);

    MarkdownSettings markdownSettings() const;
    void setMarkdownSettings(const MarkdownSettings &settings);

    int save() override;
    void restore() override;

signals:
    void settingsChanged();

private slots:
    void onRendererChanged(int index);
    void onCssPresetChanged(int index);

private:
    void setupUi();
    void fillCssPresets();
    void addRow(QVBoxLayout *layout, const QString &labelText, QWidget *widget);
    void readFromWidgets();
    void writeToWidgets();

    QComboBox *m_rendererCombo;
    QComboBox *m_cssPresetCombo;
    QLineEdit *m_cssUrlEdit;
    QLineEdit *m_apiVersionEdit;
    QLineEdit *m_modeEdit;

    MarkdownSettings m_currentSettings;
    bool m_loading;
};

#endif // MARKDOWNOPTIONSPAGE_H
