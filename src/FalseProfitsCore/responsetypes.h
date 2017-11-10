// Copyright 2017

#ifndef RESPONSETYPES_H
#define RESPONSETYPES_H

#include <QObject>

#include <QJsonDocument>
#include <QJsonObject>

// TODO(seamus): Rewrite response types

class BaseResponse : public QObject
{
    Q_OBJECT
public:
    BaseResponse() {}
    virtual ~BaseResponse() {}

    bool isFinished() const { return m_finished; }

    // TODO(seamus): Return enum of error type instead of bool
    Q_INVOKABLE
    bool hasError() const { return m_httpStatusCode < 200 || m_httpStatusCode > 299; }
    Q_INVOKABLE
    int httpStatusCode() const { return m_httpStatusCode; }
    void setHttpStatusCode(int httpStatusCode) { m_httpStatusCode = httpStatusCode; }

    Q_INVOKABLE
    QString errorMessage() const { return m_errorMessage; }
    void setErrorMessage(const QString &errorMessage) { m_errorMessage = errorMessage; }

    Q_INVOKABLE
    QByteArray payload() const { return m_payload; }
    void setPayload(const QByteArray &payload) { m_payload = payload; }

    // TODO(seamus): Extract reason text
    Q_INVOKABLE
    virtual QString getHttpStatusReason(int httpStatusCode) const;

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
    bool m_finished{ false };
    int m_httpStatusCode{ 0 };
    QString m_errorMessage;
    QByteArray m_payload;
};

inline QString BaseResponse::getHttpStatusReason(int httpStatusCode) const
{
    Q_UNUSED(httpStatusCode)
    return QString();
}

class NewUserResponse : public BaseResponse
{
    Q_OBJECT
public:
    NewUserResponse() {}
    virtual ~NewUserResponse() {}
};

class AuthenticateResponse : public BaseResponse
{
    Q_OBJECT
public:
    AuthenticateResponse() {}
    virtual ~AuthenticateResponse() {}
};

class DeleteUserResponse : public BaseResponse
{
    Q_OBJECT
public:
    DeleteUserResponse() {}
    virtual ~DeleteUserResponse() {}
};

class GetUserProfileResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetUserProfileResponse() {}
    virtual ~GetUserProfileResponse() {}
};

class EditUserProfileResponse : public BaseResponse
{
    Q_OBJECT
public:
    EditUserProfileResponse() {}
    virtual ~EditUserProfileResponse() {}
};

class GetCommissionsResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetCommissionsResponse() {}
    virtual ~GetCommissionsResponse() {}
};

class GetPositionsResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetPositionsResponse() {}
    virtual ~GetPositionsResponse() {}
};

class ResetAccountResponse : public BaseResponse
{
    Q_OBJECT
public:
    ResetAccountResponse() {}
    virtual ~ResetAccountResponse() {}
};

class GetTransactionsResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetTransactionsResponse() {}
    virtual ~GetTransactionsResponse() {}
};

class GetQuotesResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetQuotesResponse() {}
    virtual ~GetQuotesResponse() {}
};

class GetFundamentalsResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetFundamentalsResponse() {}
    virtual ~GetFundamentalsResponse() {}
};

class GetDividendsResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetDividendsResponse() {}
    virtual ~GetDividendsResponse() {}
};

class GetCandlesResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetCandlesResponse() {}
    virtual ~GetCandlesResponse() {}
};

class SymbolSearchResponse : public BaseResponse
{
    Q_OBJECT
public:
    SymbolSearchResponse() {}
    virtual ~SymbolSearchResponse() {}
};

class GetShareDetailsResponse : public BaseResponse
{
    Q_OBJECT
    Q_PROPERTY(QString symbol READ symbol WRITE setSymbol NOTIFY symbolChanged)
    Q_PROPERTY(QString companyName READ companyName WRITE setCompanyName NOTIFY companyNameChanged)
    Q_PROPERTY(QString industry READ industry WRITE setIndustry NOTIFY industryChanged)
public:
    GetShareDetailsResponse() {}
    virtual ~GetShareDetailsResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        switch (httpStatusCode) {
        case 404:
            // Custom error, set locally if the search
            // results don't contain exact symbol match
            return tr("Symbol not found: %1").arg(m_symbol);
        default:
            break;
        }
        return QString();
    }

    QString symbol() const { return m_symbol; }

    QString companyName() const { return m_companyName; }

    QString industry() const { return m_industry; }

public slots:
    void setSymbol(QString symbol)
    {
        if (m_symbol == symbol)
            return;

        m_symbol = symbol;
        emit symbolChanged(m_symbol);
    }

    void setCompanyName(QString companyName)
    {
        if (m_companyName == companyName)
            return;

        m_companyName = companyName;
        emit companyNameChanged(m_companyName);
    }

    void setIndustry(QString industry)
    {
        if (m_industry == industry)
            return;

        m_industry = industry;
        emit industryChanged(m_industry);
    }

signals:
    void symbolChanged(QString symbol);

    void companyNameChanged(QString companyName);

    void industryChanged(QString industry);

private:
    QString m_symbol;
    QString m_companyName;
    QString m_industry;
};

class SendOrderResponse : public BaseResponse
{
    Q_OBJECT
public:
    SendOrderResponse() {}
    virtual ~SendOrderResponse() {}
};

class GetWatchlistResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetWatchlistResponse() {}
    virtual ~GetWatchlistResponse() {}
};

class AddSymbolToWatchlistResponse : public BaseResponse
{
    Q_OBJECT
public:
    AddSymbolToWatchlistResponse() {}
    virtual ~AddSymbolToWatchlistResponse() {}
};

class RemoveSymbolFromWatchlistResponse : public BaseResponse
{
    Q_OBJECT
public:
    RemoveSymbolFromWatchlistResponse() {}
    virtual ~RemoveSymbolFromWatchlistResponse() {}
};

class GetLeaderboardResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetLeaderboardResponse() {}
    virtual ~GetLeaderboardResponse() {}
};

class GetLeaderboardMeResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetLeaderboardMeResponse() {}
    virtual ~GetLeaderboardMeResponse() {}
};

class GetPredictionsResponse : public BaseResponse
{
    Q_OBJECT
    Q_PROPERTY(QString symbol READ symbol WRITE setSymbol NOTIFY symbolChanged)
public:
    GetPredictionsResponse() {}
    virtual ~GetPredictionsResponse() {}

    QString symbol() const { return m_symbol; }

public slots:
    void setSymbol(QString symbol)
    {
        if (m_symbol == symbol)
            return;

        m_symbol = symbol;
        emit symbolChanged(m_symbol);
    }

signals:
    void symbolChanged(QString symbol);

private:
    QString m_symbol;
};

#endif // RESPONSETYPES_H
