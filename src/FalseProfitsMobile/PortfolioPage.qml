import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

PortfolioPageForm {
    signal symbolClicked(string symbol)
    signal marketOrderTriggered(string symbol, int quantity)
    signal portfolioLoaded(int positionCount)

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
            marketValueValue = fpLocale.toDecimalString(portfolioWrapper.marketValue, 2)
        }
        onBalanceChanged: {
            cashBalanceValue = fpLocale.toDecimalString(portfolioWrapper.balance, 2)
        }
    }

    Component.onCompleted: {
        listView.symbolClicked.connect(symbolClicked)
        listView.marketOrderTriggered.connect(marketOrderTriggered)

        accountsComboBox.model = myTradingAccounts.model
        listView.model = portfolioWrapper.model

        marketValueValue = fpLocale.toDecimalString(portfolioWrapper.marketValue, 2)
        cashBalanceValue = fpLocale.toDecimalString(portfolioWrapper.balance, 2)

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
        busyIndicator.incrementVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            busyIndicator.decrementVisibility()
            portfolioEmpty = listView.count == 0
            portfolioLoaded(listView.count)
            updateLastUpdateDisplay()
        })
    }

    onMarketOrderTriggered: {
        doPlaceOrder(symbol, quantity, OrderParams.SellSide)
    }

    function updateAccounts() {
        var notifier = myTradingAccounts.updateAccounts()
        busyIndicator.incrementVisibility()
        notifier.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
            if (accountsComboBox.currentIndex == -1) {
                accountsComboBox.incrementCurrentIndex()
            }
        })
    }

    function refreshPositions() {
        var notifier = portfolioWrapper.refreshPositions()
        busyIndicator.incrementVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            busyIndicator.decrementVisibility()
            portfolioEmpty = listView.count == 0
            updateLastUpdateDisplay()
        })
    }

    function doPlaceOrder(symbol, quantity, side) {
        var accountId = accountsComboBox.model.getAccountId(accountsComboBox.currentIndex)

        var orderArgs = fpType.makeOrderParams()
        orderArgs.symbol = symbol
        orderArgs.quantity = quantity
        orderArgs.side = side

        var resp = fpCore.sendOrder(accountId, orderArgs)
        busyIndicator.incrementVisibility()
        resp.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
            if (!resp.hasError()) {
            } else {
                errorDialogText.text = resp.errorMessage()
                errorDialog.open()
            }
        })
    }

    function updateLastUpdateDisplay() {
        lastUpdatedString = qsTr("Last updated %1").arg(fpLocale.toLocaleDateTimeStringNarrowFormat(new Date))
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
