// Copyright 2017

#ifndef FPSETTINGS_H
#define FPSETTINGS_H

#include <FalseProfitsCore/fpcore_interfaces.h>

class FpSettings : public IFpSettings
{
    Q_OBJECT
public:
    explicit FpSettings(QObject *parent = nullptr);

    LaunchSettings readLaunchSettings() override;

    void writeAccessToken(const QString &token, const QDateTime &expiryDate) override;

    void clearAccessToken() override;
};

#endif // FPSETTINGS_H
