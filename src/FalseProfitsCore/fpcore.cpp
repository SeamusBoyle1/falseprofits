// Copyright 2017

#include "fpcore.h"

#include "fpcore_interfaces.h"

#include <InvestorAPIClient/iinvestorapiclient.h>
#include <InvestorAPIClient/jsonobjectwrappers.h>

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QPointer>

FpCore::FpCore(bsmi::IInvestorAPIClient *client, IFpSettings *settings, QObject *parent)
    : QObject(parent)
    , m_client{ client }
    , m_settings{ settings }
{
    m_coreAttributes[AutoAuthenticateNewUserAttribute] = true;

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

qint64 FpCore::getNonce() const
{
    auto now = QDateTime::currentMSecsSinceEpoch();
    while (now <= m_lastNonce) {
        ++now;
    }
    m_lastNonce = now;
    return now;
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
            if (m_coreAttributes[AutoAuthenticateNewUserAttribute].toBool()) {
                signInAsNewUser(newUser);
            }
        } else {
            resp->setError(rep->errorString());

            if (resp->httpStatusCode() >= 400 || resp->httpStatusCode() <= 499) {
                resp->setPayload(rep->readAll());
            }
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

            clearAccessToken();
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

GetCommissionsResponse *FpCore::getCommissions(Fpx::CommissionSide side)
{
    QPointer<GetCommissionsResponse> resp(new GetCommissionsResponse);

    auto rep = m_client->getCommissions(side == Fpx::CommissionSide::BuySide
                                            ? bsmi::IInvestorAPIClient::CommissionSide::Buy
                                            : bsmi::IInvestorAPIClient::CommissionSide::Sell);
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

GetQuotesResponse *FpCore::getQuotes(const QStringList &symbols)
{
    QPointer<GetQuotesResponse> resp(new GetQuotesResponse);

    auto rep = m_client->getQuotes(symbols);
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

SymbolSearchResponse *FpCore::symbolSearch(const SymbolSearchQuery &query)
{
    QPointer<SymbolSearchResponse> resp(new SymbolSearchResponse);

    bsmi::IInvestorAPIClient::SymbolSearchQuery v;
    v.searchTerm = query.searchTerm();
    v.pageNumber = query.pageNumber();
    v.pageSize = query.pageSize();

    auto rep = m_client->symbolSearch(v);
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

GetShareDetailsResponse *FpCore::getShareDetails(const QString &symbol)
{
    QPointer<GetShareDetailsResponse> resp(new GetShareDetailsResponse);
    resp->setSymbol(symbol);

    bsmi::IInvestorAPIClient::SymbolSearchQuery v;
    v.searchTerm = symbol;

    auto rep = m_client->symbolSearch(v);
    connect(rep, &bsmi::INetworkReply::finished, this, [symbol, resp, rep, this]() {
        if (!resp) {
            rep->deleteLater();
            return;
        }
        resp->setHttpStatusCode(readHttpStatusCode(rep));
        if (rep->error() == QNetworkReply::NoError) {
            resp->setPayload(rep->readAll());
            auto doc = QJsonDocument::fromJson(resp->payload());
            auto parser = bsmi::JsonObjectWrappers::SymbolSearchResponse(doc.object());
            auto items = parser.items();

            auto it = std::find_if(items.jsonItems.constBegin(), items.jsonItems.constEnd(),
                                   [symbol](const QJsonValue &a) {
                                       return a.toObject().value(QLatin1String("symbol")) == symbol;
                                   });
            auto itemFound = false;
            if (it != items.jsonItems.constEnd()) {
                bsmi::JsonObjectWrappers::SymbolSearchResponseItem item;
                item.d = it->toObject();
                if (item.isValid()) {
                    itemFound = true;
                    auto cn = item.name();
                    if (cn) {
                        resp->setCompanyName(*cn);
                    }
                    auto industry = item.industry();
                    if (industry) {
                        resp->setIndustry(*industry);
                    }
                }
            }
            if (!itemFound) {
                resp->setHttpStatusCode(404);
            }
        } else {
            resp->setError(rep->errorString());
        }

        rep->deleteLater();
        resp->setFinished();
    });

    return resp;
}

SendOrderResponse *FpCore::sendOrder(const QString &accountId, const OrderParams &args)
{
    QPointer<SendOrderResponse> resp(new SendOrderResponse);

    bsmi::IInvestorAPIClient::OrderParams v;
    v.symbol = args.symbol();
    v.quantity = args.quantity();
    v.side =
        args.side() == OrderParams::Side::SellSide ? QStringLiteral("Sell") : QStringLiteral("Buy");
    v.nonce = args.nonce() != 0 ? args.nonce() : getNonce();

    auto rep = m_client->sendOrder(accountId, v);
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

    emit launchSettingsLoaded();
}

void FpCore::signOut()
{
    clearAccessToken();
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

void FpCore::clearAccessToken()
{
    m_client->setAuthToken(QString(), QDateTime());
    m_settings->clearAccessToken();
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
