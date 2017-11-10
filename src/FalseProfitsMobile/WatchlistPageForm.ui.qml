import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import io.material.xtra 1.0

Page {
    width: 400
    height: 400
    property bool watchlistEmpty: false
    property string lastUpdatedString
    property alias busyIndicator: busyIndicator
    property alias addButton: addButton
    property alias listView: listView
    property alias watchlistsComboBox: watchlistsComboBox

    ColumnLayout {
        anchors.fill: parent

        ComboBox {
            id: watchlistsComboBox
            Layout.fillWidth: true
            Layout.leftMargin: FpStyle.screenEdgeLeftMargin
            Layout.rightMargin: FpStyle.screenEdgeRightMargin
            textRole: "name"
            displayText: currentIndex == -1 ? qsTr("Select a watchlist") : currentText
            visible: count > 1
            Layout.topMargin: 8
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: !watchlistsComboBox.visible ? 8 : 0

            WatchlistListView {
                id: listView
                bottomMargin: addButton.visible ? addButton.parent.height - addButton.y : 0
            }
        }

        Label {
            text: lastUpdatedString
            font.pixelSize: 11
            opacity: ExtraMaterial.secondaryTextOpacity
            visible: lastUpdatedString && lastUpdatedString.length > 0
            Layout.leftMargin: FpStyle.screenEdgeLeftMargin
            Layout.rightMargin: FpStyle.screenEdgeRightMargin
            Layout.bottomMargin: 8
        }
    }

    RoundButton {
        id: addButton
        contentItem: Image {
            fillMode: Image.Pad
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            source: "qrc:/images/" + FpStyle.iconAccent + "/add.png"
            opacity: parent.enabled ? ExtraMaterial.activeIconOnAccentOpacity : ExtraMaterial.inactiveIconOnAccentOpacity
        }
        visible: !busyIndicator.visible
        highlighted: true
        width: 56
        height: 56
        anchors.rightMargin: FpStyle.screenEdgeRightMargin
        anchors.bottomMargin: FpStyle.screenEdgeRightMargin
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    VeryBusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }

    Label {
        anchors.centerIn: parent
        text: qsTr("No symbols in Watchlist")
        visible: !busyIndicator.visible && watchlistEmpty
    }
}
