// Copyright 2017

#include "fpaccountslistmodel.h"

#include <limits>

FpAccountsListModel::FpAccountsListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(Role::IdRole, "id");
    m_roleNames.insert(Role::NameRole, "name");
    m_roleNames.insert(Role::BalanceRole, "balance");
}

void FpAccountsListModel::resetWithData(QVector<TradingAccountItem> data)
{
    beginResetModel();
    m_rows = data;
    endResetModel();
}

/*!
 * Update accounts with data from \a data, remove accounts not in \a data.
 */
void FpAccountsListModel::updateOrRemoveData(QVector<TradingAccountItem> data)
{
    // TODO(seamus: Implement
    resetWithData(data);
}

QString FpAccountsListModel::getAccountId(int row) const
{
    if (row >= m_rows.size() || row < 0)
        return QString();
    return m_rows.at(row).id;
}

double FpAccountsListModel::getBalance(int row) const
{
    if (row >= m_rows.size() || row < 0)
        return std::numeric_limits<double>::quiet_NaN();
    return m_rows.at(row).balance;
}

int FpAccountsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rows.size();
}

QVariant FpAccountsListModel::data(const QModelIndex &index, int role) const
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
        case BalanceRole:
            return m_rows.at(row).balance;
        default:
            break;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> FpAccountsListModel::roleNames() const
{
    return m_roleNames;
}
