#include <winsock2.h>
#include <ws2tcpip.h>
#include "dbgserver.h"
#include "clocals.h"
#include "dbgserverproto.h"
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

#define DEFAULT_PORT 1491
static int max_port = DEFAULT_PORT;

Q_LOGGING_CATEGORY(dbgServer, "rsldbg.server")

DbgServer::DbgServer(QObject *parent)
    : QObject{parent}
{
    prevStack = 0;
    newDbg = true;
    ClientSocket = INVALID_SOCKET;
    oem866 = QTextCodec::codecForName("IBM 866");
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

void DbgServer::run()
{
    WSADATA wsaData = { 0 };
    int iResult = 0;

    SOCKET ListenSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0)
    {
        m_LastError = QString("WSAStartup failed with error: %1").arg(iResult);
        qCritical(dbgServer()) << m_LastError;
        emit finished();
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    //std::string port = std::to_string(++max_port);
    char port[_MAX_PATH] = {'\0'};
    sprintf(port, "%d", ++max_port);
    iResult = getaddrinfo(NULL, port, &hints, &result);

    if ( iResult != 0 )
    {
        m_LastError = QString("getaddrinfo failed with error: %1").arg(iResult);
        qCritical(dbgServer()) << m_LastError;

        WSACleanup();
        emit finished();
        return;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        m_LastError = QString("socket failed with error: %1").arg(WSAGetLastError());
        qCritical(dbgServer()) << m_LastError;

        freeaddrinfo(result);
        WSACleanup();
        emit finished();
        return;
    }

    char ipAddress[INET_ADDRSTRLEN] = { '\0' };
    inet_ntop(AF_INET, &((struct sockaddr_in *)result->ai_addr)->sin_addr, ipAddress, INET_ADDRSTRLEN);
    //qDebug() << "ipAddress" << ipAddress;

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        m_LastError = QString("bind failed with error: %1").arg(WSAGetLastError());
        qCritical(dbgServer()) << m_LastError;

        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        emit finished();
        return;
    }
    else
        qInfo(dbgServer()) << "Server binds to" << ipAddress;

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        m_LastError = QString("listen failed with error: %1").arg(WSAGetLastError());
        qCritical(dbgServer()) << m_LastError;

        closesocket(ListenSocket);
        WSACleanup();
        emit finished();
        return;
    }

    if (!startapp())
    {
        m_LastError = QString("Can't start debug app");
        qCritical(dbgServer()) << m_LastError;

        closesocket(ListenSocket);
        WSACleanup();
        emit finished();
        return;
    }

    ClientSocket = accept(ListenSocket, NULL, NULL);
    /*int WaitCount = 0;
    do {
        QThread::sleep(5);
        ClientSocket = accept(ListenSocket, (SOCKADDR*)&result->ai_addr, (int*)&result->ai_addrlen);
        qDebug() << "Waiting connection";
        WaitCount ++;

        if (ClientSocket != INVALID_SOCKET)
            WaitCount = 10;
    } while (ClientSocket == SOCKET_ERROR && WaitCount < 10);*/

    if (ClientSocket == INVALID_SOCKET)
    {
        m_LastError = QString("accept failed with error: %1").arg(WSAGetLastError());
        qCritical(dbgServer()) << m_LastError;

        closesocket(ListenSocket);
        WSACleanup();
        emit finished();
        return;
    }

    closesocket(ListenSocket);

    DBGHEADER handshake;
    DbgMakeHeader(&handshake, 0, DBG_REQUEST_HANDSHAKE);
    int iSendResult = send(ClientSocket, (char*)&handshake, sizeof(DBGHEADER), 0);
    emit started();

    do {
        QThread::usleep(100);
        qint16 action = 0;
        QByteArray data;
        iResult = read(data, &action);
        //qDebug() << "read" << iResult << WSAGetLastError();

        if (iResult)
        {
            process(data, action);
        }
    } while (iResult > 0);

    iResult = shutdown(ClientSocket, SD_SEND);
    closesocket(ClientSocket);
    WSACleanup();
    emit finished();
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

QString toolProcessStateText(qint16 State)
{
    QString result;
    switch(State)
    {
    case QProcess::Starting:
        result = "The process is starting, but the program has not yet been invoked.";
        break;
    case QProcess::Running:
        result = "The process is running and is ready for reading and writing.";
        break;
    default:
        result = "The process is not running.";
    }

    return result;
}

QString toolProcessExitStatusText(qint16 State)
{
    QString result;
    switch(State)
    {
    case QProcess::CrashExit:
        result = "The process crashed.";
        break;
    default:
        result = "The process exited normally.";
    }

    return result;
}

QString toolGetProcessErrorText(const QProcess::ProcessError &error)
{
    QString errText;
    switch(error)
    {
    case QProcess::FailedToStart:
        errText = "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program";
        break;
    case QProcess::Crashed:
        errText = "The process crashed some time after starting successfully";
        break;
    case QProcess::Timedout:
        errText = "The process timedout";
        break;
    case QProcess::WriteError:
        errText = "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel";
        break;
    case QProcess::ReadError:
        errText = "An error occurred when attempting to read from the process. For example, the process may not be running";
        break;
    case QProcess::UnknownError:
        errText = "An unknown error occurred";
        break;
    }

    return errText;
}

bool DbgServer::startapp()
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
        "--port", QString::number(max_port)
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

int DbgServer::read(QByteArray &data, qint16 *action)
{
    DBGHEADER hdr;
    int iResult = recv(ClientSocket, (char*)&hdr, sizeof(DBGHEADER), 0);

    if (iResult > 0)
    {
        if (hdr.size && !memcmp(hdr.magic, RSLDBG_MAGIC, sizeof(hdr.magic)))
        {
            *action = hdr.action;
            data.reserve(hdr.size);

            iResult = recv(ClientSocket, (char*)data.data(), hdr.size, 0);
        }
    }

    return iResult;
}

void DbgServer::process(const QByteArray &data, const qint16 &action)
{
    if (action == DBG_REQUEST_EXECCONTNUE)
    {
        DBG_EXECCONTNUE *exec = (DBG_EXECCONTNUE*)data.data();
        m_curdbg->SetDebugState(false);
        m_curdbg->do_ExecContinue(exec->trace_log);
        qDebug() << "DBG_REQUEST_EXECCONTNUE" << exec->trace_log;
    }
}

void DbgServer::write(DBGHEADER *hdr, void *data, int len, const QByteArray &adddata)
{
    QByteArray tmp;
    QDataStream stream(&tmp, QIODevice::ReadWrite);

    stream.writeRawData((char*)hdr, sizeof(DBGHEADER));

    if (data)
        stream.writeRawData((char*)data, len);

    if(!adddata.isEmpty())
        stream.writeRawData(adddata.data(), adddata.size());

    DBGHEADER tmphdr;
    memcpy(&tmphdr, hdr, sizeof(DBGHEADER));

    tmphdr.size = tmp.size();
    memcpy(tmp.data(), &tmphdr, sizeof(DBGHEADER));

    send(ClientSocket, (char*)tmp.data(), tmp.size(), 0);
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
