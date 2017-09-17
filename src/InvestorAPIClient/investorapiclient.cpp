// Copyright 2017

#include "investorapiclient.h"

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

namespace bsmi {

InvestorAPIClient::InvestorAPIClient(IRequestQueue *requestQueue, const QString &apiUrl,
                                     QObject *parent)
    : IInvestorAPIClient(parent)
    , m_requestQueue{ requestQueue }
    , m_apiUrl{ apiUrl }
{
}

QString InvestorAPIClient::authToken()
{
    return m_authToken;
}

QDateTime InvestorAPIClient::expiry()
{
    return m_expiryDate;
}

void InvestorAPIClient::setAuthToken(const QString &token, const QDateTime &expiry)
{
    if (m_authToken == token) {
        return;
    }

    m_authToken = token;
    m_authTokenBearer = QByteArrayLiteral("Bearer ") + token.toLatin1();
    m_expiryDate = expiry;

    emit authTokenChanged();
}

INetworkReply *InvestorAPIClient::createNewUser(const QHash<UserRecordField, QVariant> &params)
{
    auto r = createCreateNewUserRequest(params);
    return m_requestQueue->post(r.first, r.second);
}

INetworkReply *InvestorAPIClient::authenticate(const QString &email, const QString &password)
{
    auto r = createAuthenticateRequest(email, password);
    return m_requestQueue->post(r.first, r.second);
}

INetworkReply *InvestorAPIClient::deleteUser()
{
    return m_requestQueue->deleteResource(createDeleteUserRequest());
}

INetworkReply *InvestorAPIClient::getUserProfile()
{
    return m_requestQueue->get(createGetUserProfileRequest());
}

QPair<QNetworkRequest, QJsonObject>
InvestorAPIClient::createCreateNewUserRequest(const QHash<UserRecordField, QVariant> &params) const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/users"));
    auto req = makeRequest(url);

    QJsonObject obj;
    obj.insert(QStringLiteral("displayName"),
               params.value(UserRecordField::DisplayName).toString());
    obj.insert(QStringLiteral("email"), params.value(UserRecordField::Email).toString());
    obj.insert(QStringLiteral("password"), params.value(UserRecordField::Password).toString());

    return qMakePair(req, obj);
}

QPair<QNetworkRequest, QJsonObject>
InvestorAPIClient::createAuthenticateRequest(const QString &email, const QString &password) const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/token"));
    auto req = makeRequest(url);

    QJsonObject obj;
    obj.insert(QStringLiteral("email"), email);
    obj.insert(QStringLiteral("password"), password);

    return qMakePair(req, obj);
}

QNetworkRequest InvestorAPIClient::createDeleteUserRequest() const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/users"));
    return makeAuthenticatedRequest(url);
}

QNetworkRequest InvestorAPIClient::createGetUserProfileRequest() const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/users"));
    return makeAuthenticatedRequest(url);
}

QNetworkRequest InvestorAPIClient::makeRequest(const QUrl &url) const
{
    QNetworkRequest r{ url };
    applyJsonContentType(r);
    return r;
}

QNetworkRequest InvestorAPIClient::makeAuthenticatedRequest(const QUrl &url) const
{
    QNetworkRequest r{ url };
    applyAuthToken(r);
    applyJsonContentType(r);
    return r;
}

void InvestorAPIClient::applyAuthToken(QNetworkRequest &request) const
{
    request.setRawHeader(QByteArrayLiteral("Authorization"), m_authTokenBearer);
}

void InvestorAPIClient::applyJsonContentType(QNetworkRequest &request) const
{
    request.setRawHeader(QByteArrayLiteral("Content-Type"), QByteArrayLiteral("application/json"));
}

} // namespace bsmi
