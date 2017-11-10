// Copyright 2017

#include "fpchartdatawrapper.h"

#include <FalseProfitsCore/fpcore.h>
#include <FalseProfitsCore/fpdeclarativetypes.h>
#include <FalseProfitsCore/responsetypes.h>
#include <InvestorAPIClient/jsonobjectwrappers.h>

#include <QBarCategoryAxis>
#include <QBarSet>
#include <QCandlestickSeries>
#include <QCandlestickSet>
#include <QChart>
#include <QDateTime>
#include <QGraphicsLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPointer>
#include <QTimeZone>
#include <QUrl>
#include <QUrlQuery>
#include <QXYSeries>

#include <boost/polymorphic_cast.hpp>

#include <algorithm>
#include <limits>

FpChartDataWrapper::FpChartDataWrapper(QObject *parent)
    : QObject(parent)
    , m_network{ new QNetworkAccessManager(this) }
{
    qRegisterMetaType<FpChartCandleSeriesData>();
    qRegisterMetaType<FpChartLineSeriesData>();
    qRegisterMetaType<FpChartDateSeriesData>();
    qRegisterMetaType<FpChartLinePredictionSeriesData>();
}

GetCandlesResponse *FpChartDataWrapper::getCandles(const CandlesRequestArgs &args)
{
    Q_ASSERT(m_fpCore);
    return m_fpCore->getCandles(args);
}

GetCandlesResponse *FpChartDataWrapper::getCandlesFromYahoo(const CandlesRequestArgs &args)
{
    QPointer<GetCandlesResponse> resp(new GetCandlesResponse);

    QUrl url("https://l1-query.finance.yahoo.com/v8/finance/chart/" + args.symbol() + ".AX");

    QUrlQuery urlQuery;

    if (args.startTime().isValid()) {
        urlQuery.addQueryItem("period1", QString::number(args.startTime().toSecsSinceEpoch()));
    }
    if (args.endTime().isValid()) {
        urlQuery.addQueryItem("period2", QString::number(args.endTime().toMSecsSinceEpoch()));
    }
    if (!args.interval().isEmpty()) {
        urlQuery.addQueryItem("interval", args.interval());
    }
    if (!args.range().isEmpty()) {
        urlQuery.addQueryItem("range", args.range());
    }

    url.setQuery(urlQuery);

    auto rep = m_network->get(QNetworkRequest(url));
    connect(rep, &QNetworkReply::finished, this, [args, resp, rep, this]() {
        if (!resp) {
            rep->deleteLater();
            return;
        }
        auto httpStatusCode = rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        resp->setHttpStatusCode(httpStatusCode);
        if (rep->error() == QNetworkReply::NoError) {
            auto converted = convertYahooData(rep->readAll(), args.range());
            resp->setPayload(converted.toJson(QJsonDocument::Compact));
        } else {
            resp->setErrorMessage(rep->errorString() + QString("(code: %1)").arg(httpStatusCode));
        }

        rep->deleteLater();
        resp->setFinished();
    });

    return resp;
}

GetPredictionsResponse *FpChartDataWrapper::getPredictions(const QString &symbol)
{
    QPointer<GetPredictionsResponse> resp(new GetPredictionsResponse);
    resp->setSymbol(symbol);

    QUrl url("https://falseprophecies.herokuapp.com/1/predict/" + symbol);

    auto rep = m_network->get(QNetworkRequest(url));
    connect(rep, &QNetworkReply::finished, this, [symbol, resp, rep, this]() {
        if (!resp) {
            rep->deleteLater();
            return;
        }
        auto httpStatusCode = rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        resp->setHttpStatusCode(httpStatusCode);
        if (rep->error() == QNetworkReply::NoError) {
            resp->setPayload(rep->readAll());
        } else {
            resp->setErrorMessage(rep->errorString() + QString("(code: %1)").arg(httpStatusCode));
        }

        rep->deleteLater();
        resp->setFinished();
    });

    return resp;
}

