// Copyright 2017

#ifndef FPACCOUNTSLISTMODEL_H
#define FPACCOUNTSLISTMODEL_H

#include <QAbstractListModel>

#include <QHash>
#include <QVector>

struct TradingAccountItem
{
    QString id;
    QString name;
    double balance{ 0 };
};

class FpAccountsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        IdRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        BalanceRole = Qt::UserRole + 3,
    };

    explicit FpAccountsListModel(QObject *parent = nullptr);

    void resetWithData(QVector<TradingAccountItem> data);

    void updateOrRemoveData(QVector<TradingAccountItem> data);

signals:

public slots:

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<TradingAccountItem> m_rows;
    QHash<int, QByteArray> m_roleNames;
};

#endif // FPACCOUNTSLISTMODEL_H
