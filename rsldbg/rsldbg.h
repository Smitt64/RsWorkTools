#ifndef RSLDBG_H
#define RSLDBG_H

#define __RSLTYPE_H
#include "lf/bp_data.h"
#include "rsldbg_global.h"
#include "rsl/dbgintf.h"
#include <loki/SmartPtr.h>
#include <QObject>
#include <QStack>
#include <QEventLoop>

class CDebug;
typedef Loki::SmartPtr<CDebug, Loki::RefLinked> SpDebugPtrType;
typedef char *(*tRslGetModuleFile)(Qt::HANDLE,int*);

class MainWindow;
class QApplication;
class QLibrary;
class RSLDBG_EXPORT Rsldbg
{
public:
    Rsldbg();
    virtual ~Rsldbg();

    bool init_ui();
    void done_ui();

    Qt::HANDLE activeBeforeDbg();

    int size();
    HDBG init(HRD inst,TDbgIntf * _dbg_ftable);
    void done(HDBG hDBG);
    int DbgBreak(HDBG hDBG, uint32 data);

    void trace(HDBG hinst,const char *str);
    void remModule(HDBG hinst, RSLMODULE hmod);

    tRslGetModuleFile RslGetModuleFile;

private:
    int process(HDBG hDBG, TBpData *data);
    QApplication *m_pApp;
    char **argv;

    bool is_inited, is_ui;
    Qt::HANDLE m_ActiveBeforeDbg;
    QVector<SpDebugPtrType> m_Dbg;

    QEventLoop m_EventLoop;

    QScopedPointer<MainWindow> m_pWndMain;
    QScopedPointer<QLibrary> m_RsLeng;
};

Qt::HANDLE ActiveBeforeDbg();
char *RslGetModuleFile(Qt::HANDLE mod, int*isBtrStream);

#endif // RSLDBG_H
