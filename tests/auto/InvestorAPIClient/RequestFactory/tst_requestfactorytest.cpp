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
    void createGetCommissionsRequestTest();
    void createGetPositionsRequestTest();
    void createGetTransactionsRequestTest();
    void createGetQuotesRequestTest();
    void createSymbolSearchRequestTest();
    void createSendOrderRequestTest();
    void createGetWatchlistTest();
    void createAddSymbolToWatchlistRequestTest();
    void createRemoveSymbolFromWatchlistRequest();
    void createGetCandlesRequestTest();
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

void RequestFactoryTest::createGetCommissionsRequestTest()
{
    using namespace bsmi;

    // buy side commission
    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 12), QTime(2, 48)));

        auto resp = c.createGetCommissionsRequest(IInvestorAPIClient::CommissionSide::Buy);
        QCOMPARE(resp.url(), QUrl(QLatin1String("http://example.com/api/1.0/commissions/buy")));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
    }

    // sell side commission
    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 12), QTime(2, 48)));

        auto resp = c.createGetCommissionsRequest(IInvestorAPIClient::CommissionSide::Sell);
        QCOMPARE(resp.url(), QUrl(QLatin1String("http://example.com/api/1.0/commissions/sell")));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
    }
}

void RequestFactoryTest::createGetPositionsRequestTest()
{
    using namespace bsmi;

    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 12), QTime(2, 48)));

        auto resp = c.createGetPositionsRequest("fakeAccId");
        QCOMPARE(resp.url(), QUrl(QLatin1String("http://example.com/api/1.0/accounts/fakeAccId")));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
    }
}

void RequestFactoryTest::createGetTransactionsRequestTest()
{
    using namespace bsmi;

    // Without paging
    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 20), QTime(7, 51)));

        IInvestorAPIClient::GetTransactionsArgs query;
        query.accountId = "valid-fake-id";

        auto resp = c.createGetTransactionsRequest(query);
        auto const respUrl = resp.url();
        const QUrlQuery respUrlQuery(respUrl.query());
        QCOMPARE(
            QUrl(respUrl.toString(QUrl::RemoveQuery)),
            QUrl(QLatin1String("http://example.com/api/1.0/accounts/valid-fake-id/transactions")));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
        QCOMPARE(resp.rawHeader("Content-Type"), QByteArray("application/json"));
        QVERIFY(!respUrlQuery.hasQueryItem("pageNumber"));
        QVERIFY(!respUrlQuery.hasQueryItem("pageSize"));
    }

    // With paging
    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 20), QTime(7, 51)));

        IInvestorAPIClient::GetTransactionsArgs query;
        query.accountId = "valid-fake-id";
        query.pageNumber = 2;
        query.pageSize = 20;

        auto resp = c.createGetTransactionsRequest(query);
        auto const respUrl = resp.url();
        const QUrlQuery respUrlQuery(respUrl.query());
        QCOMPARE(
            QUrl(respUrl.toString(QUrl::RemoveQuery)),
            QUrl(QLatin1String("http://example.com/api/1.0/accounts/valid-fake-id/transactions")));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
        QCOMPARE(resp.rawHeader("Content-Type"), QByteArray("application/json"));
        QVERIFY(respUrlQuery.hasQueryItem("pageNumber"));
        QVERIFY(respUrlQuery.queryItemValue("pageNumber") == QLatin1String("2"));
        QVERIFY(respUrlQuery.hasQueryItem("pageSize"));
        QVERIFY(respUrlQuery.queryItemValue("pageSize") == QLatin1String("20"));
    }

    // Without dates in local timespec
    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 20), QTime(7, 51)));

        IInvestorAPIClient::GetTransactionsArgs query;
        query.accountId = "valid-fake-id";
        query.startDate = QDateTime(QDate(2011, 1, 1), QTime(), Qt::LocalTime);
        query.endDate = QDateTime(QDate(2011, 2, 1), QTime(), Qt::LocalTime);

        auto resp = c.createGetTransactionsRequest(query);
        auto const respUrl = resp.url();
        const QUrlQuery respUrlQuery(respUrl.query());
        QCOMPARE(
            QUrl(respUrl.toString(QUrl::RemoveQuery)),
            QUrl(QLatin1String("http://example.com/api/1.0/accounts/valid-fake-id/transactions")));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
        QCOMPARE(resp.rawHeader("Content-Type"), QByteArray("application/json"));
        QVERIFY(!respUrlQuery.hasQueryItem("pageNumber"));
        QVERIFY(!respUrlQuery.hasQueryItem("pageSize"));
        QVERIFY(respUrlQuery.hasQueryItem("startDate"));
        QCOMPARE(QDateTime::fromString(respUrlQuery.queryItemValue("startDate"), Qt::ISODate),
                 query.startDate);
        QVERIFY(respUrlQuery.hasQueryItem("endDate"));
        QCOMPARE(QDateTime::fromString(respUrlQuery.queryItemValue("endDate"), Qt::ISODate),
                 query.endDate);
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

