import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

ListView {
    id: listView
    focus: true
    clip: true
    currentIndex: -1

    Component {
        id: sectionHeading
        RowLayout {
            Label {
                width: listView.width
                Layout.leftMargin: FpStyle.screenEdgeLeftMargin
                Layout.topMargin: 3
                Layout.rightMargin: FpStyle.screenEdgeRightMargin
                Layout.bottomMargin: 16

                text: section
                font.pixelSize: 12
            }
        }
    }

    section.property: "localDate"
    section.criteria: ViewSection.FullString
    section.delegate: sectionHeading

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
                columns: 3

                Label {
                    text: model.description
                    Layout.columnSpan: 3
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                    font.pixelSize: 16
                }

                Label {
                    text: model.localTime
                    Layout.fillWidth: true
                    font.pixelSize: 13
                }

                Label {
                    text: model.amount.toFixed(2)
                    Layout.preferredWidth: 100
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: 13
                }

                Label {
                    text: model.balance.toFixed(2)
                    Layout.preferredWidth: 100
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: 13
                }
            }
        }
    }

    ScrollIndicator.vertical: ScrollIndicator {}
}
