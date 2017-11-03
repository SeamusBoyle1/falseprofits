import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import io.material.xtra 1.0

Page {
    id: page
    width: 400
    height: 400
    property string currentSymbol
    property string availableCashText
    property string lastPriceText
    property string orderValueText
    property string brokerageCostText
    property string totalText
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
                Layout.topMargin: 8
                Layout.leftMargin: FpStyle.screenEdgeLeftMargin
                Layout.rightMargin: FpStyle.screenEdgeRightMargin

                ComboBox {
                    id: accountsComboBox
                    Layout.fillWidth: true
                    textRole: "name"
                    displayText: currentIndex == -1 ? qsTr("Select an account") : currentText
                    visible: count > 1
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
                    color: symbolField.acceptableInput ? ExtraMaterial.hintTextColor : ExtraMaterial.errorHintTextColor
                    font.pixelSize: 12
                    Layout.fillWidth: true
                }

                TextField {
                    id: symbolField
                    Layout.fillWidth: true
                    placeholderText: qsTr("Ticker Symbol")
                    selectByMouse: true
                    text: currentSymbol
                    validator: RegExpValidator {
                        regExp: /\w+/
                    }
                }

                Label {
                    text: qsTr("Quantity")
                    color: quantityField.acceptableInput ? ExtraMaterial.hintTextColor : ExtraMaterial.errorHintTextColor
                    font.pixelSize: 12
                    Layout.fillWidth: true
                }

                TextField {
                    id: quantityField
                    Layout.fillWidth: true
                    placeholderText: qsTr("Number of shares")
                    selectByMouse: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    validator: IntValidator {
                        bottom: 0
                    }
                }

                GridLayout {
                    rowSpacing: 12
                    columns: 2

                    Label {
                        text: qsTr("Total Cash ($):")
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        text: availableCashText
                        horizontalAlignment: Text.AlignRight
                        Layout.fillWidth: true
                    }

                    Label {
                        text: qsTr("Last Price ($):")
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        text: lastPriceText
                        horizontalAlignment: Text.AlignRight
                        Layout.fillWidth: true
                    }

                    Label {
                        text: qsTr("Estimated Total")
                        Layout.fillWidth: true
                        Layout.columnSpan: 2
                        font.weight: Font.Bold
                    }

                    Label {
                        text: qsTr("Order Value ($):")
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        text: orderValueText
                        horizontalAlignment: Text.AlignRight
                        Layout.fillWidth: true
                    }

                    Label {
                        text: qsTr("Brokerage Cost ($):")
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        text: brokerageCostText
                        horizontalAlignment: Text.AlignRight
                        Layout.fillWidth: true
                    }

                    Label {
                        text: qsTr("Total ($):")
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        text: totalText
                        horizontalAlignment: Text.AlignRight
                        Layout.fillWidth: true
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

        ScrollIndicator.vertical: ScrollIndicator {
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
