// Copyright 2017

#include "fptransactionswrapper.h"

#include "finishnotifier.h"
#include "fpcore.h"
#include "fpdeclarativetypes.h"
#include "fptransactionslistmodel.h"
#include "responsetypes.h"

#include <InvestorAPIClient/jsonobjectwrappers.h>

#include <QPointer>

FpTransactionsWrapper::FpTransactionsWrapper(QObject *parent)
    : QObject(parent)
    , m_model{ new FpTransactionsListModel(this) }
{
}

void FpTransactionsWrapper::setCoreClient(FpCore *core)
{
    if (m_fpCore) {
        disconnect(m_fpCore, &FpCore::authStateChanged, this,
                   &FpTransactionsWrapper::unloadTransactions);
    }

    m_fpCore = core;

    if (core) {
        connect(core, &FpCore::authStateChanged, this, &FpTransactionsWrapper::unloadTransactions);
    }
}

FinishNotifier *FpTransactionsWrapper::refreshTransactions()
{
    Q_ASSERT(m_fpCore);

    m_model->resetWithData(QVector<TransactionInfo>{});
    m_pageNumber = 0;
    m_totalPageCount = 0;

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    if (m_currentAccountId.isEmpty()) {
        QMetaObject::invokeMethod(notifier.data(), "setFinished", Qt::QueuedConnection);
        return notifier;
    }

    TransactionsQuery query;
    query.setAccountId(m_currentAccountId);
    query.setStartDate(m_startDate);
    query.setEndDate(m_endDate);

    auto requestId = ++m_requestId;
    auto resp = m_fpCore->getTransactions(query);
    connect(resp, &GetPositionsResponse::finished, this, [requestId, notifier, resp, this] {
        if (m_requestId == requestId) {
            onTransactionsReceived(resp, false);
        }
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });

    return notifier;
}

FinishNotifier *FpTransactionsWrapper::getNextPage()
{
    Q_ASSERT(m_fpCore);

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    if (m_currentAccountId.isEmpty()) {
        QMetaObject::invokeMethod(notifier.data(), "setFinished", Qt::QueuedConnection);
        return notifier;
    }

    TransactionsQuery query;
    query.setAccountId(m_currentAccountId);
    query.setStartDate(m_startDate);
    query.setEndDate(m_endDate);
    query.setPageNumber(++m_pageNumber);

    auto requestId = ++m_requestId;
    auto resp = m_fpCore->getTransactions(query);
    connect(resp, &GetPositionsResponse::finished, this, [requestId, notifier, resp, this] {
        if (m_requestId == requestId) {
            onTransactionsReceived(resp, true);
        }
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });

    return notifier;
}

bool FpTransactionsWrapper::canFetchMore() const
{
    return m_pageNumber < m_totalPageCount;
}

QString FpTransactionsWrapper::currentAccountId() const
{
    return m_currentAccountId;
}

FinishNotifier *FpTransactionsWrapper::loadTransactions(const QString &accountId)
{
    m_model->resetWithData(QVector<TransactionInfo>{});
    m_pageNumber = 0;
    m_totalPageCount = 0;
    m_currentAccountId = accountId;
    return refreshTransactions();
}

QDate FpTransactionsWrapper::makeDateFromString(const QString &dt, const QString &format) const
{
    return QDate::fromString(dt, format);
}

void FpTransactionsWrapper::setDateRangeLocal(QDate start, QDate end)
{
    m_startDate = QDateTime(start, QTime(), Qt::LocalTime);
    m_endDate = QDateTime(end, QTime(), Qt::LocalTime);
}

void FpTransactionsWrapper::unloadTransactions()
{
    m_model->resetWithData(QVector<TransactionInfo>{});
    m_pageNumber = 0;
    m_totalPageCount = 0;
    m_currentAccountId.clear();
}

void FpTransactionsWrapper::onTransactionsReceived(GetTransactionsResponse *reply, bool append)
{
    if (!reply->hasError()) {
        auto doc = QJsonDocument::fromJson(reply->payload());
        bsmi::JsonObjectWrappers::TransactionsResponse responseObj(doc.object());
        auto responseItems = responseObj.items();

        QVector<TransactionInfo> rows;
        rows.reserve(responseItems.size());
        for (auto i = 0, total = responseItems.size(); i < total; ++i) {
            auto const e = responseItems.at(i);

            TransactionInfo r;
            auto dt = e.timestampUtc();
            if (dt) {
                r.timestampUtc = *dt;
            }
            auto type = e.type();
            if (type) {
                r.type = *type;
            }
            auto desc = e.description();
            if (desc) {
                r.description = *desc;
            }
            auto amount = e.amount();
            if (amount) {
                r.amount = *amount;
            }
            auto bal = e.balance();
            if (bal) {
                r.balance = *bal;
            }

            rows.append(r);
        }

        m_pageNumber = responseObj.pageNumber();
        m_totalPageCount = responseObj.totalPageCount();

        if (append) {
            m_model->appendData(rows);
        } else {
            m_model->resetWithData(rows);
        }
    } else {
        // TODO(seamus): Handle error case
        m_model->resetWithData(QVector<TransactionInfo>{});
    }
}
