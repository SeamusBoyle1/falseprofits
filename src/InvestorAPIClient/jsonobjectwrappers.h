// Copyright 2017

#ifndef BSMI_JSONOBJECTWRAPPERS_H
#define BSMI_JSONOBJECTWRAPPERS_H

#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

#include <boost/optional.hpp>

#include <limits>

namespace bsmi {

namespace JsonObjectWrappers {

namespace util {

template <typename String>
inline boost::optional<QString> getOptionalString(const QJsonObject &o, const String &key)
{
    auto v = o.value(key);
    return boost::make_optional(v.isString(), v.toString());
}

template <typename String>
inline boost::optional<QDateTime> getOptionalStringAsDateTime(const QJsonObject &o,
                                                              const String &key)
{
    auto v = o.value(key);
    if (v.isString()) {
        auto dt = QDateTime::fromString(v.toString(), Qt::ISODate);
        return boost::make_optional(dt.isValid(), dt);
    }
    return boost::none;
}

template <typename String>
inline boost::optional<double>
getOptionalDouble(const QJsonObject &o, const String &key,
                  double magic = std::numeric_limits<double>::quiet_NaN())
{
    auto v = o.value(key).toDouble(magic);
    return boost::make_optional(!qIsNaN(v), v);
}

template <typename String>
inline boost::optional<int> getOptionalInt(const QJsonObject &o, const String &key, int magic = 0)
{
    auto v = o.value(key).toInt(magic);
    return boost::make_optional(v != magic, v);
}

} // namespace util

class ErrorMessageResponse
{
public:
    QJsonObject d;

    boost::optional<QString> message() const
    {
        return util::getOptionalString(d, QLatin1String("message"));
    }
};

class TokenResponse
{
public:
    QJsonObject d;

    boost::optional<QString> accessToken() const
    {
        return util::getOptionalString(d, QLatin1String("accessToken"));
    }

    boost::optional<int> expires() const
    {
        return util::getOptionalInt(d, QLatin1String("expires"), -1);
    }
};

class TradingAccount
{
public:
    QJsonObject d;

    boost::optional<QString> id() const { return util::getOptionalString(d, QLatin1String("id")); }

    boost::optional<QString> name() const
    {
        return util::getOptionalString(d, QLatin1String("name"));
    }

    boost::optional<double> balance() const
    {
        return util::getOptionalDouble(d, QLatin1String("balance"));
    }
};

class TradingAccounts
{
public:
    QJsonArray d;

    bool isEmpty() const { return d.isEmpty(); }

    int size() const { return d.size(); }

    TradingAccount at(int i) const { return { d.at(i).toObject() }; }
};

class Watchlist
{
public:
    QJsonObject d;

    boost::optional<QString> id() const { return util::getOptionalString(d, QLatin1String("id")); }

    boost::optional<QString> name() const
    {
        return util::getOptionalString(d, QLatin1String("name"));
    }
};

class Watchlists
{
public:
    QJsonArray d;

    bool isEmpty() const { return d.isEmpty(); }

    int size() const { return d.size(); }

    Watchlist at(int i) const { return { d.at(i).toObject() }; }
};

class UserProfileResponse
{
public:
    QJsonObject d;

    boost::optional<QString> id() const { return util::getOptionalString(d, QLatin1String("id")); }

    boost::optional<QString> email() const
    {
        return util::getOptionalString(d, QLatin1String("email"));
    }

    boost::optional<QString> displayName() const
    {
        return util::getOptionalString(d, QLatin1String("displayName"));
    }

    boost::optional<QString> level() const
    {
        return util::getOptionalString(d, QLatin1String("level"));
    }

    TradingAccounts accounts() const
    {
        return { d.value(QLatin1String("accounts")).toArray() };
    }

    Watchlists watchlists() const { return { d.value(QLatin1String("watchlists")).toArray() }; }
};

class CommissionRange
{
public:
    QJsonObject d;

    boost::optional<int> min() const
    {
        return util::getOptionalInt(d, QLatin1String("min"), -1);
    }

    boost::optional<int> max() const
    {
        return util::getOptionalInt(d, QLatin1String("max"), -1);
    }

