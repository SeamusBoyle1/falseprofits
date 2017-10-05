// Copyright 2017

#ifndef FPTRANSACTIONSLISTMODEL_H
#define FPTRANSACTIONSLISTMODEL_H

#include <QAbstractListModel>

#include <QDateTime>
#include <QHash>
#include <QVector>

struct TransactionInfo
{
    QDateTime timestampUtc;
    QString type;
    QString description;
    double amount{ 0 };
    double balance{ 0 };
};

class FpTransactionsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        TimestampRole = Qt::UserRole + 1,
        TypeRole = Qt::UserRole + 2,
        DescriptionRole = Qt::UserRole + 3,
        AmountRole = Qt::UserRole + 4,
        BalanceRole = Qt::UserRole + 5,
        LocalDateRole = Qt::UserRole + 6,
        LocalTimeRole = Qt::UserRole + 7,
    };
    explicit FpTransactionsListModel(QObject *parent = nullptr);

    void resetWithData(QVector<TransactionInfo> data);

    void appendData(const QVector<TransactionInfo> &data);

signals:

public slots:

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<TransactionInfo> m_rows;
    QHash<int, QByteArray> m_roleNames;
};

#endif // FPTRANSACTIONSLISTMODEL_H
