import QtQuick 2.4
import QtQuick.Controls 2.2

import com.example.fpx 1.0

DetailedQuotePageForm {
    signal tradeButtonClicked(string symbol)
    signal chartButtonClicked(string symbol, string interval, string dataRange)

    property string currentSymbol
    property string chartDataRange: "1d"
    property string chartInterval: "2m"
    property int candlesRequestId: 0
    property int busyIndicatorVisibility: 0

    FpChartDataWrapper {
        id: chartDataWrapper
        coreClient: fpCore
    }

    Component.onCompleted: {
        chartDataWrapper.hackMargin(priceLineChart.lineSeries)
    }

    Component.onDestruction: {
        chartDataWrapper.hackRemoveAllSeriesAndAxes(priceLineChart.lineSeries)
    }

    orderButton.onClicked: {
        if (currentSymbol !== "") {
            tradeButtonClicked(currentSymbol)
        }
    }

    starButton.onClicked: {
        toggleStarred()
    }

    onCurrentSymbolChanged: {
        clearQuote()
        symbolText = currentSymbol
        newsFeedPage.currentSymbol = currentSymbol
        updateQuote()
        updateStarredState()
        fillChart()
        updateFundamentals()
        updateDividendsHistoryChart()
    }

    rangeButtonGroup.onClicked2: {
        chartDataRange = button.text

        if (chartDataRange == "1d") {chartInterval = "2m"}
        else if (chartDataRange == "5d") {chartInterval = "15m"}
        else if (chartDataRange == "1mo") {chartInterval = "1h"}
        else if (chartDataRange == "6mo") {chartInterval = "1d"}
        else if (chartDataRange == "ytd") {chartInterval = "1d"}
        else if (chartDataRange == "1y")  {chartInterval = "1wk"}
        else if (chartDataRange == "5y")  {chartInterval = "1mo"}
        else if (chartDataRange == "max") {chartInterval = "1mo"}

        fillChart()
    }

    chartFullScreenButton.onClicked: {
        if (currentSymbol !== "") {
            chartButtonClicked(currentSymbol, chartInterval, chartDataRange)
        }
    }

    Dialog {
        id: errorDialog
        parent: appNavStack
        title: qsTr("Error")
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        implicitHeight: 160
        implicitWidth: parent.width * 0.9

        Label {
            id: errorDialogText
            anchors.fill: parent
            wrapMode: Text.WordWrap
        }
    }

    function clearQuote() {
        companyNameText = ""
        industryText = ""
        lastPriceLabel.text = ""
        bidText = ""
        bidSizeText = ""
        askText = ""
        askSizeText = ""
        changePriceText = ""
        direction = 0
        changePercentText = ""
        dayLowText = ""
        dayHighText = ""
    }

    function updateQuote() {
        incrementBusyIndicatorVisibility()
        var quoteResp = fpCore.getQuotes(currentSymbol)
        quoteResp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            if (!quoteResp.hasError()) {
                var quotes = fpType.makeJsonQuotes(quoteResp.payload())
                var quote = quotes.find(currentSymbol)
                if (quote.isValid) {
                    lastPriceLabel.text = fpLocale.toShortDecimalString(quote.last)
                    bidText = fpLocale.toShortDecimalString(quote.bid)
                    bidSizeText = fpLocale.toIntString(quote.bidSize)
                    askText = fpLocale.toShortDecimalString(quote.ask)
                    askSizeText = fpLocale.toIntString(quote.askSize)

                    var change = quote.change
                    direction = change > 0 ? 1 : change < 0 ? -1 : 0
                    var changeSign = direction === 1 ? "+" : ""
                    changePriceText = change ? (changeSign +
                                                fpLocale.toShortDecimalString(quote.change)) : "0"
                    priceLineChart.lineSeries.color = direction > 0 ? "#00a95d" : "#f0162f"

                    var pctChange = quote.changePercent
                    changePercentText = pctChange ?
                                (changeSign +
                                 qsTr("%1%").arg(fpLocale.toShortDecimalString(pctChange))) :
                                qsTr("0%")

                    dayLowText = fpLocale.toShortDecimalString(quote.dayLow)
                    dayHighText = fpLocale.toShortDecimalString(quote.dayHigh)
                }
            } else {
                errorDialogText.text = quoteResp.errorMessage()
                errorDialog.open()
            }
        })

        incrementBusyIndicatorVisibility()
        var detailsResp = fpCore.getShareDetails(currentSymbol)
        detailsResp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            if (!detailsResp.hasError()) {
                companyNameText = detailsResp.companyName
                industryText = detailsResp.industry
            } else {
                errorDialogText.text = detailsResp.errorMessage()
                errorDialog.open()
            }
        })
    }

    function clearFundamentals() {
        fundamental_marketCap = ""
        fundamental_volume = ""
        fundamental_averageDailyVolume = ""
        fundamental_previousClose = ""
        fundamental_low52Weeks = ""
        fundamental_high52Weeks = ""
        fundamental_dividendShare = ""
        fundamental_dividendYield = ""
        fundamental_exDividendDate = ""
        fundamental_dividendPayDate = ""
        fundamental_peRatio = ""
        fundamental_bookValue = ""
        fundamental_priceBook = ""
        fundamental_earningsShare = ""
        fundamental_movingAverage200Days = ""
        fundamental_movingAverage50Days = ""
        fundamental_symbol = ""
        fundamental_name = ""
        fundamental_industry = ""
    }

    function updateFundamentals() {
        incrementBusyIndicatorVisibility()
        var fundataResp = fpCore.getFundamentals(currentSymbol)
        fundataResp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            clearFundamentals()
            if (!fundataResp.hasError()) {
                var fundata = fpType.makeJsonShareFundamentals(fundataResp.payload())

                var marketCap = fundata.marketCap
                fundamental_marketCap = marketCap ? fpLocale.toIntString(marketCap) : "-"

                var volume = fundata.volume
                fundamental_volume = volume ? fpLocale.toIntString(volume) : "-"

                var averageDailyVolume = fundata.averageDailyVolume
                fundamental_averageDailyVolume = averageDailyVolume ?
                            fpLocale.toIntString(averageDailyVolume) : "-"

                var previousClose = fundata.previousClose
                fundamental_previousClose = previousClose ?
                            fpLocale.toShortDecimalString(previousClose) : "-"

                var low52Weeks = fundata.low52Weeks
                fundamental_low52Weeks = low52Weeks ?
                            fpLocale.toShortDecimalString(low52Weeks) : "-"

                var high52Weeks = fundata.high52Weeks
                fundamental_high52Weeks = high52Weeks ? fpLocale.toShortDecimalString(high52Weeks) :
                                                        "-"

                var dividendShare = fundata.dividendShare
                fundamental_dividendShare = dividendShare ?
                            fpLocale.toShortDecimalString(dividendShare) : "-"

                var dividendYield = fundata.dividendYield
                fundamental_dividendYield = dividendYield ?
                            qsTr("%1%").arg(fpLocale.toShortDecimalString(dividendYield)) : "-"

                var exDividendDate = fundata.exDividendDate
                fundamental_exDividendDate = exDividendDate ?
                            fpLocale.toLocaleDateStringShortFormat(exDividendDate) : "-"

                var dividendPayDate = fundata.dividendPayDate
                fundamental_dividendPayDate = dividendPayDate ?
                            fpLocale.toLocaleDateStringShortFormat(dividendPayDate) : "-"

                var peRatio = fundata.peRatio
                fundamental_peRatio = peRatio ? fpLocale.toShortDecimalString(peRatio) : "-"

                var bookValue = fundata.bookValue
                fundamental_bookValue = bookValue ? fpLocale.toShortDecimalString(bookValue) : "-"

                var priceBook = fundata.priceBook
                fundamental_priceBook = priceBook ?
                            fpLocale.toShortDecimalString(priceBook) : "-"

                var earningsShare = fundata.earningsShare
                fundamental_earningsShare = earningsShare ?
                            fpLocale.toShortDecimalString(earningsShare) : "-"

                var movingAverage200Days = fundata.movingAverage200Days
                fundamental_movingAverage200Days = movingAverage200Days ?
                            fpLocale.toShortDecimalString(movingAverage200Days) : "-"

                var movingAverage50Days = fundata.movingAverage50Days
                fundamental_movingAverage50Days = movingAverage50Days ?
                            fpLocale.toShortDecimalString(movingAverage50Days) : "-"

                var symbol = fundata.symbol
                fundamental_symbol = symbol ? symbol : "-"

                var name = fundata.name
                fundamental_name = name ? name : "-"

                var industry = fundata.industry
                fundamental_industry = industry ? industry : "-"
            }
        })
    }

    function updateDividendsHistoryChart() {
        if (!currentSymbol || currentSymbol.length === 0) {
            return;
        }

        var resp = fpCore.getDividends(currentSymbol, "5y")
        incrementBusyIndicatorVisibility()
        dividendHistoryChart.mySeries.visible = false
        resp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            dividendHistoryChart.mySeries.visible = true
            var biannual = chartDataWrapper.loadDividends(resp.payload())
            var annual = chartDataWrapper.convertToAnnualDividends(biannual, 5)

            chartDataWrapper.updateDividendsSeries(dividendHistoryChart.xAxis,
                                                   dividendHistoryChart.mySeries, annual)
        })
    }

    function updateStarredState() {
        starred = watchlistWrapper.model.getRowOfSymbol(currentSymbol) >= 0
    }

    function toggleStarred() {
        if (currentSymbol !== "") {
            if (!starred) {
                var addNotifier = watchlistWrapper.addSymbol(currentSymbol)
                incrementBusyIndicatorVisibility()
                addNotifier.onFinished.connect(function() {
                    // TODO(seamus): Handle errors
                    decrementBusyIndicatorVisibility()
                    updateStarredState()
                })
            } else {
                var removeNotifier = watchlistWrapper.removeSymbol(currentSymbol)
                incrementBusyIndicatorVisibility()
                removeNotifier.onFinished.connect(function() {
                    // TODO(seamus): Handle errors
                    decrementBusyIndicatorVisibility()
                    updateStarredState()
                })
            }
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

            var hist = chartDataWrapper.makeCloseLineSeries(candlesResp.payload());
            maybeHasChartData = hist.xData.length > 0
            chartDataWrapper.updateSeries(priceLineChart.lineSeries, hist)

            // TODO return struct from C++ containing all min max for x&y
            var minPrice = chartDataWrapper.minPrice(hist)
            var maxPrice = chartDataWrapper.maxPrice(hist)
            if (maxPrice === 0) {
                maxPrice = 1000
            }
            var padding = maxPrice !== minPrice ? ((maxPrice - minPrice) * 0.05) : 0.01
            minPrice -= padding
            maxPrice += padding
            priceLineChart.yAxis.min = minPrice
            priceLineChart.yAxis.max = maxPrice
            priceLineChart.xAxis.min = new Date(chartDataWrapper.minDate(hist))
            var maxDate = chartDataWrapper.maxDate(hist)
            priceLineChart.xAxis.max = maxDate !== 0 ? new Date(maxDate) : new Date()

            if (reqArgs.range === "1d" || reqArgs.interval === "1m" || reqArgs.interval === "2m") {
                priceLineChart.xAxis.format = "hh:mm"
                if (reqArgs.range === "1d") {
                    var regSession = chartDataWrapper.latestTradingSession(hist)
                    priceLineChart.xAxis.min = new Date(regSession[0])
                    priceLineChart.xAxis.max = regSession[1] !== 0 ?
                                new Date(regSession[1]) : new Date()
                }
            } else if (reqArgs.interval === "1h") {
                priceLineChart.xAxis.format = "dd MMM"
            } else {
                priceLineChart.xAxis.format = "dd MMM yy"
            }
        })
    }

    function onRefreshTriggered() {
        updateQuote()
        fillChart()
        updateFundamentals()
        updateDividendsHistoryChart()
        newsFeedPage.reloadNews()
    }
}
