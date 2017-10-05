// Copyright 2017

#include <QString>
#include <QtTest>

#include <InvestorAPIClient/jsonobjectwrappers.h>

#include <QFile>

inline QString srcDir()
{
    return QString::fromUtf8(SRCDIR);
}

template <typename String>
QString srcDirFile(String filename)
{
    return srcDir() + "/" + filename;
}

template <typename String>
QByteArray readFileContents(String filename)
{
    QFile f(filename);
    return f.open(QIODevice::ReadOnly) ? f.readAll() : QByteArray();
}

template <typename String>
QJsonDocument readFileContentsAsJson(String filename)
{
    auto ba = readFileContents(filename);
    return !ba.isEmpty() ? QJsonDocument::fromJson(ba) : QJsonDocument();
}

class JsonResponseWrappersTest : public QObject
{
    Q_OBJECT

public:
    JsonResponseWrappersTest();

private Q_SLOTS:
    void errorMessageResponseTest();
    void userProfileResponseTest();
    void commissionResponseTest();
    void positionsResponseTest();
    void transactionsResponseTest();
    void symbolSearchTest();
    void watchlistResponseTest();
    void candlesResponseTest();
};

JsonResponseWrappersTest::JsonResponseWrappersTest()
{
}

void JsonResponseWrappersTest::errorMessageResponseTest()
{
    using namespace bsmi;

    {
        QString testFile("errorNewUserDuplicateEmail.json");
        auto doc = readFileContentsAsJson(srcDirFile(testFile));
        if (doc.isNull()) {
            QSKIP("Unable to open test file");
        }

        JsonObjectWrappers::ErrorMessageResponse parser;
        parser.d = doc.object();

        QVERIFY(parser.message());
        QCOMPARE(*parser.message(), QString("There is already a user with this email address."));
    }
}

void JsonResponseWrappersTest::userProfileResponseTest()
{
    using namespace bsmi;

    {
        QString testFile("userInfo.json");
        auto doc = readFileContentsAsJson(srcDirFile(testFile));
        if (doc.isNull()) {
            QSKIP("Unable to open test file");
        }

        JsonObjectWrappers::UserProfileResponse parser;
        parser.d = doc.object();

        QVERIFY(parser.id());
        QCOMPARE(*parser.id(), QString("51d3b0f3-acb3-4f9b-a4f8-9d3d31a420fb"));
        QVERIFY(parser.email());
        QCOMPARE(*parser.email(), QString("seamus@example.com"));
        QVERIFY(parser.displayName());
        QCOMPARE(*parser.displayName(), QString("Seamus"));
        QVERIFY(parser.level());
        QCOMPARE(*parser.level(), QString("Investor"));
    }
}

void JsonResponseWrappersTest::commissionResponseTest()
{
    using namespace bsmi;

    // buy commission
    {
        QString testFile("commissionBuy1.json");
        auto doc = readFileContentsAsJson(srcDirFile(testFile));
        if (doc.isNull()) {
            QSKIP("Unable to open test file");
        }

        JsonObjectWrappers::CommissionsResponse parser;
        parser.d = doc.object();

        QVERIFY(parser.isValid());

        auto fixed = parser.fixed();
        QVERIFY(!fixed.isEmpty());
        QCOMPARE(fixed.size(), 1);

        auto fixedRange1 = fixed.at(0);
        QVERIFY(fixedRange1.min());
        QCOMPARE(*fixedRange1.min(), 0);
        QVERIFY(fixedRange1.max());
        QCOMPARE(*fixedRange1.max(), 1000000);
        QVERIFY(fixedRange1.value());
        QCOMPARE(*fixedRange1.value(), 50.0);

        auto percent = parser.percent();
        QVERIFY(!percent.isEmpty());
        QCOMPARE(percent.size(), 1);

        auto percentRange1 = percent.at(0);
        QVERIFY(percentRange1.min());
        QCOMPARE(*percentRange1.min(), 0);
        QVERIFY(percentRange1.max());
        QCOMPARE(*percentRange1.max(), 1000000);
        QVERIFY(percentRange1.value());
        QCOMPARE(*percentRange1.value(), 1.0);
    }

    // sell commission
    {
        // todo
        QString testFile("commissionSell1.json");
        auto doc = readFileContentsAsJson(srcDirFile(testFile));
        if (doc.isNull()) {
            QSKIP("Unable to open test file");
        }

        JsonObjectWrappers::CommissionsResponse parser;
        parser.d = doc.object();

        QVERIFY(parser.isValid());

        auto fixed = parser.fixed();
        QVERIFY(!fixed.isEmpty());
        QCOMPARE(fixed.size(), 1);

        auto fixedRange1 = fixed.at(0);
        QVERIFY(fixedRange1.min());
        QCOMPARE(*fixedRange1.min(), 0);
        QVERIFY(fixedRange1.max());
        QCOMPARE(*fixedRange1.max(), 1000000);
        QVERIFY(fixedRange1.value());
        QCOMPARE(*fixedRange1.value(), 50.0);

        auto percent = parser.percent();
        QVERIFY(!percent.isEmpty());
        QCOMPARE(percent.size(), 1);

        auto percentRange1 = percent.at(0);
        QVERIFY(percentRange1.min());
        QCOMPARE(*percentRange1.min(), 0);
        QVERIFY(percentRange1.max());
        QCOMPARE(*percentRange1.max(), 1000000);
        QVERIFY(percentRange1.value());
        QCOMPARE(*percentRange1.value(), 0.25);
    }
}

