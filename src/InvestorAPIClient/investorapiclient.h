// Copyright 2017

#ifndef BSMI_INVESTORAPICLIENT_H
#define BSMI_INVESTORAPICLIENT_H

#include <iinvestorapiclient.h>

#include <QDateTime>
#include <QPair>

class QJsonObject;

namespace bsmi {

class IRequestQueue;

class InvestorAPIClient : public IInvestorAPIClient
{

public:
    explicit InvestorAPIClient(IRequestQueue *requestQueue, const QString &apiUrl,
                               QObject *parent = nullptr);

    QString authToken() override;
    QDateTime expiry() override;
    void setAuthToken(const QString &token, const QDateTime &expiry) override;

    INetworkReply *createNewUser(const QHash<UserRecordField, QVariant> &params) override;

    INetworkReply *authenticate(const QString &email, const QString &password) override;

    INetworkReply *deleteUser() override;

    INetworkReply *getUserProfile() override;

    INetworkReply *getCommissions(CommissionSide side) override;

    INetworkReply *getQuotes(const QStringList &symbols) override;

    INetworkReply *symbolSearch(const SymbolSearchQuery &query) override;

    INetworkReply *sendOrder(const QString &accountId, const OrderParams &args) override;

    // TODO(seamus): Extract a request factory
    QPair<QNetworkRequest, QJsonObject>
    createCreateNewUserRequest(const QHash<UserRecordField, QVariant> &params) const;

    QPair<QNetworkRequest, QJsonObject> createAuthenticateRequest(const QString &email,
                                                                  const QString &password) const;

    QNetworkRequest createDeleteUserRequest() const;

    QNetworkRequest createGetUserProfileRequest() const;

    QNetworkRequest createGetCommissionsRequest(CommissionSide side) const;

    QNetworkRequest createGetQuotesRequest(const QStringList &symbols) const;

    QNetworkRequest createSymbolSearchRequest(const SymbolSearchQuery &query) const;

    QPair<QNetworkRequest, QJsonObject> createSendOrderRequest(const QString &accountId,
                                                               const OrderParams &args);

    QNetworkRequest makeRequest(const QUrl &url) const;
    QNetworkRequest makeAuthenticatedRequest(const QUrl &url) const;
    void applyAuthToken(QNetworkRequest &request) const;
    void applyJsonContentType(QNetworkRequest &request) const;

private:
    IRequestQueue *m_requestQueue;
    QString m_apiUrl;
    QString m_authToken;
    QByteArray m_authTokenBearer;
    QDateTime m_expiryDate;
};

} // namespace bsmi

#endif // BSMI_INVESTORAPICLIENT_H
