import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ListView {
    id: listView
    focus: true
    clip: true
    currentIndex: -1

    signal symbolClicked(string symbol)

    delegate: ItemDelegate {
        height: delegateContent.height
        width: parent.width

        ColumnLayout {
            id: delegateContent

            ColumnLayout {
                Layout.margins: 6

                spacing: 0

                Label {
                    Layout.fillWidth: true
                    text: model.symbol
                    font.pixelSize: 16
                }
                Label {
                    Layout.fillWidth: true
                    text: model.companyName
                    font.pixelSize: 13
                }
                Label {
                    Layout.fillWidth: true
                    text: model.industry
                    font.pixelSize: 11
                }
            }
        }

        onClicked: {
            symbolClicked(model.symbol)
        }
    }

    ScrollIndicator.vertical: ScrollIndicator {}
}
