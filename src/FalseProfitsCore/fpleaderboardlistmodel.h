// Copyright 2017

#ifndef FPLEADERBOARDLISTMODEL_H
#define FPLEADERBOARDLISTMODEL_H

#include <QAbstractListModel>

#include <QHash>
#include <QUrl>
#include <QVector>

struct LeaderBoardUser
{
    int rank{ 0 };
    QString displayName;
    QUrl gravatarUrl;
    double totalAccountValue{ 0 };
    double profit{ 0 };
    double profitPercent{ 0 };
    bool isCurrentUser{ false };
};

class FpLeaderboardListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        RankRole = Qt::UserRole + 1,
        DisplayNameRole = Qt::UserRole + 2,
        TotalAccountValueRole = Qt::UserRole + 3,
        ProfitRole = Qt::UserRole + 4,
        ProfitPercentRole = Qt::UserRole + 5,
        IsCurrentUserRole = Qt::UserRole + 6,
        GravatarUrl = Qt::UserRole + 7,
    };

    explicit FpLeaderboardListModel(QObject *parent = nullptr);

    void resetWithData(QVector<LeaderBoardUser> data);

    void appendData(const QVector<LeaderBoardUser> &data);

signals:

public slots:

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<LeaderBoardUser> m_rows;
    QHash<int, QByteArray> m_roleNames;
};

#endif // FPLEADERBOARDLISTMODEL_H
