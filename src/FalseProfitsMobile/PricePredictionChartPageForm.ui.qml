import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Page {
    width: 400
    height: 400
    property string symbolText
    property string intervalText
    property alias busyIndicator: busyIndicator
    property alias pricePredictionChartView: pricePredictionChartView
    property alias statementLabel: statementLabel
    property bool maybeHasChartData: true

    Label {
        id: statementLabel
        text: "I predict that..."
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        visible: !busyIndicator.visible && maybeHasChartData
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.top: parent.top
        anchors.topMargin: 8
        font.pixelSize: 36
    }

    FpPricePredictionChartWidget {
        anchors.fill: parent
        id: pricePredictionChartView
        backgroundRoundness: 0
        anchors.topMargin: statementLabel.height + 8
    }

    Label {
        text: symbolText + ", " + intervalText
        font.pixelSize: 68
        anchors.centerIn: pricePredictionChartView
        opacity: 0.25
        visible: !emptyChartLabel.visible
    }

    Label {
        id: emptyChartLabel
        text: qsTr("No prediction data")
        visible: !busyIndicator.visible && !maybeHasChartData
        anchors.centerIn: pricePredictionChartView
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
