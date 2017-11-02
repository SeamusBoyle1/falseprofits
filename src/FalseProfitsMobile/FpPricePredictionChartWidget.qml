import QtQuick 2.0
import QtCharts 2.2

ChartView {
    property alias predictionSeries: predictionSeries
    property alias truePriceSeries: truePriceSeries
    property alias xAxis: dateTimeAxis2
    property alias yAxis: axisYRight2

    antialiasing: true
    legend.visible: true

    animationOptions: ChartView.SeriesAnimations

    ValueAxis {
        id: axisYRight2
        labelsFont.pixelSize: 11
    }

    ValueAxis {
        id: dateTimeAxis2
        tickCount: 3
        labelsFont.pixelSize: 11
        visible: false
    }

    LineSeries {
        id: predictionSeries
        name: "Prediction"
        axisX: dateTimeAxis2
        axisYRight: axisYRight2
    }

    LineSeries {
        id: truePriceSeries
        name: "True Price"
        axisX: dateTimeAxis2
        axisYRight: axisYRight2
    }
}
