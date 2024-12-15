#ifndef RSLDBG_H
#define RSLDBG_H

#include "lf/bp_data.h"
#include "rsldbg_global.h"
#include "rsl/dbgintf.h"
#include <loki/SmartPtr.h>
#include <QObject>
#include <QStack>
#include <QEventLoop>

class CDebug;
typedef Loki::SmartPtr<CDebug, Loki::RefLinked> SpDebugPtrType;

class MainWindow;
class QApplication;
class RSLDBG_EXPORT Rsldbg
{
public:
    Rsldbg();
    virtual ~Rsldbg();

    bool init_ui();
    void done_ui();

    int size();
    HDBG init(HRD inst,TDbgIntf * _dbg_ftable);
    void done(HDBG hDBG);
    int DbgBreak(HDBG hDBG, uint32 data);

    void trace(HDBG hinst,const char *str);
    void remModule(HDBG hinst, RSLMODULE hmod);

private:
    int process(HDBG hDBG, TBpData *data);
    QScopedPointer<QApplication> m_pApp;
    char **argv;

    bool is_inited, is_ui;
    Qt::HANDLE m_ActiveBeforeDbg;
    QVector<SpDebugPtrType> m_Dbg;

    QEventLoop m_EventLoop;

    QScopedPointer<MainWindow> m_pWndMain;
};

#endif // RSLDBG_H
