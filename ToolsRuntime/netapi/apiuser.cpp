#include "apiuser.h"
#include "toolsruntime.h"
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

Q_GLOBAL_STATIC_WITH_ARGS(ApiUser, CurrentUser, (true))

ApiUser *toolGetCurrentUser()
{
    return CurrentUser;
}

class ApiUserPrivate
{
    Q_DECLARE_PUBLIC(ApiUser)
public:
    ApiUserPrivate(ApiUser *obj)
    {
        q_ptr = obj;
        m_isValid = false;
    }

    void readUserJson(const QByteArray &data)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        m_UserName = obj["username"].toString();
        m_Email = obj["mail"].toString();
        m_Domain = obj["domain"].toString();
        m_Id = obj["domain"].toInt();
        m_CreateDate = QDateTime::fromString(obj["createdate"].toString(), Qt::ISODateWithMs);
        m_isValid = true;
    }

    bool m_isValid;
    QString m_Email, m_UserName, m_Domain;
    qint32 m_Id;
    QDateTime m_CreateDate;

    ApiUser *q_ptr;
};

ApiUser::ApiUser(bool loadCurrentUser, QObject *parent)
    : QObject{parent},
    d_ptr(new ApiUserPrivate(this))
{
    /*if (loadCurrentUser)
    {
        UserDomainTuple user = toolGetCurrentUserAndDomain();
        QString _user = std::get<0>(user);
        QString _domain = std::get<1>(user);
        if (!get(_user))
        {
            registerUser(_user, _domain);
        }
    }*/
}

ApiUser::ApiUser(const QString &User, QObject *parent)
    : QObject{parent},
    d_ptr(new ApiUserPrivate(this))
{

}

bool ApiUser::registerUser(const QString &User, const QString &Domain, const QString &Email)
{
    Q_D(ApiUser);

    QUrl serviceUrl = toolGetApiUrl("/api/v1.0/user/add");
    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject root;
    root["username"] = User;
    root["email"] = Email;
    root["domain"] = Domain;

    QJsonDocument doc;
    doc.setObject(root);

    QByteArray data = doc.toJson();
    QNetworkAccessManager ApiManager;
    QNetworkReply *Reply = toolPostApiReply(&request, &ApiManager, data, true);
    if (Reply->error() == QNetworkReply::NoError)
        d->readUserJson(Reply->readAll());
    else
    {
        qDebug() << Reply->errorString()
                 << Reply->readAll();
    }

    Reply->close();
    delete Reply;

    return d->m_isValid;
}

bool ApiUser::get(const QString &User)
{
    Q_D(ApiUser);

    QUrlQuery postData;
    postData.addQueryItem("username", User);

    QUrl serviceUrl = toolGetApiUrl("/api/v1.0/user");
    serviceUrl.setQuery(postData);

    QNetworkRequest request(serviceUrl);
    QNetworkAccessManager ApiManager;
    QNetworkReply *Reply = toolGetApiReply(&request, &ApiManager, true);

    if (Reply->error() == QNetworkReply::NoError)
        d->readUserJson(Reply->readAll());
    else
    {
        qDebug() << Reply->errorString()
                 << Reply->readAll();
    }

    Reply->close();
    delete Reply;

    return d->m_isValid;
}

bool ApiUser::isValid() const
{
    Q_D(const ApiUser);
    return d->m_isValid;
}

const QString &ApiUser::userName() const
{
    Q_D(const ApiUser);
    return d->m_UserName;
}

const QString &ApiUser::email() const
{
    Q_D(const ApiUser);
    return d->m_Email;
}

const QString &ApiUser::domain() const
{
    Q_D(const ApiUser);
    return d->m_Domain;
}

const qint32 &ApiUser::id() const
{
    Q_D(const ApiUser);
    return d->m_Id;
}

const QDateTime &ApiUser::createDate() const
{
    Q_D(const ApiUser);
    return d->m_CreateDate;
}
