import QtQuick 2.0
import QtCharts 2.2

ChartView {
    property alias candleSeries: candleSeries
    property alias lineSeries: lineSeries
    property alias xAxis: dateTimeAxis2
    property alias xAxisLabelsAxis: dateTimeLabelAxis
    property alias yAxis: axisYRight2

    antialiasing: candleSeries && !candleSeries.visible
    legend.visible: false

    animationOptions: candleSeries && !candleSeries.visible ?
                          ChartView.SeriesAnimations : ChartView.NoAnimation

    ValueAxis {
        id: axisYRight2
        labelsFont.pixelSize: 11
    }

    BarCategoryAxis {
        id: dateTimeLabelAxis
        labelsFont.pixelSize: 11
    }

    ValueAxis {
        id: dateTimeAxis2
        labelsFont.pixelSize: 11
        visible: false
    }

    LineSeries {
        id: lineSeries
        axisX: dateTimeAxis2
        axisYRight: axisYRight2
        color: "#0d47a1"
        width: 2
    }

    CandlestickSeries {
        id: candleSeries
        name: "CandleSeries"
        axisX: dateTimeAxis2
        axisYRight: axisYRight2
        bodyWidth: 0.6
        minimumColumnWidth: -1.0
        decreasingColor: "#111"
        increasingColor: "#fff"
    }

    LineSeries {
        axisX: dateTimeLabelAxis
    }
}
