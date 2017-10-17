// Copyright 2017

#include "fpleaderboardwrapper.h"

#include "finishnotifier.h"
#include "fpcore.h"
#include "fpdeclarativetypes.h"
#include "responsetypes.h"

#include <InvestorAPIClient/jsonobjectwrappers.h>

#include <QPointer>

FpLeaderboardWrapper::FpLeaderboardWrapper(QObject *parent)
    : QObject(parent)
    , m_model{ new FpLeaderboardListModel(this) }
{
}

void FpLeaderboardWrapper::setCoreClient(FpCore *core)
{
    if (m_fpCore) {
        disconnect(m_fpCore, &FpCore::authStateChanged, this,
                   &FpLeaderboardWrapper::unloadLeaderboard);
    }

    m_fpCore = core;

    if (core) {
        connect(core, &FpCore::authStateChanged, this, &FpLeaderboardWrapper::unloadLeaderboard);
    }
}

FinishNotifier *FpLeaderboardWrapper::refreshLeaderboard()
{
    Q_ASSERT(m_fpCore);

    m_model->resetWithData(QVector<LeaderBoardUser>{});
    m_pageNumber = 0;
    m_totalPageCount = 0;

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    auto requestId = ++m_leaderboardRequestId;
    auto resp = m_fpCore->getLeaderboard(LeaderboardQuery{});
    connect(resp, &GetLeaderboardResponse::finished, this, [requestId, notifier, resp, this] {
        if (m_leaderboardRequestId == requestId) {
            onLeaderboardReceived(resp, false);
        }
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });

    return notifier;
}

FinishNotifier *FpLeaderboardWrapper::getNextPage()
{
    Q_ASSERT(m_fpCore);

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    LeaderboardQuery query;
    query.setPageNumber(++m_pageNumber);

    auto requestId = ++m_leaderboardRequestId;
    auto resp = m_fpCore->getLeaderboard(query);
    connect(resp, &GetLeaderboardResponse::finished, this, [requestId, notifier, resp, this] {
        if (m_leaderboardRequestId == requestId) {
            onLeaderboardReceived(resp, true);
        }
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });

    return notifier;
}

FinishNotifier *FpLeaderboardWrapper::refreshLeaderboardMe()
{
    Q_ASSERT(m_fpCore);

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    auto requestId = ++m_leaderboardMeRequestId;
    auto resp = m_fpCore->getLeaderboardMe(-1);
    connect(resp, &GetLeaderboardMeResponse::finished, this, [requestId, notifier, resp, this] {
        if (m_leaderboardMeRequestId == requestId) {
            onLeaderboardMeReceived(resp);
        }
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });

    return notifier;
}

bool FpLeaderboardWrapper::canFetchMore() const
{
    return m_pageNumber < m_totalPageCount;
}

bool FpLeaderboardWrapper::gotCurrentUserRank() const
{
    return bool(m_currentUserRank);
}

QVariant FpLeaderboardWrapper::currentUserRank() const
{
    return m_currentUserRank && (*m_currentUserRank).rank > 0 ? (*m_currentUserRank).rank
                                                              : QVariant();
}

QVariant FpLeaderboardWrapper::currentUserDisplayName() const
{
    return m_currentUserRank && !(*m_currentUserRank).displayName.isEmpty()
        ? (*m_currentUserRank).displayName
        : QVariant();
}

QVariant FpLeaderboardWrapper::currentUserProfit() const
{
    return m_currentUserRank ? (*m_currentUserRank).profit : QVariant();
}

QVariant FpLeaderboardWrapper::currentUserProfitPercent() const
{
    return m_currentUserRank ? (*m_currentUserRank).profitPercent : QVariant();
}

void FpLeaderboardWrapper::unloadLeaderboard()
{
    m_model->resetWithData(QVector<LeaderBoardUser>{});
    m_pageNumber = 0;
    m_totalPageCount = 0;
    m_lastMeResponse.clear();
    m_currentUserRank = boost::none;
}

void FpLeaderboardWrapper::onLeaderboardReceived(GetLeaderboardResponse *reply, bool append)
{
    if (!reply->hasError()) {
        auto doc = QJsonDocument::fromJson(reply->payload());
        bsmi::JsonObjectWrappers::LeaderboardResponse responseObj(doc.object());
        auto responseItems = responseObj.items();

        QVector<LeaderBoardUser> rows;
        rows.reserve(responseItems.size());
        for (auto i = 0, total = responseItems.size(); i < total; ++i) {
            auto const e = responseItems.at(i);

            LeaderBoardUser r;
            auto rank = e.rank();
            if (rank) {
                r.rank = *rank;
            }
            auto displayName = e.displayName();
            if (displayName) {
                r.displayName = *displayName;
            }
            auto totalAccountValue = e.totalAccountValue();
            if (totalAccountValue) {
                r.totalAccountValue = *totalAccountValue;
            }
            auto profit = e.profit();
            if (profit) {
                r.profit = *profit;
            }
            auto profitPercent = e.profitPercent();
            if (profitPercent) {
                r.profitPercent = *profitPercent;
            }
            auto isCurrentUser = e.isCurrentUser();
            if (isCurrentUser) {
                r.isCurrentUser = *isCurrentUser;
            }

            rows.append(r);
        }

        m_pageNumber = responseObj.pageNumber();
        m_totalPageCount = responseObj.totalPageCount();

        if (append) {
            m_model->appendData(rows);
        } else {
            m_model->resetWithData(rows);
        }
    } else {
        // TODO(seamus): Handle error case
        m_model->resetWithData(QVector<LeaderBoardUser>{});
    }
}

void FpLeaderboardWrapper::onLeaderboardMeReceived(GetLeaderboardMeResponse *reply)
{
    if (!reply->hasError()) {
        auto doc = QJsonDocument::fromJson(reply->payload());
        bsmi::JsonObjectWrappers::LeaderboardResponseItems responseItems;
        responseItems.jsonItems = doc.array();

        m_lastMeResponse.resize(responseItems.size());
        m_currentUserRank = boost::none;
        for (auto i = 0, total = responseItems.size(); i < total; ++i) {
            auto const e = responseItems.at(i);

            LeaderBoardUser r;
            auto rank = e.rank();
            if (rank) {
                r.rank = *rank;
            }
            auto displayName = e.displayName();
            if (displayName) {
                r.displayName = *displayName;
            }
            auto totalAccountValue = e.totalAccountValue();
            if (totalAccountValue) {
                r.totalAccountValue = *totalAccountValue;
            }
            auto profit = e.profit();
            if (profit) {
                r.profit = *profit;
            }
            auto profitPercent = e.profitPercent();
            if (profitPercent) {
                r.profitPercent = *profitPercent;
            }
            auto isCurrentUser = e.isCurrentUser();
            if (isCurrentUser) {
                r.isCurrentUser = *isCurrentUser;
            }

            m_lastMeResponse[i] = r;

            if (isCurrentUser && *isCurrentUser) {
                m_currentUserRank = r;
            }
        }
    } else {
        // TODO(seamus): Handle error case
    }
}
