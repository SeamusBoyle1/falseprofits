import QtQuick 2.4
import QtQuick.Controls 2.2

import com.example.fpx 1.0

OrderTicketPageForm {
    signal orderCompleted

    property int busyIndicatorVisibility: 0

    property double lastPriceValue: 0

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

        // Workaround: For QTBUG-59908
        // Calling updateQuote() on every text change isn't
        // the best idea, use onEditingFinished when the
        // bug is fixed in Qt.
        // This is an acceptable workaround since it is unlikely
        // that the user will regularly change the symbol manually.
        updateQuote()
    }

    buySideOption.onCheckedChanged: {
        updateEstimatedTotal()
    }

    quantityField.onTextChanged: {
        updateEstimatedTotal()
    }

    onLastPriceValueChanged: {
        updateEstimatedTotal()
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

    function updateQuote() {
        lastPriceText = ""
        lastPriceValue = 0

        if (currentSymbol === "")
            return

        incrementBusyIndicatorVisibility()
        var quoteResp = fpCore.getQuotes(currentSymbol)
        quoteResp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            if (!quoteResp.hasError()) {
                var quotes = fpType.makeJsonQuotes(quoteResp.payload())
                var quote = quotes.find(currentSymbol)
                if (quote.isValid) {
                    // TODO(seamus): Don't use toFixed(), it isn't localized
                    lastPriceText = quote.last.toFixed(3)
                    lastPriceValue = quote.last
                }
            } else {
                // No error handling needed we clear the lastPriceValue
                // and lastPriceText before sending the request
            }
        })
    }

    function updateEstimatedTotal() {
        if (lastPriceValue == 0 || !quantityField.acceptableInput) {
            orderValueText = ""
            brokerageCostText = ""
            totalText = ""
        } else {
            var costArgs = fpType.makeBrokerCostCalcArgs()
            costArgs.quantity = parseInt(quantityField.text)
            costArgs.price = lastPriceValue
            costArgs.side = buySideOption.checked ? OrderParams.BuySide : OrderParams.SellSide

            var costResult = fpCore.calcBrokerageCost(costArgs)
            // TODO(seamus): Don't use toFixed(), it isn't localized
            orderValueText = costResult.orderValue.toFixed(3)
            brokerageCostText = costResult.brokerageCost.toFixed(3)
            totalText = costResult.estimatedTotal.toFixed(3)
        }
    }
}
