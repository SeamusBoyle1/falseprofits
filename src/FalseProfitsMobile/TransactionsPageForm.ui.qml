import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import io.material.xtra 1.0

Page {
//    width: 400
//    height: 400
    property bool transactionsEmpty: false
    property alias busyIndicator: busyIndicator
    property alias listView: listView
    property alias accountsComboBox: accountsComboBox
    property alias menuButton: menuButton
    property alias filterButton: filterButton
    property alias refreshButton: refreshButton

    header: ToolBar {
        implicitHeight: FpStyle.appBarHeight
        Layout.fillWidth: true
        RowLayout {
            anchors.fill: parent
            ToolButton {
                id: menuButton
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/" + FpStyle.iconAccent + "/menu.png"
                    opacity: parent.enabled ? ExtraMaterial.activeIconOnAccentOpacity : ExtraMaterial.inactiveIconOnAccentOpacity
                }
            }
            Label {
                text: qsTr("Transactions")
                elide: Label.ElideRight
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
                font.pixelSize: 16
                font.bold: true
            }
            ToolButton {
                id: filterButton
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/" + FpStyle.iconAccent + "/filter_list.png"
                    opacity: parent.enabled ? ExtraMaterial.activeIconOnAccentOpacity : ExtraMaterial.inactiveIconOnAccentOpacity
                }
            }
            ToolButton {
                id: refreshButton
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/" + FpStyle.iconAccent + "/refresh.png"
                    opacity: parent.enabled ? ExtraMaterial.activeIconOnAccentOpacity : ExtraMaterial.inactiveIconOnAccentOpacity
                }
                enabled: visible
            }
        }
    }

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

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: !accountsComboBox.visible ? 8 : 0
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            contentWidth: listView.width

            TransactionsListView {
                id: listView
            }
        }
    }

    VeryBusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }

    Label {
        anchors.centerIn: parent
        text: qsTr("No transactions")
        visible: !busyIndicator.visible && transactionsEmpty
    }
}
