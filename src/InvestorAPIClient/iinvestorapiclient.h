// Copyright 2017

#ifndef BSMI_IINVESTORAPICLIENT_H
#define BSMI_IINVESTORAPICLIENT_H

#include <QObject>
#include <QHash>

class QDateTime;
class QJsonObject;
class QNetworkReply;
class QNetworkRequest;

namespace bsmi {

enum class UserRecordField : quint8 {
    DisplayName,
    Email,
    Password,
};

inline uint qHash(UserRecordField field, uint seed = 0)
{
    return QT_PREPEND_NAMESPACE(qHash)(quint8(field), seed);
}

class IInvestorAPIClient : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    virtual QString authToken() = 0;
    virtual QDateTime expiry() = 0;
    virtual void setAuthToken(const QString &token, const QDateTime &expiry) = 0;

    virtual QNetworkReply *createNewUser(const QHash<UserRecordField, QVariant> &params) = 0;

    virtual QNetworkReply *authenticate(const QString &email, const QString &password) = 0;

    virtual QNetworkReply *deleteUser() = 0;

    virtual QNetworkReply *getUserProfile() = 0;

Q_SIGNALS:
    void authTokenChanged();
};

class IRequestQueue : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    virtual QNetworkReply *get(const QNetworkRequest &request) = 0;
    virtual QNetworkReply *post(const QNetworkRequest &request, const QJsonObject &jsonObject) = 0;
    virtual QNetworkReply *deleteResource(const QNetworkRequest &request) = 0;
};

class InvestorAPIClientFactory
{
public:
    static IInvestorAPIClient *create(QObject *parent = nullptr);
};

} // namespace bsmi

#endif // BSMI_IINVESTORAPICLIENT_H
