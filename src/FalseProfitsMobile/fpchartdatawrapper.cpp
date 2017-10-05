// Copyright 2017

#include "fpchartdatawrapper.h"

#include <FalseProfitsCore/fpcore.h>
#include <FalseProfitsCore/fpdeclarativetypes.h>
#include <FalseProfitsCore/responsetypes.h>

#include <QCandlestickSeries>
#include <QCandlestickSet>
#include <QChart>
#include <QDateTime>
#include <QGraphicsLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
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

FpChartDataWrapper::FpChartDataWrapper(QObject *parent)
    : QObject(parent)
    , m_network{ new QNetworkAccessManager(this) }
{
    qRegisterMetaType<FpChartCandleSeriesData>();
    qRegisterMetaType<FpChartLineSeriesData>();
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
