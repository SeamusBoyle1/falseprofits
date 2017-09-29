import QtQuick 2.0
import QtCharts 2.2

ChartView {
    property alias candleSeries: candleSeries
    property alias xAxis: dateTimeAxis2
    property alias xAxisLabelsAxis: dateTimeLabelAxis
    property alias yAxis: axisYRight2

    antialiasing: false
    legend.visible: false

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
