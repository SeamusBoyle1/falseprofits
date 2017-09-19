// Copyright 2017

#ifndef FPSYMBOLSEARCHWRAPPER_H
#define FPSYMBOLSEARCHWRAPPER_H

#include <QObject>

class FpCore;
class FpSymbolSearchResultsItemModel;
class SymbolSearchResponse;

class FinishNotifier : public QObject
{
    Q_OBJECT
public:
    FinishNotifier() {}
    virtual ~FinishNotifier() {}

Q_SIGNALS:
    void finished();

public Q_SLOTS:
    void setFinished()
    {
        if (!m_finished) {
            m_finished = true;
            Q_EMIT finished();
        }
    }

private:
    bool m_finished{ false };
};

class FpSymbolSearchWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FpCore *coreClient READ coreClient WRITE setCoreClient)
    Q_PROPERTY(FpSymbolSearchResultsItemModel *model READ model)
public:
    explicit FpSymbolSearchWrapper(QObject *parent = nullptr);

    FpCore *coreClient() const { return m_fpCore; }
    void setCoreClient(FpCore *core) { m_fpCore = core; }

    FpSymbolSearchResultsItemModel *model() const { return m_model; }

    Q_INVOKABLE
    void clearSearchResults();

    Q_INVOKABLE
    FinishNotifier *doSearch(const QString &q);

    Q_INVOKABLE
    FinishNotifier *getNextPage();

    Q_INVOKABLE
    bool canFetchMore() const;

signals:

public slots:

private:
    void onResponseReceived(SymbolSearchResponse *reply, bool append);

    FpSymbolSearchResultsItemModel *m_model;
    FpCore *m_fpCore{ nullptr };

    QString m_searchTerm;
    int m_pageNumber{ 0 };
    int m_totalPageCount{ 0 };
};

#endif // FPSYMBOLSEARCHWRAPPER_H
