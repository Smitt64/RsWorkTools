// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef OPTIONSDLG_H
#define OPTIONSDLG_H

#include <QDialog>
#include "ToolsRuntime_global.h"

namespace Ui {
class OptionsDlg;
}

class QSettings;
class OptionsPage;
class CommandsOptions;
class OptionsDlgPrivate;
class TOOLSRUNTIME_EXPORT OptionsDlg : public QDialog
{
    Q_OBJECT
public:
    explicit OptionsDlg(QSettings *settings, QWidget *parent = nullptr);
    ~OptionsDlg();

    int addStylePage(const QString &group, const QString &key);
    int addCodeEditorPage(const QString &group, const QString &key);
    int addRslPage();
    int addCommandsPage(bool UseRslCommands = true);
    int addCommandsPage(CommandsOptions *page);
    int addLogPage(const QString &prefix);
    void setDefaultStyle(const QString &style);
    int addUpdatePage();
    const QString &defaultStyle() const;

    QSettings *settings();
    OptionsPage *page(const int &index);

    template<class T>OptionsPage *findPage()
    {
        return findPage(&T::staticMetaObject);
    }

public slots:
    virtual int exec() Q_DECL_OVERRIDE;
    virtual void accept() Q_DECL_OVERRIDE;
    int execPage(const int index);

protected:
    int addPage(const QString &title, const QIcon &icon, OptionsPage *page);
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void done(int r) Q_DECL_OVERRIDE;
    OptionsPage *findPage(QMetaObject *obj);

private:
    Ui::OptionsDlg *ui;

    OptionsDlgPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(OptionsDlg);
};

#endif // OPTIONSDLG_H
