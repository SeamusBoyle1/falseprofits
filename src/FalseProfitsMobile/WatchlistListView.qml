import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import io.material.xtra 1.0
import "MaterialComponents"

ListView {
    id: listView
    focus: true
    clip: true
    currentIndex: -1
    boundsBehavior: Flickable.DragOverBounds

    signal symbolClicked(string symbol)
    signal triggerRemoveSymbol(string symbol)

    delegate: SwipeDelegate {
        id: swipeDelegate
        height: delegateContent.height
        width: parent.width
        hoverEnabled: true

        ListView.onRemove: SequentialAnimation {
            PropertyAction {
                target: swipeDelegate
                property: "ListView.delayRemove"
                value: true
            }
            NumberAnimation {
                target: swipeDelegate
                property: "height"
                to: 0
                easing.type: Easing.InOutQuad
            }
            PropertyAction {
                target: swipeDelegate;
                property: "ListView.delayRemove";
                value: false
            }
        }

        swipe.right: Item {
            id: deleteLabel
            width: 60
            height: parent.height
            anchors.right: parent.right

            // Workaround: this doesn't work on android, added
            // MouseArea to background for now
            // SwipeDelegate.onClicked: listView.model.remove(index)

            Rectangle {
                anchors.fill: parent
                // TODO(seamus): Use FpStyle for color
                color: deleteLabelMouseArea.pressed ? Qt.darker("tomato", 1.1) : "tomato"

                MouseArea {
                    id: deleteLabelMouseArea
                    anchors.fill: parent
                    onClicked: {
                        // Optimistic removal
                        var symbol = listView.model.getSymbolForRow(index)
                        listView.model.remove(index)
                        triggerRemoveSymbol(symbol)
                    }
                }
            }

            Image {
                anchors.fill: parent
                fillMode: Image.Pad
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignVCenter
                source: "qrc:/images/" + FpStyle.iconAccent + "/delete_forever.png"
                opacity: parent.enabled ? ExtraMaterial.activeIconOnAccentOpacity :
                                          ExtraMaterial.inactiveIconOnAccentOpacity
            }
        }

        ColumnLayout {
            id: delegateContent
            width: parent.width
            height: 60
            x: swipeDelegate.swipe.rightItem ?
                   swipeDelegate.swipe.rightItem.width * swipeDelegate.swipe.position : 0
            spacing: 0

            GridLayout {
                id: mainGrid
                Layout.leftMargin: FpStyle.screenEdgeLeftMargin
                Layout.topMargin: 3
                Layout.rightMargin: FpStyle.screenEdgeRightMargin
                Layout.bottomMargin: 3
                columns: 3

                Label {
                    id: symbolLabel
                    text: model.symbol
                    font.pixelSize: 18
                    Layout.fillWidth: true
                    Layout.preferredWidth: 100
                }

                Label {
                    id: directionLabel
                    text: model.direction === 1 ? '▲' : model.direction === -1 ? '▼' : '='
                    // TODO(seamus): Use FpStyle for color
                    color: model.direction === 1 ? "#0b893e" : model.direction
                                                   === -1 ? "#bf1722" : "black"
                    font.pixelSize: 18
                }

                Label {
                    text: model.lastPrice ? fpLocale.toShortDecimalString(model.lastPrice) : ""
                    font.pixelSize: 20
                    Layout.preferredWidth: 100
                    horizontalAlignment: Text.AlignRight
                }

                Label {
                    text: model.name
                    font.pixelSize: 12
                    Layout.columnSpan: 2
                    elide: Text.ElideRight
                    Layout.maximumWidth: Math.min(symbolLabel.width + directionLabel.width -
                                         mainGrid.columnSpacing,
                                                  listView.width - changeLabel.width -
                                                  mainGrid.columnSpacing)
                }

                Label {
                    id: changeLabel
                    text: qsTr("%1 (%2%)").arg(fpLocale.toShortDecimalString(model.change)).arg(
                              fpLocale.toShortDecimalString(model.changePercent))
                    // TODO(seamus): Use FpStyle for color
                    color: model.direction === 1 ? "#0b893e" : model.direction
                                                   === -1 ? "#bf1722" : "black"
                    font.pixelSize: 13
                    Layout.preferredWidth: 100
                    horizontalAlignment: Text.AlignRight
                }
            }

            HorizontalDivider {
                Layout.fillWidth: true
            }
        }

        onClicked: {
            symbolClicked(model.symbol)
        }
    }

    // Pull to refresh.
    onContentYChanged: {
        // If listView is pulled 200 points beyond it's maximum,
        // and it's not busy doing something, then refresh.
        if (contentY < -30 && contentY > -150)
        {
            if(!busyIndicator.visible)
                refreshTip.visible = true;
        }else {
            refreshTip.visible = false;
        }

        if(contentY < -200){
            if(busyIndicator.visible){
                return;
            } else {
                refreshWatchlist()
            }
        }
    }

    // Label to indicate Pull to Refresh feature
    // Appears slightly below top of list, when list is pulled down a bit.
    Label {
        id: refreshTip
        text: "Pull to refresh..."
        visible: false
        parent: listView
        font.italic: true
        topPadding: 20
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
