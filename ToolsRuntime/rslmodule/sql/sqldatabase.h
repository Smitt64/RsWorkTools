#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include <QObject>

class QSqlDatabase;
class SqlDatabase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError CONSTANT)
    Q_PROPERTY(QString connectionName READ connectionName CONSTANT)
    Q_PROPERTY(QString databaseName READ databaseName WRITE setDatabaseName)
    Q_PROPERTY(bool isOpen READ isOpen CONSTANT)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName)
    Q_PROPERTY(QString userName READ userName WRITE setUserName)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(int port READ port WRITE setPort)
    Q_PROPERTY(QString connectOptions READ connectOptions WRITE setConnectOptions)
public:
    Q_INVOKABLE SqlDatabase(const QString& driver);

    Q_INVOKABLE bool open();
    Q_INVOKABLE void close();

    QString connectionName() const;

    QSqlDatabase &database();
    bool isOpen() const;
    QString lastError() const;

    void setHostName(const QString& hostName);
    QString hostName() const;

    QString databaseName() const;
    void setDatabaseName(const QString& name);

    void setUserName(const QString& userName);
    QString userName() const;

    void setPassword(const QString& password);
    QString password() const;

    void setPort(int port);
    int port() const;

    void setConnectOptions(const QString& connectOptions);
    QString connectOptions() const;

private:
    QScopedPointer<QSqlDatabase> db;
};

#endif // SQLDATABASE_H
