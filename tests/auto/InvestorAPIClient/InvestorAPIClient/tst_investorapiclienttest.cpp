// Copyright 2017

#include <QString>
#include <QtTest>

#include <InvestorAPIClient/investorapiclient.h>

#include <QSignalSpy>

class InvestorAPIClientTest : public QObject
{
    Q_OBJECT

public:
    InvestorAPIClientTest();

private Q_SLOTS:
    void setAuthTokenTest();
};

InvestorAPIClientTest::InvestorAPIClientTest()
{
}

void InvestorAPIClientTest::setAuthTokenTest()
{
    using namespace bsmi;

    {
        InvestorAPIClient c(nullptr, QStringLiteral("http://example.com"));

        // Auth token default empty, and default invalid expiry date
        QVERIFY(c.authToken().isEmpty());
        QVERIFY(!c.expiry().isValid());

        auto aToken = QLatin1String("exampleA-token-contents-doesn't-matter");
        auto aDt = QDateTime(QDate(2017, 9, 12), QTime(2, 33));

        // Verify can set token and expiry
        QSignalSpy spyAuthTokenChanged(&c, &IInvestorAPIClient::authTokenChanged);
        c.setAuthToken(aToken, aDt);
        QCOMPARE(c.authToken(), aToken);
        QCOMPARE(c.expiry(), aDt);

        // the authTokenChanged should be emit once
        QCOMPARE(spyAuthTokenChanged.count(), 1);
        spyAuthTokenChanged.removeFirst();

        // no authTokenChanged signal for identical arg
        // (i.e. when token hasn't changed)
        c.setAuthToken(aToken, aDt);
        QCOMPARE(spyAuthTokenChanged.count(), 0);

        // able to replace auth token and expiry
        auto bToken = QLatin1String("exampleB-token-contents-doesn't-matter");
        auto bDt = QDateTime(QDate(2017, 9, 12), QTime(2, 42));
        c.setAuthToken(bToken, bDt);
        QCOMPARE(c.authToken(), bToken);
        QCOMPARE(c.expiry(), bDt);
        QCOMPARE(spyAuthTokenChanged.count(), 1);
        spyAuthTokenChanged.removeFirst();
    }
}

QTEST_APPLESS_MAIN(InvestorAPIClientTest)

#include "tst_investorapiclienttest.moc"
