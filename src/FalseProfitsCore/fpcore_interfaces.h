// Copyright 2017

#ifndef FPCORE_INTERFACES_H
#define FPCORE_INTERFACES_H

#include <QObject>

#include <QDateTime>

#include <boost/optional.hpp>

class IFpSettings : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    virtual ~IFpSettings() {}

    struct LaunchSettings
    {
        boost::optional<QString> accessToken;
        boost::optional<QDateTime> expiry;
    };
    /*!
     * Reads the settings normally required near the applications
     * launch time.
     */
    virtual LaunchSettings readLaunchSettings() = 0;

    /*!
     * Save the authentication token \a token and expiry date-time
     * \a expiryDate.
     */
    virtual void writeAccessToken(const QString &token, const QDateTime &expiryDate) = 0;

    /*!
     * Clear any saved token and expiry date-time.
     */
    virtual void clearAccessToken() = 0;
};

#endif // FPCORE_INTERFACES_H
