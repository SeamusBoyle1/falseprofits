import QtQuick 2.4
import QtQuick.Controls 2.2

import com.example.fpx 1.0

OrderTicketPageForm {
    signal orderCompleted

    property int busyIndicatorVisibility: 0

    FpTradingAccounts {
        id: myTradingAccounts
        coreClient: fpCore
    }

    Component.onCompleted: {
        accountsComboBox.model = myTradingAccounts.model
        updateAccounts()
    }

    placeOrderButton.onClicked: {
        doPlaceOrder()
    }

    symbolField.onTextChanged: {
        currentSymbol = symbolField.text.toUpperCase()
    }

    function doPlaceOrder() {
        var accountId = accountsComboBox.model.getAccountId(accountsComboBox.currentIndex)

        var orderArgs = fpType.makeOrderParams()
        orderArgs.symbol = currentSymbol
        orderArgs.quantity = quantityField.text
        orderArgs.side = buySideOption.checked ? OrderParams.BuySide : OrderParams.SellSide

        var resp = fpCore.sendOrder(accountId, orderArgs)
        incrementBusyIndicatorVisibility()
        resp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            if (!resp.hasError()) {
                infoDialogText.text = resp.httpStatusReason()
                infoDialog.open()
                orderCompleted()
            } else {
                errorDialogText.text = resp.httpStatusReason()
                errorDialog.open()
            }
        })
    }

    Dialog {
        id: errorDialog
        title: qsTr("Error")
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        implicitHeight: 160
        implicitWidth: parent.width * 0.9

        Label {
            id: errorDialogText
        }
    }

    Dialog {
        id: infoDialog
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        implicitHeight: 160
        implicitWidth: parent.width * 0.9

        Label {
            id: infoDialogText
        }
    }

    function incrementBusyIndicatorVisibility() {
        busyIndicator.visible = true
        busyIndicatorVisibility = busyIndicatorVisibility + 1
    }

    function decrementBusyIndicatorVisibility() {
        busyIndicatorVisibility = busyIndicatorVisibility - 1
        if (busyIndicatorVisibility == 0) {
            busyIndicator.visible = false
        }
    }

    function updateAccounts() {
        var notifier = myTradingAccounts.updateAccounts()
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            if (accountsComboBox.currentIndex == -1) {
                accountsComboBox.incrementCurrentIndex()
            }
        })
    }
}
