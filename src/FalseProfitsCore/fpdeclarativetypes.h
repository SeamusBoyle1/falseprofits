// Copyright 2017

#ifndef FPDECLARATIVETYPES_H
#define FPDECLARATIVETYPES_H

#include <QObject>

#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>
#include <QJsonDocument>

class NewUserDetails
{
    Q_GADGET
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName)
    Q_PROPERTY(QString email READ email WRITE setEmail)
    Q_PROPERTY(QString password READ password WRITE setPassword)
public:
    QString displayName() const { return m_displayName; }
    void setDisplayName(QString displayName) { m_displayName = displayName; }

    QString email() const { return m_email; }
    void setEmail(QString email) { m_email = email; }

    QString password() const { return m_password; }
    void setPassword(QString password) { m_password = password; }

private:
    QString m_displayName;
    QString m_email;
    QString m_password;
};

class JsonUserDetails
{
    Q_GADGET
    Q_PROPERTY(QVariant id READ id)
    Q_PROPERTY(QVariant email READ email)
    Q_PROPERTY(QVariant displayName READ displayName)
    Q_PROPERTY(QVariant level READ level)
public:
    QJsonObject m_d;

    QVariant id() const { return m_d.value(QLatin1String("id")).toVariant(); }

    QVariant email() const { return m_d.value(QLatin1String("email")).toVariant(); }

    QVariant displayName() const { return m_d.value(QLatin1String("displayName")).toVariant(); }

    QVariant level() const { return m_d.value(QLatin1String("level")).toVariant(); }
};

class JsonQuotesQuote
{
    Q_GADGET
    Q_PROPERTY(bool isValid READ isValid)
    Q_PROPERTY(QVariant symbol READ symbol)
    Q_PROPERTY(QVariant ask READ ask)
    Q_PROPERTY(QVariant askSize READ askSize)
    Q_PROPERTY(QVariant bid READ bid)
    Q_PROPERTY(QVariant bidSize READ bidSize)
    Q_PROPERTY(QVariant last READ last)
    Q_PROPERTY(QVariant lastSize READ lastSize)
    Q_PROPERTY(QVariant dayLow READ dayLow)
    Q_PROPERTY(QVariant dayHigh READ dayHigh)
public:
    QJsonObject m_d;

    bool isValid() const { return !m_d.isEmpty() && m_d.contains(QLatin1String("symbol")); }

    QVariant symbol() const { return m_d.value(QLatin1String("symbol")).toVariant(); }

    QVariant ask() const { return m_d.value(QLatin1String("ask")).toVariant(); }

    QVariant askSize() const { return m_d.value(QLatin1String("askSize")).toVariant(); }

    QVariant bid() const { return m_d.value(QLatin1String("bid")).toVariant(); }

    QVariant bidSize() const { return m_d.value(QLatin1String("bidSize")).toVariant(); }

    QVariant last() const { return m_d.value(QLatin1String("last")).toVariant(); }

    QVariant lastSize() const { return m_d.value(QLatin1String("lastSize")).toVariant(); }

    QVariant dayLow() const { return m_d.value(QLatin1String("dayLow")).toVariant(); }

    QVariant dayHigh() const { return m_d.value(QLatin1String("dayHigh")).toVariant(); }
};

class JsonQuotes
{
    Q_GADGET
    Q_PROPERTY(bool isEmpty READ isEmpty)
    Q_PROPERTY(int size READ size)
public:
    QJsonArray m_d;

    bool isEmpty() const { return m_d.isEmpty(); }

    int size() const { return m_d.size(); }

    Q_INVOKABLE
    JsonQuotesQuote at(int i) const { return { m_d.at(i).toObject() }; }

    Q_INVOKABLE
    JsonQuotesQuote find(const QString &symbol) const
    {
        auto const symbolJsonVal = QJsonValue(symbol);
        auto it =
            std::find_if(m_d.constBegin(), m_d.constEnd(), [symbolJsonVal](const QJsonValue &a) {
                auto const obj = a.toObject();
                return obj.value(QLatin1String("symbol")) == symbolJsonVal;
            });
        if (it != m_d.constEnd()) {
            return { it->toObject() };
        }
        return JsonQuotesQuote();
    }
};

Q_DECLARE_METATYPE(NewUserDetails)
Q_DECLARE_METATYPE(JsonUserDetails)
Q_DECLARE_METATYPE(JsonQuotesQuote)
Q_DECLARE_METATYPE(JsonQuotes)

class FalseProfitsDeclarativeTypes : public QObject
{
    Q_OBJECT
public:
    explicit FalseProfitsDeclarativeTypes(QObject *parent = nullptr)
        : QObject(parent)
    {
        qRegisterMetaType<NewUserDetails>("NewUserDetails");
        qRegisterMetaType<JsonUserDetails>("JsonUserDetails");
        qRegisterMetaType<JsonQuotesQuote>("JsonQuotesQuote");
        qRegisterMetaType<JsonQuotes>("JsonQuotes");
    }

    Q_INVOKABLE
    NewUserDetails makeNewUserDetails() { return NewUserDetails{}; }

    Q_INVOKABLE
    JsonUserDetails makeJsonUserDetails(const QByteArray &json)
    {
        return { QJsonDocument::fromJson(json).object() };
    }

    Q_INVOKABLE
    JsonQuotes makeJsonQuotes(const QByteArray &json)
    {
        return { QJsonDocument::fromJson(json).array() };
    }
};

#endif // FPDECLARATIVETYPES_H
