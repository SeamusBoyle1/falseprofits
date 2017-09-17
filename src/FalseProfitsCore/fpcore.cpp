// Copyright 2017

#include "fpcore.h"

#include "fpcore_interfaces.h"

#include <InvestorAPIClient/iinvestorapiclient.h>
#include <InvestorAPIClient/jsonobjectwrappers.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QPointer>

FpCore::FpCore(bsmi::IInvestorAPIClient *client, IFpSettings *settings, QObject *parent)
    : QObject(parent)
    , m_client{ client }
    , m_settings{ settings }
{
    connect(m_client, &bsmi::IInvestorAPIClient::authTokenChanged, this,
            &FpCore::onClientAuthenticated);
}

QString FpCore::authToken()
{
    return m_client->authToken();
}

QDateTime FpCore::expiry()
{
    return m_client->expiry();
}

void FpCore::setAccessToken(const QString &accessToken, const QDateTime &expiry)
{
    m_client->setAuthToken(accessToken, expiry);
}

NewUserResponse *FpCore::createNewUser(const NewUserDetails &newUser)
{
    QPointer<NewUserResponse> resp(new NewUserResponse);

    using bsmi::UserRecordField;

    QHash<UserRecordField, QVariant> v;
    v.insert(UserRecordField::DisplayName, newUser.displayName());
    v.insert(UserRecordField::Email, newUser.email());
    v.insert(UserRecordField::Password, newUser.password());

    auto rep = m_client->createNewUser(v);
    connect(rep, &bsmi::INetworkReply::finished, this, [newUser, resp, rep, this]() {
        if (!resp) {
            rep->deleteLater();
            return;
        }
        resp->setHttpStatusCode(readHttpStatusCode(rep));
        if (rep->error() == QNetworkReply::NoError) {
            resp->setPayload(rep->readAll());

            // auto sign in as user that was just created
            signInAsNewUser(newUser);
        } else {
            resp->setError(rep->errorString());
        }

        rep->deleteLater();
        resp->setFinished();
    });

    return resp;
}

AuthenticateResponse *FpCore::authenticate(const QString &email, const QString &password)
{
    QPointer<AuthenticateResponse> resp(new AuthenticateResponse);

    auto rep = m_client->authenticate(email, password);
    connect(rep, &bsmi::INetworkReply::finished, this, [resp, rep, this]() {
        if (!resp) {
            rep->deleteLater();
            return;
        }
        resp->setHttpStatusCode(readHttpStatusCode(rep));
        if (rep->error() == QNetworkReply::NoError) {
            resp->setPayload(rep->readAll());
            bsmi::JsonObjectWrappers::TokenResponse jsonReader;
            jsonReader.d = QJsonDocument::fromJson(resp->payload()).object();

            auto token = jsonReader.accessToken();
            auto expires = jsonReader.expires();
            if (token && expires) {
                auto expiryDt =
                    *expires != 0 ? QDateTime::currentDateTimeUtc().addSecs(*expires) : QDateTime();
                setAccessToken(*token, expiryDt);

                m_settings->writeAccessToken(*token, expiryDt);
            }
        } else {
            resp->setError(rep->errorString());
        }

        rep->deleteLater();
        resp->setFinished();
    });

    return resp;
}

DeleteUserResponse *FpCore::deleteUser()
{
    QPointer<DeleteUserResponse> resp(new DeleteUserResponse);

    auto rep = m_client->deleteUser();
    connect(rep, &bsmi::INetworkReply::finished, this, [resp, rep, this]() {
        if (!resp) {
            rep->deleteLater();
            return;
        }
        resp->setHttpStatusCode(readHttpStatusCode(rep));
        if (rep->error() == QNetworkReply::NoError) {
            resp->setPayload(rep->readAll());
        } else {
            resp->setError(rep->errorString());
        }

        rep->deleteLater();
        resp->setFinished();
    });

    return resp;
}

GetUserProfileResponse *FpCore::getUserProfile()
{
    QPointer<GetUserProfileResponse> resp(new GetUserProfileResponse);

    auto rep = m_client->getUserProfile();
    connect(rep, &bsmi::INetworkReply::finished, this, [resp, rep, this]() {
        if (!resp) {
            rep->deleteLater();
            return;
        }
        resp->setHttpStatusCode(readHttpStatusCode(rep));
        if (rep->error() == QNetworkReply::NoError) {
            resp->setPayload(rep->readAll());
        } else {
            resp->setError(rep->errorString());
        }

        rep->deleteLater();
        resp->setFinished();
    });

    return resp;
}

Fpx::AuthenticationState FpCore::authState() const
{
    return m_authenticationState;
}

void FpCore::loadLaunchSettings()
{
    auto launchSettings = m_settings->readLaunchSettings();

    if (launchSettings.accessToken && launchSettings.expiry) {
        // Pretend the token is expired if it will expire shortly
        auto minutesInFuture = 5;
        auto futureNow = QDateTime::currentDateTimeUtc().addSecs(minutesInFuture * 60);
        if (*launchSettings.expiry > futureNow) {
            setAccessToken(*launchSettings.accessToken, *launchSettings.expiry);
        }
    }
}

void FpCore::signOut()
{
    m_client->setAuthToken(QString(), QDateTime());
    m_settings->clearAccessToken();
}

void FpCore::onClientAuthenticated()
{
    if (!m_client->authToken().isEmpty()) {
        if (m_client->expiry() > QDateTime::currentDateTimeUtc()) {
            setAuthenticationStateImpl(Fpx::AuthenticationState::AuthenticatedState);
        } else {
            setAuthenticationStateImpl(Fpx::AuthenticationState::NotAuthenticatedState);
        }
    } else {
        setAuthenticationStateImpl(Fpx::AuthenticationState::NotAuthenticatedState);
    }
}

void FpCore::setAuthenticationStateImpl(Fpx::AuthenticationState state)
{
    if (m_authenticationState == state) {
        return;
    }

    m_authenticationState = state;
    emit authStateChanged(state);
}

void FpCore::signInAsNewUser(const NewUserDetails &newUser)
{
    auto auth = authenticate(newUser.email(), newUser.password());
    connect(auth, &BaseResponse::finished, this, [auth, this]() {
        if (!auth->hasError()) {
            //
        } else {
            //
        }
        auth->deleteLater();
    });
}

int FpCore::readHttpStatusCode(bsmi::INetworkReply *reply)
{
    return reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}
