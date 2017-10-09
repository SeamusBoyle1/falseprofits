// Copyright 2017

#ifndef FPWATCHLISTWRAPPER_H
#define FPWATCHLISTWRAPPER_H

#include <QObject>

class AddSymbolToWatchlistResponse;
class FinishNotifier;
class FpCore;
class FpWatchlistListModel;
class GetWatchlistResponse;
class RemoveSymbolFromWatchlistResponse;

class FpWatchlistWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FpCore *coreClient READ coreClient WRITE setCoreClient)
    Q_PROPERTY(FpWatchlistListModel *model READ model)
    Q_PROPERTY(bool isDirty READ isDirty WRITE setDirty NOTIFY dirtyChanged)
public:
    explicit FpWatchlistWrapper(QObject *parent = nullptr);

    FpCore *coreClient() const { return m_fpCore; }
    void setCoreClient(FpCore *core);

    FpWatchlistListModel *model() const { return m_model; }

    Q_INVOKABLE
    FinishNotifier *refreshWatchlist();

    Q_INVOKABLE
    QString currentWatchlistId() const;

    Q_INVOKABLE
    FinishNotifier *loadWatchlist(const QString &watchlistId);

    Q_INVOKABLE
    FinishNotifier *addSymbol(const QString &symbol);

    Q_INVOKABLE
    FinishNotifier *removeSymbol(const QString &symbol);

    bool isDirty() const;

signals:

    void dirtyChanged(bool isDirty);

public slots:
    void unloadWatchlist();

    void setDirty(bool dirty);

private:
    void slotWatchlistChanged(const QString watchlistId, const QString &symbol, bool added);

    void onGetResponseReceived(GetWatchlistResponse *reply);

    void onAddResponseReceived(AddSymbolToWatchlistResponse *reply);

    void onRemoveResponseReceived(RemoveSymbolFromWatchlistResponse *reply);

    FpWatchlistListModel *m_model;
    FpCore *m_fpCore{ nullptr };
    QString m_currentWatchlistId;
    bool m_dirty{ true };
};

#endif // FPWATCHLISTWRAPPER_H
