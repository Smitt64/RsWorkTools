// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SARibbon.h"
#include <QSettings>

class ApplicationWidgetBase;
class SARibbonActionsManager;
class RslExecutor;

class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private:
    void initRibbon();
    void initApplicationWidget();
    void loadMacrosAndApplyCustomization();

    ApplicationWidgetBase *m_appWidget;
    SARibbonActionsManager *m_actionsManager;
    RslExecutor *m_executor;
    QSettings *m_settings;
    bool m_hasApplyCustomizeXml;
};

#endif // MAINWINDOW_H
