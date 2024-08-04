#ifndef SQLQUERY_H
#define SQLQUERY_H

#include <QObject>
#include <QVariant>

class QSqlQuery;
class SqlDatabase;
class SqlQuery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString executedQuery READ executedQuery CONSTANT)
    Q_PROPERTY(QString lastQuery READ lastQuery CONSTANT)
    Q_PROPERTY(QString lastError READ lastError CONSTANT)
    Q_PROPERTY(int numRowsAffected READ numRowsAffected CONSTANT)
    Q_PROPERTY(bool isActive READ isActive CONSTANT)
    Q_PROPERTY(bool isSelect READ isSelect CONSTANT)
    Q_PROPERTY(bool isValid READ isValid CONSTANT)
    Q_PROPERTY(int pos READ at CONSTANT)
    Q_PROPERTY(int size READ size CONSTANT)
public:
    Q_INVOKABLE SqlQuery(const QString &connectionName);
    Q_INVOKABLE SqlQuery(SqlDatabase *db);
    virtual ~SqlQuery();

    Q_INVOKABLE void addBindValue(const QVariant &val);
    int at() const;

    Q_INVOKABLE void bindValue(const QString &placeholder, const QVariant &val);
    Q_INVOKABLE void bindValue(int pos, const QVariant &val);
    Q_INVOKABLE void clear();

    Q_INVOKABLE bool exec(const QString &query = QString());

    QString executedQuery() const;
    QString lastQuery() const;
    QString lastError() const;
    int numRowsAffected() const;
    int size() const;

    bool isActive() const;
    bool isSelect() const;
    bool isValid() const;

    Q_INVOKABLE void finish();
    Q_INVOKABLE bool first();
    Q_INVOKABLE bool last();
    Q_INVOKABLE bool next();
    Q_INVOKABLE bool previous();

    Q_INVOKABLE bool isNull(int field) const;
    Q_INVOKABLE bool isNull(const QString &name) const;

    Q_INVOKABLE bool prepare(const QString &query);
    Q_INVOKABLE bool seek(int index, bool relative = false);

    Q_INVOKABLE QVariant value(int index) const;
    Q_INVOKABLE QVariant value(const QString &name) const;

private:
    QScopedPointer<QSqlQuery> m_Query;
};

#endif // SQLQUERY_H
