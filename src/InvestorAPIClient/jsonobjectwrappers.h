// Copyright 2017

#ifndef BSMI_JSONOBJECTWRAPPERS_H
#define BSMI_JSONOBJECTWRAPPERS_H

#include <QJsonArray>
#include <QJsonObject>

#include <boost/optional.hpp>

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
inline boost::optional<int> getOptionalInt(const QJsonObject &o, const String &key, int magic = 0)
{
    auto v = o.value(key).toInt(magic);
    return boost::make_optional(v != magic, v);
}

} // namespace util

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

} // namespace JsonObjectWrappers

} // namespace bsmi

#endif // BSMI_JSONOBJECTWRAPPERS_H
