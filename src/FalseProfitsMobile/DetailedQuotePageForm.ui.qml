import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: page
    width: 400
    height: 400
    property string symbolText
    property string companyNameText
    property string industryText
    property alias lastPriceLabel: lastPriceLabel
    property alias orderButton: orderButton
    property alias busyIndicator: busyIndicator

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: mlay.width
        contentHeight: mlay.height
        clip: true

        ColumnLayout {
            id: mlay
            width: page.width

            RowLayout {
                spacing: 20
                Layout.fillWidth: true
                Layout.leftMargin: 16
                Layout.rightMargin: 16

                ColumnLayout {
                    Layout.fillWidth: true

                    Label {
                        id: symbolLabel
                        text: symbolText
                        font.pixelSize: 32
                        Layout.fillWidth: true
                    }

                    Label {
                        id: companyNameLabel
                        text: companyNameText
                        font.pixelSize: 12
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }

                    Label {
                        id: industryLabel
                        text: industryText
                        font.pixelSize: 11
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }
                }

                Button {
                    id: orderButton
                    text: qsTr("Trade")
                    flat: true
                }
            }

            ColumnLayout {
                Layout.leftMargin: 16
                Layout.rightMargin: 16

                Label {
                    id: lastPriceLabel
                    Layout.fillWidth: true
                    font.pixelSize: 32
                }
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
