import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: page
    width: 400
    height: 400
    property string currentSymbol
    property alias sellSideOption: sellSideOption
    property alias buySideOption: buySideOption
    property alias accountsComboBox: accountsComboBox
    property alias symbolField: symbolField
    property alias quantityField: quantityField
    property alias placeOrderButton: placeOrderButton
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

            ColumnLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 16
                Layout.rightMargin: 16

                ComboBox {
                    id: accountsComboBox
                    Layout.fillWidth: true
                    textRole: "name"
                    displayText: currentIndex == -1 ? qsTr("Select an account") : currentText
                }

                RowLayout {
                    RadioButton {
                        id: buySideOption
                        text: qsTr("Buy")
                        checked: true
                    }
                    RadioButton {
                        id: sellSideOption
                        text: qsTr("Sell")
                    }
                }

                Label {
                    text: qsTr("Symbol")
                    Layout.fillWidth: true
                }

                TextField {
                    id: symbolField
                    Layout.fillWidth: true
                    selectByMouse: true
                    text: currentSymbol
                    validator: RegExpValidator {
                        regExp: /\w+/
                    }
                }

                Label {
                    text: qsTr("Quantity")
                    Layout.fillWidth: true
                }

                TextField {
                    id: quantityField
                    Layout.fillWidth: true
                    selectByMouse: true
                    validator: IntValidator {
                        bottom: 0
                    }
                }

                Button {
                    id: placeOrderButton
                    text: qsTr("Place Order")
                    Layout.fillWidth: true
                    enabled: accountsComboBox.currentIndex != -1
                             && symbolField.acceptableInput
                             && quantityField.acceptableInput
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
