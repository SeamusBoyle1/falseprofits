// Copyright 2017

#ifndef FPDECLARATIVETYPES_H
#define FPDECLARATIVETYPES_H

#include <QObject>

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

Q_DECLARE_METATYPE(NewUserDetails)
Q_DECLARE_METATYPE(JsonUserDetails)

class FalseProfitsDeclarativeTypes : public QObject
{
    Q_OBJECT
public:
    explicit FalseProfitsDeclarativeTypes(QObject *parent = nullptr)
        : QObject(parent)
    {
        qRegisterMetaType<NewUserDetails>("NewUserDetails");
        qRegisterMetaType<JsonUserDetails>("JsonUserDetails");
    }

    Q_INVOKABLE
    NewUserDetails makeNewUserDetails() { return NewUserDetails{}; }

    Q_INVOKABLE
    JsonUserDetails makeJsonUserDetails(const QByteArray &json)
    {
        return { QJsonDocument::fromJson(json).object() };
    }
};

#endif // FPDECLARATIVETYPES_H
