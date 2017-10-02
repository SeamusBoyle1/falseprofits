// Copyright 2017

#include "fpwatchlistlistmodel.h"

FpWatchlistListModel::FpWatchlistListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(SymbolRole, "symbol");
    m_roleNames.insert(NameRole, "name");
    m_roleNames.insert(LastPriceRole, "lastPrice");
    m_roleNames.insert(ChangeRole, "change");
    m_roleNames.insert(ChangePercentRole, "changePercent");
    m_roleNames.insert(DirectionRole, "direction");
}

void FpWatchlistListModel::resetWithData(QVector<WatchlistItem> data)
{
    beginResetModel();
    m_rows = data;
    endResetModel();
}

int FpWatchlistListModel::addSymbol(const QString &symbol)
{
    auto it = constFindItemBySymbol(symbol);
    if (it != m_rows.constEnd()) {
        // If already exists, return row number
        return int(std::distance(m_rows.cbegin(), it));
    }

    auto nextRow = m_rows.size();
    beginInsertRows(QModelIndex(), nextRow, nextRow);
    WatchlistItem e;
    e.symbol = symbol;
    m_rows.append(e);
    endInsertRows();
    return m_rows.size() - 1;
}

void FpWatchlistListModel::removeSymbol(const QString &symbol)
{
    auto it = constFindItemBySymbol(symbol);
    if (it != m_rows.constEnd()) {
        auto row = int(std::distance(m_rows.cbegin(), it));
        beginRemoveRows(QModelIndex(), row, row);
        m_rows.remove(row);
        endRemoveRows();
    }
}

void FpWatchlistListModel::remove(int row)
{
    if (row >= m_rows.size() || row < 0)
        return;
    beginRemoveRows(QModelIndex(), row, row);
    m_rows.remove(row);
    endRemoveRows();
}

QString FpWatchlistListModel::getSymbolForRow(int row) const
{
    if (row >= m_rows.size() || row < 0)
        return QString();
    return m_rows.at(row).symbol;
}

int FpWatchlistListModel::getRowOfSymbol(const QString &symbol) const
{
    auto it = constFindItemBySymbol(symbol);
    if (it != m_rows.constEnd()) {
        return int(std::distance(m_rows.cbegin(), it));
    }
    return -1;
}

int FpWatchlistListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_rows.size();
}

QVariant FpWatchlistListModel::data(const QModelIndex &index, int role) const
{
    auto const row = index.row();
    if (row >= m_rows.size() || row < 0)
        return QVariant();

    if (role > Qt::UserRole) {
        switch (role) {
        case SymbolRole:
            return m_rows.at(row).symbol;
        case NameRole:
            return m_rows.at(row).name;
        case LastPriceRole: {
            auto last = m_rows.at(row).lastPrice;
            return last > 0 ? last : QVariant();
        }
        case ChangeRole:
            return m_rows.at(row).change;
        case ChangePercentRole:
            return m_rows.at(row).changePercent;
        case DirectionRole: {
            auto change = m_rows.at(row).change;
            return change > 0 ? 1 : change < 0 ? -1 : 0;
        }
        default:
            break;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> FpWatchlistListModel::roleNames() const
{
    return m_roleNames;
}
