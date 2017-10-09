// Copyright 2017

#ifndef FPTRADINGACCOUNTS_H
#define FPTRADINGACCOUNTS_H

#include <QObject>

class FinishNotifier;
class FpCore;
class FpAccountsListModel;
class GetUserProfileResponse;

class FpTradingAccounts : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FpCore *coreClient READ coreClient WRITE setCoreClient)
    Q_PROPERTY(FpAccountsListModel *model READ model)
public:
    explicit FpTradingAccounts(QObject *parent = nullptr);

    FpCore *coreClient() const { return m_fpCore; }
    void setCoreClient(FpCore *core);

    FpAccountsListModel *model() const { return m_model; }

    Q_INVOKABLE
    FinishNotifier *updateAccounts();

signals:

public slots:
    void unloadAccounts();

private:
    void onResponseReceived(GetUserProfileResponse *reply, bool updateNotReplace);

    FpAccountsListModel *m_model;
    FpCore *m_fpCore{ nullptr };
};

#endif // FPTRADINGACCOUNTS_H
