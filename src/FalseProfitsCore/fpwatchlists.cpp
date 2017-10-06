// Copyright 2017

#include "fpwatchlists.h"

#include "finishnotifier.h"
#include "fpwatchlistslistmodel.h"
#include "fpcore.h"

#include <InvestorAPIClient/jsonobjectwrappers.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QPointer>

FpWatchlists::FpWatchlists(QObject *parent)
    : QObject(parent)
    , m_model{ new FpWatchlistsListModel(this) }
{
}

void FpWatchlists::setCoreClient(FpCore *core)
{
    if (m_fpCore) {
        disconnect(m_fpCore, &FpCore::authStateChanged, this, &FpWatchlists::unloadWatchlistList);
    }

    m_fpCore = core;

    if (core) {
        connect(core, &FpCore::authStateChanged, this, &FpWatchlists::unloadWatchlistList);
    }
}

FinishNotifier *FpWatchlists::updateWatchlistList()
{
    Q_ASSERT(m_fpCore);

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    auto resp = m_fpCore->getUserProfile();
    connect(resp, &GetUserProfileResponse::finished, this, [notifier, resp, this] {
        onResponseReceived(resp);
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });

    return notifier;
}

void FpWatchlists::unloadWatchlistList()
{
    m_model->resetWithData(QVector<WatchlistListItem>());
}

void FpWatchlists::onResponseReceived(GetUserProfileResponse *reply)
{
    if (!reply->hasError()) {
        auto doc = QJsonDocument::fromJson(reply->payload());
        bsmi::JsonObjectWrappers::UserProfileResponse responseObj;
        responseObj.d = doc.object();
        auto jsonWatchlists = responseObj.watchlists();

        QVector<WatchlistListItem> watchlists;
        watchlists.reserve(jsonWatchlists.size());
        for (auto i = 0, total = jsonWatchlists.size(); i < total; ++i) {
            auto const e = jsonWatchlists.at(i);

            WatchlistListItem r;
            auto id = e.id();
            if (!id) {
                continue;
            }
            r.id = *id;
            auto name = e.name();
            if (name) {
                r.name = *name;
            }

            watchlists.append(r);
        }

        m_model->resetWithData(watchlists);
    } else {
        // TODO(seamus): Handle error case
    }
}
