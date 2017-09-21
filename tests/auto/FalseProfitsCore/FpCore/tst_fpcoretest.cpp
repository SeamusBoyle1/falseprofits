// Copyright 2017

#include <QString>
#include <QtTest>

#include <FalseProfitsCore/fpcore.h>
#include <FalseProfitsCore/fpsettings.h>
#include <InvestorAPIClient/iinvestorapiclient.h>

class MockFpSettings : public IFpSettings
{
    Q_OBJECT
public:
    explicit MockFpSettings(QObject *parent = nullptr)
        : IFpSettings(parent)
    {
    }

    LaunchSettings readLaunchSettings() override
    {
        LaunchSettings r;
        return r;
    }

    void writeAccessToken(const QString &token, const QDateTime &expiryDate) override
    {
        m_token = token;
        m_expiryDate = expiryDate;
    }

    void clearAccessToken() override
    {
        m_token.clear();
        m_expiryDate = QDateTime();
    }

    QString m_token;
    QDateTime m_expiryDate;
};

class InvestorAPIClientMock : public bsmi::IInvestorAPIClient
{
    Q_OBJECT
public:
    using INetworkReply = bsmi::INetworkReply;

    explicit InvestorAPIClientMock(QObject *parent = 0) : bsmi::IInvestorAPIClient(parent) {}

    QString m_authToken;
    QDateTime m_expiryDate;

    QString authToken() override { return m_authToken; }
    QDateTime expiry() override { return m_expiryDate; }
    void setAuthToken(const QString &token, const QDateTime &expiry) override
    {
        if (m_authToken == token) {
            return;
        }

        m_authToken = token;
        m_expiryDate = expiry;

        emit authTokenChanged();
    }

    INetworkReply *createNewUser(const QHash<bsmi::UserRecordField, QVariant> &params) override
    {
        Q_UNUSED(params)
        return nullptr;
    }

    INetworkReply *authenticate(const QString &email, const QString &password) override
    {
        Q_UNUSED(email)
        Q_UNUSED(password)
        return nullptr;
    }

    INetworkReply *deleteUser() override
    {
        return nullptr;
    }

    INetworkReply *getUserProfile() override
    {
        return nullptr;
    }

    INetworkReply *getQuotes(const QStringList &symbols) override
    {
        Q_UNUSED(symbols)
        return nullptr;
    }

    INetworkReply *symbolSearch(const SymbolSearchQuery &query) override
    {
        Q_UNUSED(query)
        return nullptr;
    }

    INetworkReply *sendOrder(const QString &accountId, const OrderParams &args) override
    {
        Q_UNUSED(accountId)
        Q_UNUSED(args)
        return nullptr;
    }
};

static bsmi::IInvestorAPIClient *createMockFpCore(QObject *parent = 0)
{
    return new InvestorAPIClientMock(parent);
}

class FpCoreTest : public QObject
{
    Q_OBJECT

public:
    FpCoreTest();

private Q_SLOTS:
    void setAccessTokenTest();
    void signOutTest();
};

FpCoreTest::FpCoreTest()
{
    qRegisterMetaType<Fpx::AuthenticationState>("Fpx::AuthenticationState");
}

void FpCoreTest::setAccessTokenTest()
{
    {
        auto client = createMockFpCore();
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        QVERIFY(fpCore->authToken().isEmpty());
        QVERIFY(!fpCore->expiry().isValid());
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);

        // Verify setting EXPIRED auth token and verify signals
        QSignalSpy spyAuthStateChanged(fpCore, &FpCore::authStateChanged);
        fpCore->setAccessToken("i_am_an_expired_token",
                               QDateTime(QDate(2017, 9, 17), QTime(6, 18)));
        // NOTE: expires tokens can be set on bsmi::IInvestorAPIClient
        QCOMPARE(fpCore->authToken(), QString("i_am_an_expired_token"));
        QCOMPARE(fpCore->expiry(), QDateTime(QDate(2017, 9, 17), QTime(6, 18)));

        // the authStateChanged should not be emit since
        // token is expired so state didn't change
        QCOMPARE(spyAuthStateChanged.count(), 0);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);

        // Verify setting NON-EXPIRED auth token and verify signals
        auto expiryDate = QDateTime::currentDateTime().addDays(7);
        fpCore->setAccessToken("i_am_a_valid_token", expiryDate);
        QCOMPARE(fpCore->authToken(), QString("i_am_a_valid_token"));
        QCOMPARE(fpCore->expiry(), expiryDate);

        // the authStateChanged should be emit once with AuthenticatedState
        QCOMPARE(spyAuthStateChanged.count(), 1);
        auto signalArgs = spyAuthStateChanged.takeFirst();
        QCOMPARE(signalArgs.at(0).value<Fpx::AuthenticationState>(),
                 Fpx::AuthenticationState::AuthenticatedState);

        // Clear token by setting invalid values
        fpCore->setAccessToken(QString(), QDateTime());
        QCOMPARE(fpCore->authToken(), QString());
        QCOMPARE(fpCore->expiry(), QDateTime());

        // the authStateChanged should be emit once with NotAuthenticatedState
        QCOMPARE(spyAuthStateChanged.count(), 1);
        signalArgs = spyAuthStateChanged.takeFirst();
        QCOMPARE(signalArgs.at(0).value<Fpx::AuthenticationState>(),
                 Fpx::AuthenticationState::NotAuthenticatedState);
    }
}

void FpCoreTest::signOutTest()
{
    {
        auto client = createMockFpCore();
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        QVERIFY(fpCore->authToken().isEmpty());
        QVERIFY(!fpCore->expiry().isValid());
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);

        auto expiryDate = QDateTime::currentDateTime().addDays(7);
        fpCore->setAccessToken("i_am_a_valid_token", expiryDate);

        // Need to manually store token/expiry in settings as
        // it is only automatically "persisted" on a a success
        // response from POST /api/1.0/token
        fpCoreSettings->m_token = "i_am_a_valid_token";
        fpCoreSettings->m_expiryDate = expiryDate;

        QVERIFY(!fpCore->authToken().isEmpty());
        QVERIFY(fpCore->expiry().isValid());
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        // actually signout
        // should emit authStateChanged
        QSignalSpy spyAuthStateChanged(fpCore, &FpCore::authStateChanged);
        fpCore->signOut();

        QCOMPARE(spyAuthStateChanged.count(), 1);
        auto signalArgs = spyAuthStateChanged.takeFirst();
        QCOMPARE(signalArgs.at(0).value<Fpx::AuthenticationState>(),
                 Fpx::AuthenticationState::NotAuthenticatedState);

        QVERIFY(fpCore->authToken().isEmpty());
        QVERIFY(!fpCore->expiry().isValid());
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
    }
}

QTEST_APPLESS_MAIN(FpCoreTest)

#include "tst_fpcoretest.moc"