QJsonDocument FpChartDataWrapper::convertYahooData(const QByteArray &ba, const QString &range) const
{
    auto doc = QJsonDocument::fromJson(ba);
    auto obj = doc.object().value("chart").toObject();
    auto resultObj = obj.value("result").toArray().at(0).toObject();
    auto meta = resultObj.value("meta").toObject();
    auto interval = meta.value(QLatin1String("dataGranularity")).toString();
    auto timestamps = resultObj.value("timestamp").toArray();
    auto indicators = resultObj.value("indicators").toObject();
    auto quotes = indicators.value("quote").toArray().at(0).toObject();
    auto opens = quotes.value("open").toArray();
    auto highs = quotes.value("high").toArray();
    auto lows = quotes.value("low").toArray();
    auto closes = quotes.value("close").toArray();

    QJsonArray r;

    QString kTimestamp("t");
    QString kOpen("o");
    QString kHigh("h");
    QString kLow("l");
    QString kClose("c");

    for (auto i = 0, total = timestamps.size(); i < total; ++i) {
        auto jsonOpenVal = opens.at(i);
        auto jsonHighVal = highs.at(i);
        auto jsonLowVal = lows.at(i);
        auto jsonCloseVal = closes.at(i);

        // All OHLC values are optional (for Investor-API)
        // but if all fields provided by Yahoo are not doubles
        // skip the candle.
        if (!jsonCloseVal.isDouble() && !jsonOpenVal.isDouble() && !jsonHighVal.isDouble()
            && !jsonLowVal.isDouble()) {
            continue;
        }

        QJsonObject bar;
        bar[kTimestamp] =
            QDateTime::fromSecsSinceEpoch(qint64(timestamps.at(i).toDouble()), Qt::LocalTime)
                .toString(Qt::ISODate);

        // Yahoo returns null for bars containing no change
        if (jsonOpenVal.isDouble()) {
            bar[kOpen] = jsonOpenVal.toDouble();
        }
        if (jsonHighVal.isDouble()) {
            bar[kHigh] = jsonHighVal.toDouble();
        }
        if (jsonLowVal.isDouble()) {
            bar[kLow] = jsonLowVal.toDouble();
        }
        if (jsonCloseVal.isDouble()) {
            bar[kClose] = jsonCloseVal.toDouble();
        }

        r.append(bar);
    }

    QJsonObject wrapObj;
    if (!range.isEmpty()) {
        wrapObj.insert(QStringLiteral("range"), range);
    }
    if (!interval.isEmpty()) {
        wrapObj.insert(QStringLiteral("interval"), interval);
    }
    wrapObj.insert(QStringLiteral("prices"), r);
    return QJsonDocument(wrapObj);
}

QList<qreal> FpChartDataWrapper::latestTradingSession(const QVector<qreal> &xData) const
{
    if (xData.isEmpty()) {
        return { 0.0, 0.0 };
    }

    QTimeZone tzMelbourne("Australia/Melbourne");
    QDateTime start = QDateTime::fromMSecsSinceEpoch(qint64(xData.last()), tzMelbourne);
    start.setTime(QTime(10, 0));
    QDateTime end = start;
    end.setTime(QTime(16, 0));

    return { double(start.toMSecsSinceEpoch()), double(end.toMSecsSinceEpoch()) };
}

FpChartCandleSeriesData FpChartDataWrapper::makeCandleSeries(const QByteArray &json) const
{
    auto doc = QJsonDocument::fromJson(json);
    auto const obj = doc.object();
    auto const arr = obj.value(QLatin1String("prices")).toArray();

    FpChartCandleSeriesData r;
    r.m_x.reserve(arr.size());
    r.m_open.reserve(arr.size());
    r.m_high.reserve(arr.size());
    r.m_low.reserve(arr.size());
    r.m_close.reserve(arr.size());

    for (auto const &in : arr) {
        auto const sample = in.toObject();
        auto sampleTs = sample.value(QLatin1String("t"));
        if (!sampleTs.isString()) {
            continue;
        }
        auto sampleTsDt = QDateTime::fromString(sampleTs.toString(), Qt::ISODate);
        if (!sampleTsDt.isValid()) {
            continue;
        }
        auto sampleOpen = sample.value(QLatin1String("o"));
        if (!sampleOpen.isDouble()) {
            continue;
        }
        auto sampleHigh = sample.value(QLatin1String("h"));
        if (!sampleHigh.isDouble()) {
            continue;
        }
        auto sampleLow = sample.value(QLatin1String("l"));
        if (!sampleLow.isDouble()) {
            continue;
        }
        auto sampleClose = sample.value(QLatin1String("c"));
        if (!sampleClose.isDouble()) {
            continue;
        }
        r.m_x.append(sampleTsDt.toMSecsSinceEpoch());
        r.m_open.append(sampleOpen.toDouble());
        r.m_high.append(sampleHigh.toDouble());
        r.m_low.append(sampleLow.toDouble());
        r.m_close.append(sampleClose.toDouble());
    }

    return r;
}

