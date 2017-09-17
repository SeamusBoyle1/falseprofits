// Copyright 2017

#ifndef FPCORE_H
#define FPCORE_H

#include <QObject>

#include "fpdeclarativetypes.h"
#include "responsetypes.h" // TODO(seamus): Replace with forward decl

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
    NewUserResponse *createNewUser(const NewUserDetails &newUser);

    Q_INVOKABLE
    AuthenticateResponse *authenticate(const QString &email, const QString &password);

    Q_INVOKABLE
    DeleteUserResponse *deleteUser();

    Q_INVOKABLE
    GetUserProfileResponse *getUserProfile();

    Fpx::AuthenticationState authState() const;

    void loadLaunchSettings();

Q_SIGNALS:
    void authStateChanged(Fpx::AuthenticationState);

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

    void setAuthenticationStateImpl(Fpx::AuthenticationState state);

    void signInAsNewUser(const NewUserDetails &newUser);

    static int readHttpStatusCode(bsmi::INetworkReply *reply);
};

#endif // FPCORE_H
