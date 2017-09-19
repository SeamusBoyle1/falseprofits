// Copyright 2017

#include "fpsymbolsearchresultsitemmodel.h"

FpSymbolSearchResultsItemModel::FpSymbolSearchResultsItemModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(Role::SymbolRole, "symbol");
    m_roleNames.insert(Role::CompanyNameRole, "companyName");
    m_roleNames.insert(Role::IndustryRole, "industry");
}

void FpSymbolSearchResultsItemModel::resetWithData(QVector<SymbolSearchResultItem> data)
{
    beginResetModel();
    m_rows = data;
    endResetModel();
}

void FpSymbolSearchResultsItemModel::appendData(const QVector<SymbolSearchResultItem> &data)
{
    auto firstNewRow = m_rows.size();
    beginInsertRows(QModelIndex(), firstNewRow, firstNewRow + data.size() - 1);
    m_rows += data;
    endInsertRows();
}

int FpSymbolSearchResultsItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rows.size();
}

QVariant FpSymbolSearchResultsItemModel::data(const QModelIndex &index, int role) const
{
    auto const row = index.row();
    if (row >= m_rows.size() || row < 0)
        return QVariant();

    if (role > Qt::UserRole) {
        switch (role) {
        case SymbolRole:
            return m_rows.at(row).symbol;
        case CompanyNameRole:
            return m_rows.at(row).companyName;
        case IndustryRole:
            return m_rows.at(row).industry;
        default:
            break;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> FpSymbolSearchResultsItemModel::roleNames() const
{
    return m_roleNames;
}