void JsonResponseWrappersTest::positionsResponseTest()
{
    using namespace bsmi;

    {
        QString testFile("accountInfoPositions1.json");
        auto doc = readFileContentsAsJson(srcDirFile(testFile));
        if (doc.isNull()) {
            QSKIP("Unable to open test file");
        }

        JsonObjectWrappers::PositionsResponse parser;
        parser.d = doc.object();

        QCOMPARE(*parser.id(), QString("cbb4bc3e-d3a1-4563-bc19-deb1a9e04918"));
        QCOMPARE(*parser.name(), QString("Default Account"));
        QCOMPARE(*parser.balance(), 928139.5);

        auto itemParser = parser.positions();
        QVERIFY(!itemParser.isEmpty());
        QCOMPARE(itemParser.size(), 2);

        auto foundWBC = false;
        for (auto i = 0, total = 5; i < total; ++i) {
            auto e = itemParser.at(i);
            auto sym = e.symbol();
            if (sym && *sym == QLatin1String("WBC")) {
                foundWBC = true;
                QCOMPARE(*e.name(), QLatin1String("WESTPAC BANKING CORPORATION"));
                QCOMPARE(*e.quantity(), 1000);
                QCOMPARE(*e.averagePrice(), 31.78);
                QCOMPARE(*e.lastPrice(), 31.81);
                QCOMPARE(*e.change(), -0.05);
                QCOMPARE(*e.changePercent(), -0.16);
                break;
            }
        }
        QVERIFY(foundWBC);
    }
}

void JsonResponseWrappersTest::transactionsResponseTest()
{
    using namespace bsmi;

    {
        QString testFile("transactionsOnlyReqArgs.json");
        auto doc = readFileContentsAsJson(srcDirFile(testFile));
        if (doc.isNull()) {
            QSKIP("Unable to open test file");
        }

        auto parser = JsonObjectWrappers::TransactionsResponse(doc.object());

        QCOMPARE(parser.pageNumber(), 1);
        QCOMPARE(parser.pageSize(), 100);
        QCOMPARE(parser.totalPageCount(), 1);
        QCOMPARE(parser.totalRowCount(), 7);

        auto itemParser = parser.items();
        QVERIFY(!itemParser.isEmpty());
        QCOMPARE(itemParser.size(), 7);

        auto foundBHP = false;
        for (auto i = 0, total = 5; i < total; ++i) {
            auto e = itemParser.at(i);
            auto desc = e.description();
            if (desc && *desc == QLatin1String("Purchased 1500 shares of BHP for $26.18 each")) {
                foundBHP = true;
                QVERIFY(e.isValid());
                QVERIFY(e.amount());
                QCOMPARE(*e.amount(), -39270.0);
                QVERIFY(e.balance());
                QCOMPARE(*e.balance(), 928582.2);
                QVERIFY(e.timestampUtc());
                QCOMPARE(*e.timestampUtc(),
                         QDateTime::fromString("2017-10-04T23:49:48.512186", Qt::ISODate));
                QVERIFY(e.type());
                QCOMPARE(*e.type(), QLatin1String("Buy"));
                break;
            }
        }
        QVERIFY(foundBHP);
    }
}

