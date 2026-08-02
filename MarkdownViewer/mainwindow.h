// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SARibbon.h"
#include "markdownsettings.h"

#include <QMainWindow>
#include <QStringList>

// Ключ локального сервера single-instance: второй процесс передаёт пути
// к файлам уже запущенному экземпляру через QLocalSocket (см. main.cpp)
inline const QString kSingleInstanceKey = QStringLiteral("RsWorkTools.MarkdownViewer.SingleInstance");

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
class QComboBox;
class QMenu;
class QLocalServer;

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
    void onReloadDocument();
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
    void initQuickAccessBar();
    void initOutlineDock();
    void initStatusBar();
    void initSingleInstanceServer();
    void updateOutline();
    void updateStatusBar();
    void refreshWindowsCombo();
    void rebuildRecentFilesMenu();
    void addToRecentFiles(const QString &filePath);
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
    QComboBox *m_windowsCombo;
    QMenu *m_recentFilesMenu;
    QStringList m_recentFiles;
    QLocalServer *m_singleInstanceServer;
    MarkdownSettings m_markdownSettings;
};

#endif // MAINWINDOW_H
