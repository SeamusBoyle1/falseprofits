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

    signal symbolClicked(string symbol)

    delegate: ItemDelegate {
        id: itemDelegate
        height: delegateContent.height
        width: parent.width
        hoverEnabled: true

        ColumnLayout {
            id: delegateContent
            width: parent.width
            height: 76
            spacing: 0

            ColumnLayout {
                Layout.leftMargin: FpStyle.screenEdgeLeftMargin
                Layout.topMargin: 3
                Layout.rightMargin: FpStyle.screenEdgeRightMargin
                Layout.bottomMargin: 3

                spacing: 0

                Label {
                    Layout.fillWidth: true
                    text: model.symbol
                    font.pixelSize: 16
                }
                Label {
                    Layout.fillWidth: true
                    text: model.companyName
                    font.pixelSize: 14
                    elide: Text.ElideRight
                    Layout.maximumWidth: itemDelegate.width - FpStyle.screenEdgeLeftMargin -
                                         FpStyle.screenEdgeRightMargin
                }
                Label {
                    Layout.fillWidth: true
                    text: model.industry
                    font.pixelSize: 14
                    opacity: ExtraMaterial.secondaryTextOpacity
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
                fetchMoreResults()
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
