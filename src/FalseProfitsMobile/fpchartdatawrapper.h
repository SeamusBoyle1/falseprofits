// Copyright 2017

#ifndef FPCHARTDATAWRAPPER_H
#define FPCHARTDATAWRAPPER_H

#include <QObject>

#include <QAbstractSeries>
#include <QAbstractAxis>
#include <QBarSeries>
#include <QDate>
#include <QString>
#include <QVector>

QT_CHARTS_USE_NAMESPACE

class QJsonDocument;
class QNetworkAccessManager;

class CandlesRequestArgs;
class FpCore;
class GetCandlesResponse;
class GetPredictionsResponse;

class FpChartCandleSeriesData
{
    Q_GADGET
    Q_PROPERTY(QVector<qreal> xData READ xData)
    Q_PROPERTY(QVector<qreal> open READ open)
    Q_PROPERTY(QVector<qreal> high READ high)
    Q_PROPERTY(QVector<qreal> low READ low)
    Q_PROPERTY(QVector<qreal> close READ close)
public:
    QVector<qreal> m_x;
    QVector<qreal> m_open;
    QVector<qreal> m_high;
    QVector<qreal> m_low;
    QVector<qreal> m_close;

    QVector<qreal> xData() const { return m_x; }
    QVector<qreal> open() const { return m_open; }
    QVector<qreal> high() const { return m_high; }
    QVector<qreal> low() const { return m_low; }
    QVector<qreal> close() const { return m_close; }
};

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

class FpChartDateSeriesData
{
    Q_GADGET
    Q_PROPERTY(QVector<QDate> xData READ xData)
    Q_PROPERTY(QVector<qreal> yData READ yData)
public:
    QVector<QDate> m_x;
    QVector<qreal> m_y;

    QVector<QDate> xData() const { return m_x; }
    QVector<qreal> yData() const { return m_y; }
};

class FpChartLinePredictionSeriesData
{
    Q_GADGET
    Q_PROPERTY(QVector<qreal> xData READ xData)
    Q_PROPERTY(QVector<qreal> yPredictionData READ yPredictionData)
    Q_PROPERTY(QVector<qreal> yTruePriceData READ yTruePriceData)
public:
    QVector<qreal> m_x;
    QVector<qreal> m_yPrediction;
    QVector<qreal> m_yTruePrice;

    QVector<qreal> xData() const { return m_x; }
    QVector<qreal> yPredictionData() const { return m_yPrediction; }
    QVector<qreal> yTruePriceData() const { return m_yTruePrice; }
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

    Q_INVOKABLE
    GetPredictionsResponse *getPredictions(const QString &symbol);

    QJsonDocument convertYahooData(const QByteArray &json, const QString &range) const;

    Q_INVOKABLE
    QList<qreal> latestTradingSession(const QVector<qreal> &xData) const;

    Q_INVOKABLE
    QList<qreal> latestTradingSession(const FpChartLineSeriesData &data) const
    {
        return latestTradingSession(data.m_x);
    }

    Q_INVOKABLE
    FpChartCandleSeriesData makeCandleSeries(const QByteArray &json) const;

    Q_INVOKABLE
    FpChartLineSeriesData makeCloseLineSeries(const QByteArray &json) const;

    Q_INVOKABLE
    QStringList makeDateCategoryLabels(const FpChartCandleSeriesData &data, int tickCount,
                                       const QString &dateFormat) const;

    enum class CandleOpenMode { FirstTick, PrevClose };
    void updateCandleSeries(QAbstractSeries *series, const FpChartCandleSeriesData &data,
                            CandleOpenMode mode) const;

    Q_INVOKABLE
    void updateSeriesFirstTick(QAbstractSeries *series, const FpChartCandleSeriesData &data) const
    {
        updateCandleSeries(series, data, CandleOpenMode::FirstTick);
    }

    Q_INVOKABLE
    void updateSeriesPrevClose(QAbstractSeries *series, const FpChartCandleSeriesData &data) const
    {
        updateCandleSeries(series, data, CandleOpenMode::PrevClose);
    }

    Q_INVOKABLE
    void updateCloseSeries(QAbstractSeries *series, const FpChartCandleSeriesData &data) const;

