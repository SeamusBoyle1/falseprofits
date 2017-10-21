// Copyright 2017

#ifndef FPLOCALE_H
#define FPLOCALE_H

#include <QObject>

class FpLocale : public QObject
{
    Q_OBJECT
public:
    explicit FpLocale(QObject *parent = nullptr);

    Q_INVOKABLE
    QString toIntString(qint64 i) const;

    Q_INVOKABLE
    QString toIntString(qint32 i) const;

    Q_INVOKABLE
    QString toDecimalString(double d, int prec = 6) const;

    Q_INVOKABLE
    QString toShortDecimalString(double d) const;

signals:

public slots:
};

#endif // FPLOCALE_H
