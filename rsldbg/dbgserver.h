#ifndef DBGSERVER_H
#define DBGSERVER_H

#include "dbgserverproto.h"
#include "lf/bp_data.h"
#include <QObject>
#include <QRunnable>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(dbgServer)

class CDebug;
class QProcess;
class DbgServer : public QObject, public QRunnable
{
    Q_OBJECT
public:
    DbgServer(QObject *parent = nullptr);
    virtual ~DbgServer();
    virtual void run();

    CDebug *m_curdbg;

    void UpdateDbgInfo(TBpData *data);

public slots:
    void sendEventBreakPoint(Qt::HANDLE BpData);

signals:
    void finished();
    void started();

private:
    bool startapp();
    void process(const QByteArray &data, const qint16 &action);
    int read(QByteArray &data, qint16 *action);
    void write(DBGHEADER *hdr, void *data, int len);
    void fillBpData(DBGBPDATA *body, Qt::HANDLE BpData);
    QString m_LastError;

    unsigned int ClientSocket;

    QScopedPointer<QProcess> m_pProc;
};

#endif // DBGSERVER_H
