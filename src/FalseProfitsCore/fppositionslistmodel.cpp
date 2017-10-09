// Copyright 2017

#include "fppositionslistmodel.h"

FpPositionsListModel::FpPositionsListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(SymbolRole, "symbol");
    m_roleNames.insert(NameRole, "name");
    m_roleNames.insert(QuantityRole, "quantity");
    m_roleNames.insert(AveragePriceRole, "averagePrice");
    m_roleNames.insert(LastPriceRole, "lastPrice");
    m_roleNames.insert(ChangeRole, "change");
    m_roleNames.insert(ChangePercentRole, "changePercent");
    m_roleNames.insert(DirectionRole, "direction");
    m_roleNames.insert(MarketValueRole, "marketValue");
    m_roleNames.insert(CostRole, "cost");
    m_roleNames.insert(ProfitLossRole, "profitLoss");
    m_roleNames.insert(ProfitLossPercentRole, "profitLossPercent");
}

void FpPositionsListModel::resetWithData(QVector<PositionItem> data)
{
    beginResetModel();
    m_rows = data;
    endResetModel();
}

QString FpPositionsListModel::getSymbolForRow(int row) const
{
    if (row >= m_rows.size() || row < 0)
        return QString();
    return m_rows.at(row).symbol;
}

int FpPositionsListModel::getQuantityForRow(int row) const
{
    if (row >= m_rows.size() || row < 0)
        return -1;
    return m_rows.at(row).quantity;
}

int FpPositionsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rows.size();
}

QVariant FpPositionsListModel::data(const QModelIndex &index, int role) const
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
        case QuantityRole:
            return m_rows.at(row).quantity;
        case AveragePriceRole:
            return m_rows.at(row).averagePrice;
        case LastPriceRole:
            return m_rows.at(row).lastPrice;
        case ChangeRole:
            return m_rows.at(row).change;
        case ChangePercentRole:
            return m_rows.at(row).changePercent;
        case DirectionRole: {
            auto const &e = m_rows.at(row);
            return e.lastPrice > e.averagePrice ? 1 : e.lastPrice < e.averagePrice ? -1 : 0;
        }
        case MarketValueRole: {
            auto const &e = m_rows.at(row);
            return e.lastPrice > 0 ? e.quantity * e.lastPrice : QVariant();
        }
        case CostRole: {
            auto const &e = m_rows.at(row);
            return e.quantity * e.averagePrice;
        }
        case ProfitLossRole: {
            auto const &e = m_rows.at(row);
            if (!(e.lastPrice > 0))
                return QVariant();
            return e.quantity * (e.lastPrice - e.averagePrice);
        }
        case ProfitLossPercentRole: {
            auto const &e = m_rows.at(row);
            if (!(e.lastPrice > 0))
                return QVariant();
            auto marketValue = e.quantity * e.lastPrice;
            auto cost = e.quantity * e.averagePrice;
            return ((marketValue - cost) / cost) * 100;
        }
        default:
            break;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> FpPositionsListModel::roleNames() const
{
    return m_roleNames;
}
