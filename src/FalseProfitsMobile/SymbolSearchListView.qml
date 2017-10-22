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

    ScrollIndicator.vertical: ScrollIndicator {}
}
