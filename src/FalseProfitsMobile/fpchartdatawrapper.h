// Copyright 2017

#ifndef FPCHARTDATAWRAPPER_H
#define FPCHARTDATAWRAPPER_H

#include <QObject>

#include <QAbstractSeries>
#include <QString>
#include <QVector>

QT_CHARTS_USE_NAMESPACE

class QJsonDocument;
class QNetworkAccessManager;

class CandlesRequestArgs;
class FpCore;
class GetCandlesResponse;

class FpChartLineSeriesData
{
    Q_GADGET
    Q_PROPERTY(QVector<qreal> xData READ xData)
    Q_PROPERTY(QVector<qreal> yData READ yData)
public:
    QVector<qreal> m_x;
    QVector<qreal> m_y;

    QVector<qreal> xData() const { return m_x; }
    QVector<qreal> yData() const { return m_y; }
};

class FpChartDataWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FpCore *coreClient READ coreClient WRITE setCoreClient)
public:
    explicit FpChartDataWrapper(QObject *parent = nullptr);

    FpCore *coreClient() const { return m_fpCore; }
    void setCoreClient(FpCore *core) { m_fpCore = core; }

    Q_INVOKABLE
    GetCandlesResponse *getCandles(const CandlesRequestArgs &args);

    Q_INVOKABLE
    GetCandlesResponse *getCandlesFromYahoo(const CandlesRequestArgs &args);

    QJsonDocument convertYahooData(const QByteArray &json) const;

    Q_INVOKABLE
    QList<qreal> latestTradingSession(const QVector<qreal> &xData) const;

    Q_INVOKABLE
    QList<qreal> latestTradingSession(const FpChartLineSeriesData &data) const
    {
        return latestTradingSession(data.m_x);
    }

    Q_INVOKABLE
    FpChartLineSeriesData makeCloseLineSeries(const QByteArray &json) const;

    Q_INVOKABLE
    void updateSeries(QAbstractSeries *series, const FpChartLineSeriesData &data) const;

    Q_INVOKABLE
    double minPrice(const QVector<qreal> &yData) const
    {
        auto it = std::min_element(yData.begin(), yData.end());
        if (it != yData.end())
            return *it;
        return 0;
    }

    Q_INVOKABLE
    double minPrice(const FpChartLineSeriesData &data) const { return minPrice(data.m_y); }

    Q_INVOKABLE
    double maxPrice(const QVector<qreal> &yData) const
    {
        auto it = std::max_element(yData.begin(), yData.end());
        if (it != yData.end())
            return *it;
        return 0;
    }

    Q_INVOKABLE
    double maxPrice(const FpChartLineSeriesData &data) const { return maxPrice(data.m_y); }

    Q_INVOKABLE
    double minDate(const QVector<qreal> &xData) const
    {
        if (!xData.isEmpty())
            return xData.first();
        return 0;
    }

    Q_INVOKABLE
    double minDate(const FpChartLineSeriesData &data) const { return minDate(data.m_x); }

    Q_INVOKABLE
    double maxDate(const QVector<qreal> &xData) const
    {
        if (!xData.isEmpty())
            return xData.last();
        return 0;
    }

    Q_INVOKABLE
    double maxDate(const FpChartLineSeriesData &data) const { return maxDate(data.m_x); }

    Q_INVOKABLE
    void hackMargin(QAbstractSeries *s) const;

signals:

public slots:

private:
    FpCore *m_fpCore{ nullptr };
    QNetworkAccessManager *m_network{ nullptr };
};

Q_DECLARE_METATYPE(FpChartLineSeriesData)

#endif // FPCHARTDATAWRAPPER_H
