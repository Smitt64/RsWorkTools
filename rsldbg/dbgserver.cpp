#include <winsock2.h>
#include <ws2tcpip.h>
#include "dbgserver.h"
#include "clocals.h"
#include "dbgserverproto.h"
#include "server/breakpointevent.h"
#include "server/serveractionevent.h"
#include "server/updatedbginfoevent.h"
#include <QThread>
#include <QProcess>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QApplication>
#include "cdebug.h"
#include <QEvent>
#include <QFile>
#include <QDataStream>
#include <QTextCodec>
#include "rsldbg.h"

QString toolProcessStateText(qint16 State);
QString toolProcessExitStatusText(qint16 State);
QString toolGetProcessErrorText(const QProcess::ProcessError &error);

DbgServer::DbgServer(QObject *parent)
    : DbgServerBase{parent}
{
    prevStack = 0;
    newDbg = true;
}

DbgServer::~DbgServer()
{
    //closesocket(ClientSocket);
    qDebug() << "~DbgServer()";
}

bool DbgServer::isConnected()
{
    return ClientSocket != INVALID_SOCKET;
}

void DbgServer::setIsNewDebug(const bool &v)
{
    newDbg = v;
}

QString DbgServer::ReadTextFileContent(const QString &filename, const QString &encode)
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

bool DbgServer::startApp()
{
    QNetworkAddressEntry adress;
    foreach(const QNetworkInterface &qNetInterface, QNetworkInterface::allInterfaces())
    {
        foreach(const QNetworkAddressEntry &qNetInterfaceAddress, qNetInterface.addressEntries())
        {
            if (qNetInterfaceAddress.ip().protocol() == QAbstractSocket::IPv4Protocol
                && qNetInterfaceAddress.ip() != QHostAddress(QHostAddress::LocalHost)
                && qNetInterfaceAddress.netmask().toString() != "")
            {
                adress = qNetInterfaceAddress;
                break;
                //qDebug() << qNetInterfaceAddress.ip().toString();
                //qDebug() << qNetInterfaceAddress.netmask().toString();
            }
        }
    }

    m_pProc.reset(new QProcess());
    m_pProc->setProgram("rsldbgclnt.exe");
    m_pProc->setArguments(
    {
        "--host", "127.0.0.1",//adress.ip().toString(),
        "--port", QString::number(port())
    });

    bool hr = true;
    QObject::connect(m_pProc.data(), &QProcess::stateChanged, [](QProcess::ProcessState newState)
                     {
                        qCInfo(dbgServer()) << QString("Process state changed to: %1 (%2)")
                        .arg(newState).arg(toolProcessStateText(newState));
                     });
    QObject::connect(m_pProc.data(), &QProcess::errorOccurred, [&hr](QProcess::ProcessError error)
                     {
                        qCInfo(dbgServer()) << QString("Process error occurred: %1 (%2)")
                        .arg(error).arg(toolGetProcessErrorText(error));

                        hr = false;
                     });

    m_pProc->start();
    m_pProc->waitForStarted();

    if (m_pProc->state() == QProcess::NotRunning)
        hr = false;

    //qDebug() << m_pProc->state() << m_pProc->exitStatus() << m_pProc->exitCode() << m_pProc->error();

    return hr;
}

void DbgServer::process(const QByteArray &data, const qint16 &action)
{
    /*if (action == DBG_REQUEST_EXECCONTNUE)
    {
        DBG_EXECCONTNUE *exec = (DBG_EXECCONTNUE*)data.data();
        m_curdbg->SetDebugState(false);
        m_curdbg->do_ExecContinue(exec->trace_log);
        qDebug() << "DBG_REQUEST_EXECCONTNUE" << exec->trace_log;
    }*/
}

void DbgServer::fillBpData(DBGBPDATA *body, Qt::HANDLE BpData)
{
    TBpData *data = (TBpData*)BpData;
    body->bp_type = data->bp_type;
    body->offs = data->offs;
    body->len = data->len;
    body->line = data->line;
    body->key = data->key;
    body->mod = reinterpret_cast<qint64>(data->mod);
    body->stmt = reinterpret_cast<qint64>(data->stmt);
    qstrcpy(body->modname, data->modname.toLocal8Bit().data());
}

