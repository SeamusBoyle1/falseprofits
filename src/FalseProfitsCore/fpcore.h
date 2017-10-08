// Copyright 2017

#ifndef FPCORE_H
#define FPCORE_H

#include <QObject>

#include "fpdeclarativetypes.h"
#include "responsetypes.h" // TODO(seamus): Replace with forward decl

#include <QMap>

namespace bsmi {
class IInvestorAPIClient;
class INetworkReply;
} // namespace bsmi

class IFpSettings;

namespace Fpx {
Q_NAMESPACE

enum class AuthenticationState {
    AuthenticatedState,
    NotAuthenticatedState,
};
Q_ENUM_NS(AuthenticationState)

enum class CommissionSide {
    BuySide,
    SellSide,
};
Q_ENUM_NS(CommissionSide)
}

class FpCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Fpx::AuthenticationState authState READ authState NOTIFY authStateChanged)
public:
    explicit FpCore(bsmi::IInvestorAPIClient *client, IFpSettings *settings,
                    QObject *parent = nullptr);

    QString authToken();
    QDateTime expiry();
    void setAccessToken(const QString &accessToken, const QDateTime &expiry);

    Q_INVOKABLE
    qint64 getNonce() const;

    Q_INVOKABLE
    NewUserResponse *createNewUser(const NewUserDetails &newUser);

    Q_INVOKABLE
    AuthenticateResponse *authenticate(const QString &email, const QString &password);

    Q_INVOKABLE
    DeleteUserResponse *deleteUser();

    Q_INVOKABLE
    GetUserProfileResponse *getUserProfile();

    Q_INVOKABLE
    GetCommissionsResponse *getCommissions(Fpx::CommissionSide side);

    Q_INVOKABLE
    GetPositionsResponse *getPositions(const QString &accountId);

    Q_INVOKABLE
    GetTransactionsResponse *getTransactions(const TransactionsQuery &query);

    Q_INVOKABLE
    GetQuotesResponse *getQuotes(const QStringList &symbols);

    Q_INVOKABLE
    GetCandlesResponse *getCandles(const CandlesRequestArgs &args);

    Q_INVOKABLE
    SymbolSearchResponse *symbolSearch(const SymbolSearchQuery &query);

    Q_INVOKABLE
    GetShareDetailsResponse *getShareDetails(const QString &symbol);

    Q_INVOKABLE
    SendOrderResponse *sendOrder(const QString &accountId, const OrderParams &args);

    Q_INVOKABLE
    GetWatchlistResponse *getWatchlist(const QString &watchlistId);

    Q_INVOKABLE
    AddSymbolToWatchlistResponse *addSymbolToWatchlist(const QString &watchlistId,
                                                       const QString &symbol);

    Q_INVOKABLE
    RemoveSymbolFromWatchlistResponse *removeSymbolFromWatchlist(const QString &watchlistId,
                                                                 const QString &symbol);

    Fpx::AuthenticationState authState() const;

    void loadLaunchSettings();

    enum CoreAttribute { AutoAuthenticateNewUserAttribute };
    void setCoreAttribute(CoreAttribute a, QVariant v) { m_coreAttributes[a] = v; }

Q_SIGNALS:
    void launchSettingsLoaded();
    void authStateChanged(Fpx::AuthenticationState);
    void positionsChanged();

public Q_SLOTS:
    void signOut();

private Q_SLOTS:
    void onClientAuthenticated();

private:
    bsmi::IInvestorAPIClient *m_client;
    Fpx::AuthenticationState m_authenticationState{
        Fpx::AuthenticationState::NotAuthenticatedState
    };
    IFpSettings *m_settings;
    QMap<CoreAttribute, QVariant> m_coreAttributes;
    mutable qint64 m_lastNonce{ 0 };

    void clearAccessToken();

    void setAuthenticationStateImpl(Fpx::AuthenticationState state);

    void signInAsNewUser(const NewUserDetails &newUser);

    static int readHttpStatusCode(bsmi::INetworkReply *reply);

    static QString readErrorMessage(BaseResponse *response, bsmi::INetworkReply *reply,
                                    int httpStatusCode);
};

#endif // FPCORE_H
