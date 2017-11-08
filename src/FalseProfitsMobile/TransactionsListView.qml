import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

import com.example.fpx 1.0
import io.material.xtra 1.0
import "MaterialComponents"

ListView {
    id: listView
    focus: true
    clip: true
    currentIndex: -1

    Component {
        id: sectionHeading
        RowLayout {
            height: 48

            Label {
                width: listView.width
                Layout.leftMargin: FpStyle.screenEdgeLeftMargin
                Layout.rightMargin: FpStyle.screenEdgeRightMargin
                color: Material.primary

                text: section
                font.pixelSize: 14
                font.weight: Font.Medium
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
                    font.pixelSize: 14
                    opacity: ExtraMaterial.secondaryTextOpacity
                }

                Label {
                    text: fpLocale.toDecimalString(model.amount, 2)
                    Layout.preferredWidth: 100
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: 14
                }

                Label {
                    text: fpLocale.toDecimalString(model.balance, 2)
                    Layout.preferredWidth: 100
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: 14
                    opacity: ExtraMaterial.secondaryTextOpacity
                }
            }

            HorizontalDivider {
                Layout.fillWidth: true
            }
        }
    }

    // Pull to refresh.
    onContentYChanged: {
        // If listView is pulled 200 points beyond it's maximum,
        // and it's not busy doing something, then refresh.
        if(contentY < -200){
            if(busyIndicator.visible){
                return;
            } else {
                refreshTransactions()
            }
        }
    }
}
