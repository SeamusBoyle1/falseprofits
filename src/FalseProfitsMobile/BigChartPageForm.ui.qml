import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Page {
    width: 400
    height: 400
    property alias chartTypeButtonGroup: chartTypeButtonGroup
    property alias candleCharTypeButton: candleCharTypeButton
    property alias lineChartTypeButton: lineChartTypeButton
    property alias rangeButtonGroup: rangeButtonGroup
    property string symbolText
    property string intervalText
    property alias busyIndicator: busyIndicator
    property alias bigChartView: bigChartView
    property bool maybeHasChartData: true

    ButtonGroup2 {
        id: rangeButtonGroup
    }

    ButtonGroup2 {
        id: chartTypeButtonGroup
    }

    FpCandleChartWidget {
        anchors.fill: parent
        id: bigChartView
        backgroundRoundness: 0
    }

    Label {
        text: symbolText + ", " + intervalText
        font.pixelSize: 68
        anchors.centerIn: bigChartView
        opacity: 0.25
        visible: !emptyChartLabel.visible
    }

    Label {
        id: emptyChartLabel
        text: qsTr("No chart data")
        visible: !busyIndicator.visible && !maybeHasChartData
        anchors.centerIn: bigChartView
    }

    header: Pane {
        padding: 0
        hoverEnabled: true

        RowLayout {
            anchors.fill: parent

            Flickable {
                id: flickable2
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumHeight: rangeButtons.implicitHeight
                contentWidth: rangeButtons.implicitWidth
                contentHeight: rangeButtons.implicitHeight
                clip: true

                RowLayout {
                    id: rangeButtons
                    spacing: 0

                    ToolButton {
                        text: qsTr("1d")
                        checkable: true
                        ButtonGroup.group: rangeButtonGroup
                    }
                    ToolButton {
                        text: qsTr("5d")
                        checkable: true
                        ButtonGroup.group: rangeButtonGroup
                    }
                    ToolButton {
                        text: qsTr("1mo")
                        checkable: true
                        checked: true
                        ButtonGroup.group: rangeButtonGroup
                    }
                    ToolButton {
                        text: qsTr("6mo")
                        checkable: true
                        ButtonGroup.group: rangeButtonGroup
                    }
                    ToolButton {
                        text: qsTr("ytd")
                        checkable: true
                        ButtonGroup.group: rangeButtonGroup
                    }
                    ToolButton {
                        text: qsTr("1y")
                        checkable: true
                        ButtonGroup.group: rangeButtonGroup
                    }
                    ToolButton {
                        text: qsTr("5y")
                        checkable: true
                        ButtonGroup.group: rangeButtonGroup
                    }
                    ToolButton {
                        text: qsTr("max")
                        checkable: true
                        ButtonGroup.group: rangeButtonGroup
                    }

                    ToolSeparator {
                    }

                    ToolButton {
                        id: candleCharTypeButton
                        text: qsTr("Candle")
                        checkable: true
                        ButtonGroup.group: chartTypeButtonGroup
                    }
                    ToolButton {
                        id: lineChartTypeButton
                        text: qsTr("Line")
                        checkable: true
                        checked: true
                        ButtonGroup.group: chartTypeButtonGroup
                    }
                }
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
