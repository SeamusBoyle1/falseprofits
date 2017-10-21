// Copyright 2017

#include "fpleaderboardlistmodel.h"

FpLeaderboardListModel::FpLeaderboardListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(RankRole, "rank");
    m_roleNames.insert(DisplayNameRole, "displayName");
    m_roleNames.insert(TotalAccountValueRole, "totalAccountValue");
    m_roleNames.insert(ProfitRole, "profit");
    m_roleNames.insert(ProfitPercentRole, "profitPercent");
    m_roleNames.insert(IsCurrentUserRole, "isCurrentUser");
    m_roleNames.insert(GravatarUrl, "gravatarUrl");
}

void FpLeaderboardListModel::resetWithData(QVector<LeaderBoardUser> data)
{
    beginResetModel();
    m_rows = data;
    endResetModel();
}

void FpLeaderboardListModel::appendData(const QVector<LeaderBoardUser> &data)
{
    auto firstNewRow = m_rows.size();
    beginInsertRows(QModelIndex(), firstNewRow, firstNewRow + data.size() - 1);
    m_rows += data;
    endInsertRows();
}

int FpLeaderboardListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rows.size();
}

QVariant FpLeaderboardListModel::data(const QModelIndex &index, int role) const
{
    auto const row = index.row();
    if (row >= m_rows.size() || row < 0)
        return QVariant();

    if (role > Qt::UserRole) {
        switch (role) {
        case RankRole:
            return m_rows.at(row).rank;
        case DisplayNameRole:
            return m_rows.at(row).displayName;
        case TotalAccountValueRole:
            return m_rows.at(row).totalAccountValue;
        case ProfitRole:
            return m_rows.at(row).profit;
        case ProfitPercentRole:
            return m_rows.at(row).profitPercent;
        case IsCurrentUserRole:
            return m_rows.at(row).isCurrentUser;
        case GravatarUrl:
            return m_rows.at(row).gravatarUrl;
        default:
            break;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> FpLeaderboardListModel::roleNames() const
{
    return m_roleNames;
}
