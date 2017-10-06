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

    virtual QString authToken() = 0;
    virtual QDateTime expiry() = 0;
    virtual void setAuthToken(const QString &token, const QDateTime &expiry) = 0;

    virtual INetworkReply *createNewUser(const QHash<UserRecordField, QVariant> &params) = 0;

    virtual INetworkReply *authenticate(const QString &email, const QString &password) = 0;

    virtual INetworkReply *deleteUser() = 0;

    virtual INetworkReply *getUserProfile() = 0;

    enum class CommissionSide {
        Buy,
        Sell,
    };
    virtual INetworkReply *getCommissions(CommissionSide side) = 0;

    virtual INetworkReply *getPositions(const QString &accountId) = 0;

    struct GetTransactionsArgs
    {
        QString accountId;
        QDateTime startDate;
        QDateTime endDate;
        int pageNumber{ -1 };
        int pageSize{ -1 };
    };
    virtual INetworkReply *getTransactions(const GetTransactionsArgs &query) = 0;

    virtual INetworkReply *getQuotes(const QStringList &symbols) = 0;

    struct CandlesRequestArgs
    {
        QString symbol;
        QDateTime startTime;
        QDateTime endTime;
        QString interval;
        QString range;
    };
    virtual INetworkReply *getCandles(const CandlesRequestArgs &args) = 0;

    struct SymbolSearchQuery
    {
        QString searchTerm;
        int pageNumber{ -1 };
        int pageSize{ -1 };
    };
    virtual INetworkReply *symbolSearch(const SymbolSearchQuery &query) = 0;

    struct OrderParams
    {
        QString symbol;
        QString side;
        qint64 nonce{ 0 };
        int quantity{ 0 };
    };
    virtual INetworkReply *sendOrder(const QString &accountId, const OrderParams &args) = 0;

    virtual INetworkReply *getWatchlist(const QString &watchlistId) = 0;

    virtual INetworkReply *addSymbolToWatchlist(const QString &watchlistId,
                                                const QString &symbol) = 0;

    virtual INetworkReply *removeSymbolFromWatchlist(const QString &watchlistId,
                                                     const QString &symbol) = 0;

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

    virtual INetworkReply *get(const QNetworkRequest &request) = 0;
    virtual INetworkReply *post(const QNetworkRequest &request, const QJsonObject &jsonObject) = 0;
    virtual INetworkReply *deleteResource(const QNetworkRequest &request) = 0;
};

class InvestorAPIClientFactory
{
public:
    static IInvestorAPIClient *create(const QString &apiUrl, QObject *parent = nullptr);
};

} // namespace bsmi

#endif // BSMI_IINVESTORAPICLIENT_H
