#ifndef APPOPTIONSCONTENTWIDGET_H
#define APPOPTIONSCONTENTWIDGET_H

#include "categorycontentwidget.h"
#include "ToolsRuntime_global.h"

class OptionsPage;
class QSettings;
class StringListEditor;
class AppOptionsContentWidgetPrivate;
class TOOLSRUNTIME_EXPORT AppOptionsContentWidget : public CategoryContentWidget
{
    Q_OBJECT
public:
    AppOptionsContentWidget(QWidget *parent = nullptr);
    virtual ~AppOptionsContentWidget();

    void setSettings(QSettings *pSettings);
    void prepareExec();

    int addPage(const QString &title, const QIcon &icon, OptionsPage *page);
    int addPage(int row, const QString &title, const QIcon &icon, OptionsPage *page);

    int addRslPage(const QIcon &icon);
    int addLogPage(const QIcon &icon, const QString &prefix);

    void deletePage(const int &row);
    QList<OptionsPage*> pages();

    void configureStringListEditor(StringListEditor *_editor);
    void configureStringListEditors(QWidget *container);

private:
    AppOptionsContentWidgetPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(AppOptionsContentWidget);
};

#endif // APPOPTIONSCONTENTWIDGET_H