    Q_INVOKABLE
    void updateSeries(QAbstractSeries *series, const FpChartLineSeriesData &data) const;

    Q_INVOKABLE
    void updatePredictionSeries(QAbstractSeries *predictionSeries, QAbstractSeries *truePriceSeries,
                                const FpChartLinePredictionSeriesData &data) const;

    Q_INVOKABLE
    void updatePredictionHorizontalLine(QAbstractSeries *predictionHorizontalLine,
                                        const FpChartLinePredictionSeriesData &data) const;

    Q_INVOKABLE
    double minPrice(const QVector<qreal> &yData) const
    {
        auto it = std::min_element(yData.begin(), yData.end());
        if (it != yData.end())
            return *it;
        return 0;
    }

    Q_INVOKABLE
    double minPrice(const FpChartCandleSeriesData &data) const { return minPrice(data.m_low); }

    Q_INVOKABLE
    double minPrice(const FpChartLineSeriesData &data) const { return minPrice(data.m_y); }

    Q_INVOKABLE
    double minPrice(const FpChartLinePredictionSeriesData &data) const
    {
        return std::min(minPrice(data.m_yPrediction), minPrice(data.m_yTruePrice));
    }

    Q_INVOKABLE
    double maxPrice(const QVector<qreal> &yData) const
    {
        auto it = std::max_element(yData.begin(), yData.end());
        if (it != yData.end())
            return *it;
        return 0;
    }

    Q_INVOKABLE
    double maxPrice(const FpChartCandleSeriesData &data) const { return maxPrice(data.m_high); }

    Q_INVOKABLE
    double maxPrice(const FpChartLineSeriesData &data) const { return maxPrice(data.m_y); }

    Q_INVOKABLE
    double maxPrice(const FpChartLinePredictionSeriesData &data) const
    {
        return std::max(maxPrice(data.m_yPrediction), maxPrice(data.m_yTruePrice));
    }

    Q_INVOKABLE
    double minDate(const QVector<qreal> &xData) const
    {
        if (!xData.isEmpty())
            return xData.first();
        return 0;
    }

    Q_INVOKABLE
    double minDate(const FpChartCandleSeriesData &data) const { return minDate(data.m_x); }

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
    double maxDate(const FpChartCandleSeriesData &data) const { return maxDate(data.m_x); }

    Q_INVOKABLE
    double maxDate(const FpChartLineSeriesData &data) const { return maxDate(data.m_x); }

    Q_INVOKABLE
    void hackMargin(QAbstractSeries *s) const;

    Q_INVOKABLE
    void hackCandlestickSeriesPen(QAbstractSeries *s, const QPen &pen) const;

    Q_INVOKABLE
    void hackCandlestickSeriesPen(QAbstractSeries *s, const QColor &color, qreal width,
                                  bool cosmetic) const;

    Q_INVOKABLE
    void hackRemoveAllSeriesAndAxes(QAbstractSeries *s) const;

    Q_INVOKABLE
    FpChartDateSeriesData loadDividends(const QByteArray &ba) const;

    Q_INVOKABLE
    FpChartDateSeriesData convertToAnnualDividends(const FpChartDateSeriesData &biannual,
                                                   int numYears) const;

    Q_INVOKABLE
    void updateDividendsSeries(QAbstractAxis *categories, QBarSeries *values,
                               const FpChartDateSeriesData &in) const;

    Q_INVOKABLE
    FpChartLinePredictionSeriesData loadPredictions(const QByteArray &json) const;

    Q_INVOKABLE
    double predictionPercentDifference(const FpChartLinePredictionSeriesData &data) const;

signals:

public slots:

private:
    FpCore *m_fpCore{ nullptr };
    QNetworkAccessManager *m_network{ nullptr };
};

Q_DECLARE_METATYPE(FpChartCandleSeriesData)
Q_DECLARE_METATYPE(FpChartLineSeriesData)
Q_DECLARE_METATYPE(FpChartDateSeriesData)
Q_DECLARE_METATYPE(FpChartLinePredictionSeriesData)

#endif // FPCHARTDATAWRAPPER_H
