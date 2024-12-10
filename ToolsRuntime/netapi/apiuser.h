#ifndef APIUSER_H
#define APIUSER_H

#include <QObject>
#include "ToolsRuntime_global.h"
#include <QDateTime>

class ApiUserPrivate;
class TOOLSRUNTIME_EXPORT ApiUser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint32 Id READ id CONSTANT)
    Q_PROPERTY(QString UserName READ userName CONSTANT)
    Q_PROPERTY(QString Email READ email CONSTANT)
    Q_PROPERTY(QString Domain READ domain CONSTANT)
    Q_PROPERTY(QDateTime CreateDate READ createDate CONSTANT)
public:
    ApiUser(bool loadCurrentUser, QObject *parent = nullptr);
    ApiUser(const QString &User, QObject *parent = nullptr);

    bool get(const QString &User);
    bool registerUser(const QString &User, const QString &Domain, const QString &Email = QString());
    bool isValid() const;

    const QString &userName() const;
    const QString &email() const;
    const QString &domain() const;
    const qint32 &id() const;
    const QDateTime &createDate() const;

private:
    ApiUserPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ApiUser);
};

TOOLSRUNTIME_EXPORT ApiUser *toolGetCurrentUser();

#endif // APIUSER_H