void DbgServer::sendEventBreakPoint(Qt::HANDLE BpData)
{
    TBpData *data = (TBpData*)BpData;

    DBGEVENT event;
    event.event = MSG_BREAKPOINT;

    if (data)
    {
        event.data.bp_event.BpSetted = 1;
        fillBpData(&event.data.bp_event, data);
    }
    else
    {
        event.data.bp_event.BpSetted = 0;
        memset(&event.data.bp_event, 0, sizeof(DBGBPDATA));
    }

    DBGHEADER handshake;
    DbgMakeHeader(&handshake, 0, DBG_REQUEST_EVENT);
    write(&handshake, &event, sizeof(DBGEVENT));
}

void DbgServer::UpdateDbgInfo(TBpData *data)
{
    m_curdbg->GetRemoteVersion();
    m_curdbg->UpdateDbgInfo();
    m_curdbg->SetDebugState(true);
    m_curdbg->SetCurModule((RSLMODULE)-1);

    int offs, len;
    if (data && data->bp_type == BP_INVISIBLE)
    {
        m_curdbg->do_ClrBreakPoint (m_curdbg->GetCurStatement (&offs, &len));
        m_curdbg->DelBp(data);
    }

    m_curdbg->SetIndex(0);
    UpdateDbgInfo(0);
}

void DbgServer::UpdateVariables(const int &index)
{
    CLocals* pLocals = m_curdbg->GetLocals ();
    RSLSTACK st = m_curdbg->GetStackAt (index);
    pLocals->CollectLocals (st, &prevStack, newDbg);

    qDebug() << "UpdateVariables" << newDbg;
    ShowVariables(index);
}

void DbgServer::ShowVariables(const int &index)
{
    RSLSTACK st = m_curdbg->GetStackAt (index);
    CLocals* pLocals = m_curdbg->GetLocals ();

    CLocals::iterator i;
    //int nsize = (int)pLocals->size();
    /*int ctr = 0;

    for (i = pLocals->begin(); i != pLocals->end(); ++i)
    {
        ++ctr;
        if ((*i)->is_object && !(*i)->is_expanded)
            ++ctr;
    }

    int row = 0;
    unsigned char* ar_depth = new unsigned char[ctr];
    unsigned char max_depth = 0;*/

    for (i = pLocals->begin (); i != pLocals->end (); ++i)
    {
        qDebug() << "ShowVariables" << (*i)->str_name;
    }
}

void DbgServer::UpdateDbgInfo(const int &index)
{
    UpdateStack();
    UpdateWatch();
    UpdateVariables(index);
    UpdateText(index);   
}

void DbgServer::UpdateWatch()
{
    m_curdbg->UpdateSurvey();
    //ShowSurvey (/*index*/);
}

void DbgServer::UpdateStack()
{
    QVector<DBG_UPDATSTACK> packet;
    int rv = m_curdbg->GetStackCount();
    for (int i = 0; i < rv; i++)
    {
        DBG_UPDATSTACK item;
        Qt::HANDLE stack = m_curdbg->GetStackAt(i);
        Qt::HANDLE module = RslGetModuleFromStack(stack);
        Qt::HANDLE hst = RslGetStatementFromStack(stack);

        int isBtrStream = 0;
        RslGetStatementPos(hst, &item.offs, &item.len);
        item.line = RslGetModuleLine(module, item.offs, item.len);

        qstrcpy(item.fullfilename, RslGetModuleFile(module, &isBtrStream));
        qstrcpy(item.func, RslGetProcNameFromStack(stack));

        packet.append(item);
    }

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.writeRawData((char*)&packet[0], sizeof(DBG_UPDATSTACK) * packet.size());

    DBGHEADER hdr;
    DbgMakeHeader(&hdr, sizeof(DBG_UPDATEBP), DBG_REQUEST_UPDATESTACK);
    write(&hdr, nullptr, 0, data);
}

