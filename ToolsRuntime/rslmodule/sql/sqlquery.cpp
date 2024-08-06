#include "sqlquery.h"
#include "sqldatabase.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include "rsl/dlmintf.h"
#include "statvars.h"
#include <rsl/ierror.h>
#include <QSqlError>

#define CHECK_CONNECTION(action) if (!m_Query) {iError(IER_RUNTIME, "Сonnection not found"); action; }

SqlQuery::SqlQuery(const QString &connectionName)
    : QObject{}
{
    if (QSqlDatabase::contains(connectionName))
    {
        QSqlDatabase _db = QSqlDatabase::database(connectionName);
        m_Query.reset(new QSqlQuery(_db));
    }
}

SqlQuery::SqlQuery(SqlDatabase *db)
{
    m_Query.reset(new QSqlQuery(db->database()));
}

SqlQuery::~SqlQuery()
{

}

void SqlQuery::addBindValue(const QVariant &val)
{
    CHECK_CONNECTION(return)
    m_Query->addBindValue(val);
}

int SqlQuery::at() const
{
    CHECK_CONNECTION(return 0)
    return m_Query->at();
}

void SqlQuery::bindValue(const QString &placeholder, const QVariant &val)
{
    CHECK_CONNECTION(return)
    m_Query->bindValue(placeholder, val);
}

void SqlQuery::bindValue(int pos, const QVariant &val)
{
    CHECK_CONNECTION(return)
    m_Query->bindValue(pos, val);
}

void SqlQuery::clear()
{
    CHECK_CONNECTION(return)
    m_Query->clear();
}

bool SqlQuery::exec(const QString &query)
{
    CHECK_CONNECTION(return false)

    if (query.isEmpty())
        return m_Query->exec();

    return m_Query->exec(query);
}

QString SqlQuery::executedQuery() const
{
    CHECK_CONNECTION(return QString())
    return m_Query->executedQuery();
}

QString SqlQuery::lastQuery() const
{
    CHECK_CONNECTION(return QString())
    return m_Query->lastQuery();
}

QString SqlQuery::lastError() const
{
    return m_Query->lastError().text();
}

int SqlQuery::numRowsAffected() const
{
    CHECK_CONNECTION(return 0)
    return m_Query->numRowsAffected();
}

void SqlQuery::finish()
{
    CHECK_CONNECTION(return)
    m_Query->finish();
}

bool SqlQuery::first()
{
    CHECK_CONNECTION(return false)
    return m_Query->first();
}

bool SqlQuery::last()
{
    CHECK_CONNECTION(return false)
    return m_Query->last();
}

bool SqlQuery::next()
{
    CHECK_CONNECTION(return false)
    return m_Query->next();
}

bool SqlQuery::previous()
{
    CHECK_CONNECTION(return false)
    return m_Query->previous();
}

bool SqlQuery::isActive() const
{
    CHECK_CONNECTION(return false)
    return m_Query->isActive();
}

bool SqlQuery::isSelect() const
{
    CHECK_CONNECTION(return false)
    return m_Query->isSelect();
}

bool SqlQuery::isValid() const
{
    CHECK_CONNECTION(return false)
    return m_Query->isValid();
}

bool SqlQuery::isNull(int field) const
{
    CHECK_CONNECTION(return false)
    return m_Query->isNull(field);
}

bool SqlQuery::isNull(const QString &name) const
{
    CHECK_CONNECTION(return false)
    return m_Query->isNull(name);
}

bool SqlQuery::prepare(const QString &query)
{
    CHECK_CONNECTION(return false)
    return m_Query->prepare(query);
}

bool SqlQuery::seek(int index, bool relative)
{
    CHECK_CONNECTION(return false)
    return m_Query->seek(index, relative);
}

int SqlQuery::size() const
{
    CHECK_CONNECTION(return 0)
    return m_Query->size();
}

QVariant SqlQuery::value(int index) const
{
    CHECK_CONNECTION(return QVariant());
    return m_Query->value(index);
}

QVariant SqlQuery::value(const QString &name) const
{
    CHECK_CONNECTION(return QVariant());
    return m_Query->value(name);
}

QSqlQuery *SqlQuery::query()
{
    return m_Query.data();
}
