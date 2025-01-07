#include <winsock2.h>
#include <ws2tcpip.h>
#include "dbgserverbase.h"
#include "serveractionevent.h"
#include <QThread>
#include <QProcess>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QApplication>
#include <QTextCodec>
#include <QStack>
#include <QMutexLocker>
#include <QDataStream>

#define DEFAULT_PORT 1491
static int max_port = DEFAULT_PORT;

Q_LOGGING_CATEGORY(dbgServer, "rsldbg.server")

DbgServerBase::DbgServerBase(QObject *parent)
    : QObject{parent},
    QRunnable()
{
    //prevStack = 0;
    //newDbg = true;
    ClientSocket = INVALID_SOCKET;
    oem866 = QTextCodec::codecForName("IBM 866");
}

DbgServerBase::~DbgServerBase()
{

}

bool cstartApp()
{
    return false;
}

bool DbgServerBase::isConnected()
{
    return ClientSocket != INVALID_SOCKET;
}

int DbgServerBase::port()
{
    return m_Port;
}

void DbgServerBase::write(DBGHEADER *hdr, void *data, int len, const QByteArray &adddata)
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

void DbgServerBase::write(DBGHEADER *hdr, const QByteArray &adddata)
{
    write(hdr, nullptr, 0, adddata);
}

void DbgServerBase::run()
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

    char port[_MAX_PATH] = {'\0'};
    m_Port = ++max_port;
    sprintf(port, "%d", m_Port);
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

    if (!startApp())
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

    QByteArray buffer;
    QStack<DBGHEADER> headers;
    bool stop = false;

    do {
        QEvent *event = peekServerEvent();
        if (event)
        {
            serverEvent(event);
            delete event;
        }

        QThread::usleep(100);

        if (headers.empty())
        {
            unsigned long l;
            ioctlsocket(ClientSocket, FIONREAD, &l);

            if (l >= sizeof(DBGHEADER))
            {
                DBGHEADER hdr;
                iResult = recv(ClientSocket, (char*)&hdr, sizeof(DBGHEADER), 0);
                headers.push(hdr);
            }
        }
        else
        {
            const DBGHEADER &hdr = headers.top();

            unsigned long l;
            ioctlsocket(ClientSocket, FIONREAD, &l);

            if (l >= hdr.size)
            {
                char *tmp = new char[hdr.size];
                iResult = recv(ClientSocket, tmp, hdr.size - buffer.size(), 0);

                if (iResult > 0)
                    buffer.append(tmp, hdr.size);

                if (buffer.size() == hdr.size)
                {
                    QEvent *event = new ServerActionEvent(hdr.action, buffer);
                    sendServerAction(event);

                    buffer.clear();
                    headers.pop();
                }

                delete[] tmp;
            }
        }
    } while (!stop);

    iResult = shutdown(ClientSocket, SD_SEND);
    closesocket(ClientSocket);
    WSACleanup();
    emit finished();
}

bool DbgServerBase::startApp()
{
    return false;
}

QEvent* DbgServerBase::peekServerEvent()
{
    QMutexLocker locker(&m_Mutex);

    if (m_Messages.empty())
        return nullptr;

    return m_Messages.dequeue();
}

void DbgServerBase::sendServerAction(QEvent *e)
{
    qDebug() << "DbgServerBase::sendServerAction";

    QMutexLocker locker(&m_Mutex);
    m_Messages.enqueue(e);
}

bool DbgServerBase::serverEvent(QEvent *e)
{
    if (e->type() == MSG_SERVER_EVENT)
        return serverActionEvent(dynamic_cast<ServerActionEvent*>(e));

    return false;
}

bool DbgServerBase::serverActionEvent(ServerActionEvent *e)
{
    qDebug() << "DbgServerBase::serverActionEvent";
    return false;
}
