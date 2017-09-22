import QtQuick 2.4
import QtQuick.Controls 2.2

import com.example.fpx 1.0

OrderTicketPageForm {
    signal orderCompleted

    FpTradingAccounts {
        id: myTradingAccounts
        coreClient: fpCore
    }

    Component.onCompleted: {
        accountsComboBox.model = myTradingAccounts.model
        myTradingAccounts.updateAccounts()
    }

    placeOrderButton.onClicked: {
        doPlaceOrder()
    }

    function doPlaceOrder() {
        var accountId = accountsComboBox.model.getAccountId(accountsComboBox.currentIndex)

        var orderArgs = fpType.makeOrderParams()
        orderArgs.symbol = symbolField.text.trim().toUpperCase()
        orderArgs.quantity = quantityField.text
        orderArgs.side = buySideOption.checked ? OrderParams.BuySide : OrderParams.SellSide

        var resp = fpCore.sendOrder(accountId, orderArgs)
        busyIndicator.visible = true
        resp.onFinished.connect(function() {
            busyIndicator.visible = false;
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
}
