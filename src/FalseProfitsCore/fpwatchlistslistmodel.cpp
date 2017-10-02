// Copyright 2017

#include "fpwatchlistslistmodel.h"

FpWatchlistsListModel::FpWatchlistsListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(Role::IdRole, "id");
    m_roleNames.insert(Role::NameRole, "name");
}

void FpWatchlistsListModel::resetWithData(QVector<WatchlistListItem> data)
{
    beginResetModel();
    m_rows = data;
    endResetModel();
}

QString FpWatchlistsListModel::getWatchlistId(int row) const
{
    if (row >= m_rows.size() || row < 0)
        return QString();
    return m_rows.at(row).id;
}

int FpWatchlistsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rows.size();
}

QVariant FpWatchlistsListModel::data(const QModelIndex &index, int role) const
{
    auto const row = index.row();
    if (row >= m_rows.size() || row < 0)
        return QVariant();

    if (role > Qt::UserRole) {
        switch (role) {
        case IdRole:
            return m_rows.at(row).id;
        case NameRole:
            return m_rows.at(row).name;
        default:
            break;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> FpWatchlistsListModel::roleNames() const
{
    return m_roleNames;
}
