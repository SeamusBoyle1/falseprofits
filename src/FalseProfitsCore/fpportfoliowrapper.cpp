// Copyright 2017

#include "fpportfoliowrapper.h"

#include "finishnotifier.h"
#include "fpcore.h"
#include "fppositionslistmodel.h"
#include "responsetypes.h"

#include <InvestorAPIClient/jsonobjectwrappers.h>

#include <QPointer>

FpPortfolioWrapper::FpPortfolioWrapper(QObject *parent)
    : QObject(parent)
    , m_model{ new FpPositionsListModel(this) }
{
}

FinishNotifier *FpPortfolioWrapper::refreshPositions()
{
    Q_ASSERT(m_fpCore);

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    if (m_currentAccountId.isEmpty()) {
        QMetaObject::invokeMethod(notifier.data(), "setFinished", Qt::QueuedConnection);
        return notifier;
    }

    auto resp = m_fpCore->getPositions(m_currentAccountId);
    connect(resp, &GetPositionsResponse::finished, this, [notifier, resp, this] {
        onPositionsReceived(resp);
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });

    return notifier;
}

QString FpPortfolioWrapper::currentAccountId() const
{
    return m_currentAccountId;
}

FinishNotifier *FpPortfolioWrapper::loadPositions(const QString &accountId)
{
    m_model->resetWithData(QVector<PositionItem>{});
    m_currentAccountId = accountId;
    return refreshPositions();
}

double FpPortfolioWrapper::marketValue() const
{
    return m_marketValue;
}

double FpPortfolioWrapper::balance() const
{
    return m_balance;
}

void FpPortfolioWrapper::setMarketValue(double marketValue)
{
    if (qFuzzyCompare(m_marketValue, marketValue))
        return;

    m_marketValue = marketValue;
    emit marketValueChanged(m_marketValue);
}

void FpPortfolioWrapper::setBalance(double balance)
{
    if (qFuzzyCompare(m_balance, balance))
        return;

    m_balance = balance;
    emit balanceChanged(m_balance);
}

void FpPortfolioWrapper::onPositionsReceived(GetPositionsResponse *reply)
{
    if (!reply->hasError()) {
        auto doc = QJsonDocument::fromJson(reply->payload());
        bsmi::JsonObjectWrappers::PositionsResponse responseObj;
        responseObj.d = doc.object();
        auto responseShares = responseObj.positions();

        auto accountId = responseObj.id();
        if (!accountId || *accountId != m_currentAccountId) {
            return;
        }

        QVector<PositionItem> rows;
        rows.reserve(responseShares.size());
        for (auto i = 0, total = responseShares.size(); i < total; ++i) {
            auto const e = responseShares.at(i);

            PositionItem r;
            auto symbol = e.symbol();
            if (symbol) {
                r.symbol = *symbol;
            }
            auto name = e.name();
            if (name) {
                r.name = *name;
            }
            auto qty = e.quantity();
            if (qty) {
                r.quantity = *qty;
            }
            auto avgPrice = e.averagePrice();
            if (avgPrice) {
                r.averagePrice = *avgPrice;
            }
            auto lastPrice = e.lastPrice();
            if (lastPrice) {
                r.lastPrice = *lastPrice;
            }
            auto change = e.change();
            if (change) {
                r.change = *change;
            }
            auto changePercent = e.changePercent();
            if (changePercent) {
                r.changePercent = *changePercent;
            }

            rows.append(r);
        }

        m_model->resetWithData(rows);

        // update market value
        auto marketValue_ = 0.0;
        for (auto &e : qAsConst(rows)) {
            marketValue_ += e.quantity * e.lastPrice;
        }
        setMarketValue(marketValue_);

        // update balance
        auto bal = responseObj.balance() ? *responseObj.balance() : 0.0;
        setBalance(bal);
    } else {
        // TODO(seamus): Handle error case
        m_model->resetWithData(QVector<PositionItem>{});
    }
}