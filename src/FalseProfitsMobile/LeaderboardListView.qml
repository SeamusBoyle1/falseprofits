import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

ListView {
    id: listView
    focus: true
    clip: true
    currentIndex: -1

    delegate: ItemDelegate {
        height: delegateContent.height
        width: parent.width
        hoverEnabled: true

        ColumnLayout {
            id: delegateContent
            width: parent.width
            spacing: 0
            height: 60

            GridLayout {
                id: mainGrid
                Layout.leftMargin: FpStyle.screenEdgeLeftMargin
                Layout.topMargin: 3
                Layout.rightMargin: FpStyle.screenEdgeRightMargin
                Layout.bottomMargin: 3
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                columns: 4

                Label {
                    text: model.rank
                    font.pixelSize: 18
                    font.weight: Font.Medium
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredWidth: 40
                    Layout.rowSpan: 2
                }

                Label {
                    text: model.displayName + (model.isCurrentUser ? " (me)" : "")
                    elide: Text.ElideRight
                    font.pixelSize: 16
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    Layout.rowSpan: 2
                }

                Label {
                    text: model.profit.toFixed(2)
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 100
                }

                Label {
                    text: qsTr("%1%").arg(model.profitPercent.toFixed(2))
                    font.pixelSize: 16
                    font.weight: Font.Medium
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 100
                }
            }
        }
    }

    ScrollIndicator.vertical: ScrollIndicator {}
}