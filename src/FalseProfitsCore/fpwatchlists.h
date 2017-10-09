// Copyright 2017

#ifndef FPWATCHLISTS_H
#define FPWATCHLISTS_H

#include <QObject>

class FinishNotifier;
class FpCore;
class FpWatchlistsListModel;
class GetUserProfileResponse;

class FpWatchlists : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FpCore *coreClient READ coreClient WRITE setCoreClient)
    Q_PROPERTY(FpWatchlistsListModel *model READ model)
public:
    explicit FpWatchlists(QObject *parent = nullptr);

    FpCore *coreClient() const { return m_fpCore; }
    void setCoreClient(FpCore *core);

    FpWatchlistsListModel *model() const { return m_model; }

    Q_INVOKABLE
    FinishNotifier *updateWatchlistList();

signals:

public slots:
    void unloadWatchlistList();

private:
    void onResponseReceived(GetUserProfileResponse *reply);

    FpWatchlistsListModel *m_model;
    FpCore *m_fpCore{ nullptr };
};

#endif // FPWATCHLISTS_H
