// Copyright 2017

#ifndef FPSYMBOLSEARCHRESULTSITEMMODEL_H
#define FPSYMBOLSEARCHRESULTSITEMMODEL_H

#include <QAbstractListModel>

#include <QHash>
#include <QVector>

struct SymbolSearchResultItem
{
    QString symbol;
    QString companyName;
    QString industry;
};

class FpSymbolSearchResultsItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        SymbolRole = Qt::UserRole + 1,
        CompanyNameRole = Qt::UserRole + 2,
        IndustryRole = Qt::UserRole + 3,
    };

    explicit FpSymbolSearchResultsItemModel(QObject *parent = nullptr);

    void resetWithData(QVector<SymbolSearchResultItem> data);

    void appendData(const QVector<SymbolSearchResultItem> &data);

signals:

public slots:

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<SymbolSearchResultItem> m_rows;
    QHash<int, QByteArray> m_roleNames;
};

#endif // FPSYMBOLSEARCHRESULTSITEMMODEL_H
