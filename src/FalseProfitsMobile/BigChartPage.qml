import QtQuick 2.4
import QtQuick.Controls 2.2

import com.example.fpx 1.0

BigChartPageForm {
    property string currentSymbol
    property string chartDataRange: "1mo"
    property string chartInterval: "1h"
    property int candleOpenMode: 0
    property int candlesRequestId: 0
    property var historyData
    property int busyIndicatorVisibility: 0
    property int tickCountForWidth: 5

    FpChartDataWrapper {
        id: chartDataWrapper
        coreClient: fpCore
    }

    Component.onCompleted: {
        chartDataWrapper.hackMargin(bigChartView.candleSeries)
        chartDataWrapper.hackCandlestickSeriesPen(bigChartView.candleSeries, "#000", 1.0, true)
        bigChartView.candleSeries.visible = candleCharTypeButton.checked

        intervalText = chartInterval.toUpperCase()
    }

    onCurrentSymbolChanged: {
        symbolText = currentSymbol
        fillChart()
    }

    onChartIntervalChanged: {
        intervalText = chartInterval.toUpperCase()
    }

    onChartDataRangeChanged: {
        if (chartDataRange == "1d") {chartInterval = "2m"}
        else if (chartDataRange == "5d") {chartInterval = "15m"}
        else if (chartDataRange == "1mo") {chartInterval = "1h"}
        else if (chartDataRange == "6mo") {chartInterval = "1d"}
        else if (chartDataRange == "ytd") {chartInterval = "1d"}
        else if (chartDataRange == "1y")  {chartInterval = "1wk"}
        else if (chartDataRange == "5y") {chartInterval = "1mo"}
        else if (chartDataRange == "max") {chartInterval = "1mo"}

        syncRangeButtonGroupWithChartInterval()

        fillChart()
    }

    rangeButtonGroup.onClicked2: {
        chartDataRange = button.text
    }

    onWidthChanged: {
        tickCountForWidth = Math.round(Math.min(5, width / 140))
    }

    onTickCountForWidthChanged: {
        bigChartView.xAxis.tickCount = tickCountForWidth
        updateTickLabels()
    }

    chartTypeButtonGroup.onClicked2: {
        bigChartView.lineSeries.visible = lineChartTypeButton.checked
        bigChartView.candleSeries.visible = candleCharTypeButton.checked
    }

    bigChartView.candleSeries.onVisibleChanged: {
        if (!bigChartView.candleSeries.visible) {
            bigChartView.candleSeries.clear()
            updateLineSeries()
        } else {
            updateCandleSeries()
        }
    }

    function updateLineSeries() {
        chartDataWrapper.updateCloseSeries(bigChartView.lineSeries, historyData)
    }

    function updateCandleSeries() {
        if (candleOpenMode === 1) {
            chartDataWrapper.updateSeriesPrevClose(bigChartView.candleSeries, historyData)
        } else {
            chartDataWrapper.updateSeriesFirstTick(bigChartView.candleSeries, historyData)
        }
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

    function syncRangeButtonGroupWithChartInterval() {
        for (var i = 0; i < rangeButtonGroup.buttons.length; ++i) {
            if (rangeButtonGroup.buttons[i].text.toLowerCase() === chartDataRange.toLowerCase()) {
                if (!rangeButtonGroup.buttons[i].checked) {
                    rangeButtonGroup.buttons[i].checked = true
                }

                break
            }
        }
    }

    function fitDataRange() {
        if (!historyData) {
            return
        }

        // TODO return struct from C++ containing all min max for x&y
        var minPrice = chartDataWrapper.minPrice(historyData)
        var maxPrice = chartDataWrapper.maxPrice(historyData)
        if (maxPrice === 0) {
            maxPrice = 1000
        }
        var padding = maxPrice !== minPrice ? ((maxPrice - minPrice) * 0.05) : 0.01
        minPrice -= padding
        maxPrice += padding
        bigChartView.yAxis.min = minPrice
        bigChartView.yAxis.max = maxPrice

        bigChartView.xAxis.min = 0
        bigChartView.xAxis.max = historyData.xData.length
    }

    function updateTickLabels() {
        if (!historyData) {
            return
        }

        var dateFormat = ""
        if (chartDataRange === "1d" || chartInterval === "1m" || chartInterval === "2m") {
            dateFormat = "hh:mm"
        } else if (chartInterval === "1h") {
            dateFormat = "dd MMM"
        } else {
            dateFormat = "dd MMM yy"
        }

        bigChartView.xAxisLabelsAxis.categories = chartDataWrapper.makeDateCategoryLabels(
                    historyData, bigChartView.xAxis.tickCount, dateFormat)
    }

    function fillChart() {
        var reqArgs = fpType.makeCandlesRequestArgs()
        reqArgs.symbol = currentSymbol
        reqArgs.range = chartDataRange
        reqArgs.interval = chartInterval

        var candlesResp = chartDataWrapper.getCandles(reqArgs)
        var thisRequestId = candlesRequestId + 1
        candlesRequestId = thisRequestId
        incrementBusyIndicatorVisibility()
        candlesResp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()

            if (thisRequestId < candlesRequestId) {
                return
            }

            var hist = chartDataWrapper.makeCandleSeries(candlesResp.payload());
            historyData = hist
            maybeHasChartData = hist.xData.length > 0

            if (bigChartView.candleSeries.visible) {
                updateCandleSeries()
            }
            if (bigChartView.lineSeries.visible) {
                updateLineSeries()
            }

            fitDataRange()
            updateTickLabels()
        })
    }
}
