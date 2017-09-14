// Copyright 2017

#ifndef RESPONSETYPES_H
#define RESPONSETYPES_H

#include <QObject>

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
    void setError(const QString &errorMessage) { m_errorMessage = errorMessage; }

    Q_INVOKABLE
    QByteArray payload() const { return m_payload; }
    void setPayload(const QByteArray &payload) { m_payload = payload; }

    Q_INVOKABLE
    QString httpStatusReason() { return getHttpStatusReason(m_httpStatusCode); }
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
        case 400:
            return "Request failed validation.";
        default:
            break;
        }
        return errorMessage();
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
        return errorMessage();
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
        return errorMessage();
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
        return errorMessage();
    }
};

#endif // RESPONSETYPES_H
