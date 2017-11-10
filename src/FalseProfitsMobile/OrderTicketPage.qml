import QtQuick 2.4
import QtQuick.Controls 2.2

import com.example.fpx 1.0

OrderTicketPageForm {
    signal orderCompleted

    property double lastPriceValue: 0

    FpTradingAccounts {
        id: myTradingAccounts
        coreClient: fpCore
    }

    BrokerageCostCalculator {
        id: brokerageCostCalculator
        coreClient: fpCore
    }

    Connections {
        target: fpCore
        onAuthStateChanged: {
            if (fpCore.authState === Fpx.AuthenticatedState) {
                updateAccounts()
                updateCommissionTables()
            }
        }
        onPositionsChanged: {
            // Update accounts to get the new available cash balance
            updateAccounts()
        }
    }

    Component.onCompleted: {
        accountsComboBox.model = myTradingAccounts.model

        if (fpCore.authState === Fpx.AuthenticatedState) {
            updateAccounts()
            updateCommissionTables()
        }
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
        maxOrderSize = buySideOption.checked ? brokerageCostCalculator.maxBuyQuantity() :
                                               brokerageCostCalculator.maxSellQuantity()
    }

    quantityField.onTextChanged: {
        updateEstimatedTotal()
    }

    onLastPriceValueChanged: {
        updateEstimatedTotal()
    }

    accountsComboBox.onCurrentIndexChanged: {
        updateAvailableCashDisplay()
    }

    function doPlaceOrder() {
        var accountId = accountsComboBox.model.getAccountId(accountsComboBox.currentIndex)

        var orderArgs = fpType.makeOrderParams()
        orderArgs.symbol = currentSymbol
        orderArgs.quantity = quantityField.text
        orderArgs.side = buySideOption.checked ? OrderParams.BuySide : OrderParams.SellSide

        var resp = fpCore.sendOrder(accountId, orderArgs)
        busyIndicator.incrementVisibility()
        resp.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
            if (!resp.hasError()) {
                // TODO(seamus): Add order price (from response) to text
                infoDialogText.text = infoDialogText.text = qsTr("%1 %2 %3 shares.").arg(
                            orderArgs.side === OrderParams.BuySide ? "Bought" : "Sold").arg(
                            orderArgs.quantity).arg(orderArgs.symbol)
                infoDialog.open()
                orderCompleted()
            } else {
                errorDialogText.text = resp.errorMessage()
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
            anchors.fill: parent
            wrapMode: Text.WordWrap
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
            anchors.fill: parent
            wrapMode: Text.WordWrap
        }
    }

    function updateAccounts() {
        var notifier = myTradingAccounts.updateAccounts()
        busyIndicator.incrementVisibility()
        notifier.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
            if (accountsComboBox.currentIndex == -1) {
                accountsComboBox.incrementCurrentIndex()
            } else {
                updateAvailableCashDisplay()
            }
        })
    }

    function updateCommissionTables() {
        var buyNotifier = brokerageCostCalculator.updateBuyCommission()
        busyIndicator.incrementVisibility()
        buyNotifier.onFinished.connect(function() {
            busyIndicator.decrementVisibility()

            if (buySideOption.checked) {
                maxOrderSize =  brokerageCostCalculator.maxBuyQuantity()
            }
        })

        var sellNotifier = brokerageCostCalculator.updateSellCommission()
        busyIndicator.incrementVisibility()
        sellNotifier.onFinished.connect(function() {
            busyIndicator.decrementVisibility()

            if (!buySideOption.checked) {
                maxOrderSize = brokerageCostCalculator.maxSellQuantity()
            }
        })
    }

    function updateQuote() {
        lastPriceText = ""
        lastPriceValue = 0

        if (currentSymbol === "")
            return

        busyIndicator.incrementVisibility()
        var quoteResp = fpCore.getQuotes(currentSymbol)
        quoteResp.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
            if (!quoteResp.hasError()) {
                var quotes = fpType.makeJsonQuotes(quoteResp.payload())
                var quote = quotes.find(currentSymbol)
                if (quote.isValid) {
                    lastPriceText = fpLocale.toShortDecimalString(quote.last)
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

            var costResult = brokerageCostCalculator.calcBrokerageCost(costArgs)
            orderValueText = fpLocale.toDecimalString(costResult.orderValue, 3)
            brokerageCostText = fpLocale.toDecimalString(costResult.brokerageCost, 3)
            totalText = fpLocale.toDecimalString(costResult.estimatedTotal, 3)
        }
    }

    function updateAvailableCashDisplay() {
        var cashBalance = accountsComboBox.model.getBalance(accountsComboBox.currentIndex)
        availableCashText = cashBalance ? fpLocale.toDecimalString(cashBalance, 2) : "-"
    }

    function onRefreshTriggered() {
        updateAccounts()
        updateCommissionTables()
        updateQuote()
    }
}
