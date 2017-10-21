// Copyright 2017

#include "fplocale.h"

#include <QLocale>

FpLocale::FpLocale(QObject *parent)
    : QObject(parent)
{
}

QString FpLocale::toIntString(qint64 i) const
{
    QLocale locale_;
    return locale_.toString(i);
}

QString FpLocale::toIntString(qint32 i) const
{
    QLocale locale_;
    return locale_.toString(i);
}

QString FpLocale::toDecimalString(double d, int prec) const
{
    QLocale locale_;
    return locale_.toString(d, 'f', prec);
}

/*!
 * omit trailing zeros
 */
QString FpLocale::toShortDecimalString(double d) const
{
    QLocale locale_;
    return locale_.toString(d, 'g');
}
