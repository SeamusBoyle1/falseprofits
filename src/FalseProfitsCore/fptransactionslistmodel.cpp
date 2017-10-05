// Copyright 2017

#include "fptransactionslistmodel.h"

#include <QLocale>

FpTransactionsListModel::FpTransactionsListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(TimestampRole, "timestamp");
    m_roleNames.insert(TypeRole, "type");
    m_roleNames.insert(DescriptionRole, "description");
    m_roleNames.insert(AmountRole, "amount");
    m_roleNames.insert(BalanceRole, "balance");
    m_roleNames.insert(LocalDateRole, "localDate");
    m_roleNames.insert(LocalTimeRole, "localTime");
}

void FpTransactionsListModel::resetWithData(QVector<TransactionInfo> data)
{
    beginResetModel();
    m_rows = data;
    endResetModel();
}

void FpTransactionsListModel::appendData(const QVector<TransactionInfo> &data)
{
    auto firstNewRow = m_rows.size();
    beginInsertRows(QModelIndex(), firstNewRow, firstNewRow + data.size() - 1);
    m_rows += data;
    endInsertRows();
}

int FpTransactionsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rows.size();
}

QVariant FpTransactionsListModel::data(const QModelIndex &index, int role) const
{
    auto const row = index.row();
    if (row >= m_rows.size() || row < 0)
        return QVariant();

    if (role > Qt::UserRole) {
        switch (role) {
        case TimestampRole:
            return m_rows.at(row).timestampUtc;
        case TypeRole:
            return m_rows.at(row).type;
        case DescriptionRole:
            return m_rows.at(row).description;
        case AmountRole:
            return m_rows.at(row).amount;
        case BalanceRole:
            return m_rows.at(row).balance;
        case LocalDateRole: {
            QLocale locale_;
            return locale_.toString(m_rows.at(row).timestampUtc.toLocalTime(),
                                    locale_.dateFormat(QLocale::LongFormat));
        }
        case LocalTimeRole: {
            QLocale locale_;
            return locale_.toString(m_rows.at(row).timestampUtc.toLocalTime(),
                                    locale_.timeFormat(QLocale::NarrowFormat));
        }
        default:
            break;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> FpTransactionsListModel::roleNames() const
{
    return m_roleNames;
}
