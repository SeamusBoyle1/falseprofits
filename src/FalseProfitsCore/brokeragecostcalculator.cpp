// Copyright 2017

#include "brokeragecostcalculator.h"

#include "finishnotifier.h"
#include "fpcore.h"

#include <InvestorAPIClient/jsonobjectwrappers.h>

#include <QPointer>

BrokerageCostCalculator::BrokerageCostCalculator(QObject *parent)
    : QObject(parent)
{
}

void BrokerageCostCalculator::setCoreClient(FpCore *core)
{
    if (m_fpCore) {
        disconnect(m_fpCore, &FpCore::authStateChanged, this,
                   &BrokerageCostCalculator::unloadCommissions);
    }

    m_fpCore = core;

    if (core) {
        connect(core, &FpCore::authStateChanged, this, &BrokerageCostCalculator::unloadCommissions);
    }
}

FinishNotifier *BrokerageCostCalculator::updateBuyCommission()
{
    return updateCommission(true);
}

FinishNotifier *BrokerageCostCalculator::updateSellCommission()
{
    return updateCommission(false);
}

BrokerCostCalcResult
BrokerageCostCalculator::calcBrokerageCost(const BrokerCostCalcArgs &args) const
{
    return util::calcBrokerageCost(
        args.side() == OrderParams::Side::BuySide ? m_buyCommissionTable : m_sellCommissionTable,
        args);
}

void BrokerageCostCalculator::unloadCommissions()
{
    m_buyCommissionTable.clear();
    m_sellCommissionTable.clear();
}

FinishNotifier *BrokerageCostCalculator::updateCommission(bool buyNotSell)
{
    Q_ASSERT(m_fpCore);

    QPointer<FinishNotifier> notifier(new FinishNotifier);

    auto resp = m_fpCore->getCommissions(buyNotSell ? Fpx::CommissionSide::BuySide
                                                    : Fpx::CommissionSide::SellSide);
    connect(resp, &GetCommissionsResponse::finished, this, [notifier, resp, this, buyNotSell] {
        onResponseReceived(resp, buyNotSell);
        resp->deleteLater();
        if (notifier) {
            notifier->setFinished();
        }
    });

    return notifier;
}

void BrokerageCostCalculator::onResponseReceived(GetCommissionsResponse *reply, bool buyNotSell)
{
    if (!reply->hasError()) {
        auto doc = QJsonDocument::fromJson(reply->payload());
        bsmi::JsonObjectWrappers::CommissionsResponse responseObj;
        responseObj.d = doc.object();

        auto jsonFixed = responseObj.fixed();
        auto jsonPercent = responseObj.percent();

        auto &table = buyNotSell ? m_buyCommissionTable : m_sellCommissionTable;

        table.clear();

        for (auto i = 0, total = jsonFixed.size(); i < total; ++i) {
            auto e = jsonFixed.at(i);
            auto max = e.max();
            auto value = e.value();
            if (max && value) {
                table.insertFixed(*max, *value);
            }
        }

        for (auto j = 0, total = jsonPercent.size(); j < total; ++j) {
            auto e = jsonPercent.at(j);
            auto max = e.max();
            auto percentVal = e.value();
            if (max && percentVal) {
                table.insertPercent(*max, *percentVal);
            }
        }

        table.sort();
    } else {
        // TODO(seamus): Handle error case
    }
}
