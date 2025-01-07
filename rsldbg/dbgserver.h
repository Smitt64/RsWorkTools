#ifndef DBGSERVER_H
#define DBGSERVER_H

#include "dbgserverproto.h"
#include "lf/bp_data.h"
#include "server/dbgserverbase.h"
#include <QObject>

class CDebug;
class QProcess;
class QTextCodec;
class DbgServer : public DbgServerBase
{
    Q_OBJECT
public:
    DbgServer(QObject *parent = nullptr);
    virtual ~DbgServer();

    CDebug *m_curdbg;

    void UpdateDbgInfo(TBpData *data);

    bool isConnected();
    void setIsNewDebug(const bool &v);

public slots:
    void sendEventBreakPoint(Qt::HANDLE BpData);

protected:
    virtual bool startApp() Q_DECL_OVERRIDE;
    virtual bool serverActionEvent(ServerActionEvent *e) Q_DECL_OVERRIDE;
    virtual bool serverEvent(QEvent *e) Q_DECL_OVERRIDE;

signals:
    void finished();
    void started();

private:
    QTextCodec *oem866;
    int RslGetModuleLine(Qt::HANDLE module, int offs, int len);

    void process(const QByteArray &data, const qint16 &action);
    void fillBpData(DBGBPDATA *body, Qt::HANDLE BpData);

    QString ReadTextFileContent(const QString &filename, const QString &encode);
    void UpdateText(const int &index);
    void UpdateStack();
    void UpdateWatch();
    void UpdateDbgInfo(const int &index);
    void UpdateVariables(const int &index);
    void ShowVariables(const int &index);
    QString m_LastError;
    RSLSTACK prevStack;
    bool newDbg;

    unsigned int ClientSocket;
    Qt::HANDLE m_curModuleInView;

    QScopedPointer<QProcess> m_pProc;
};

#endif // DBGSERVER_H
