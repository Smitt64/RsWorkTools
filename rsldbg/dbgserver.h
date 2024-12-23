#ifndef DBGSERVER_H
#define DBGSERVER_H

#include <QObject>
#include <QRunnable>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(dbgServer)

class QProcess;
class DbgServer : public QObject, public QRunnable
{
    Q_OBJECT
public:
    DbgServer(QObject *parent = nullptr);
    virtual ~DbgServer();
    virtual void run();

signals:
private:
    bool startapp();
    void process(const QByteArray &data, const qint16 &action);
    int read(QByteArray &data, qint16 *action);
    QString m_LastError;

    unsigned int ClientSocket;

    QScopedPointer<QProcess> m_pProc;
};

#endif // DBGSERVER_H
