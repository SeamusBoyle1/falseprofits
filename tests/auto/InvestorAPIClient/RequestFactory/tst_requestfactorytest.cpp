// Copyright 2017

#include <QString>
#include <QtTest>

#include <InvestorAPIClient/investorapiclient.h>

#include <QUrlQuery>

class RequestFactoryTest : public QObject
{
    Q_OBJECT

public:
    RequestFactoryTest();

private Q_SLOTS:
    void createCreateNewUserRequestTest();
    void createAuthenticateRequestTest();
    void createDeleteUserRequestTest();
    void createGetUserProfileRequestTest();
    void createGetQuotesRequestTest();
};

RequestFactoryTest::RequestFactoryTest()
{
}

void RequestFactoryTest::createCreateNewUserRequestTest()
{
    using namespace bsmi;

    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 12), QTime(2, 48)));

        QHash<UserRecordField, QVariant> userA;
        userA.insert(UserRecordField::DisplayName, QLatin1String("Bill"));
        userA.insert(UserRecordField::Email, QLatin1String("bill@example.com"));
        userA.insert(UserRecordField::Password, QLatin1String("p@ssword"));

        auto resp = c.createCreateNewUserRequest(userA);
        QCOMPARE(resp.first.url(), QUrl(QLatin1String("http://example.com/api/1.0/users")));
        QVERIFY(resp.first.rawHeader("Authorization").isEmpty());
        QCOMPARE(resp.second.value("displayName").toString(), QString(QLatin1String("Bill")));
        QCOMPARE(resp.second.value("email").toString(), QString(QLatin1String("bill@example.com")));
        QCOMPARE(resp.second.value("password").toString(), QString(QLatin1String("p@ssword")));
    }
}

void RequestFactoryTest::createAuthenticateRequestTest()
{
    using namespace bsmi;

    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 12), QTime(2, 48)));

        auto resp = c.createAuthenticateRequest(QLatin1String("bill@example.com"),
                                                QLatin1String("p@ssword"));
        QCOMPARE(resp.first.url(), QUrl(QLatin1String("http://example.com/api/1.0/token")));
        QVERIFY(resp.first.rawHeader("Authorization").isEmpty());
        QCOMPARE(resp.second.value("email").toString(), QString(QLatin1String("bill@example.com")));
        QCOMPARE(resp.second.value("password").toString(), QString(QLatin1String("p@ssword")));
    }
}

void RequestFactoryTest::createDeleteUserRequestTest()
{
    using namespace bsmi;

    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 12), QTime(2, 48)));

        auto resp = c.createDeleteUserRequest();
        QCOMPARE(resp.url(), QUrl(QLatin1String("http://example.com/api/1.0/users")));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
    }
}

void RequestFactoryTest::createGetUserProfileRequestTest()
{
    using namespace bsmi;

    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 12), QTime(2, 48)));

        auto resp = c.createGetUserProfileRequest();
        QCOMPARE(resp.url(), QUrl(QLatin1String("http://example.com/api/1.0/users")));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
    }
}

void RequestFactoryTest::createGetQuotesRequestTest()
{
    using namespace bsmi;

    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 20), QTime(6, 48)));

        auto symbols = QStringList{ QLatin1String("ASYMBOL1"), QLatin1String("ASYMBOL2"),
                                    QLatin1String("ASYMBOL3") };

        auto resp = c.createGetQuotesRequest(symbols);
        auto const respUrl = resp.url();
        const QUrlQuery respUrlQuery(respUrl.query());
        QCOMPARE(QUrl(respUrl.toString(QUrl::RemoveQuery)),
                 QUrl(QLatin1String("http://example.com/api/1.0/shares/quotes")));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
        QCOMPARE(resp.rawHeader("Content-Type"), QByteArray("application/json"));
        QVERIFY(respUrlQuery.hasQueryItem("symbols"));
        auto respSymbols = respUrlQuery.queryItemValue("symbols").split(QLatin1Char(','));
        QCOMPARE(respSymbols.size(), 3 /* the size if the input symbols, less empty parts */);
    }
}

QTEST_APPLESS_MAIN(RequestFactoryTest)

#include "tst_requestfactorytest.moc"
