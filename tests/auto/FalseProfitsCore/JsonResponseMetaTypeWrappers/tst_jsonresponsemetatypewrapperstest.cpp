// Copyright 2017

#include <QString>
#include <QtTest>

#include <FalseProfitsCore/fpdeclarativetypes.h>

#include <QFile>
#include <QMetaObject>

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

class JsonResponseMetaTypeWrappersTest : public QObject
{
    Q_OBJECT

public:
    JsonResponseMetaTypeWrappersTest();

private Q_SLOTS:
    void factoryFalseProfitsDeclarativeTypesTest();
    void jsonUserDetailsTest();
};

JsonResponseMetaTypeWrappersTest::JsonResponseMetaTypeWrappersTest()
{
}

void JsonResponseMetaTypeWrappersTest::factoryFalseProfitsDeclarativeTypesTest()
{
    // Test FalseProfitsDeclarativeTypes class
    {
        FalseProfitsDeclarativeTypes fpType;

        // Test invoke by name makeJsonUserDetails
        {
            auto ba = readFileContents(srcDirFile("userInfo.json"));
            if (ba.isEmpty()) {
                QSKIP("Unable to open test file");
            }
            JsonUserDetails rv;
            auto ok =
                QMetaObject::invokeMethod(&fpType, "makeJsonUserDetails",
                                          Q_RETURN_ARG(JsonUserDetails, rv), Q_ARG(QByteArray, ba));
            QVERIFY2(ok, "Failed to invoke makeJsonUserDetails");
        }
    }
}

void JsonResponseMetaTypeWrappersTest::jsonUserDetailsTest()
{
    // Tests for GET /api/1.0/users
    // Swagger UserInfo

    {
        QString testFile("userInfo.json");
        auto doc = readFileContentsAsJson(srcDirFile(testFile));
        if (doc.isNull()) {
            QSKIP("Unable to open test file");
        }

        JsonUserDetails parser;
        parser.m_d = doc.object();

        // Test getters
        QCOMPARE(parser.id(), QVariant(QString("51d3b0f3-acb3-4f9b-a4f8-9d3d31a420fb")));
        QCOMPARE(parser.email(), QVariant(QString("seamus@example.com")));
        QCOMPARE(parser.displayName(), QVariant(QString("Seamus")));
        QCOMPARE(parser.level(), QVariant(QString("Investor")));

        // Test property access by name
        const QStringList properties{{ "id", "email", "displayName", "gravatarUrl", "level"}};

        for (auto const &propertyName : properties) {
            auto propertyIndex = parser.staticMetaObject.indexOfProperty(propertyName.toLatin1());
            QVERIFY(propertyIndex != -1);
            auto property = parser.staticMetaObject.property(propertyIndex);
            QVERIFY(property.userType() != QMetaType::UnknownType);
        }

        // Test value can be converted to and from QVariant
        auto parserVar = QVariant::fromValue<JsonUserDetails>(parser);
        QVERIFY(parserVar.isValid());
        QCOMPARE(parserVar.value<JsonUserDetails>().m_d, parser.m_d);

        // Test construction by class name
        int id = QMetaType::type("JsonUserDetails");
        QVERIFY(id != QMetaType::UnknownType);
    }
}

QTEST_APPLESS_MAIN(JsonResponseMetaTypeWrappersTest)

#include "tst_jsonresponsemetatypewrapperstest.moc"
