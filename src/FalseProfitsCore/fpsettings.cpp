// Copyright 2017

#include "fpsettings.h"

#include <QDateTime>
#include <QSettings>

FpSettings::FpSettings(QObject *parent)
    : IFpSettings(parent)
{
}

FpSettings::LaunchSettings FpSettings::readLaunchSettings()
{
    LaunchSettings r;
    QSettings settings;
    settings.beginGroup("userAccount");
    auto token = settings.value("accessToken").toString();
    if (!token.isEmpty()) {
        r.accessToken = token;

        bool expiresOk;
        auto expires = settings.value("expires").toLongLong(&expiresOk);
        if (expiresOk) {
            r.expiry =
                expires == 0 ? QDateTime() : QDateTime::fromMSecsSinceEpoch(expires, Qt::UTC);
        }
    }
    settings.endGroup();
    return r;
}

void FpSettings::writeAccessToken(const QString &token, const QDateTime &expiryDate)
{
    QSettings settings;
    settings.beginGroup("userAccount");
    settings.setValue("accessToken", token);
    settings.setValue("expires", expiryDate.toMSecsSinceEpoch());
    settings.endGroup();
}

void FpSettings::clearAccessToken()
{
    QSettings settings;
    settings.remove("userAccount");
}
