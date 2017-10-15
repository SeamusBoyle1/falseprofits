// Copyright 2017

#ifndef FPLEADERBOARDWRAPPER_H
#define FPLEADERBOARDWRAPPER_H

#include <QObject>

#include "fpleaderboardlistmodel.h"

#include <boost/optional.hpp>

class FinishNotifier;
class FpCore;
class GetLeaderboardResponse;
class GetLeaderboardMeResponse;

class FpLeaderboardWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FpCore *coreClient READ coreClient WRITE setCoreClient)
    Q_PROPERTY(FpLeaderboardListModel *model READ model)
public:
    explicit FpLeaderboardWrapper(QObject *parent = nullptr);

    FpCore *coreClient() const { return m_fpCore; }
    void setCoreClient(FpCore *core);

    FpLeaderboardListModel *model() const { return m_model; }

    Q_INVOKABLE
    FinishNotifier *refreshLeaderboard();

    Q_INVOKABLE
    FinishNotifier *getNextPage();

    Q_INVOKABLE
    FinishNotifier *refreshLeaderboardMe();

    Q_INVOKABLE
    bool canFetchMore() const;

    Q_INVOKABLE
    bool gotCurrentUserRank() const;

    Q_INVOKABLE
    QVariant currentUserRank() const;

    Q_INVOKABLE
    QVariant currentUserDisplayName() const;

    Q_INVOKABLE
    QVariant currentUserProfit() const;

    Q_INVOKABLE
    QVariant currentUserProfitPercent() const;

signals:

public slots:
    void unloadLeaderboard();

private:
    void onLeaderboardReceived(GetLeaderboardResponse *reply, bool append);

    void onLeaderboardMeReceived(GetLeaderboardMeResponse *reply);

    FpLeaderboardListModel *m_model;
    FpCore *m_fpCore{ nullptr };
    qint64 m_leaderboardRequestId{ 0 };
    qint64 m_leaderboardMeRequestId{ 0 };
    QVector<LeaderBoardUser> m_lastMeResponse;
    boost::optional<LeaderBoardUser> m_currentUserRank;
    int m_pageNumber{ 0 };
    int m_totalPageCount{ 0 };
};

#endif // FPLEADERBOARDWRAPPER_H
