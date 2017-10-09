// Copyright 2017

#include "investorapiclient.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>

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

INetworkReply *InvestorAPIClient::getCommissions(IInvestorAPIClient::CommissionSide side)
{
    return m_requestQueue->get(createGetCommissionsRequest(side));
}

INetworkReply *InvestorAPIClient::getPositions(const QString &accountId)
{
    return m_requestQueue->get(createGetPositionsRequest(accountId));
}

INetworkReply *
InvestorAPIClient::getTransactions(const IInvestorAPIClient::GetTransactionsArgs &query)
{
    return m_requestQueue->get(createGetTransactionsRequest(query));
}

INetworkReply *InvestorAPIClient::getQuotes(const QStringList &symbols)
{
    return m_requestQueue->get(createGetQuotesRequest(symbols));
}

INetworkReply *InvestorAPIClient::getCandles(const CandlesRequestArgs &args)
{
    return m_requestQueue->get(createGetCandlesRequest(args));
}

INetworkReply *InvestorAPIClient::symbolSearch(const SymbolSearchQuery &query)
{
    return m_requestQueue->get(createSymbolSearchRequest(query));
}

INetworkReply *InvestorAPIClient::sendOrder(const QString &accountId,
                                            const IInvestorAPIClient::OrderParams &args)
{
    auto r = createSendOrderRequest(accountId, args);
    return m_requestQueue->post(r.first, r.second);
}

INetworkReply *InvestorAPIClient::getWatchlist(const QString &watchlistId)
{
    return m_requestQueue->get(createGetWatchlist(watchlistId));
}

INetworkReply *InvestorAPIClient::addSymbolToWatchlist(const QString &watchlistId,
                                                       const QString &symbol)
{
    auto r = createAddSymbolToWatchlistRequest(watchlistId, symbol);
    return m_requestQueue->post(r.first, r.second);
}

INetworkReply *InvestorAPIClient::removeSymbolFromWatchlist(const QString &watchlistId,
                                                            const QString &symbol)
{
    return m_requestQueue->deleteResource(createRemoveSymbolFromWatchlistRequest(watchlistId,
                                                                                 symbol));
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

QNetworkRequest InvestorAPIClient::createGetCommissionsRequest(CommissionSide side) const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/commissions/")
             + (side == CommissionSide::Buy ? QStringLiteral("buy") : QStringLiteral("sell")));
    return makeAuthenticatedRequest(url);
}

QNetworkRequest InvestorAPIClient::createGetPositionsRequest(const QString &accountId) const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/accounts/") + accountId);
    return makeAuthenticatedRequest(url);
}

QNetworkRequest InvestorAPIClient::createGetTransactionsRequest(
    const IInvestorAPIClient::GetTransactionsArgs &query) const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/accounts/") + query.accountId + "/transactions");

    QUrlQuery urlQuery;

    if (query.startDate.isValid()) {
        urlQuery.addQueryItem(QStringLiteral("startDate"),
                              query.startDate.toUTC().toString(Qt::ISODate));
    }
    if (query.endDate.isValid()) {
        urlQuery.addQueryItem(QStringLiteral("endDate"),
                              query.endDate.toUTC().toString(Qt::ISODate));
    }
    if (query.pageNumber > 0) {
        urlQuery.addQueryItem(QStringLiteral("pageNumber"), QString::number(query.pageNumber));
    }
    if (query.pageSize > 0) {
        urlQuery.addQueryItem(QStringLiteral("pageSize"), QString::number(query.pageSize));
    }

    url.setQuery(urlQuery);

    return makeAuthenticatedRequest(url);
}

QNetworkRequest InvestorAPIClient::createGetWatchlist(const QString &watchlistId) const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/watchlists/") + watchlistId);
    return makeAuthenticatedRequest(url);
}

QPair<QNetworkRequest, QJsonObject> InvestorAPIClient::createAddSymbolToWatchlistRequest(
        const QString &watchlistId, const QString &symbol) const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/watchlists/") + watchlistId
             + QStringLiteral("/shares"));
    auto req = makeAuthenticatedRequest(url);
    QJsonObject obj;
    obj.insert(QStringLiteral("symbol"), symbol);

    return qMakePair(req, obj);
}

QNetworkRequest InvestorAPIClient::createRemoveSymbolFromWatchlistRequest(
        const QString &watchlistId, const QString &symbol) const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/watchlists/") + watchlistId
                                       + QStringLiteral("/shares/") + symbol);
    return makeAuthenticatedRequest(url);
}

QNetworkRequest InvestorAPIClient::createGetCandlesRequest(const CandlesRequestArgs &args) const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/shares/") + args.symbol + "/prices");

    QUrlQuery urlQuery;

    if (args.startTime.isValid()) {
        urlQuery.addQueryItem(QStringLiteral("startTime"), args.startTime.toString(Qt::ISODate));
    }
    if (args.endTime.isValid()) {
        urlQuery.addQueryItem(QStringLiteral("endTime"), args.endTime.toString(Qt::ISODate));
    }
    if (!args.interval.isEmpty()) {
        urlQuery.addQueryItem(QStringLiteral("interval"), args.interval);
    }
    if (!args.range.isEmpty()) {
        urlQuery.addQueryItem(QStringLiteral("range"), args.range);
    }

    url.setQuery(urlQuery);

    return makeAuthenticatedRequest(url);
}

QNetworkRequest InvestorAPIClient::createGetQuotesRequest(const QStringList &symbols) const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/shares/quotes"));

    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("symbols"), symbols.join(QLatin1Char(',')));

    url.setQuery(urlQuery);

    return makeAuthenticatedRequest(url);
}

QNetworkRequest InvestorAPIClient::createSymbolSearchRequest(const SymbolSearchQuery &query) const
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/shares"));

    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("searchTerm"), query.searchTerm);
    if (query.pageNumber > 0) {
        urlQuery.addQueryItem(QStringLiteral("pageNumber"), QString::number(query.pageNumber));
    }
    if (query.pageSize > 0) {
        urlQuery.addQueryItem(QStringLiteral("pageSize"), QString::number(query.pageSize));
    }

    url.setQuery(urlQuery);

    return makeAuthenticatedRequest(url);
}

QPair<QNetworkRequest, QJsonObject>
InvestorAPIClient::createSendOrderRequest(const QString &accountId,
                                          const IInvestorAPIClient::OrderParams &args)
{
    QUrl url(m_apiUrl + QStringLiteral("/api/1.0/accounts/") + accountId
             + QStringLiteral("/orders"));
    auto req = makeAuthenticatedRequest(url);

    QJsonObject obj;
    obj.insert(QStringLiteral("symbol"), args.symbol);
    obj.insert(QStringLiteral("side"), args.side);
    obj.insert(QStringLiteral("quantity"), args.quantity);
    obj.insert(QStringLiteral("nonce"), args.nonce);

    return qMakePair(req, obj);
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
