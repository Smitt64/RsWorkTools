#include "sqldatabase.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QUuid>

SqlDatabase::SqlDatabase(const QString &driver)
    : QObject{}
{
    db.reset(new QSqlDatabase(QSqlDatabase::addDatabase(driver, QUuid::createUuid().toString())));
}

QSqlDatabase &SqlDatabase::database()
{
    return *db;
}

QString SqlDatabase::connectionName() const
{
    return db->connectionName();
}

bool SqlDatabase::open()
{
    return db->open();
}

void SqlDatabase::close()
{
    db->close();
}

bool SqlDatabase::isOpen() const
{
    return db->isOpen();
}

QString SqlDatabase::lastError() const
{
    return db->lastError().text();
}

void SqlDatabase::setHostName(const QString& hostName)
{
    db->setHostName(hostName);
}

QString SqlDatabase::hostName() const
{
    return db->hostName();
}

QString SqlDatabase::databaseName() const
{
    return db->databaseName();
}

void SqlDatabase::setDatabaseName(const QString& name)
{
    db->setDatabaseName(name);
}

void SqlDatabase::setUserName(const QString& userName)
{
    db->setUserName(userName);
}

QString SqlDatabase::userName() const
{
    return db->userName();
}

void SqlDatabase::setPassword(const QString& password)
{
    db->setPassword(password);
}

QString SqlDatabase::password() const
{
    return db->password();
}

void SqlDatabase::setPort(int port)
{
    db->setPort(port);
}

int SqlDatabase::port() const
{
    return db->port();
}

void SqlDatabase::setConnectOptions(const QString& connectOptions)
{
    db->setConnectOptions(connectOptions);
}

QString SqlDatabase::connectOptions() const
{
    return db->connectOptions();
}
