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
    using NetworkReply = INetworkReply;

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

class QNetworkReplyMock : public QNetworkReply
{
    Q_OBJECT
public:
    QNetworkReplyMock() { setOpenMode(ReadOnly); }

    qint64 m_readUpTo{ 0 };
    QByteArray m_payload;

    void overrideAttribute(QNetworkRequest::Attribute code, const QVariant &value)
    {
        setAttribute(code, value);
    }

    // QIODevice interface
protected:
    qint64 readData(char *data, qint64 maxlen) override
    {
        // TODO(seamus): This needs its own test
        auto len = qMin(qint64(m_payload.size()) - m_readUpTo, maxlen);
        Q_ASSERT(len >= 0);
        qstrncpy(data, m_payload.constData() + m_readUpTo, uint(len) + 1);
        m_readUpTo += len;
        return len;
    }

    // QNetworkReply interface
public slots:
    void abort() override {}
};

class InvestorAPIClientCreateUserMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientCreateUserMock() {}

    QHash<bsmi::UserRecordField, QVariant> m_lastParams;
    NetworkReply *m_nextCreateUserReply{ nullptr };

    INetworkReply *createNewUser(const QHash<bsmi::UserRecordField, QVariant> &params) override
    {
        m_lastParams = params;
        return m_nextCreateUserReply;
    }
};

class InvestorAPIClientAuthenticateMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientAuthenticateMock() {}

    QString m_email;
    QString m_password;
    NetworkReply *m_nextAuthReply{ nullptr };

    INetworkReply *authenticate(const QString &email, const QString &password) override
    {
        m_email = email;
        m_password = password;
        return m_nextAuthReply;
    }
};

class InvestorAPIClientDeleteUserMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientDeleteUserMock() {}

    NetworkReply *m_nextDeleteReply{ nullptr };
    bool m_deleteUserWasCalled{ false };

    INetworkReply *deleteUser() override
    {
        m_deleteUserWasCalled = true;
        return m_nextDeleteReply;
    }
};

class InvestorAPIClientGetUserProfileMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientGetUserProfileMock() {}

    NetworkReply *m_nextUserProfileReply{ nullptr };
    bool m_getUserProfileWasCalled{ false };

    INetworkReply *getUserProfile() override
    {
        m_getUserProfileWasCalled = true;
        return m_nextUserProfileReply;
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
    void createNewUserTest();
    void authenticateTest();
    void deleteUserTest();
    void getUserProfileTest();
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

void FpCoreTest::createNewUserTest()
{
    {
        auto client = new InvestorAPIClientCreateUserMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);
        fpCore->setCoreAttribute(FpCore::CoreAttribute::AutoAuthenticateNewUserAttribute, false);

        NewUserDetails newUserA;
        newUserA.setDisplayName("Seamus");
        newUserA.setEmail("seamus@example.com");
        newUserA.setPassword("p@ssword42");
        // mock success response
        auto netCreateRep = new QNetworkReplyMock;
        client->m_nextCreateUserReply = new bsmi::INetworkReply(netCreateRep);
        auto resp = fpCore->createNewUser(newUserA);
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient got args
        auto receivedNewUserA = client->m_lastParams;
        QCOMPARE(receivedNewUserA[bsmi::UserRecordField::DisplayName],
                 QVariant(newUserA.displayName()));
        QCOMPARE(receivedNewUserA[bsmi::UserRecordField::Email], QVariant(newUserA.email()));
        QCOMPARE(receivedNewUserA[bsmi::UserRecordField::Password], QVariant(newUserA.password()));

        netCreateRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 201);
        client->m_nextCreateUserReply->setFinished();
    }
}

void FpCoreTest::authenticateTest()
{
    {
        auto client = new InvestorAPIClientAuthenticateMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        const QString email("seamus@example.com");
        const QString password("p@ssword42");

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextAuthReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        auto resp = fpCore->authenticate(email, password);
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient got args
        QCOMPARE(client->m_email, email);
        QCOMPARE(client->m_password, password);

        netAuthRep->m_payload = "{"
                                "\"accessToken\": \"i_am_very_token\","
                                "\"expires\": 604800"
                                "}";
        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
        client->m_nextAuthReply->setFinished();

        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);
        // TODO(seamus): Test expires is set
        QCOMPARE(fpCore->authToken(), QString("i_am_very_token"));

        // On a successful authenticate response the auth token and expiry are persisted
        // TODO(seamus): Test expires is saved in settings
        QCOMPARE(fpCoreSettings->m_token, QString("i_am_very_token"));
    }
}

void FpCoreTest::deleteUserTest()
{
    {
        auto client = new InvestorAPIClientDeleteUserMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        // in test token/expiry are in the saved settings, but loaded manually
        fpCoreSettings->m_token = "i_am_a_valid_token";
        fpCoreSettings->m_expiryDate = QDateTime::currentDateTimeUtc().addDays(7);

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextDeleteReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        fpCore->setAccessToken(fpCoreSettings->m_token, fpCoreSettings->m_expiryDate);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        auto resp = fpCore->deleteUser();
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient::deleteUser was called
        QVERIFY(client->m_deleteUserWasCalled);

        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 204);
        client->m_nextDeleteReply->setFinished();

        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        QVERIFY(fpCore->authToken().isEmpty());
        QVERIFY(!fpCore->expiry().isValid());

        // On a successful delete response the auth token and expiry
        // are removed from settings
        QVERIFY(fpCoreSettings->m_token.isEmpty());
        QVERIFY(!fpCoreSettings->m_expiryDate.isValid());
    }
}

void FpCoreTest::getUserProfileTest()
{
    {
        auto client = new InvestorAPIClientGetUserProfileMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextUserProfileReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        fpCore->setAccessToken("i_am_very_token", QDateTime::currentDateTimeUtc().addDays(7));
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        auto resp = fpCore->getUserProfile();
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient::getUserProfile was called
        QVERIFY(client->m_getUserProfileWasCalled);

        netAuthRep->m_payload = "{"
                                "  \"id\": \"string\","
                                "  \"email\": \"string\","
                                "  \"displayName\": \"string\","
                                "  \"level\": \"Friend\","
                                "  \"accounts\": ["
                                "    {"
                                "      \"id\": \"string\","
                                "      \"name\": \"string\","
                                "      \"balance\": 0"
                                "    }"
                                "  ]"
                                "};";
        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
        client->m_nextUserProfileReply->setFinished();

        QVERIFY(!netAuthRep->m_payload.isEmpty());
        QCOMPARE(QJsonDocument::fromJson(resp->payload()),
                 QJsonDocument::fromJson(netAuthRep->m_payload));
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
