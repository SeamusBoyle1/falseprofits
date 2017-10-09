// Copyright 2017

#include "fpsymbolsearchwrapper.h"

#include "finishnotifier.h"
#include "fpsymbolsearchresultsitemmodel.h"
#include "fpcore.h"

#include <InvestorAPIClient/iinvestorapiclient.h>
#include <InvestorAPIClient/jsonobjectwrappers.h>

FpSymbolSearchWrapper::FpSymbolSearchWrapper(QObject *parent)
    : QObject(parent)
    , m_model{ new FpSymbolSearchResultsItemModel(this) }
{
}

void FpSymbolSearchWrapper::clearSearchResults()
{
    m_model->resetWithData(QVector<SymbolSearchResultItem>{});
}

FinishNotifier *FpSymbolSearchWrapper::doSearch(const QString &q)
{
    Q_ASSERT(m_fpCore);

    clearSearchResults();
    m_pageNumber = 0;
    m_totalPageCount = 0;

    m_searchTerm = q;

    SymbolSearchQuery v;
    v.setSearchTerm(q);

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    auto resp = m_fpCore->symbolSearch(v);
    connect(resp, &SymbolSearchResponse::finished, this, [notifier, resp, this] {
        onResponseReceived(resp, false);
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });

    return notifier;
}

FinishNotifier *FpSymbolSearchWrapper::getNextPage()
{
    Q_ASSERT(m_fpCore);

    SymbolSearchQuery v;
    v.setSearchTerm(m_searchTerm);
    v.setPageNumber(++m_pageNumber);

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    auto resp = m_fpCore->symbolSearch(v);
    connect(resp, &SymbolSearchResponse::finished, this, [notifier, resp, this] {
        onResponseReceived(resp, true);
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });

    return notifier;
}

bool FpSymbolSearchWrapper::canFetchMore() const
{
    return m_pageNumber < m_totalPageCount;
}

void FpSymbolSearchWrapper::onResponseReceived(SymbolSearchResponse *reply, bool append)
{
    if (!reply->hasError()) {
        auto doc = QJsonDocument::fromJson(reply->payload());
        auto responseObj = bsmi::JsonObjectWrappers::SymbolSearchResponse(doc.object());
        auto responseItems = responseObj.items();

        QVector<SymbolSearchResultItem> resultItems;
        resultItems.reserve(responseItems.size());
        for (auto i = 0, total = responseItems.size(); i < total; ++i) {
            auto const e = responseItems.at(i);

            SymbolSearchResultItem r;
            auto symbol = e.symbol();
            if (symbol) {
                r.symbol = *symbol;
            }
            auto name = e.name();
            if (name) {
                r.companyName = *name;
            }
            auto industry = e.industry();
            if (industry) {
                r.industry = *industry;
            }

            resultItems.append(r);
        }

        m_pageNumber = responseObj.pageNumber();
        m_totalPageCount = responseObj.totalPageCount();

        if (append) {
            m_model->appendData(resultItems);
        } else {
            m_model->resetWithData(resultItems);
        }
    } else {
        // TODO(seamus): Handle error case
    }
}
