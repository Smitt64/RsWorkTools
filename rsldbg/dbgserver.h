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
class QTextCodec;
class DbgServer : public QObject, public QRunnable
{
    Q_OBJECT
public:
    DbgServer(QObject *parent = nullptr);
    virtual ~DbgServer();
    virtual void run();

    CDebug *m_curdbg;

    void UpdateDbgInfo(TBpData *data);

    bool isConnected();

public slots:
    void sendEventBreakPoint(Qt::HANDLE BpData);

signals:
    void finished();
    void started();

private:
    QTextCodec *oem866;
    int RslGetModuleLine(Qt::HANDLE module, int offs, int len);

    bool startapp();
    void process(const QByteArray &data, const qint16 &action);
    int read(QByteArray &data, qint16 *action);
    void write(DBGHEADER *hdr, void *data, int len, const QByteArray &adddata = QByteArray());
    void fillBpData(DBGBPDATA *body, Qt::HANDLE BpData);

    QString ReadTextFileContent(const QString &filename, const QString &encode);
    void UpdateText(const int &index);
    void UpdateStack();
    void UpdateDbgInfo(const int &index);
    QString m_LastError;

    unsigned int ClientSocket;
    Qt::HANDLE m_curModuleInView;

    QScopedPointer<QProcess> m_pProc;
};

#endif // DBGSERVER_H
