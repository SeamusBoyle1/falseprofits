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

    INetworkReply *getCommissions(CommissionSide side) override
    {
        Q_UNUSED(side)
        return nullptr;
    }

    INetworkReply *getPositions(const QString &accountId) override
    {
        Q_UNUSED(accountId)
        return nullptr;
    }

    INetworkReply *getTransactions(const GetTransactionsArgs &query) override
    {
        Q_UNUSED(query)
        return nullptr;
    }

    INetworkReply *getQuotes(const QStringList &symbols) override
    {
        Q_UNUSED(symbols)
        return nullptr;
    }

    INetworkReply *getCandles(const CandlesRequestArgs &args) override
    {
        Q_UNUSED(args)
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

    INetworkReply *getWatchlist(const QString &watchlistId) override
    {
        Q_UNUSED(watchlistId)
        return nullptr;
    }

    INetworkReply *addSymbolToWatchlist(const QString &watchlistId, const QString &symbol) override
    {
        Q_UNUSED(watchlistId)
        Q_UNUSED(symbol)
        return nullptr;
    }

    INetworkReply *removeSymbolFromWatchlist(const QString &watchlistId,
                                             const QString &symbol) override
    {
        Q_UNUSED(watchlistId)
        Q_UNUSED(symbol)
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

class InvestorAPIClientGetPositionsMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientGetPositionsMock() {}

    NetworkReply *m_nextGetPositionsReply{ nullptr };
    QString m_accountId;

    INetworkReply *getPositions(const QString &accountId) override
    {
        m_accountId = accountId;
        return m_nextGetPositionsReply;
    }
};

class InvestorAPIClientGetTransactionsMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientGetTransactionsMock() {}

    NetworkReply *m_nextTransactionsReply{ nullptr };
    GetTransactionsArgs m_query;

    INetworkReply *getTransactions(const GetTransactionsArgs &query) override
    {
        m_query = query;
        return m_nextTransactionsReply;
    }
};

class InvestorAPIClientGetQuotesMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientGetQuotesMock() {}

    NetworkReply *m_nextQuotesReply{ nullptr };
    QStringList m_symbols;

    INetworkReply *getQuotes(const QStringList &symbols) override
    {
        m_symbols = symbols;
        return m_nextQuotesReply;
    }
};

class InvestorAPIClientGetCandlesMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientGetCandlesMock() {}

    NetworkReply *m_nextCandlesReply{ nullptr };
    CandlesRequestArgs m_args;

    INetworkReply *getCandles(const CandlesRequestArgs &args) override
    {
        m_args = args;
        return m_nextCandlesReply;
    }
};

class InvestorAPIClientSymbolSearchMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientSymbolSearchMock() {}

    NetworkReply *m_nextSearchReply{ nullptr };
    SymbolSearchQuery m_query;

    INetworkReply *symbolSearch(const SymbolSearchQuery &query) override
    {
        m_query = query;
        return m_nextSearchReply;
    }
};

class InvestorAPIClientSendOrderMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientSendOrderMock() {}

    NetworkReply *m_nextSendOrderReply{ nullptr };
    QString m_accountId;
    OrderParams m_orderArgs;

    INetworkReply *sendOrder(const QString &accountId, const OrderParams &args) override
    {
        m_accountId = accountId;
        m_orderArgs = args;
        return m_nextSendOrderReply;
    }
};

class InvestorAPIClientGetWatchlistMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientGetWatchlistMock() {}

    NetworkReply *m_nextGetWatchlistReply{ nullptr };
    QString m_watchlistId;

    INetworkReply *getWatchlist(const QString &watchlistId) override
    {
        m_watchlistId = watchlistId;
        return m_nextGetWatchlistReply;
    }
};

class InvestorAPIClientAddWatchlistSymbolMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientAddWatchlistSymbolMock() {}

    NetworkReply *m_nextAddWatchlistSymbolReply{ nullptr };
    QString m_watchlistId;
    QString m_symbol;

    INetworkReply *addSymbolToWatchlist(const QString &watchlistId, const QString &symbol) override
    {
        m_watchlistId = watchlistId;
        m_symbol = symbol;
        return m_nextAddWatchlistSymbolReply;
    }
};

