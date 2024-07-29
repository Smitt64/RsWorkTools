#ifndef CODEEDITOROPTIONSPAGE_H
#define CODEEDITOROPTIONSPAGE_H

#include "OptionsPage.h"
#include <QWidget>

namespace Ui {
class CodeEditorOptionsPage;
}

class CodeEditorOptionsPagePrivate;
class CodeEditorOptionsPage : public OptionsPage
{
    Q_OBJECT

public:
    explicit CodeEditorOptionsPage(QWidget *parent = nullptr);
    ~CodeEditorOptionsPage();

    void setSettingsKey(const QString &group, const QString &key);

    virtual int save();
    virtual void restore();

private:
    Ui::CodeEditorOptionsPage *ui;

    CodeEditorOptionsPagePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(CodeEditorOptionsPage);
};

#endif // CODEEDITOROPTIONSPAGE_H
