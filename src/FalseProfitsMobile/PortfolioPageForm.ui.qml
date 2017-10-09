import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

Page {
    width: 400
    height: 400
    property string marketValueValue
    property string cashBalanceValue
    property bool portfolioEmpty: false
    property alias busyIndicator: busyIndicator
    property alias listView: listView
    property alias accountsComboBox: accountsComboBox

    ColumnLayout {
        anchors.fill: parent

        ComboBox {
            id: accountsComboBox
            Layout.fillWidth: true
            Layout.leftMargin: FpStyle.screenEdgeLeftMargin
            Layout.rightMargin: FpStyle.screenEdgeRightMargin
            textRole: "name"
            displayText: currentIndex == -1 ? qsTr("Select an account") : currentText
            visible: count > 1
            Layout.topMargin: 8
        }

        PortfolioListView {
            id: listView
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: !accountsComboBox.visible ? 8 : 0
        }

        GridLayout {
            Layout.fillHeight: false
            columns: 2
            Layout.leftMargin: FpStyle.screenEdgeLeftMargin
            Layout.rightMargin: FpStyle.screenEdgeRightMargin
            Layout.bottomMargin: FpStyle.screenEdgeRightMargin

            Label {
                text: qsTr("Total Stocks ($):")
                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
            }
            Label {
                text: marketValueValue
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            Label {
                text: qsTr("Total Cash ($):")
                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
            }
            Label {
                text: cashBalanceValue
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }

    Label {
        anchors.centerIn: parent
        text: qsTr("No shares in your portfolio")
        visible: !busyIndicator.visible && portfolioEmpty
    }
}
