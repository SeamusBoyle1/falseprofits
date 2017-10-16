import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

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
            height: 76

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
                    font.pixelSize: 13
                    elide: Text.ElideRight
                    Layout.maximumWidth: itemDelegate.width - FpStyle.screenEdgeLeftMargin -
                                         FpStyle.screenEdgeRightMargin
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
