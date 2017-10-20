// Copyright 2017

#include "fpwatchlistwrapper.h"

#include "finishnotifier.h"
#include "fpcore.h"
#include "fpwatchlistlistmodel.h"

#include <InvestorAPIClient/jsonobjectwrappers.h>

#include <QPointer>

#define WATCHLIST_DEBUG_OFFLINE 0

FpWatchlistWrapper::FpWatchlistWrapper(QObject *parent)
    : QObject(parent)
    , m_model{ new FpWatchlistListModel(this) }
{
}

void FpWatchlistWrapper::setCoreClient(FpCore *core)
{
    if (m_fpCore) {
        disconnect(m_fpCore, &FpCore::authStateChanged, this, &FpWatchlistWrapper::unloadWatchlist);
        disconnect(m_fpCore, &FpCore::watchlistChanged, this,
                   &FpWatchlistWrapper::slotWatchlistChanged);
    }

    m_fpCore = core;

    if (core) {
        connect(core, &FpCore::authStateChanged, this, &FpWatchlistWrapper::unloadWatchlist);
        connect(core, &FpCore::watchlistChanged, this, &FpWatchlistWrapper::slotWatchlistChanged);
    }
}

FinishNotifier *FpWatchlistWrapper::refreshWatchlist()
{
    Q_ASSERT(m_fpCore);

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    if (m_currentWatchlistId.isEmpty()) {
        QMetaObject::invokeMethod(notifier.data(), "setFinished", Qt::QueuedConnection);
        return notifier;
    }

#if !WATCHLIST_DEBUG_OFFLINE
    auto resp = m_fpCore->getWatchlist(m_currentWatchlistId);
    connect(resp, &GetWatchlistResponse::finished, this, [notifier, resp, this] {
        onGetResponseReceived(resp);
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });
#else
    QMetaObject::invokeMethod(notifier.data(), "setFinished", Qt::QueuedConnection);
#endif

    setDirty(false);

    return notifier;
}

QString FpWatchlistWrapper::currentWatchlistId() const
{
    return m_currentWatchlistId;
}

FinishNotifier *FpWatchlistWrapper::loadWatchlist(const QString &watchlistId)
{
    m_model->resetWithData(QVector<WatchlistItem>{});
    m_currentWatchlistId = watchlistId;
    return refreshWatchlist();
}

FinishNotifier *FpWatchlistWrapper::addSymbol(const QString &symbol)
{
    Q_ASSERT(m_fpCore);

    // insert placeholder
    m_model->addSymbol(symbol);

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    if (m_currentWatchlistId.isEmpty()) {
        QMetaObject::invokeMethod(notifier.data(), "setFinished", Qt::QueuedConnection);
        return notifier;
    }

#if !WATCHLIST_DEBUG_OFFLINE
    auto resp = m_fpCore->addSymbolToWatchlist(m_currentWatchlistId, symbol);
    connect(resp, &GetUserProfileResponse::finished, this, [notifier, resp, this] {
        onAddResponseReceived(resp);
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });
#else
    QMetaObject::invokeMethod(notifier.data(), "setFinished", Qt::QueuedConnection);
#endif

    return notifier;
}

FinishNotifier *FpWatchlistWrapper::removeSymbol(const QString &symbol)
{
    Q_ASSERT(m_fpCore);

    // optimistic removal
    m_model->removeSymbol(symbol);

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    if (m_currentWatchlistId.isEmpty()) {
        QMetaObject::invokeMethod(notifier.data(), "setFinished", Qt::QueuedConnection);
        return notifier;
    }

#if !WATCHLIST_DEBUG_OFFLINE
    auto resp = m_fpCore->removeSymbolFromWatchlist(m_currentWatchlistId, symbol);
    connect(resp, &GetUserProfileResponse::finished, this, [notifier, resp, this] {
        onRemoveResponseReceived(resp);
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });
#else
    QMetaObject::invokeMethod(notifier.data(), "setFinished", Qt::QueuedConnection);
#endif

    return notifier;
}

bool FpWatchlistWrapper::isDirty() const
{
    return m_dirty;
}

void FpWatchlistWrapper::unloadWatchlist()
{
    m_model->resetWithData(QVector<WatchlistItem>{});
    m_currentWatchlistId.clear();
}

void FpWatchlistWrapper::setDirty(bool dirty)
{
    if (m_dirty == dirty)
        return;

    m_dirty = dirty;
    emit dirtyChanged(m_dirty);
}

void FpWatchlistWrapper::slotWatchlistChanged(const QString watchlistId, const QString &symbol,
                                              bool added)
{
    Q_UNUSED(symbol)
    if (m_currentWatchlistId != watchlistId)
        return;

    if (added) {
        setDirty(true);
    }
}

void FpWatchlistWrapper::onGetResponseReceived(GetWatchlistResponse *reply)
{
    if (!reply->hasError()) {
        auto doc = QJsonDocument::fromJson(reply->payload());
        bsmi::JsonObjectWrappers::WatchlistResponse responseObj;
        responseObj.d = doc.object();
        auto responseShares = responseObj.shares();

        auto watchlistId = responseObj.id();
        if (!watchlistId || *watchlistId != m_currentWatchlistId) {
            return;
        }

        QVector<WatchlistItem> rows;
        rows.reserve(responseShares.size());
        for (auto i = 0, total = responseShares.size(); i < total; ++i) {
            auto const e = responseShares.at(i);

            WatchlistItem r;
            auto symbol = e.symbol();
            if (symbol) {
                r.symbol = *symbol;
            }
            auto name = e.name();
            if (name) {
                r.name = *name;
            }
            auto lastPrice = e.lastPrice();
            if (lastPrice) {
                r.lastPrice = *lastPrice;
            }
            auto change = e.change();
            if (change) {
                r.change = *change;
            }
            auto changePercent = e.changePercent();
            if (changePercent) {
                r.changePercent = *changePercent;
            }

            rows.append(r);
        }

        util::sortBySymbol(rows.begin(), rows.end());

        m_model->resetWithData(rows);
    } else {
        // TODO(seamus): Handle error case
        m_model->resetWithData(QVector<WatchlistItem>{});
    }
}

void FpWatchlistWrapper::onAddResponseReceived(AddSymbolToWatchlistResponse *reply)
{
    if (!reply->hasError()) {
    } else {
        // TODO(seamus): Handle error case
    }
}

void FpWatchlistWrapper::onRemoveResponseReceived(RemoveSymbolFromWatchlistResponse *reply)
{
    if (!reply->hasError()) {
    } else {
        // TODO(seamus): Handle error case
    }
}
