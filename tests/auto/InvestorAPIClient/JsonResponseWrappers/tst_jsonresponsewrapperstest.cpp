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
