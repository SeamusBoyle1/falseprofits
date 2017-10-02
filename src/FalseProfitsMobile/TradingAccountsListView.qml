import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

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
                        text: model.balance.toFixed(2)
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 13
                        Layout.fillWidth: true
                    }
                }

            }
        }

        onClicked: {
            accountClicked(model.id)
        }
    }

    ScrollIndicator.vertical: ScrollIndicator {}
}
