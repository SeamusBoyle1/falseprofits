import QtQuick 2.0
import QtCharts 2.2

ChartView {
    property alias mySeries : mySeries
    property alias xAxis: xAxis

    title: qsTr("Dividends History")
    antialiasing: true
    legend.visible: false

    animationOptions: ChartView.SeriesAnimations

    BarSeries {
        id: mySeries
        axisX: BarCategoryAxis {
            id: xAxis
            labelsFont.pixelSize: 11
        }
    }
}
