// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SARibbon.h"
#include "markdownsettings.h"

#include <QMainWindow>

class QMdiArea;
class QMdiSubWindow;
class ApplicationWidgetBase;
class MarkdownOptionsPage;
class MarkdownView;
class QSettings;
class QDragEnterEvent;
class QDropEvent;
class QPoint;
class QDockWidget;
class QTreeWidget;
class QTreeWidgetItem;
class QStatusBar;

class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void openFile(const QString &filePath);

private slots:
    void onOpenDocument();
    void onSaveDocument();
    void onCloseActiveDocument();
    void onCloseAllDocuments();
    void onToggleFullscreen();
    void onAbout();
    void onSettingsChanged();
    void onTabContextMenuRequested(const QPoint &pos);
    void onOutlineItemClicked(QTreeWidgetItem *item, int column);
    void onFindInPage();

private:
    void initRibbon();
    void initMdiArea();
    void initApplicationWidget();
    void initWindowButtonBar();
    void initOutlineDock();
    void initStatusBar();
    void updateOutline();
    void updateStatusBar();
    void loadSettings();
    void saveSettings();
    void applySettingsToAllViews();

    void showEvent(QShowEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    MarkdownView *createMarkdownView(const QString &filePath = QString());
    QMdiSubWindow *findSubWindow(const QString &filePath) const;

    QMdiArea *m_mdiArea;
    ApplicationWidgetBase *m_appWidget;
    MarkdownOptionsPage *m_optionsPage;
    QSettings *m_settings;
    QDockWidget *m_outlineDock;
    QTreeWidget *m_outlineTree;
    QStatusBar *m_statusBar;
    MarkdownSettings m_markdownSettings;
};

#endif // MAINWINDOW_H