void DbgServer::UpdateText(const int &index)
{
    RSLMODULE mod = m_curdbg->GetCurModule(index);

    int offs = 0;
    int len = 0;
    int line = 0;

    RSLSTMT statement = m_curdbg->GetCurStatement(&offs, &len, index);
    line = m_curdbg->GetStatementLine(offs, len, mod);

    DBGHEADER hdr;
    DBG_UPDATETEXT updtext;
    memset(&updtext, 0, sizeof(DBG_UPDATETEXT));

    updtext.offs = offs;
    updtext.len = len;
    updtext.line = line;

    if (m_curModuleInView != mod)
    {
        //m_curModuleInView = mod;

        int isBtrStream = 0;
        char *filename = RslGetModuleFile((Qt::HANDLE)mod, &isBtrStream);

        QString content = ReadTextFileContent(filename, "IBM 866");
        QByteArray text = content.toLocal8Bit();

        qstrncpy(updtext.filename, filename, sizeof(updtext.filename));
        updtext.size = text.size();

        DbgMakeHeader(&hdr, sizeof(DBG_UPDATETEXT) + updtext.size, DBG_REQUEST_UPDATETEXT);
        write(&hdr, &updtext, sizeof(DBG_UPDATETEXT), text);
    }

    TBpData* bp;
    QVector<DBGBPDATA> tmpbp;

    int nBP = m_curdbg->GetBPCount();
    for (int i = 0; i < nBP; ++i)
    {
        bp = m_curdbg->GetBP(i);
        if (mod == bp->mod)
        {
            DBGBPDATA dbgbp;
            fillBpData(&dbgbp, bp);
            tmpbp.push_back(dbgbp);
            /*if (bp->bp_type == BP_DISABLED)
                color = RGB (0, 0, 255);
            else if (bp->bp_type == BP_ENABLED)
                color = RGB(0, 255, 0);
            else
                color = GetSysColor (COLOR_WINDOW);

            SetTextColor (bp->offs, bp->len, color, bp->line);*/
        }
    }

    if (!tmpbp.empty())
    {
        QByteArray bpdata;
        QDataStream stream(&bpdata, QIODevice::WriteOnly);
        stream.writeRawData((char*)&tmpbp[0], sizeof(DBGBPDATA) * tmpbp.size());

        DBG_UPDATEBP upd;
        upd.count = tmpbp.size();

        DBGHEADER hdr;
        DbgMakeHeader(&hdr, sizeof(DBG_UPDATEBP), DBG_REQUEST_UPDATEBP);
        write(&hdr, &upd, sizeof(DBG_UPDATETEXT), bpdata);
    }
}

int DbgServer::RslGetModuleLine(Qt::HANDLE module, int offs, int len)
{
    int      line = -1;
    int      realoffs, reallen;
    RSLSTMT  stmt;
    //RslGetStatementPos(hst, offs, len);
    m_curdbg->do_GetStatementOfPos((RSLMODULE)module, offs, len, &realoffs, &reallen, &stmt, &line);

    return line;
}

bool DbgServer::serverActionEvent(ServerActionEvent *e)
{
    QByteArray &data = e->buffer();
    if (e->action() == DBG_REQUEST_EXECCONTNUE)
    {
        DBG_EXECCONTNUE *exec = (DBG_EXECCONTNUE*)data.data();
        m_curdbg->SetDebugState(false);
        m_curdbg->do_ExecContinue(exec->trace_log);
        //qDebug() << "DBG_REQUEST_EXECCONTNUE" << exec->trace_log;
    }
    return DbgServerBase::serverActionEvent(e);
}

bool DbgServer::serverEvent(QEvent *event)
{
    if (event->type() == MSG_UPDATEDBGINFO_EVENT)
    {
        qDebug() << "MSG_UPDATEDBGINFO_EVENT";
        UpdateDbgInfoEvent *e = dynamic_cast<UpdateDbgInfoEvent*>(event);
        UpdateDbgInfo((TBpData*)e->data());
    }
    else if (event->type() == MSG_BREAKPOINT)
    {
        qDebug() << "MSG_BREAKPOINT";
        BreakPointEvent *e = dynamic_cast<BreakPointEvent*>(event);
        sendEventBreakPoint(e->data());
    }

    return DbgServerBase::serverEvent(event);
}