FpChartLineSeriesData FpChartDataWrapper::makeCloseLineSeries(const QByteArray &json) const
{
    auto doc = QJsonDocument::fromJson(json);
    auto const obj = doc.object();
    auto const arr = obj.value(QLatin1String("prices")).toArray();

    FpChartLineSeriesData r;
    r.m_x.reserve(arr.size());
    r.m_y.reserve(arr.size());

    for (auto const &in : arr) {
        auto const sample = in.toObject();
        auto sampleTs = sample.value(QLatin1String("t"));
        if (!sampleTs.isString()) {
            continue;
        }
        auto sampleTsDt = QDateTime::fromString(sampleTs.toString(), Qt::ISODate);
        if (!sampleTsDt.isValid()) {
            continue;
        }
        auto sampleClose = sample.value(QLatin1String("c"));
        if (!sampleClose.isDouble()) {
            continue;
        }
        r.m_x.append(sampleTsDt.toMSecsSinceEpoch());
        r.m_y.append(sampleClose.toDouble());
    }

    return r;
}

QStringList FpChartDataWrapper::makeDateCategoryLabels(const FpChartCandleSeriesData &data,
                                                       int tickCount,
                                                       const QString &dateFormat) const
{
    QStringList r;
    if (data.m_x.isEmpty()) {
        return r;
    }

    if (tickCount == 1) {
        auto middle = qRound(double(data.m_x.size()) * 0.5);
        middle = qBound(0, middle, data.m_x.size() - 1);
        r.append(QDateTime::fromMSecsSinceEpoch(qint64(data.m_x.at(middle))).toString(dateFormat));
        return r;
    }

    // NOTE: category labels are center aligned, so the value in the middle
    // of each tick is used.
    auto ticksEvery = qRound(double(data.m_x.size()) / (tickCount));
    auto atPos = ticksEvery / 2;
    for (auto i = 0; i < tickCount; ++i, atPos = atPos + ticksEvery) {
        r.append(
            QDateTime::fromMSecsSinceEpoch(qint64(data.m_x.value(atPos))).toString(dateFormat));
    }

    return r;
}

void FpChartDataWrapper::updateCandleSeries(QAbstractSeries *series,
                                            const FpChartCandleSeriesData &data,
                                            FpChartDataWrapper::CandleOpenMode mode) const
{
    if (series == nullptr) {
        return;
    }

    //
    // TODO(seamus): Rewrite to avoid allocations
    //

    auto candleSeries = boost::polymorphic_downcast<QCandlestickSeries *>(series);
    candleSeries->clear();

    QList<QCandlestickSet *> sets;
    sets.reserve(data.m_x.size());
    if (mode == CandleOpenMode::FirstTick) {
        for (auto i = 0, total = data.m_x.size(); i < total; ++i) {
            auto candlestickSet = new QCandlestickSet(data.m_open.at(i), data.m_high.at(i),
                                                      data.m_low.at(i), data.m_close.at(i), i);
            sets.append(candlestickSet);
        }
    } else if (mode == CandleOpenMode::PrevClose) {
        auto prevClose = data.m_open.value(0);
        for (auto i = 0, total = data.m_x.size(); i < total; ++i) {
            auto candlestickSet = new QCandlestickSet(prevClose, data.m_high.at(i),
                                                      data.m_low.at(i), data.m_close.at(i), i);
            sets.append(candlestickSet);
            prevClose = data.m_close.at(i);
        }
    }

    candleSeries->append(sets);
}

void FpChartDataWrapper::updateCloseSeries(QAbstractSeries *series,
                                           const FpChartCandleSeriesData &data) const
{
    if (series == nullptr) {
        return;
    }

    auto xySeries = boost::polymorphic_downcast<QXYSeries *>(series);

    QVector<QPointF> points(data.m_x.size());
    for (auto i = 0, total = data.m_x.size(); i < total; ++i) {
        points[i].rx() = i;
        points[i].ry() = data.m_close.at(i);
    }

    // Use replace instead of clear + append, it's optimized for performance
    xySeries->replace(points);
}

void FpChartDataWrapper::updateSeries(QAbstractSeries *series,
                                      const FpChartLineSeriesData &data) const
{
    if (series == nullptr) {
        return;
    }

    auto xySeries = static_cast<QXYSeries *>(series);

    QVector<QPointF> points(data.m_x.size());
    for (auto i = 0, total = data.m_x.size(); i < total; ++i) {
        points[i].rx() = data.m_x.at(i);
        points[i].ry() = data.m_y.at(i);
    }

    // Use replace instead of clear + append, it's optimized for performance
    xySeries->replace(points);
}

