// Copyright 2017

#ifndef FPPORTFOLIOWRAPPER_H
#define FPPORTFOLIOWRAPPER_H

#include <QObject>

class FinishNotifier;
class FpPositionsListModel;
class FpCore;
class GetPositionsResponse;

class FpPortfolioWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FpCore *coreClient READ coreClient WRITE setCoreClient)
    Q_PROPERTY(FpPositionsListModel *model READ model)
    Q_PROPERTY(bool isDirty READ isDirty WRITE setDirty NOTIFY dirtyChanged)
    Q_PROPERTY(double marketValue READ marketValue WRITE setMarketValue NOTIFY marketValueChanged)
    Q_PROPERTY(double balance READ balance WRITE setBalance NOTIFY balanceChanged)
public:
    explicit FpPortfolioWrapper(QObject *parent = nullptr);

    FpCore *coreClient() const { return m_fpCore; }
    void setCoreClient(FpCore *core);

    FpPositionsListModel *model() const { return m_model; }

    Q_INVOKABLE
    FinishNotifier *refreshPositions();

    Q_INVOKABLE
    QString currentAccountId() const;

    Q_INVOKABLE
    FinishNotifier *loadPositions(const QString &accountId);

    bool isDirty() const;

    double marketValue() const;

    double balance() const;

signals:

    void marketValueChanged(double marketValue);

    void balanceChanged(double balance);

    void dirtyChanged(bool dirty);

public slots:
    void unloadPortfolio();

    void setDirty(bool dirty);

    void setMarketValue(double marketValue);

    void setBalance(double balance);

private:
    void slotPositionsChanged();

    void onPositionsReceived(GetPositionsResponse *reply);

    FpPositionsListModel *m_model;
    FpCore *m_fpCore{ nullptr };
    QString m_currentAccountId;
    double m_marketValue{ 0.0 };
    double m_balance{ 0.0 };
    bool m_dirty{ true };
};

#endif // FPPORTFOLIOWRAPPER_H
