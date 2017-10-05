// Copyright 2017

#ifndef FPPOSITIONSLISTMODEL_H
#define FPPOSITIONSLISTMODEL_H

#include <QAbstractListModel>

#include <QHash>
#include <QVector>

struct PositionItem
{
    QString symbol;
    QString name;
    int quantity{ 0 };
    double averagePrice{ 0 };
    double lastPrice{ 0 };
    double change{ 0 };
    double changePercent{ 0 };
};

class FpPositionsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        SymbolRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        QuantityRole = Qt::UserRole + 3,
        AveragePriceRole = Qt::UserRole + 4,
        LastPriceRole = Qt::UserRole + 5,
        ChangeRole = Qt::UserRole + 6,
        ChangePercentRole = Qt::UserRole + 7,
        DirectionRole = Qt::UserRole + 8,
        MarketValueRole = Qt::UserRole + 9,
        CostRole = Qt::UserRole + 10,
        ProfitLossRole = Qt::UserRole + 11,
        ProfitLossPercentRole = Qt::UserRole + 12,
    };
    explicit FpPositionsListModel(QObject *parent = nullptr);

    void resetWithData(QVector<PositionItem> data);

    Q_INVOKABLE
    QString getSymbolForRow(int row) const;

    Q_INVOKABLE
    int getQuantityForRow(int row) const;

signals:

public slots:

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<PositionItem> m_rows;
    QHash<int, QByteArray> m_roleNames;
};

#endif // FPPOSITIONSLISTMODEL_H
