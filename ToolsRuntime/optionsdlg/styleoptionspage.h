// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef STYLEOPTIONSPAGE_H
#define STYLEOPTIONSPAGE_H

#include "optionsdlg/OptionsPage.h"
#include <QWidget>

namespace Ui {
class StyleOptionsPage;
}

class StyleOptionsPagePrivate;
class StyleOptionsPage : public OptionsPage
{
    Q_OBJECT

public:
    explicit StyleOptionsPage(QWidget *parent = nullptr);
    ~StyleOptionsPage();

    void setSettingsKey(const QString &group, const QString &key);

    virtual int save() Q_DECL_OVERRIDE;
    virtual void restore() Q_DECL_OVERRIDE;

    QString currentStyle() const;

private:
    Ui::StyleOptionsPage *ui;

    StyleOptionsPagePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(StyleOptionsPage);
};

#endif // STYLEOPTIONSPAGE_H
