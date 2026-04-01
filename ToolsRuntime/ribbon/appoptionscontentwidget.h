#ifndef APPOPTIONSCONTENTWIDGET_H
#define APPOPTIONSCONTENTWIDGET_H

#include "categorycontentwidget.h"
#include "ToolsRuntime_global.h"

class OptionsPage;
class QSettings;
class AppOptionsContentWidgetPrivate;
class TOOLSRUNTIME_EXPORT AppOptionsContentWidget : public CategoryContentWidget
{
    Q_OBJECT
public:
    AppOptionsContentWidget(QWidget *parent = nullptr);
    virtual ~AppOptionsContentWidget();

    void setSettings(QSettings *pSettings);

    int addPage(const QString &title, const QIcon &icon, OptionsPage *page);
    int addRslPage(const QIcon &icon);
    int addLogPage(const QIcon &icon, const QString &prefix);

private:
    AppOptionsContentWidgetPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(AppOptionsContentWidget);
};

#endif // APPOPTIONSCONTENTWIDGET_H