void FpChartDataWrapper::updatePredictionSeries(QAbstractSeries *predictionSeries,
                                                QAbstractSeries *truePriceSeries,
                                                const FpChartLinePredictionSeriesData &data) const
{
    if (predictionSeries == nullptr || truePriceSeries == nullptr) {
        return;
    }

    auto predictionSeries_ = static_cast<QXYSeries *>(predictionSeries);
    auto truePriceSeries_ = static_cast<QXYSeries *>(truePriceSeries);

    QVector<QPointF> predictionPoints(data.m_x.size());
    for (auto i = 0, total = data.m_x.size(); i < total; ++i) {
        predictionPoints[i].rx() = data.m_x.at(i);
        predictionPoints[i].ry() = data.m_yPrediction.at(i);
    }

    QVector<QPointF> truePricePoints(data.m_x.size());
    for (auto i = 0, total = data.m_x.size(); i < total; ++i) {
        truePricePoints[i].rx() = data.m_x.at(i);
        truePricePoints[i].ry() = data.m_yTruePrice.at(i);
    }

    // Use replace instead of clear + append, it's optimized for performance
    predictionSeries_->replace(predictionPoints);
    truePriceSeries_->replace(truePricePoints);
}

void FpChartDataWrapper::updatePredictionHorizontalLine(
    QAbstractSeries *predictionHorizontalLine, const FpChartLinePredictionSeriesData &data) const
{
    if (predictionHorizontalLine == nullptr) {
        return;
    }

    auto predictionHorizontalLine_ = static_cast<QXYSeries *>(predictionHorizontalLine);

    auto lastPredictedPrice = !data.m_yPrediction.isEmpty() ? data.m_yPrediction.last() : 0;
    QVector<QPointF> predictionPoints(data.m_x.size());
    for (auto i = 0, total = data.m_x.size(); i < total; ++i) {
        predictionPoints[i].rx() = data.m_x.at(i);
        predictionPoints[i].ry() = lastPredictedPrice;
    }

    // Use replace instead of clear + append, it's optimized for performance
    predictionHorizontalLine_->replace(predictionPoints);
}

void FpChartDataWrapper::hackMargin(QAbstractSeries *s) const
{
    // Source: https://stackoverflow.com/a/39243275
    if (s == nullptr)
        return;

    if (auto chart = s->chart()) {
        chart->layout()->setContentsMargins(0, 0, 0, 0);
        chart->setBackgroundRoundness(0);
    }
}

void FpChartDataWrapper::hackCandlestickSeriesPen(QAbstractSeries *s, const QPen &pen) const
{
    auto candleSeries = boost::polymorphic_downcast<QCandlestickSeries *>(s);
    candleSeries->setPen(pen);
}

void FpChartDataWrapper::hackCandlestickSeriesPen(QAbstractSeries *s, const QColor &color,
                                                  qreal width, bool cosmetic) const
{
    QPen pen(color, width);
    pen.setCosmetic(cosmetic);
    hackCandlestickSeriesPen(s, pen);
}

/*!
 * Workaround related to QTBUG-63078 and
 * http://lists.qt-project.org/pipermail/interest/2017-September/028113.html
 *
 * The app will crash on Mac (maybe others) when the chart begins
 * destructing children. Removing all the series and axes prior to
 * the chart destructing seems to avoid the crash.
 */
void FpChartDataWrapper::hackRemoveAllSeriesAndAxes(QAbstractSeries *s) const
{
    if (s == nullptr) {
        return;
    }

    auto chart = s->chart();

    chart->removeAllSeries();

    auto axes = chart->axes();
    for (auto axis : qAsConst(axes)) {
        chart->removeAxis(axis);
    }

    qDeleteAll(axes);
}

FpChartDateSeriesData FpChartDataWrapper::loadDividends(const QByteArray &ba) const
{
    auto doc = QJsonDocument::fromJson(ba);
    bsmi::JsonObjectWrappers::ShareDividendsArray d;
    d.jsonItems = doc.array();

    FpChartDateSeriesData r;
    r.m_x.resize(d.size());
    r.m_y.resize(d.size());

    for (auto i = 0, total = d.size(); i < total; ++i) {
        auto const e = d.at(i);
        auto dt = e.date();
        auto val = e.value();
        r.m_x[i] = dt ? *dt : QDate();
        r.m_y[i] = val ? *val : std::numeric_limits<double>::quiet_NaN();
    }

    return r;
}

