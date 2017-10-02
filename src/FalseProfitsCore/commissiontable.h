// Copyright 2017

#ifndef COMMISSIONTABLE_H
#define COMMISSIONTABLE_H

#include <QVector>

#include <boost/optional.hpp>

#include <algorithm>
#include <tuple>

struct FixedCommission
{
    double value;
    int upperBound;
};

struct PercentageCommission
{
    double percentValue;
    int upperBound;
};

struct CommissionTable
{
    QVector<FixedCommission> fixed;
    QVector<PercentageCommission> percent;

    void clear();

    void sort();

    void insertFixed(int upperBound, double value);

    void insertPercent(int upperBound, double percentValue);

    boost::optional<double> fixedCommission(int quantity) const;

    boost::optional<double> percentCommission(int quantity) const;

    std::tuple<boost::optional<double>, boost::optional<double>> commissions(int quantity) const;
};

inline void CommissionTable::clear()
{
    fixed.clear();
    percent.clear();
}

inline void CommissionTable::sort()
{
    std::sort(fixed.begin(), fixed.end(), [](const FixedCommission &a, const FixedCommission &b) {
        return a.upperBound < b.upperBound;
    });
    std::sort(percent.begin(), percent.end(),
              [](const PercentageCommission &a, const PercentageCommission &b) {
                  return a.upperBound < b.upperBound;
              });
}

inline void CommissionTable::insertFixed(int upperBound, double value)
{
    fixed.append({ value, upperBound });
}

inline void CommissionTable::insertPercent(int upperBound, double percentValue)
{
    percent.append({ percentValue, upperBound });
}

inline boost::optional<double> CommissionTable::fixedCommission(int quantity) const
{
    auto it = std::upper_bound(fixed.cbegin(), fixed.cend(), quantity,
                               [](int a, const FixedCommission &b) { return a <= b.upperBound; });
    if (it != fixed.cend()) {
        return it->value;
    }
    return boost::none;
}

inline boost::optional<double> CommissionTable::percentCommission(int quantity) const
{
    auto it =
        std::upper_bound(percent.cbegin(), percent.cend(), quantity,
                         [](int a, const PercentageCommission &b) { return a <= b.upperBound; });
    if (it != percent.cend()) {
        return it->percentValue;
    }
    return boost::none;
}

inline std::tuple<boost::optional<double>, boost::optional<double>>
CommissionTable::commissions(int quantity) const
{
    return std::make_tuple(fixedCommission(quantity), percentCommission(quantity));
}

#endif // COMMISSIONTABLE_H
