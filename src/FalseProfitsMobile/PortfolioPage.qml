import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

PortfolioPageForm {
    signal symbolClicked(string symbol)
    signal marketOrderTriggered(string symbol, int quantity)

    property int busyIndicatorVisibility: 0

    FpPortfolioWrapper {
        id: portfolioWrapper
        coreClient: fpCore
    }

    FpTradingAccounts {
        id: myTradingAccounts
        coreClient: fpCore
    }

    Connections {
        target: fpCore
        onAuthStateChanged: {
            if (fpCore.authState === Fpx.AuthenticatedState) {
                updateAccounts()
                if (portfolioWrapper.currentAccountId() !== "") {
                    refreshPositions()
                }
            }
        }
    }

    Connections {
        target: portfolioWrapper

        onDirtyChanged: {
            if (portfolioWrapper.isDirty) {
                refreshPositions()
            }
        }

        onMarketValueChanged: {
            marketValueValue = portfolioWrapper.marketValue.toFixed(2)
        }
        onBalanceChanged: {
            cashBalanceValue = portfolioWrapper.balance.toFixed(2)
        }
    }

    Component.onCompleted: {
        listView.symbolClicked.connect(symbolClicked)
        listView.marketOrderTriggered.connect(marketOrderTriggered)

        accountsComboBox.model = myTradingAccounts.model
        listView.model = portfolioWrapper.model

        marketValueValue = portfolioWrapper.marketValue.toFixed(2)
        cashBalanceValue = portfolioWrapper.balance.toFixed(2)

        if (fpCore.authState === Fpx.AuthenticatedState) {
            updateAccounts()
            if (portfolioWrapper.currentAccountId() !== "") {
                refreshPositions()
            }
        }
    }

    accountsComboBox.onActivated: {
        var accountId = myTradingAccounts.model.getAccountId(accountsComboBox.currentIndex)
        var notifier = portfolioWrapper.loadPositions(accountId)
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            decrementBusyIndicatorVisibility()
            portfolioEmpty = listView.count == 0
        })
    }

    onMarketOrderTriggered: {
        doPlaceOrder(symbol, quantity, OrderParams.SellSide)
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

    function refreshPositions() {
        var notifier = portfolioWrapper.refreshPositions()
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            decrementBusyIndicatorVisibility()
            portfolioEmpty = listView.count == 0
        })
    }

    function doPlaceOrder(symbol, quantity, side) {
        var accountId = accountsComboBox.model.getAccountId(accountsComboBox.currentIndex)

        var orderArgs = fpType.makeOrderParams()
        orderArgs.symbol = symbol
        orderArgs.quantity = quantity
        orderArgs.side = side

        var resp = fpCore.sendOrder(accountId, orderArgs)
        incrementBusyIndicatorVisibility()
        resp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            if (!resp.hasError()) {
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
}
