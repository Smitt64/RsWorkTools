#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Windows.h>
#include "rsl/dbgintf.h"
#include <QMainWindow>
//#define _INC_WINDOWS

namespace Ui {
class MainWindow;
}

class CDebug;
class CodeEditor;
class DbgBreakpointEvent;
class DbgEditorLineWidgetProvider;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    CDebug *m_curdbg;

    void UpdateDbgInfo(int index);
    void AddTraceMsg(const QString &str, const int &level);

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    void InitMainToolBar();
    void UpdateStack();
    void UpdateText(RSLMODULE mod, bool notActivModule);
    void UpdateText(int index);
    void ProcessBreakPoint(DbgBreakpointEvent *ev);
    QString ReadTextFileContent(const QString &filename, const QString &encode = QString());
    Ui::MainWindow *ui;

    bool newDbg;

    RSLMODULE m_curModuleInView;
    RSLSTMT m_lastStmt;

    CodeEditor *m_pCodeEditor;
    DbgEditorLineWidgetProvider *m_pCodeEditorProvider;
    Qt::HANDLE m_tracemsg;

    QAction *m_pRunAction;
};

#endif // MAINWINDOW_H
