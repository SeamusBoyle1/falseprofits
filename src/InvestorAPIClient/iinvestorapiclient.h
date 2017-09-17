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
    static IInvestorAPIClient *create(QObject *parent = nullptr);
};

} // namespace bsmi

#endif // BSMI_IINVESTORAPICLIENT_H