void RequestFactoryTest::createSymbolSearchRequestTest()
{
    using namespace bsmi;

    // Without paging
    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 20), QTime(7, 51)));

        IInvestorAPIClient::SymbolSearchQuery query;
        query.searchTerm = "Hello, World";

        auto resp = c.createSymbolSearchRequest(query);
        auto const respUrl = resp.url();
        const QUrlQuery respUrlQuery(respUrl.query());
        QCOMPARE(QUrl(respUrl.toString(QUrl::RemoveQuery)),
                 QUrl(QLatin1String("http://example.com/api/1.0/shares")));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
        QCOMPARE(resp.rawHeader("Content-Type"), QByteArray("application/json"));
        QVERIFY(respUrlQuery.hasQueryItem("searchTerm"));
        QVERIFY(!respUrlQuery.hasQueryItem("pageNumber"));
        QVERIFY(!respUrlQuery.hasQueryItem("pageSize"));
    }

    // With paging
    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 20), QTime(7, 51)));

        IInvestorAPIClient::SymbolSearchQuery query;
        query.searchTerm = "Hello, World";
        query.pageNumber = 2;
        query.pageSize = 20;

        auto resp = c.createSymbolSearchRequest(query);
        auto const respUrl = resp.url();
        const QUrlQuery respUrlQuery(respUrl.query());
        QCOMPARE(QUrl(respUrl.toString(QUrl::RemoveQuery)),
                 QUrl(QLatin1String("http://example.com/api/1.0/shares")));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
        QCOMPARE(resp.rawHeader("Content-Type"), QByteArray("application/json"));
        QVERIFY(respUrlQuery.hasQueryItem("searchTerm"));
        QVERIFY(respUrlQuery.hasQueryItem("pageNumber"));
        QVERIFY(respUrlQuery.queryItemValue("pageNumber") == QLatin1String("2"));
        QVERIFY(respUrlQuery.hasQueryItem("pageSize"));
        QVERIFY(respUrlQuery.queryItemValue("pageSize") == QLatin1String("20"));
    }
}

void RequestFactoryTest::createSendOrderRequestTest()
{
    using namespace bsmi;

    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 20), QTime(1, 37)));

        QString accountId("myFirstAccount");

        IInvestorAPIClient::OrderParams args;
        args.symbol = "BHP";
        args.side = "Buy";
        args.quantity = 111;
        args.nonce = 1505878737000;

        auto resp = c.createSendOrderRequest(accountId, args);
        QCOMPARE(resp.first.url(),
                 QUrl(QLatin1String("http://example.com/api/1.0/accounts/myFirstAccount/orders")));
        QCOMPARE(resp.first.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
        QCOMPARE(resp.first.rawHeader("Content-Type"), QByteArray("application/json"));
        QCOMPARE(resp.second.value("symbol").toString(), QString(QLatin1String("BHP")));
        QCOMPARE(resp.second.value("side").toString(), QString(QLatin1String("Buy")));
        QCOMPARE(resp.second.value("quantity").toInt(), 111);
        QCOMPARE(resp.second.value("nonce").toDouble(), double(1505878737000));
    }
}

void RequestFactoryTest::createGetWatchlistTest()
{
    using namespace bsmi;

    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        QString watchlistId("i_am_a_valid_watchlist_id");

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 12), QTime(2, 48)));

        auto resp = c.createGetWatchlist(watchlistId);
        QCOMPARE(resp.url(),
                 QUrl(QLatin1String("http://example.com/api/1.0/watchlists/") + watchlistId));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
    }
}

void RequestFactoryTest::createAddSymbolToWatchlistRequestTest()
{
    using namespace bsmi;

    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        QString watchlistId("i_am_a_valid_watchlist_id");
        QString symbol("i_am_a_valid_symbol");

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 12), QTime(2, 48)));

        auto resp = c.createAddSymbolToWatchlistRequest(watchlistId, symbol);
        QCOMPARE(resp.first.url(), QUrl(QLatin1String("http://example.com/api/1.0/watchlists/")
                                        + watchlistId + "/shares"));
        QCOMPARE(resp.first.rawHeader("Authorization"), QByteArray("Bearer fake-token"));

        QCOMPARE(resp.second.value("symbol").toString(), symbol);
    }
}

void RequestFactoryTest::createRemoveSymbolFromWatchlistRequest()
{
    using namespace bsmi;

    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        QString watchlistId("i_am_a_valid_watchlist_id");
        QString symbol("i_am_a_valid_symbol");

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 12), QTime(2, 48)));

        auto resp = c.createRemoveSymbolFromWatchlistRequest(watchlistId, symbol);
        QCOMPARE(resp.url(), QUrl(QLatin1String("http://example.com/api/1.0/watchlists/")
                                  + watchlistId + "/shares/" + symbol));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
    }
}

