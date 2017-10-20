// Copyright 2017

#ifndef FPWATCHLISTLISTMODEL_H
#define FPWATCHLISTLISTMODEL_H

#include <QAbstractListModel>

#include <QHash>
#include <QVector>

#include <algorithm>
#include <iterator>

struct WatchlistItem
{
    QString symbol;
    QString name;
    double lastPrice{ 0 };
    double change{ 0 };
    double changePercent{ 0 };
};

namespace util {

template <class RandomIt>
typename std::enable_if<std::is_same<typename std::iterator_traits<RandomIt>::value_type,
                                     WatchlistItem>::value>::type
sortBySymbol(RandomIt first, RandomIt last)
{
    std::sort(first, last,
              [](const WatchlistItem &a, const WatchlistItem &b) { return a.symbol < b.symbol; });
}

} // namespace util

class FpWatchlistListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        SymbolRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        LastPriceRole = Qt::UserRole + 3,
        ChangeRole = Qt::UserRole + 4,
        ChangePercentRole = Qt::UserRole + 5,
        DirectionRole = Qt::UserRole + 6,
    };
    explicit FpWatchlistListModel(QObject *parent = nullptr);

    void resetWithData(QVector<WatchlistItem> data);

    int addSymbol(const QString &symbol);

    void removeSymbol(const QString &symbol);

    Q_INVOKABLE
    void remove(int row);

    Q_INVOKABLE
    QString getSymbolForRow(int row) const;

    Q_INVOKABLE
    int getRowOfSymbol(const QString &symbol) const;

signals:

public slots:

private:
    QVector<WatchlistItem> m_rows;
    QHash<int, QByteArray> m_roleNames;

    QVector<WatchlistItem>::ConstIterator constFindItemBySymbol(const QString &symbol) const
    {
        return std::find_if(m_rows.cbegin(), m_rows.cend(),
                            [symbol](const WatchlistItem &a) { return a.symbol == symbol; });
    }

    QVector<WatchlistItem>::Iterator findItemBySymbol(const QString &symbol)
    {
        return std::find_if(m_rows.begin(), m_rows.end(),
                            [symbol](const WatchlistItem &a) { return a.symbol == symbol; });
    }

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // FPWATCHLISTLISTMODEL_H