    boost::optional<double> value() const
    {
        return util::getOptionalDouble(d, QLatin1String("value"));
    }
};

class Commissions
{
public:
    QJsonArray jsonItems;

    bool isEmpty() const { return jsonItems.isEmpty(); }

    int size() const { return jsonItems.size(); }

    CommissionRange at(int i) const { return { jsonItems.at(i).toObject() }; }
};

class CommissionsResponse
{
public:
    QJsonObject d;

    bool isValid() const { return !d.isEmpty(); }

    Commissions fixed() const { return { d.value(QLatin1String("fixed")).toArray() }; }

    Commissions percent() const { return { d.value(QLatin1String("percentage")).toArray() }; }
};

class PositionInfo
{
public:
    QJsonObject d;

    boost::optional<QString> symbol() const
    {
        return util::getOptionalString(d, QLatin1String("symbol"));
    }

    boost::optional<QString> name() const
    {
        return util::getOptionalString(d, QLatin1String("name"));
    }

    boost::optional<int> quantity() const
    {
        return util::getOptionalInt(d, QLatin1String("quantity"), -1);
    }

    boost::optional<double> averagePrice() const
    {
        return util::getOptionalDouble(d, QLatin1String("averagePrice"));
    }

    boost::optional<double> lastPrice() const
    {
        return util::getOptionalDouble(d, QLatin1String("lastPrice"));
    }

    boost::optional<double> change() const
    {
        return util::getOptionalDouble(d, QLatin1String("change"));
    }

    boost::optional<double> changePercent() const
    {
        return util::getOptionalDouble(d, QLatin1String("changePercent"));
    }
};

class Positions
{
public:
    QJsonArray jsonItems;

    bool isEmpty() const { return jsonItems.isEmpty(); }

    int size() const { return jsonItems.size(); }

    PositionInfo at(int i) const { return { jsonItems.at(i).toObject() }; }
};

class PositionsResponse
{
public:
    QJsonObject d;

    Positions positions() const { return { d.value(QLatin1String("positions")).toArray() }; }

    boost::optional<QString> id() const { return util::getOptionalString(d, QLatin1String("id")); }

    boost::optional<QString> name() const
    {
        return util::getOptionalString(d, QLatin1String("name"));
    }

    boost::optional<double> balance() const
    {
        return util::getOptionalDouble(d, QLatin1String("balance"));
    }
};

class TransactionsResponseItem
{
public:
    QJsonObject d;

    bool isValid() const { return !d.isEmpty(); }

    boost::optional<QDateTime> timestampUtc() const
    {
        return util::getOptionalStringAsDateTime(d, QLatin1String("timestampUtc"));
    }

    boost::optional<QString> type() const
    {
        return util::getOptionalString(d, QLatin1String("type"));
    }

    boost::optional<QString> description() const
    {
        return util::getOptionalString(d, QLatin1String("description"));
    }

    boost::optional<double> amount() const
    {
        return util::getOptionalDouble(d, QLatin1String("amount"));
    }

    boost::optional<double> balance() const
    {
        return util::getOptionalDouble(d, QLatin1String("balance"));
    }
};

class TransactionsResponseItems
{
public:
    QJsonArray jsonItems;

    bool isEmpty() const { return jsonItems.isEmpty(); }

    int size() const { return jsonItems.size(); }

    TransactionsResponseItem at(int i) const { return { jsonItems.at(i).toObject() }; }
};

class TransactionsResponse
{
public:
    TransactionsResponse(QJsonObject obj)
        : d{ std::move(obj) }
    {
        jsonItems = obj.value(QLatin1String("items")).toArray();
    }

    TransactionsResponseItems items() const { return { jsonItems }; }

    QJsonObject d;
    QJsonArray jsonItems;

    int pageNumber() const { return d.value(QLatin1String("pageNumber")).toInt(); }

    int pageSize() const { return d.value(QLatin1String("pageSize")).toInt(); }

    int totalPageCount() const { return d.value(QLatin1String("totalPageCount")).toInt(); }

    int totalRowCount() const { return d.value(QLatin1String("totalRowCount")).toInt(); }
};

class SymbolSearchResponseItem
{
public:
    QJsonObject d;