class InvestorAPIClientRemoveWatchlistSymbolMock : public InvestorAPIClientMock
{
    Q_OBJECT
public:
    InvestorAPIClientRemoveWatchlistSymbolMock() {}

    NetworkReply *m_nextRemoveWatchlistSymbolReply{ nullptr };
    QString m_watchlistId;
    QString m_symbol;

    INetworkReply *removeSymbolFromWatchlist(const QString &watchlistId,
                                             const QString &symbol) override
    {
        m_watchlistId = watchlistId;
        m_symbol = symbol;
        return m_nextRemoveWatchlistSymbolReply;
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
    void getPositionsTest();
    void getTransactionsTest();
    void getQuotesTest();
    void getCandlesTest();
    void symbolSearchTest();
    void sendOrderTest();
    void signOutTest();
    void getWatchlistTest();
    void addSymbolToWatchlistTest();
    void removeSymbolFromWatchlistTest();
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

void FpCoreTest::getPositionsTest()
{
    {
        auto client = new InvestorAPIClientGetPositionsMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        QVERIFY(client->m_accountId.isEmpty());

        QString accIda("valid-fake-id");

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextGetPositionsReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        fpCore->setAccessToken("i_am_very_token", QDateTime::currentDateTimeUtc().addDays(7));
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        auto resp = fpCore->getPositions(accIda);
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient::getPositions was called
        QVERIFY(!client->m_accountId.isEmpty());

        netAuthRep->m_payload = "{"
                                "  \"positions\": ["
                                "    {"
                                "      \"symbol\": \"string\","
                                "      \"name\": \"string\","
                                "      \"quantity\": 0,"
                                "      \"averagePrice\": 0,"
                                "      \"lastPrice\": 0,"
                                "      \"change\": 0,"
                                "      \"changePercent\": 0"
                                "    }"
                                "  ],"
                                "  \"id\": \"string\","
                                "  \"name\": \"string\","
                                "  \"balance\": 0"
                                "}";
        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
        client->m_nextGetPositionsReply->setFinished();

        QVERIFY(!netAuthRep->m_payload.isEmpty());
        QCOMPARE(QJsonDocument::fromJson(resp->payload()),
                 QJsonDocument::fromJson(netAuthRep->m_payload));
    }
}

void FpCoreTest::getTransactionsTest()
{
    {
        auto client = new InvestorAPIClientGetTransactionsMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        TransactionsQuery queryA;
        queryA.setAccountId("valid-fake-id");
        queryA.setStartDate(QDateTime(QDate(2017, 1, 1), QTime(), Qt::LocalTime));
        queryA.setEndDate(QDateTime(QDate(2017, 2, 1), QTime(), Qt::LocalTime));
        queryA.setPageSize(20);
        queryA.setPageNumber(2);

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextTransactionsReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        fpCore->setAccessToken("i_am_very_token", QDateTime::currentDateTimeUtc().addDays(7));
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        auto resp = fpCore->getTransactions(queryA);
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient got args
        QCOMPARE(client->m_query.accountId, queryA.accountId());
        QCOMPARE(client->m_query.startDate, queryA.startDate());
        QCOMPARE(client->m_query.endDate, queryA.endDate());
        QCOMPARE(client->m_query.pageSize, queryA.pageSize());
        QCOMPARE(client->m_query.pageNumber, queryA.pageNumber());

        netAuthRep->m_payload = "{"
                                "  \"items\": ["
                                "    {"
                                "      \"timestampUtc\": \"2017-10-04T23:48:48.848Z\","
                                "      \"type\": \"Transfer\","
                                "      \"description\": \"string\","
                                "      \"amount\": 0,"
                                "      \"balance\": 0"
                                "    }"
                                "  ],"
                                "  \"pageNumber\": 0,"
                                "  \"pageSize\": 0,"
                                "  \"totalPageCount\": 0,"
                                "  \"totalRowCount\": 0"
                                "}";
        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
        client->m_nextTransactionsReply->setFinished();

        QVERIFY(!netAuthRep->m_payload.isEmpty());
        QCOMPARE(QJsonDocument::fromJson(resp->payload()),
                 QJsonDocument::fromJson(netAuthRep->m_payload));
    }
}

void FpCoreTest::getQuotesTest()
{
    {
        auto client = new InvestorAPIClientGetQuotesMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        QVERIFY(client->m_symbols.isEmpty());

        QStringList symbolsA{ { "SYM1" }, { "SYM2" } };

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextQuotesReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        fpCore->setAccessToken("i_am_very_token", QDateTime::currentDateTimeUtc().addDays(7));
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        auto resp = fpCore->getQuotes(symbolsA);
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient::getQuotes was called
        QVERIFY(!client->m_symbols.isEmpty());

        netAuthRep->m_payload = "["
                                "  {"
                                "    \"symbol\": \"SYM1\","
                                "    \"last\": 0,"
                                "  }, {"
                                "    \"symbol\": \"SYM2\","
                                "    \"last\": 0,"
                                "    },"
                                "]";
        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
        client->m_nextQuotesReply->setFinished();

        QVERIFY(!netAuthRep->m_payload.isEmpty());
        QCOMPARE(QJsonDocument::fromJson(resp->payload()),
                 QJsonDocument::fromJson(netAuthRep->m_payload));
    }
}

void FpCoreTest::getCandlesTest()
{
    // Success request using symbol and range params
    {
        auto client = new InvestorAPIClientGetCandlesMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        CandlesRequestArgs queryA;
        queryA.setSymbol("Hello");
        queryA.setRange("1d");

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextCandlesReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        fpCore->setAccessToken("i_am_very_token", QDateTime::currentDateTimeUtc().addDays(7));
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        auto resp = fpCore->getCandles(queryA);
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient got args
        QCOMPARE(client->m_args.symbol, queryA.symbol());
        QCOMPARE(client->m_args.range, queryA.range());

        netAuthRep->m_payload = "["
                                "  {"
                                "    \"timestamp\": \"2017-09-28T02:26:54.048Z\","
                                "    \"open\": 0,"
                                "    \"high\": 0,"
                                "    \"low\": 0,"
                                "    \"close\": 0"
                                "  }"
                                "]";
        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
        client->m_nextCandlesReply->setFinished();

        QVERIFY(!netAuthRep->m_payload.isEmpty());
        QCOMPARE(QJsonDocument::fromJson(resp->payload()),
                 QJsonDocument::fromJson(netAuthRep->m_payload));
    }
}

void FpCoreTest::symbolSearchTest()
{
    {
        auto client = new InvestorAPIClientSymbolSearchMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        SymbolSearchQuery queryA;
        queryA.setSearchTerm("Hello");
        queryA.setPageSize(20);
        queryA.setPageNumber(2);

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextSearchReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        fpCore->setAccessToken("i_am_very_token", QDateTime::currentDateTimeUtc().addDays(7));
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        auto resp = fpCore->symbolSearch(queryA);
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient got args
        QCOMPARE(client->m_query.searchTerm, queryA.searchTerm());
        QCOMPARE(client->m_query.pageSize, queryA.pageSize());
        QCOMPARE(client->m_query.pageNumber, queryA.pageNumber());

        netAuthRep->m_payload = "{"
                                "  \"items\": ["
                                "    {"
                                "      \"industry\": \"string\","
                                "      \"symbol\": \"string\","
                                "      \"name\": \"string\""
                                "    }"
                                "  ],"
                                "  \"pageNumber\": 0,"
                                "  \"pageSize\": 0,"
                                "  \"totalPageCount\": 0,"
                                "  \"totalRowCount\": 0"
                                "}";
        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
        client->m_nextSearchReply->setFinished();

        QVERIFY(!netAuthRep->m_payload.isEmpty());
        QCOMPARE(QJsonDocument::fromJson(resp->payload()),
                 QJsonDocument::fromJson(netAuthRep->m_payload));
    }
}

void FpCoreTest::sendOrderTest()
{
    {
        auto client = new InvestorAPIClientSendOrderMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        // any id can be used, sendOrder() doesn't validate
        QString accountIdA("anyUnverifiedId");

        // any order can be used, sendOrder() doesn't validate
        OrderParams orderArgsA;
        orderArgsA.setSymbol("BHP");
        orderArgsA.setQuantity(20);
        orderArgsA.setSide(OrderParams::Side::SellSide);

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextSendOrderReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        fpCore->setAccessToken("i_am_very_token", QDateTime::currentDateTimeUtc().addDays(7));
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        auto resp = fpCore->sendOrder(accountIdA, orderArgsA);
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient got args
        QCOMPARE(client->m_accountId, accountIdA);
        QCOMPARE(client->m_orderArgs.symbol, orderArgsA.symbol());
        QCOMPARE(client->m_orderArgs.quantity, orderArgsA.quantity());
        auto sideStr =
            orderArgsA.side() == OrderParams::Side::SellSide ? QString("Sell") : QString("Buy");
        QCOMPARE(client->m_orderArgs.side, sideStr);

        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 201);
        client->m_nextSendOrderReply->setFinished();
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

void FpCoreTest::getWatchlistTest()
{
    {
        auto client = new InvestorAPIClientGetWatchlistMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        QVERIFY(client->m_watchlistId.isEmpty());

        QString watchlistIdA("i_am_a_valid_watchlist_id");

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextGetWatchlistReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        fpCore->setAccessToken("i_am_very_token", QDateTime::currentDateTimeUtc().addDays(7));
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        auto resp = fpCore->getWatchlist(watchlistIdA);
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient::getWatchlist was called
        QVERIFY(!client->m_watchlistId.isEmpty());

        netAuthRep->m_payload = "{"
                                "  \"shares\": ["
                                "    {"
                                "      \"symbol\": \"string\","
                                "      \"name\": \"string\","
                                "      \"lastPrice\": 0,"
                                "      \"change\": 0,"
                                "      \"changePercent\": 0"
                                "    }"
                                "  ],"
                                "  \"id\": \"string\","
                                "  \"name\": \"string\""
                                "}";
        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
        client->m_nextGetWatchlistReply->setFinished();

        QVERIFY(!netAuthRep->m_payload.isEmpty());
        QCOMPARE(QJsonDocument::fromJson(resp->payload()),
                 QJsonDocument::fromJson(netAuthRep->m_payload));
    }
}

void FpCoreTest::addSymbolToWatchlistTest()
{
    {
        auto client = new InvestorAPIClientAddWatchlistSymbolMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        QString watchlistIdA("i_am_a_valid_watchlist_id");
        QString symbolA("a_symbol");

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextAddWatchlistSymbolReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        fpCore->setAccessToken("i_am_very_token", QDateTime::currentDateTimeUtc().addDays(7));
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        auto resp = fpCore->addSymbolToWatchlist(watchlistIdA, symbolA);
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient::deleteUser was called
        QVERIFY(!client->m_watchlistId.isEmpty());
        QVERIFY(!client->m_symbol.isEmpty());

        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
        client->m_nextAddWatchlistSymbolReply->setFinished();
    }
}

void FpCoreTest::removeSymbolFromWatchlistTest()
{
    {
        auto client = new InvestorAPIClientRemoveWatchlistSymbolMock;
        auto fpCoreSettings = new MockFpSettings;
        auto fpCore = new FpCore(client, fpCoreSettings);
        client->setParent(fpCore);

        QString watchlistIdA("i_am_a_valid_watchlist_id");
        QString symbolA("a_symbol");

        // mock success response
        auto netAuthRep = new QNetworkReplyMock;
        client->m_nextRemoveWatchlistSymbolReply = new bsmi::INetworkReply(netAuthRep);
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::NotAuthenticatedState);
        fpCore->setAccessToken("i_am_very_token", QDateTime::currentDateTimeUtc().addDays(7));
        QCOMPARE(fpCore->authState(), Fpx::AuthenticationState::AuthenticatedState);

        auto resp = fpCore->removeSymbolFromWatchlist(watchlistIdA, symbolA);
        QVERIFY(resp);

        // Test bsmi::IInvestorAPIClient::deleteUser was called
        QVERIFY(!client->m_watchlistId.isEmpty());
        QVERIFY(!client->m_symbol.isEmpty());

        netAuthRep->overrideAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
        client->m_nextRemoveWatchlistSymbolReply->setFinished();
    }
}

QTEST_APPLESS_MAIN(FpCoreTest)

#include "tst_fpcoretest.moc"
