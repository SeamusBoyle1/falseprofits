// Copyright 2017

#ifndef BSMI_IINVESTORAPICLIENT_H
#define BSMI_IINVESTORAPICLIENT_H

#include <QObject>
#include <QHash>
#include <QNetworkReply>
#include <QPointer>

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

class INetworkReply;

class IInvestorAPIClient : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    /*!
     * Returns the authentication token token. If no token was set
     * an empty QString is returned.
     *
     * \sa setAuthToken()
     */
    virtual QString authToken() = 0;

    /*!
     * Returns the authentication token's expiry date-time. If no
     * expiry date-time was set an invalid QDateTime is returned.
     *
     * \sa authToken(), setAuthToken()
     */
    virtual QDateTime expiry() = 0;

    /*!
     * Sets the authentication token to \a token and the expiry
     * date-time to \a expiry.
     *
     * \sa authToken(), expiry()
     */
    virtual void setAuthToken(const QString &token, const QDateTime &expiry) = 0;

    /*!
     * Sends a request to the InvestorAPI server to create a new user
     * account using the values specified in \a params, and returns
     * a new INetworkReply object used for reading the response
     * send by the server.
     */
    virtual INetworkReply *createNewUser(const QHash<UserRecordField, QVariant> &params) = 0;

    /*!
     * Sends a request to the InvestorAPI server to retrieve an
     * authentication token for the user, and returns
     * a new INetworkReply object used for reading the response
     * send by the server.
     */
    virtual INetworkReply *authenticate(const QString &email, const QString &password) = 0;

    /*!
     * Sends a request to the InvestorAPI server to delete the
     * current user, and returns
     * a new INetworkReply object used for reading the response
     * send by the server.
     *
     * \sa authToken()
     */
    virtual INetworkReply *deleteUser() = 0;

    /*!
     * Sends a request to the InvestorAPI server to get the
     * current user's user profile, and returns
     * a new INetworkReply object used for reading the response
     * send by the server.
     */
    virtual INetworkReply *getUserProfile() = 0;

    struct EditUserArgs
    {
        QString displayName;
        QString email;
    };
    /*!
     * Sends a request to the InvestorAPI server to edit the current
     * user's user profile, and returns
     * a new INetworkReply object used for reading the response
     * send by the server.
     *
     * All values in \a args are optional.
     */
    virtual INetworkReply *editUserProfile(const EditUserArgs &args) = 0;

    enum class CommissionSide {
        Buy,
        Sell,
    };
    /*!
     * Sends a request to the InvestorAPI server to get the current
     * Buy and Sell order commissions, and returns
     * a new INetworkReply object used for reading the response
     * send by the server.
     */
    virtual INetworkReply *getCommissions(CommissionSide side) = 0;

    /*!
     * Sends a request to the InvestorAPI server to get the current
     * user's open positions for the account specified by
     * \a accountId, and returns a new INetworkReply object used for
     * reading the response send by the server.
     */
    virtual INetworkReply *getPositions(const QString &accountId) = 0;

    /*!
     * Sends a request to the InvestorAPI server to reset the current
     * the current users trading account specified by \a accountId,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     */
    virtual INetworkReply *resetAccount(const QString &accountId) = 0;

    struct GetTransactionsArgs
    {
        QString accountId;
        QDateTime startDate;
        QDateTime endDate;
        int pageNumber{ -1 };
        int pageSize{ -1 };
    };
    /*!
     * Sends a request to the InvestorAPI server to get a list of
     * transactions that match the query \a query,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     *
     * Only the GetTransactionsArgs::accountId is required all other
     * values are optional.
     */
    virtual INetworkReply *getTransactions(const GetTransactionsArgs &query) = 0;

    /*!
     * Sends a request to the InvestorAPI server to get current
     * quotes for each symbol specified by \a symbols,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     */
    virtual INetworkReply *getQuotes(const QStringList &symbols) = 0;

    /*!
     * Sends a request to the InvestorAPI server to get current
     * fundamental quotes for the symbol \a symbols,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     */
    virtual INetworkReply *getFundamentals(const QString &symbol) = 0;

    /*!
     * Sends a request to the InvestorAPI server to get historial
     * dividends for the symbol \a symbols for the range \a range,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     *
     * Refer to InvestorAPI documentation for valid range values.
     */
    virtual INetworkReply *getDividends(const QString &symbol, const QString &range) = 0;

    struct CandlesRequestArgs
    {
        QString symbol;
        QDateTime startTime;
        QDateTime endTime;
        QString interval;
        QString range;
    };
    /*!
     * Sends a request to the InvestorAPI server to get price history
     * for the symbol specified in \a args,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     *
     * Refer to InvestorAPI documentation for valid interval and
     * range values.
     */
    virtual INetworkReply *getCandles(const CandlesRequestArgs &args) = 0;

    struct SymbolSearchQuery
    {
        QString searchTerm;
        int pageNumber{ -1 };
        int pageSize{ -1 };
    };
    /*!
     * Sends a request to the InvestorAPI server to search for shares
     * that have a symbol or company name that matches the query
     * \a query,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     *
     * The SymbolSearchQuery::searchTerm should not be empty, all other
     * values are optional.
     */
    virtual INetworkReply *symbolSearch(const SymbolSearchQuery &query) = 0;

    struct OrderParams
    {
        QString symbol;
        QString side;
        qint64 nonce{ 0 };
        int quantity{ 0 };
    };
    /*!
     * Sends a request to the InvestorAPI server to fill the order
     * specified by \a args in the account \a accountId,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     *
     * Refer to InvestorAPI documentation for use of \c nonce
     */
    virtual INetworkReply *sendOrder(const QString &accountId, const OrderParams &args) = 0;

    /*!
     * Sends a request to the InvestorAPI server to retrieve the
     * watchlist specified by \a watchlistId,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     */
    virtual INetworkReply *getWatchlist(const QString &watchlistId) = 0;

    /*!
     * Sends a request to the InvestorAPI server to add the symbol
     * \a symbol to the watchlist with ID \a watchlistId,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     */
    virtual INetworkReply *addSymbolToWatchlist(const QString &watchlistId,
                                                const QString &symbol) = 0;

    /*!
     * Sends a request to the InvestorAPI server to remove the symbol
     * \a symbol from the watchlist with ID \a watchlistId,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     */
    virtual INetworkReply *removeSymbolFromWatchlist(const QString &watchlistId,
                                                     const QString &symbol) = 0;

    struct LeaderboardQuery
    {
        int pageNumber{ -1 };
        int pageSize{ -1 };
    };
    /*!
     * Sends a request to the InvestorAPI server to query the
     * Leaderboard,
     * and returns a new INetworkReply object used for
     * reading the response send by the server.
     */
    virtual INetworkReply *getLeaderboard(const LeaderboardQuery &query) = 0;

    /*!
     * Sends a request to the InvestorAPI server to get the
     * current users Leaderboard position, the \a neighborCount
     * argument will include users neibouring the current
     * user. Returns a new INetworkReply object used for
     * reading the response send by the server.
     *
     * Use neighborCount = -1 for server default.
     */
    virtual INetworkReply *getLeaderboardMe(int neighborCount) = 0;

