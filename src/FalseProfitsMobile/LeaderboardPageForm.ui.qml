import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

import com.example.fpx 1.0

Page {
//    width: 400
//    height: 400
    property bool leaderboardEmpty: false
    property bool gotCurrentUserRank: false
    property int currentUserRankNumber: 0
    property string currentUserDisplayNameValue: ""
    property string currentUserProfitNumber: ""
    property string currentUserProfitPercentNumber: ""
    property alias busyIndicator: busyIndicator
    property alias listView: listView
    property alias currentUserRank: currentUserRank
    property alias menuButton: menuButton
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
                }
            }
            Label {
                text: qsTr("Leaderboard")
                elide: Label.ElideRight
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
                font.pixelSize: 16
                font.bold: true
            }
            ToolButton {
                id: refreshButton
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/" + FpStyle.iconAccent + "/refresh.png"
                }
                enabled: visible
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

        LeaderboardListView {
            id: listView
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: 8
            bottomMargin: gotCurrentUserRank ? currentUserRank.height : 0
        }
    }

    AccentPane {
        id: currentUserRank
        anchors.leftMargin: FpStyle.screenEdgeLeftMargin
        anchors.rightMargin: FpStyle.screenEdgeRightMargin
        anchors.left: parent.left
        anchors.right: parent.right
        y: gotCurrentUserRank ? parent.height - height : parent.height

        Material.elevation: 6

        RowLayout {
            id: currentUserPanelContents

            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                text: currentUserRankNumber
                font.pixelSize: 18
                font.weight: Font.Medium
                horizontalAlignment: Text.AlignHCenter
                Layout.preferredWidth: 40
            }
            Label {
                text: currentUserDisplayNameValue
                font.pixelSize: 13
                Layout.fillWidth: true
            }
            Label {
                text: currentUserProfitNumber
                font.pixelSize: 16
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: 100
            }
            Label {
                text: currentUserProfitPercentNumber
                font.pixelSize: 18
                font.weight: Font.Medium
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: 100
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
        text: qsTr("No leaderboard received")
        visible: !busyIndicator.visible && leaderboardEmpty
    }
}
