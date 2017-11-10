import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import "MaterialComponents"

ListView {
    id: listView
    focus: true
    clip: true
    currentIndex: -1

    signal accountClicked(string accountId)

    delegate: ItemDelegate {
        height: delegateContent.height
        width: parent.width
        hoverEnabled: true

        ColumnLayout {
            id: delegateContent
            width: parent.width
            height: 72
            spacing: 0

            ColumnLayout {
                Layout.leftMargin: FpStyle.screenEdgeLeftMargin
                Layout.topMargin: 3
                Layout.rightMargin: FpStyle.screenEdgeRightMargin
                Layout.bottomMargin: 3

                spacing: 0

                Label {
                    text: model.name
                    font.pixelSize: 24
                    Layout.fillWidth: true
                    Layout.bottomMargin: 6
                }

                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        text: qsTr("Total Cash ($):")
                    }

                    Label {
                        text: fpLocale.toDecimalString(model.balance, 2)
                        horizontalAlignment: Text.AlignLeft
                        font.pixelSize: 13
                        Layout.fillWidth: true
                    }
                }

            }

            HorizontalDivider {
                Layout.fillWidth: true
            }
        }

        onClicked: {
            accountClicked(model.id)
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
                updateAccounts()
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