void RequestFactoryTest::createGetCandlesRequestTest()
{
    using namespace bsmi;

    // With symbol only
    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 20), QTime(7, 51)));

        IInvestorAPIClient::CandlesRequestArgs args;
        args.symbol = QString("i_am_a_valid_symbol");

        auto resp = c.createGetCandlesRequest(args);
        auto const respUrl = resp.url();
        const QUrlQuery respUrlQuery(respUrl.query());
        QCOMPARE(
            QUrl(respUrl.toString(QUrl::RemoveQuery)),
            QUrl(QLatin1String("http://example.com/api/1.0/shares/") + args.symbol + "/prices"));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
        QCOMPARE(resp.rawHeader("Content-Type"), QByteArray("application/json"));

        QVERIFY(!respUrlQuery.hasQueryItem("symbol"));
        QVERIFY(!respUrlQuery.hasQueryItem("startTime"));
        QVERIFY(!respUrlQuery.hasQueryItem("endTime"));
        QVERIFY(!respUrlQuery.hasQueryItem("interval"));
        QVERIFY(!respUrlQuery.hasQueryItem("range"));
    }

    // With range=1d and symbol only
    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 20), QTime(7, 51)));

        IInvestorAPIClient::CandlesRequestArgs args;
        args.symbol = QString("i_am_a_valid_symbol");
        args.range = "1d";

        auto resp = c.createGetCandlesRequest(args);
        auto const respUrl = resp.url();
        const QUrlQuery respUrlQuery(respUrl.query());
        QCOMPARE(
            QUrl(respUrl.toString(QUrl::RemoveQuery)),
            QUrl(QLatin1String("http://example.com/api/1.0/shares/") + args.symbol + "/prices"));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
        QCOMPARE(resp.rawHeader("Content-Type"), QByteArray("application/json"));

        QVERIFY(!respUrlQuery.hasQueryItem("symbol"));
        QVERIFY(!respUrlQuery.hasQueryItem("startTime"));
        QVERIFY(!respUrlQuery.hasQueryItem("endTime"));
        QVERIFY(!respUrlQuery.hasQueryItem("interval"));
        QVERIFY(respUrlQuery.hasQueryItem("range"));
        QVERIFY(respUrlQuery.queryItemValue("range") == QLatin1String("1d"));
    }

    // With interval=1m and symbol only
    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 20), QTime(7, 51)));

        IInvestorAPIClient::CandlesRequestArgs args;
        args.symbol = QString("i_am_a_valid_symbol");
        args.interval = "1m";

        auto resp = c.createGetCandlesRequest(args);
        auto const respUrl = resp.url();
        const QUrlQuery respUrlQuery(respUrl.query());
        QCOMPARE(
            QUrl(respUrl.toString(QUrl::RemoveQuery)),
            QUrl(QLatin1String("http://example.com/api/1.0/shares/") + args.symbol + "/prices"));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
        QCOMPARE(resp.rawHeader("Content-Type"), QByteArray("application/json"));

        QVERIFY(!respUrlQuery.hasQueryItem("symbol"));
        QVERIFY(!respUrlQuery.hasQueryItem("startTime"));
        QVERIFY(!respUrlQuery.hasQueryItem("endTime"));
        QVERIFY(respUrlQuery.hasQueryItem("interval"));
        QVERIFY(respUrlQuery.queryItemValue("interval") == QLatin1String("1m"));
        QVERIFY(!respUrlQuery.hasQueryItem("range"));
    }

    // With startDate, endDate, and symbol only
    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        c.setAuthToken(QLatin1String("fake-token"), QDateTime(QDate(2017, 9, 20), QTime(7, 51)));

        IInvestorAPIClient::CandlesRequestArgs args;
        args.symbol = QString("i_am_a_valid_symbol");
        args.startTime = QDateTime::currentDateTimeUtc();
        args.endTime = args.startTime.addDays(5);

        auto resp = c.createGetCandlesRequest(args);
        auto const respUrl = resp.url();
        const QUrlQuery respUrlQuery(respUrl.query());
        QCOMPARE(
            QUrl(respUrl.toString(QUrl::RemoveQuery)),
            QUrl(QLatin1String("http://example.com/api/1.0/shares/") + args.symbol + "/prices"));
        QCOMPARE(resp.rawHeader("Authorization"), QByteArray("Bearer fake-token"));
        QCOMPARE(resp.rawHeader("Content-Type"), QByteArray("application/json"));

        QVERIFY(!respUrlQuery.hasQueryItem("symbol"));
        QVERIFY(respUrlQuery.hasQueryItem("startTime"));
        QCOMPARE(QDateTime::fromString(respUrlQuery.queryItemValue("startTime"), Qt::ISODate)
                     .toString(Qt::ISODate),
                 args.startTime.toString(Qt::ISODate));
        QVERIFY(respUrlQuery.hasQueryItem("endTime"));
        QCOMPARE(QDateTime::fromString(respUrlQuery.queryItemValue("endTime"), Qt::ISODate)
                     .toString(Qt::ISODate),
                 args.endTime.toString(Qt::ISODate));
        QVERIFY(!respUrlQuery.hasQueryItem("interval"));
        QVERIFY(!respUrlQuery.hasQueryItem("range"));
    }
}

QTEST_APPLESS_MAIN(RequestFactoryTest)

#include "tst_requestfactorytest.moc"
