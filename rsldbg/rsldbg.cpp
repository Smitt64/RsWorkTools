#include "cdebug.h"
#include "rsldbg.h"
#include "rsl/dbgintf.h"
#include "saver.h"
#include "ui/events/dbgbreakpointevent.h"
#include "ui/events/dbgtraceevent.h"
#include "ui/mainwindow.h"
#include "cqsurvey.h"
#include "csurvey.h"
#include "clocals.h"
#include "cwatchv.h"
#include <windows.h>
#include <QApplication>

Q_GLOBAL_STATIC(Rsldbg, rsldbg)

Rsldbg::Rsldbg() :
    m_pApp(nullptr),
    argv(nullptr),
    is_inited(false),
    is_ui(false)
{
    QStringList arguments = QApplication::arguments();

    int argc = arguments.count();

    argv = new char*[argc];
    for (int i = 0; i < argc; i++)
    {
        QString element = arguments.at(i);
        argv[i] = new char[element.size() + 1];
        strcpy(argv[i], element.toStdString().c_str());
    }

    if (qApp)
        m_pApp = qApp;
    else
        m_pApp = new QApplication(argc, argv);
    //m_pApp.reset();
}

Rsldbg::~Rsldbg()
{
    if (m_pApp)
        delete m_pApp;

    for (int i = 0; i < m_pApp->arguments().count(); i++)
        delete[] argv[i];

    delete[] argv;
}

int Rsldbg::size()
{
    return m_Dbg.size();
}

HDBG Rsldbg::init(HRD inst,TDbgIntf * _dbg_ftable)
{
    if(!is_ui)
    {
        if(!init_ui())
            return NULL;

        is_ui = true;
    }

    QVector<SpDebugPtrType>::iterator i;
    for(i = m_Dbg.begin(); i != m_Dbg.end(); ++i)
    {
        if ((*i)->GetInst() == inst)
            return (HDBG)GetImpl (*i);
    }

    CDebug* new_dbg = new CDebug(inst, _dbg_ftable);
    SpDebugPtrType new_dbg_aptr (new_dbg);
    m_Dbg.push_back (new_dbg_aptr);

    return (HDBG)new_dbg;
}

void Rsldbg::done(HDBG hDBG)
{
    if (m_Dbg.size() == 1)
    {
        is_ui = false;

        if (m_pWndMain)
        {
            m_pWndMain->close();
            m_pWndMain.reset();
        }

        CDebug* aDBG = (CDebug*)hDBG;
        QVector<SpDebugPtrType>::iterator i;
        for(i = m_Dbg.begin(); i != m_Dbg.end(); ++i)
        {
            if ((*i)->GetInst() == aDBG->GetInst())
            {
                m_Dbg.erase(i);
                break;
            }
        }

        done_ui();
    }
    else if (m_Dbg.size() > 1)
    {
        CDebug* aDBG = (CDebug* )hDBG;
        QVector<SpDebugPtrType>::iterator i;

        int ctr = 0;
        int cur_ctr = 0;

        for(i = m_Dbg.begin(); i != m_Dbg.end(); ++i)
        {
            if ((*i)->GetInst() == aDBG->GetInst())
            {
                if(m_pWndMain->m_curdbg == GetImpl(m_Dbg[0]))
                {
                    if(cur_ctr != 1){
                        ctr = 1;
                    } else {
                        ctr = cur_ctr - 1;
                    }
                }
                m_pWndMain->m_curdbg = GetImpl(m_Dbg[ctr]);
                m_Dbg.erase(i);
                break;
            }
            ++cur_ctr;
        }
    }
}

int Rsldbg::DbgBreak(HDBG hDBG, uint32 data)
{
    CDebug* aDBG = (CDebug* )hDBG;

    TBpData *bp = NULL;
    if (data)
    {
        bp = aDBG->FindBPbyKey (data);
        if (bp && bp->bp_type == BP_DISABLED)
            return FALSE;
    }

    return process(hDBG, bp);
}

int Rsldbg::process(HDBG hDBG, TBpData *data)
{
    m_pWndMain->showMaximized();

    DbgBreakpointEvent event((CDebug*)hDBG, data);
    QApplication::sendEvent(m_pWndMain.data(), &event);

    return m_EventLoop.exec();
}

