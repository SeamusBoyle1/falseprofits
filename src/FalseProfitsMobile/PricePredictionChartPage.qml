import QtQuick 2.4
import QtQuick.Controls 2.2

import com.example.fpx 1.0

PricePredictionChartPageForm {
    property string currentSymbol : "market"
    property int predictionRequestId: 0
    property int busyIndicatorVisibility: 0

    FpChartDataWrapper {
        id: chartDataWrapper
        coreClient: fpCore
    }

    Component.onCompleted: {
        chartDataWrapper.hackMargin(pricePredictionChartView.predictionSeries)

        // TODO(seamus): Don't hard-code when server supports multiple intervals
        intervalText = "1D"
    }

    Component.onDestruction: {
        chartDataWrapper.hackRemoveAllSeriesAndAxes(pricePredictionChartView.predictionSeries)
    }

    onCurrentSymbolChanged: {
        symbolText = currentSymbol
        fillChart()
    }

    function incrementBusyIndicatorVisibility() {
        busyIndicator.visible = true
        busyIndicatorVisibility = busyIndicatorVisibility + 1
    }

    function decrementBusyIndicatorVisibility() {
        busyIndicatorVisibility = busyIndicatorVisibility - 1
        if (busyIndicatorVisibility == 0) {
            busyIndicator.visible = false
        }
    }

    function fillChart() {
        var predictionResp = chartDataWrapper.getPredictions(currentSymbol)
        var thisRequestId = predictionRequestId + 1
        predictionRequestId = thisRequestId
        incrementBusyIndicatorVisibility()
        predictionResp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()

            if (thisRequestId < predictionRequestId) {
                return
            }

            var preds = chartDataWrapper.loadPredictions(predictionResp.payload());
            maybeHasChartData = preds.xData.length > 0

            chartDataWrapper.updatePredictionSeries(pricePredictionChartView.predictionSeries,
                                                    pricePredictionChartView.truePriceSeries,
                                                    preds)
            chartDataWrapper.updatePredictionHorizontalLine(
                        pricePredictionChartView.predictionHorizontalLine, preds)

            var minPrice = chartDataWrapper.minPrice(preds)
            var maxPrice = chartDataWrapper.maxPrice(preds)
            if (maxPrice === 0) {
                maxPrice = 1000
            }
            var padding = maxPrice !== minPrice ? ((maxPrice - minPrice) * 0.05) : 0.01
            minPrice -= padding
            maxPrice += padding
            pricePredictionChartView.yAxis.min = minPrice
            pricePredictionChartView.yAxis.max = maxPrice

            pricePredictionChartView.xAxis.min = 0
            pricePredictionChartView.xAxis.max = preds.xData.length

            var predDiff = chartDataWrapper.predictionPercentDifference(preds);
            var predictionStatement = qsTr("Prediction price is %1% %2 than True Price").
            arg(fpLocale.toDecimalString(Math.abs(predDiff), 2)).
            arg(predDiff > 0 ? qsTr("higher") : qsTr("lower"))
            statementLabel.text = predictionStatement
        })
    }

    function onRefreshTriggered() {
        fillChart()
    }
}