Q_SIGNALS:
    void authTokenChanged();
};

class INetworkReply : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    explicit INetworkReply(QNetworkReply *rep)
        : m_networkReply{ rep }
        , m_operation{ rep->operation() }
    {
        connect(m_networkReply, &QNetworkReply::finished, this, &INetworkReply::setFinished);
    }

    QNetworkAccessManager::Operation operation() const { return m_operation; }

    QNetworkReply::NetworkError error() const
    {
        return m_networkReply ? m_networkReply->error() : QNetworkReply::NoError;
    }

    bool isFinished() const { return m_finished; }

    QByteArray readAll() { return m_networkReply ? m_networkReply->readAll() : QByteArray(); }

    QString errorString() const
    {
        return m_networkReply ? m_networkReply->errorString() : QString();
    }

    QVariant attribute(QNetworkRequest::Attribute code) const
    {
        return m_networkReply ? m_networkReply->attribute(code) : QVariant();
    }

    // internal
    void setNetworkReply(QNetworkReply *rep)
    {
        if (m_networkReply) {
            disconnect(m_networkReply, &QNetworkReply::finished, this, &INetworkReply::setFinished);
        }
        m_networkReply = rep;
        if (rep) {
            m_operation = rep->operation();

            connect(rep, &QNetworkReply::finished, this, &INetworkReply::setFinished);
        } else {
            m_operation = QNetworkAccessManager::UnknownOperation;
        }
    }

Q_SIGNALS:
    void finished();

public Q_SLOTS:
    void setFinished()
    {
        if (!m_finished) {
            m_finished = true;
            Q_EMIT finished();
        }
    }

private:
    QPointer<QNetworkReply> m_networkReply;
    QNetworkAccessManager::Operation m_operation{ QNetworkAccessManager::UnknownOperation };
    bool m_finished{ false };
};

class IRequestQueue : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    /*!
     * Posts a HTTP GET request to the URL specified by
     * \a request and returns a new INetworkReply object
     * used for reading the response send by the server.
     */
    virtual INetworkReply *get(const QNetworkRequest &request) = 0;

    /*!
     * Posts a HTTP POST request to the URL specified by
     * \a request and returns a new INetworkReply object
     * used for reading the response send by the server.
     * The contents of \a jsonObject will be sent to the
     * server.
     */
    virtual INetworkReply *post(const QNetworkRequest &request, const QJsonObject &jsonObject) = 0;

    /*!
     * Posts a HTTP PUT request to the URL specified by
     * \a request and returns a new INetworkReply object
     * used for reading the response send by the server.
     * The contents of \a jsonObject will be sent to the
     * server.
     */
    virtual INetworkReply *put(const QNetworkRequest &request, const QJsonObject &jsonObject) = 0;

    /*!
     * Posts a HTTP PUT request to the URL specified by
     * \a request and returns a new INetworkReply object
     * used for reading the response send by the server.
     */
    virtual INetworkReply *put(const QNetworkRequest &request) = 0;

    /*!
     * Posts a HTTP PUT request to the URL specified by
     * \a request and returns a new INetworkReply object
     * used for reading the response send by the server.
     */
    virtual INetworkReply *deleteResource(const QNetworkRequest &request) = 0;
};

class InvestorAPIClientFactory
{
public:
    static IInvestorAPIClient *create(const QString &apiUrl, QObject *parent = nullptr);
};

} // namespace bsmi

#endif // BSMI_IINVESTORAPICLIENT_H
