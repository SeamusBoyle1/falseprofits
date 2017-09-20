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

QTEST_APPLESS_MAIN(JsonResponseWrappersTest)

#include "tst_jsonresponsewrapperstest.moc"
