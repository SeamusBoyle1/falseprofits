// Copyright 2017

#include <QLocale>
#include "utilityfunctionswrapper.h"

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
    int diffSecs = int(dt.secsTo(current_dt));;
    int minutes = 60;
    int hours = 60 * minutes;

    if(diffSecs < minutes)
    {
        return tr("%1 seconds ago").arg(diffSecs);
    }
    else if (diffSecs < hours )
    {
        return tr("%1 minutes ago").arg(diffSecs / minutes);
    }
    else if ( diffSecs < (24 * hours) )
    {
        return tr("%1 hours ago").arg(diffSecs / hours);
    }
    else
    {
        QLocale locale;

        // Returns as string
        return locale.toString(dt, locale.dateFormat(QLocale::LongFormat));
    }
}

