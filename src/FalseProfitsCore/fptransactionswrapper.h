// Copyright 2017

#ifndef FPTRANSACTIONSWRAPPER_H
#define FPTRANSACTIONSWRAPPER_H

#include <QObject>

#include <QDateTime>

class FinishNotifier;
class FpCore;
class FpTransactionsListModel;
class GetTransactionsResponse;

class FpTransactionsWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FpCore *coreClient READ coreClient WRITE setCoreClient)
    Q_PROPERTY(FpTransactionsListModel *model READ model)
public:
    explicit FpTransactionsWrapper(QObject *parent = nullptr);

    FpCore *coreClient() const { return m_fpCore; }
    void setCoreClient(FpCore *core);

    FpTransactionsListModel *model() const { return m_model; }

    Q_INVOKABLE
    FinishNotifier *refreshTransactions();

    Q_INVOKABLE
    FinishNotifier *getNextPage();

    Q_INVOKABLE
    bool canFetchMore() const;

    Q_INVOKABLE
    QString currentAccountId() const;

    Q_INVOKABLE
    FinishNotifier *loadTransactions(const QString &accountId);

    Q_INVOKABLE
    QDate makeDateFromString(const QString &dt, const QString &format) const;

    Q_INVOKABLE
    void setDateRangeLocal(QDate start, QDate end);

signals:

public slots:
    void unloadTransactions();

private:
    void onTransactionsReceived(GetTransactionsResponse *reply, bool append);

    FpTransactionsListModel *m_model;
    FpCore *m_fpCore{ nullptr };
    QString m_currentAccountId;
    QDateTime m_startDate;
    QDateTime m_endDate;
    qint64 m_requestId{ 0 };
    int m_pageNumber{ 0 };
    int m_totalPageCount{ 0 };
};

#endif // FPTRANSACTIONSWRAPPER_H
