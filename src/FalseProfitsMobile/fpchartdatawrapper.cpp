// Copyright 2017

#include "fpchartdatawrapper.h"

#include <FalseProfitsCore/fpcore.h>
#include <FalseProfitsCore/fpdeclarativetypes.h>
#include <FalseProfitsCore/responsetypes.h>

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

#include <algorithm>

FpChartDataWrapper::FpChartDataWrapper(QObject *parent)
    : QObject(parent)
    , m_network{ new QNetworkAccessManager(this) }
{
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
    connect(rep, &QNetworkReply::finished, this, [resp, rep, this]() {
        if (!resp) {
            rep->deleteLater();
            return;
        }
        auto httpStatusCode = rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        resp->setHttpStatusCode(httpStatusCode);
        if (rep->error() == QNetworkReply::NoError) {
            auto converted = convertYahooData(rep->readAll());
            resp->setPayload(converted.toJson(QJsonDocument::Compact));
        } else {
            resp->setErrorMessage(rep->errorString() + QString("(code: %1)").arg(httpStatusCode));
        }

        rep->deleteLater();
        resp->setFinished();
    });

    return resp;
}

QJsonDocument FpChartDataWrapper::convertYahooData(const QByteArray &ba) const
{
    auto doc = QJsonDocument::fromJson(ba);
    auto obj = doc.object().value("chart").toObject();
    auto resultObj = obj.value("result").toArray().at(0).toObject();
    auto meta = resultObj.value("meta").toObject();
    auto timestamps = resultObj.value("timestamp").toArray();
    auto indicators = resultObj.value("indicators").toObject();
    auto quotes = indicators.value("quote").toArray().at(0).toObject();
    auto opens = quotes.value("open").toArray();
    auto highs = quotes.value("high").toArray();
    auto lows = quotes.value("low").toArray();
    auto closes = quotes.value("close").toArray();

    QJsonArray r;

    QString kTimestamp("timestamp");
    QString kOpen("open");
    QString kHigh("high");
    QString kLow("low");
    QString kClose("close");

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

    return QJsonDocument(r);
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

FpChartLineSeriesData FpChartDataWrapper::makeCloseLineSeries(const QByteArray &json) const
{
    auto doc = QJsonDocument::fromJson(json);
    auto const arr = doc.array();

    FpChartLineSeriesData r;
    r.m_x.reserve(arr.size());
    r.m_y.reserve(arr.size());

    for (auto const &in : arr) {
        auto const sample = in.toObject();
        auto sampleTs = sample.value(QLatin1String("timestamp"));
        if (!sampleTs.isString()) {
            continue;
        }
        auto sampleTsDt = QDateTime::fromString(sampleTs.toString(), Qt::ISODate);
        if (!sampleTsDt.isValid()) {
            continue;
        }
        auto sampleClose = sample.value(QLatin1String("close"));
        if (!sampleClose.isDouble()) {
            continue;
        }
        r.m_x.append(sampleTsDt.toMSecsSinceEpoch());
        r.m_y.append(sampleClose.toDouble());
    }

    return r;
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
