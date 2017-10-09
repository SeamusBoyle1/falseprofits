// Copyright 2017

#include "utilityfunctionswrapper.h"

UtilityFunctionsWrapper::UtilityFunctionsWrapper(QObject *parent)
    : QObject(parent)
{
}

QDate UtilityFunctionsWrapper::makeDateFromString(const QString &dt, const QString &format) const
{
    return QDate::fromString(dt, format);
}
