// Copyright 2017

#include "utilityfunctionswrapper.h"
#include <QDebug>

UtilityFunctionsWrapper::UtilityFunctionsWrapper(QObject *parent)
    : QObject(parent)
{
}

QDateTime UtilityFunctionsWrapper::makeDateFromString(const QString &dt, enum Qt::DateFormat format = Qt::RFC2822Date) const
{
    return QDateTime::fromString(dt, format);
}

QString UtilityFunctionsWrapper::timeDifference(const QDateTime &dt, const QDateTime &current_dt) const
{
    int diffSecs = dt.msecsTo(current_dt) / 1000;
    int minutes = 60;
    int hours = 60 * minutes;

    if(diffSecs < minutes)
    {
        return QString::number(diffSecs) + " seconds ago.";
    }
    else if (diffSecs < hours )
    {
        return QString::number(diffSecs / minutes) + " minutes ago.";
    }
    else if ( diffSecs < (24 * hours) )
    {
        return QString::number(diffSecs / hours) + " hours ago.";
    }
    else
    {
        return dt.toString();
    }
}