FpChartDateSeriesData
FpChartDataWrapper::convertToAnnualDividends(const FpChartDateSeriesData &biannual,
                                             int numYears) const
{
    QMap<QDate, double> mapped;

    for (auto i = 0, total = std::min(biannual.m_x.size(), biannual.m_y.size()); i < total; ++i) {
        auto cat = biannual.m_x[i];
        if (!cat.isValid()) {
            continue;
        }
        auto newCat = QDate(cat.year(), 1, 1);

        auto val = biannual.m_y[i];
        if (!qIsNaN(val)) {
            if (!qIsNaN(mapped.value(newCat))) {
                mapped[newCat] += val;
            }
        } else {
            mapped[newCat] = std::numeric_limits<double>::quiet_NaN();
        }
    }

    FpChartDateSeriesData r;
    r.m_x.reserve(mapped.size());
    r.m_y.reserve(mapped.size());

    auto first = mapped.constBegin();
    if (numYears > 0 && numYears < mapped.size()) {
        auto trimStart = mapped.size() - numYears;
        std::advance(first, trimStart);
    }

    for (auto it = first, last = mapped.constEnd(); it != last; ++it) {
        if (!qIsNaN(it.value())) {
            r.m_x.append(it.key());
            r.m_y.append(it.value());
        }
    }

    return r;
}

void FpChartDataWrapper::updateDividendsSeries(QAbstractAxis *categories, QBarSeries *values,
                                               const FpChartDateSeriesData &in) const
{
    auto categoryAxis = boost::polymorphic_downcast<QBarCategoryAxis *>(categories);
    auto barSeries = boost::polymorphic_downcast<QBarSeries *>(values);

    // Update category labels
    if (categoryAxis != nullptr) {
        QStringList newCategories;
        QString dateFormat{ "yyyy" };
        QLocale locale_;
        for (auto i = 0, total = in.m_x.size(); i < total; ++i) {
            newCategories.append(locale_.toString(in.m_x.at(i), dateFormat));
        }
        categoryAxis->setCategories(newCategories);
    }

    // Update values and scale y axis from 0 to max dividend + 3% padding
    if (barSeries != nullptr) {
        barSeries->clear();

        auto barSet = new QBarSet("", barSeries);
        for (auto i = 0, total = in.m_y.size(); i < total; ++i) {
            barSet->append(in.m_y.at(i));
        }
        barSeries->append(barSet);

        if (barSeries->chart()) {
            auto chart = barSeries->chart();
            if (auto yAxis = chart->axisY(barSeries)) {
                yAxis->setMin(0);
                auto maxIt = std::max_element(in.m_y.constBegin(), in.m_y.constEnd());
                if (maxIt != in.m_y.constEnd()) {
                    auto padding = *maxIt * 0.03;
                    yAxis->setMax(*maxIt + padding);
                }
            }
        }
    }
}

FpChartLinePredictionSeriesData FpChartDataWrapper::loadPredictions(const QByteArray &json) const
{
    auto doc = QJsonDocument::fromJson(json);
    auto const obj = doc.object();
    auto const predictionArr = obj.value(QLatin1String("prediction")).toArray();
    auto const true_dataArr = obj.value(QLatin1String("true_data")).toArray();

    FpChartLinePredictionSeriesData r;
    auto size = std::max(predictionArr.size(), true_dataArr.size());
    r.m_x.resize(size);
    r.m_yPrediction.reserve(size);
    r.m_yTruePrice.reserve(size);

    for (auto i = 0; i < size; ++i) {
        r.m_x[i] = i;
    }

    if (predictionArr.size() < size) {
        for (auto i = 0, totalShort = predictionArr.size() - size; i < totalShort; ++i) {
            r.m_yPrediction.append(0);
        }
    }

    for (auto const &in : predictionArr) {
        r.m_yPrediction.append(in.toDouble());
    }

    if (true_dataArr.size() < size) {
        for (auto i = 0, totalShort = true_dataArr.size() - size; i < totalShort; ++i) {
            r.m_yTruePrice.append(0);
        }
    }

    for (auto const &in : true_dataArr) {
        r.m_yTruePrice.append(in.toDouble());
    }

    return r;
}

double FpChartDataWrapper::predictionPercentDifference(const FpChartLinePredictionSeriesData &data) const
{
    if (data.m_yPrediction.isEmpty() || data.m_yTruePrice.isEmpty()) {
        return 0;
    }

    auto lastPrediction = data.m_yPrediction.last();
    auto lastTruePrice = data.m_yTruePrice.last();

    return (lastPrediction - lastTruePrice) / lastTruePrice * 100;
}
