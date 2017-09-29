import QtQuick 2.0
import QtCharts 2.2

ChartView {
    property alias lineSeries: lineSeries
    property alias xAxis: dateTimeAxis2
    property alias yAxis: axisYRight2

    antialiasing: true
    legend.visible: false

    animationOptions: ChartView.SeriesAnimations

    ValueAxis {
        id: axisYRight2
        labelsFont.pixelSize: 11
    }

    DateTimeAxis {
        id: dateTimeAxis2
        tickCount: 3
        labelsFont.pixelSize: 11
    }

    LineSeries {
        id: lineSeries
        name: "LineSeries"
        axisX: dateTimeAxis2
        axisYRight: axisYRight2
    }
}
