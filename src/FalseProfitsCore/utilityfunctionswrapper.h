// Copyright 2017

#ifndef UTILITYFUNCTIONSWRAPPER_H
#define UTILITYFUNCTIONSWRAPPER_H

#include <QObject>

#include <QDate>

class UtilityFunctionsWrapper : public QObject
{
    Q_OBJECT
public:
    explicit UtilityFunctionsWrapper(QObject *parent = nullptr);

    Q_INVOKABLE
    QDate makeDateFromString(const QString &dt, const QString &format) const;

    Q_INVOKABLE
    QString timeDifference(const QDateTime &dt, const QDateTime &currentDate) const;
signals:

public slots:
};

#endif // UTILITYFUNCTIONSWRAPPER_H
