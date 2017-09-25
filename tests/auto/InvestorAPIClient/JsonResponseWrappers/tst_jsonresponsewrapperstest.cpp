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
    void userProfileResponseTest();
    void commissionResponseTest();
    void symbolSearchTest();
};

JsonResponseWrappersTest::JsonResponseWrappersTest()
{
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

QTEST_APPLESS_MAIN(JsonResponseWrappersTest)

#include "tst_jsonresponsewrapperstest.moc"
