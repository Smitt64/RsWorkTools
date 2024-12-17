#include "cdebug.h"
#include "saver.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rsl/dbgintf.h"
#include "rsldbg.h"
//#include "rsl/idebug.h"
#include "events/dbgbreakpointevent.h"
#include <QFile>
#include <QTextStream>
#include <codeeditor/codeeditor.h>
#include <codeeditor/cppcodehighlighter.h>
#include <QDebug>
#include <QMessageBox>

#define tracemsg (*((CTraceMsg*)m_tracemsg))

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_curdbg(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pCodeEditor = new CodeEditor(this);
    m_pCodeEditor->setReadOnly(true);
    setCentralWidget(m_pCodeEditor);
    m_tracemsg = new CTraceMsg();

    ToolApplyHighlighter(m_pCodeEditor, HighlighterRsl);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit closed();
    QMainWindow::closeEvent(event);
}

QString MainWindow::ReadTextFileContent(const QString &filename, const QString &encode)
{
    QString content;
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (encode.isEmpty())
            content = f.readAll();
        else
        {
            QTextStream stream(&f);
            stream.setCodec(encode.toLocal8Bit().data());

            content = stream.readAll();
        }

        f.close();
    }
    return content;
}

void MainWindow::AddTraceMsg(const QString &str, const int &level)
{
    TTraceMsg* msg = new TTraceMsg;
    msg->msg = str;
    msg->level = (MSGLEVEL)level;

    elem_tracemsg msg_aptr (msg);
    tracemsg.push_back(msg_aptr);

    qDebug() << str;
}

void MainWindow::UpdateStack()
{
    int rv = m_curdbg->GetStackCount();
    for (int i = 0; i < rv; i++)
    {
        QString proc = m_curdbg->GetProcName(m_curdbg->GetStackAt(i));
        //m_wndCallStack.m_wndCallStackList.AddString (buf);
    }

    // m_wndCallStack.m_wndCallStackList.SetCurSel (0);
}

void MainWindow::ProcessBreakPoint(DbgBreakpointEvent *ev)
{
    /*
     * KillTimer (1);

   HWND wnd = ::GetForegroundWindow ();

   if (wnd != *this)
      ActiveBeforeDbg = wnd;

   // Принадлежит ли окно верхнего уровня отлаживаемому процессу?
   if (ActiveBeforeDbg && IsWindow (ActiveBeforeDbg))
      {
      DWORD pid;
      GetWindowThreadProcessId  (ActiveBeforeDbg, &pid);
      if (pid != GetCurrentProcessId  ())
         ActiveBeforeDbg = NULL;
      }

   ::ReplyMessage((LRESULT)TRUE);

   SetForegroundWindow();

   if (needRefresh)
      {
      SetWindowPlacement (&wndPlacement);
      needRefresh = false;
      }
     * */
    newDbg = (m_curdbg != ev->dbg());
    m_curdbg = ev->dbg();
    //m_curdbg->SetChildsLimit (m_dbgregistry.GetLimitChilds ());

    TBpData* cur_bpdata = (TBpData*)ev->data();
    m_curdbg->GetRemoteVersion();

    if (m_curdbg->inInteractiveExp)
    {
        char buff [512] = { '\0' };
        m_curdbg->inInteractiveExp = false;
        unsigned short stat = m_curdbg->do_GetExpInteractiveStatus ();

        if (stat == INTR_EXP_ERROR)
        {
            m_curdbg->do_GetLastErrorText(buff, sizeof buff);
            //s.LoadString (IDS_INTEREXPERR);
            //MessageBox (buff, s, MB_OK | MB_ICONERROR);
            QMessageBox::critical(this, "IDS_INTEREXPERR", buff);
        }
        else if (stat == INTR_EXP_OK)
        {
            m_curdbg->do_GetLastErrorText (buff, sizeof buff);
            //s.LoadString (IDS_INTERRESULT);
            //MessageBox (buff, s, MB_OK);
            QMessageBox::critical(this, "IDS_INTERRESULT", buff);
        }
    }

    m_curdbg->UpdateDbgInfo();
    m_curdbg->SetDebugState(true);
    m_curdbg->SetCurModule((RSLMODULE)-1);
    m_curdbg->GetSaver()->Load();

    QString str_trace = "trace";
    m_curdbg->SetIndex(0);

    /*QString framename = frame_name();
    framename += " - ";
    framename += str_trace;
    setWindowTitle(framename);*/

    int offs, len;
    if (ev->dbg() && ev->data() && cur_bpdata->bp_type == BP_INVISIBLE)
    {
        m_curdbg->do_ClrBreakPoint(m_curdbg->GetCurStatement (&offs, &len));
        m_curdbg->DelBp(cur_bpdata);
    }

    m_curdbg->SetIndex(0);
    UpdateDbgInfo(0);
}

void MainWindow::UpdateDbgInfo(int index)
{
    UpdateStack();
    UpdateText(index);
}

void MainWindow::UpdateText(int index)
{
    int offs = 0;
    int len = 0;
    int line;
    RSLSRC hsrc;
    RSLMODULE mod = m_curdbg->GetCurModule(index);
    RSLSTMT statement = m_curdbg->GetCurStatement(&offs, &len, index);
    line = m_curdbg->GetStatementLine(offs, len, mod);

    if (m_curModuleInView == mod)
    {

    }
    else
    {
        int encode = -1;
        int isBtrStream = 0;
        char *filename = RslGetModuleFile((Qt::HANDLE)mod, &isBtrStream);

        QString content = ReadTextFileContent(filename, "IBM 866");
        m_pCodeEditor->setPlainText(content);

        //bool rv = m_curdbg->do_OpenSrc (mod, &hsrc, &encode);
        //m_curdbg->do_CloseSrc(hsrc);
        /*if (hsrc <= 0)
        {
            TRACE("do_OpenSrc returned bad value\n");
            if (m_curdbg->GetStackSize () == 1)
                exec_continue(RSL_STEP_OUT);
            else if (m_curdbg->GetStackSize () > 1)
                exec_continue(RSL_STEP_OVER);
            return;
        }*/
    }
}

void UpdateText(RSLMODULE mod, bool notActivModule)
{

}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == MSG_BREAKPOINT)
    {
        qDebug() << "MSG_BREAKPOINT 111";
        DbgBreakpointEvent *ev = dynamic_cast<DbgBreakpointEvent*>(event);
        ProcessBreakPoint(ev);
    }

    return QMainWindow::event(event);
}