void Rsldbg::trace(HDBG hinst,const char *str)
{
    CDebug* pDebug = (CDebug* )hinst;

    DbgTraceEvent event(pDebug);
    QApplication::sendEvent(m_pWndMain.data(), &event);
    m_pWndMain->AddTraceMsg(str, MSGLEVEL_NORMAL);
}

void Rsldbg::remModule(HDBG hinst, RSLMODULE hmod)
{
    CDebug* pDebug = (CDebug* )hinst;
    QVector<SpDebugPtrType>::iterator i;
    for(i = m_Dbg.begin(); i != m_Dbg.end(); ++i)
    {
        if ((*i)->GetInst() == pDebug->GetInst())
            break;
    }

    if(i != m_Dbg.end ())
    {
        std::vector <elem_bpdata>::iterator j;
        CBPData* pBPData = (*i)->GetBP ();
        for(j = pBPData->begin (); j!=pBPData->end (); )
        {
            if( hmod == (*j)->mod)
                j = pBPData->erase (j);
            else
                ++j;
        }

        CWatchV::iterator k;
        CWatchV* watch[3];
        watch[0] = (*i)->GetLocals ();
        watch[1] = (*i)->GetSurvey ();
        watch[2] = (*i)->GetQSurvey ();
        int l;
        for ( l = 0; l < 3; l++ )
        {
            if ( watch[l] )
            {
                for (k = watch[l]->begin(); k != watch[l]->end (); ++k)
                {
                    if ( (*k)->mod == hmod )
                        (*k)->Free();
                }
            }
        }

    }
}

void Rsldbg::done_ui()
{
    //if  (!m_pWndMain.empty())

}

bool Rsldbg::init_ui()
{
    done_ui();
    m_ActiveBeforeDbg = GetForegroundWindow();

    m_pWndMain.reset(new MainWindow());
    m_pWndMain->setWindowModality(Qt::ApplicationModal);
    QObject::connect(m_pWndMain.data(), &MainWindow::closed, &m_EventLoop, &QEventLoop::quit);

    return true;
}

// ------------------------------------------------------------------------------

static int RSDBG DbgCount(void)
{
    return rsldbg->size();
}

static HDBG RSDBG DbgInit(HRD inst, TDbgIntf *_dbg_ftable)
{
    return rsldbg->init(inst, _dbg_ftable);
}

static void RSDBG DbgDone(HDBG hDBG)
{
    rsldbg->done(hDBG);
}

static int RSDBG DbgBreak (HDBG hDBG, uint32 data)
{
    return rsldbg->DbgBreak(hDBG, data);
}

static void RSDBG DbgTrace (HDBG hinst,const char *str)
{
    CDebug* pDebug = (CDebug* )hinst;

    if (pDebug && pDebug->NeedConvert())
        ; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!

    rsldbg->trace(hinst, str);
}

static int RSDBG DbgVersion(void)
{
    return 2; //version 2, now supports decoding  dos<->win cp
}

static void RSDBG DbgAddModule(HDBG hinst, RSLMODULE hmod)
{
    CDebug* dbg = reinterpret_cast<CDebug*> (hinst);
    dbg->GetSaver()->Command (CMD_MODULE_LOAD, reinterpret_cast<void*>(hmod));
}

static void RSDBG DbgRemModule(HDBG hinst, RSLMODULE hmod)
{
    CDebug* pDebug = (CDebug* )hinst;

    if(!pDebug)
        return;

    pDebug->GetSaver()->Command (CMD_MODULE_UNLOAD, reinterpret_cast<void*>(hmod));
    rsldbg->remModule(hinst, hmod);
}

static void RSDBG DbgSetMode(HDBG hInst, int mode)
{
    CDebug* pDebug = (CDebug* )hInst;

    if(0!=mode)
        pDebug->SetExternalMode(FALSE);
}

// ------------------------------------------------------------------------------

static TRslDbg dbgInterface =
{
        DbgVersion,
        DbgCount,
        DbgInit,
        DbgDone,
        DbgBreak,
        DbgTrace,
        DbgAddModule,
        DbgRemModule,
        DbgSetMode
};

extern "C" __declspec(dllexport)
TRslDbg *RSDBG RslGetInterface (void)
{
    return &dbgInterface;
}
