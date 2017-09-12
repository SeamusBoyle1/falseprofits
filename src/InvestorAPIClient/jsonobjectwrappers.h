// Copyright 2017

#ifndef BSMI_JSONOBJECTWRAPPERS_H
#define BSMI_JSONOBJECTWRAPPERS_H

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

} // namespace JsonObjectWrappers

} // namespace bsmi

#endif // BSMI_JSONOBJECTWRAPPERS_H
