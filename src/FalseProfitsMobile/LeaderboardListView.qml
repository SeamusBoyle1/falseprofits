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
                    text: fpLocale.toIntString(model.rank)
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
                    text: fpLocale.toDecimalString(model.profit, 2)
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 100
                }

                Label {
                    text: qsTr("%1%").arg(fpLocale.toDecimalString(model.profitPercent, 2))
                    font.pixelSize: 16
                    font.weight: Font.Medium
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 100
                }
            }

            HorizontalDivider {
                Layout.fillWidth: true
            }
        }
    }

    ScrollIndicator.vertical: ScrollIndicator {}
}
