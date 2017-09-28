// Copyright 2017

#ifndef FPWATCHLISTSLISTMODEL_H
#define FPWATCHLISTSLISTMODEL_H

#include <QAbstractListModel>

#include <QHash>
#include <QVector>

struct WatchlistListItem
{
    QString id;
    QString name;
};

class FpWatchlistsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        IdRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
    };

    explicit FpWatchlistsListModel(QObject *parent = nullptr);

    void resetWithData(QVector<WatchlistListItem> data);

    Q_INVOKABLE
    QString getWatchlistId(int row) const;

signals:

public slots:

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<WatchlistListItem> m_rows;
    QHash<int, QByteArray> m_roleNames;
};

#endif // FPWATCHLISTSLISTMODEL_H