    bool isValid() const { return !d.isEmpty() && d.contains(QLatin1String("symbol")); }

    boost::optional<QString> symbol() const
    {
        return util::getOptionalString(d, QLatin1String("symbol"));
    }

    boost::optional<QString> name() const
    {
        return util::getOptionalString(d, QLatin1String("name"));
    }

    boost::optional<QString> industry() const
    {
        return util::getOptionalString(d, QLatin1String("industry"));
    }
};

class SymbolSearchResponseItems
{
public:
    QJsonArray jsonItems;

    bool isEmpty() const { return jsonItems.isEmpty(); }

    int size() const { return jsonItems.size(); }

    SymbolSearchResponseItem at(int i) const { return { jsonItems.at(i).toObject() }; }
};

class SymbolSearchResponse
{
public:
    SymbolSearchResponse(QJsonObject obj)
        : d{ std::move(obj) }
    {
        jsonItems = obj.value(QLatin1String("items")).toArray();
    }

    SymbolSearchResponseItems items() const { return { jsonItems }; }

    QJsonObject d;
    QJsonArray jsonItems;

    int pageNumber() const { return d.value(QLatin1String("pageNumber")).toInt(); }

    int pageSize() const { return d.value(QLatin1String("pageSize")).toInt(); }

    int totalPageCount() const { return d.value(QLatin1String("totalPageCount")).toInt(); }

    int totalRowCount() const { return d.value(QLatin1String("totalRowCount")).toInt(); }
};

class WatchlistResponseShare
{
public:
    QJsonObject d;

    bool isValid() const { return !d.isEmpty() && d.contains(QLatin1String("symbol")); }

    boost::optional<QString> symbol() const
    {
        return util::getOptionalString(d, QLatin1String("symbol"));
    }

    boost::optional<QString> name() const
    {
        return util::getOptionalString(d, QLatin1String("name"));
    }

    boost::optional<double> lastPrice() const
    {
        return util::getOptionalDouble(d, QLatin1String("lastPrice"));
    }

    boost::optional<double> change() const
    {
        return util::getOptionalDouble(d, QLatin1String("change"));
    }

    boost::optional<double> changePercent() const
    {
        return util::getOptionalDouble(d, QLatin1String("changePercent"));
    }
};

class WatchlistResponseShares
{
public:
    QJsonArray jsonItems;

    bool isEmpty() const { return jsonItems.isEmpty(); }

    int size() const { return jsonItems.size(); }

    WatchlistResponseShare at(int i) const { return { jsonItems.at(i).toObject() }; }
};

class WatchlistResponse
{
public:
    QJsonObject d;

    boost::optional<QString> id() const { return util::getOptionalString(d, QLatin1String("id")); }

    boost::optional<QString> name() const
    {
        return util::getOptionalString(d, QLatin1String("name"));
    }

    WatchlistResponseShares shares() const
    {
        return { d.value(QLatin1String("shares")).toArray() };
    }
};

class CandleResponse
{
public:
    QJsonObject d;

    bool isValid() const { return !d.isEmpty(); }

    boost::optional<QDateTime> timestamp() const
    {
        return util::getOptionalStringAsDateTime(d, QLatin1String("timestamp"));
    }

    boost::optional<double> open() const
    {
        return util::getOptionalDouble(d, QLatin1String("open"));
    }

    boost::optional<double> high() const
    {
        return util::getOptionalDouble(d, QLatin1String("high"));
    }

    boost::optional<double> low() const { return util::getOptionalDouble(d, QLatin1String("low")); }

    boost::optional<double> close() const
    {
        return util::getOptionalDouble(d, QLatin1String("close"));
    }
};

class CandlesResponse
{
public:
    QJsonArray jsonItems;

    bool isEmpty() const { return jsonItems.isEmpty(); }

    int size() const { return jsonItems.size(); }

    CandleResponse at(int i) const { return { jsonItems.at(i).toObject() }; }
};

} // namespace JsonObjectWrappers

} // namespace bsmi

#endif // BSMI_JSONOBJECTWRAPPERS_H
