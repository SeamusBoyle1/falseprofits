// Copyright 2017

#ifndef BROKERAGECOSTCALCULATOR_H
#define BROKERAGECOSTCALCULATOR_H

#include <QObject>

#include "commissiontable.h"
#include "fpdeclarativetypes.h"

#include <QVector>

#include <boost/optional.hpp>

#include <algorithm>
#include <tuple>

class FinishNotifier;
class FpCore;
class GetCommissionsResponse;

namespace util {
inline BrokerCostCalcResult calcBrokerageCost(const CommissionTable &table, const BrokerCostCalcArgs &args)
{
    auto orderValue = args.quantity() * args.price();
    boost::optional<double> brokerageCostFixed;
    boost::optional<double> brokerageCostPercent;
    std::tie(brokerageCostFixed, brokerageCostPercent) = table.commissions(args.quantity());
    if (!brokerageCostFixed || !brokerageCostFixed) {
        return BrokerCostCalcResult{};
    }
    auto brokerageCostTotal = *brokerageCostFixed + (orderValue * (*brokerageCostPercent / 100));
    auto estimatedTotal = orderValue + brokerageCostTotal;

    BrokerCostCalcResult r;
    r.setOrderValue(orderValue);
    r.setBrokerageCost(brokerageCostTotal);
    r.setEstimatedTotal(estimatedTotal);
    return r;
}
} // namespace util

class BrokerageCostCalculator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FpCore *coreClient READ coreClient WRITE setCoreClient)
public:
    explicit BrokerageCostCalculator(QObject *parent = nullptr);

    FpCore *coreClient() const { return m_fpCore; }
    void setCoreClient(FpCore *core);

    Q_INVOKABLE
    FinishNotifier *updateBuyCommission();

    Q_INVOKABLE
    FinishNotifier *updateSellCommission();

    Q_INVOKABLE
    BrokerCostCalcResult calcBrokerageCost(const BrokerCostCalcArgs &args) const;

public Q_SLOTS:
    void unloadCommissions();

private:
    FinishNotifier *updateCommission(bool buyNotSell);
    void onResponseReceived(GetCommissionsResponse *reply, bool buyNotSell);

    FpCore *m_fpCore{ nullptr };

    CommissionTable m_buyCommissionTable;
    CommissionTable m_sellCommissionTable;
};

#endif // BROKERAGECOSTCALCULATOR_H
