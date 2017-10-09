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
    virtual QString getHttpStatusReason(int httpStatusCode) const = 0;

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

class NewUserResponse : public BaseResponse
{
    Q_OBJECT
public:
    NewUserResponse() {}
    virtual ~NewUserResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 201:
            return "New user successfully created.";
        case 400: {
            return "Request failed validation.";
        }
        default:
            break;
        }
        return QString();
    }
};

class AuthenticateResponse : public BaseResponse
{
    Q_OBJECT
public:
    AuthenticateResponse() {}
    virtual ~AuthenticateResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 400:
            return "The request failed validation.";
        case 401:
            return "Authentication failure.";
        default:
            break;
        }
        return QString();
    }
};

class DeleteUserResponse : public BaseResponse
{
    Q_OBJECT
public:
    DeleteUserResponse() {}
    virtual ~DeleteUserResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 204:
            return "User successfully deleted";
        case 401:
            return "User not authenticated.";
        default:
            break;
        }
        return QString();
    }
};

class GetUserProfileResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetUserProfileResponse() {}
    virtual ~GetUserProfileResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 401:
            return "User not authenticated.";
        default:
            break;
        }
        return QString();
    }
};

class GetCommissionsResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetCommissionsResponse() {}
    virtual ~GetCommissionsResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        Q_UNUSED(httpStatusCode);
        // no codes in swagger docs
        return QString();
    }
};

class GetPositionsResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetPositionsResponse() {}
    virtual ~GetPositionsResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 401:
            return "Authorization failed";
        case 404:
            return "Account not found";
        default:
            break;
        }
        return QString();
    }
};

class GetTransactionsResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetTransactionsResponse() {}
    virtual ~GetTransactionsResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 401:
            return "User not authenticated";
        case 403:
            return "User not authorized";
        case 404:
            return "Account not found";
        default:
            break;
        }
        return QString();
    }
};

class GetQuotesResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetQuotesResponse() {}
    virtual ~GetQuotesResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 401:
            return "User not authenticated.";
        default:
            break;
        }
        return QString();
    }
};

class GetCandlesResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetCandlesResponse() {}
    virtual ~GetCandlesResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 401:
            return "Authorization failed";
        case 404:
            return "Share not found.";
        default:
            break;
        }
        return QString();
    }
};

class SymbolSearchResponse : public BaseResponse
{
    Q_OBJECT
public:
    SymbolSearchResponse() {}
    virtual ~SymbolSearchResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 401:
            return "User not authenticated.";
        default:
            break;
        }
        return QString();
    }
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
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 401:
            return "User not authenticated.";
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

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 400:
            return "Invalid order.";
        case 401:
            return "Authorization failed";
        case 404:
            return "Account or share not found";
        default:
            break;
        }
        return QString();
    }
};

class GetWatchlistResponse : public BaseResponse
{
    Q_OBJECT
public:
    GetWatchlistResponse() {}
    virtual ~GetWatchlistResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 401:
            return "Authorization failed";
        case 404:
            return "Watchlist not found";
        default:
            break;
        }
        return QString();
    }
};

class AddSymbolToWatchlistResponse : public BaseResponse
{
    Q_OBJECT
public:
    AddSymbolToWatchlistResponse() {}
    virtual ~AddSymbolToWatchlistResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 201:
            return "Share successfully added to watchlist.";
        case 400:
            return "Request failed validation.";
        case 401:
            return "Authorization failed";
        default:
            break;
        }
        return QString();
    }
};

class RemoveSymbolFromWatchlistResponse : public BaseResponse
{
    Q_OBJECT
public:
    RemoveSymbolFromWatchlistResponse() {}
    virtual ~RemoveSymbolFromWatchlistResponse() {}

    virtual QString getHttpStatusReason(int httpStatusCode) const override
    {
        // TODO(seamus): Define enums for errors
        switch (httpStatusCode) {
        case 201:
            return "Share successfully removed from watchlist.";
        case 400:
            return "Request failed validation.";
        case 401:
            return "Authorization failed";
        default:
            break;
        }
        return QString();
    }
};

#endif // RESPONSETYPES_H