void JsonResponseWrappersTest::symbolSearchTest()
{
    using namespace bsmi;

    {
        QString testFile("symbolSearchBanks5.json");
        auto doc = readFileContentsAsJson(srcDirFile(testFile));
        if (doc.isNull()) {
            QSKIP("Unable to open test file");
        }

        auto parser = JsonObjectWrappers::SymbolSearchResponse(doc.object());

        QCOMPARE(parser.pageNumber(), 1);
        QCOMPARE(parser.pageSize(), 5);
        QCOMPARE(parser.totalPageCount(), 4);
        QCOMPARE(parser.totalRowCount(), 16);

        auto itemParser = parser.items();
        QVERIFY(!itemParser.isEmpty());
        QCOMPARE(itemParser.size(), 5);

        auto foundBEN = false;
        for (auto i = 0, total = 5; i < total; ++i) {
            auto e = itemParser.at(i);
            auto sym = e.symbol();
            if (sym && *sym == QLatin1String("BEN")) {
                foundBEN = true;
                QVERIFY(e.isValid());
                QVERIFY(e.name());
                QCOMPARE(*e.name(), QLatin1String("BENDIGO AND ADELAIDE BANK LIMITED"));
                QVERIFY(e.industry());
                QCOMPARE(*e.industry(), QLatin1String("Banks"));
                break;
            }
        }
        QVERIFY(foundBEN);
    }
}

void JsonResponseWrappersTest::watchlistResponseTest()
{
    using namespace bsmi;

    {
        QString testFile("watchlistDefault1.json");
        auto doc = readFileContentsAsJson(srcDirFile(testFile));
        if (doc.isNull()) {
            QSKIP("Unable to open test file");
        }

        JsonObjectWrappers::WatchlistResponse parser;
        parser.d = doc.object();

        QVERIFY(parser.id());
        QCOMPARE(*parser.id(), QString("f48f8496-d4be-46e1-9dfa-8a7c90d2cad7"));
        QVERIFY(parser.name());
        QCOMPARE(*parser.name(), QString("Default Watchlist"));

        auto sharesParser = parser.shares();
        QVERIFY(!sharesParser.isEmpty());
        QCOMPARE(sharesParser.size(), 4);

        auto foundCBA = false;
        for (auto i = 0, total = 4; i < total; ++i) {
            auto e = sharesParser.at(i);
            auto sym = e.symbol();
            if (sym && *sym == QLatin1String("CBA")) {
                foundCBA = true;
                QVERIFY(e.isValid());
                QVERIFY(e.name());
                QCOMPARE(*e.name(), QLatin1String("COMMONWEALTH BANK OF AUSTRALIA."));
                QVERIFY(e.lastPrice());
                QCOMPARE(*e.lastPrice(), 75.0);
                QVERIFY(e.change());
                QCOMPARE(*e.change(), 0.15);
                QVERIFY(e.changePercent());
                QCOMPARE(*e.changePercent(), 0.2);
                break;
            }
        }
        QVERIFY(foundCBA);
    }
}

void JsonResponseWrappersTest::candlesResponseTest()
{
    using namespace bsmi;

    {
        // JSON is missing high and low values on second candle
        QByteArray json = "["
                          "  {"
                          "    \"timestamp\": \"2017-09-27T02:26:54.048Z\","
                          "    \"open\": 10.5,"
                          "    \"high\": 12.5,"
                          "    \"low\": 9.5,"
                          "    \"close\": 11.5"
                          "  },"
                          "  {"
                          "    \"timestamp\": \"2017-09-28T02:26:54.048Z\","
                          "    \"open\": 11.5,"
                          "    \"close\": 12.5"
                          "  }"
                          "]";
        auto doc = QJsonDocument::fromJson(json);

        JsonObjectWrappers::CandlesResponse parser;
        parser.jsonItems = doc.array();

        QVERIFY(!parser.isEmpty());
        QCOMPARE(parser.size(), 2);

        auto cndl1 = parser.at(0);
        QVERIFY(cndl1.isValid());
        QCOMPARE(*cndl1.timestamp(),
                 QDateTime::fromString("2017-09-27T02:26:54.048Z", Qt::ISODate));
        QCOMPARE(*cndl1.open(), 10.5);
        QCOMPARE(*cndl1.high(), 12.5);
        QCOMPARE(*cndl1.low(), 9.5);
        QCOMPARE(*cndl1.close(), 11.5);

        auto cndl2 = parser.at(1);
        QVERIFY(cndl2.isValid());
        QCOMPARE(*cndl2.timestamp(),
                 QDateTime::fromString("2017-09-28T02:26:54.048Z", Qt::ISODate));
        QCOMPARE(*cndl2.open(), 11.5);
        QVERIFY(!cndl2.high());
        QVERIFY(!cndl2.low());
        QCOMPARE(*cndl2.close(), 12.5);

        // out-of-range, candle should not be valid
        auto cndl3 = parser.at(2);
        QVERIFY(!cndl3.isValid());
    }
}

QTEST_APPLESS_MAIN(JsonResponseWrappersTest)

#include "tst_jsonresponsewrapperstest.moc"
