import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

TransactionsPageForm {
    property int busyIndicatorVisibility: 0

    FpTransactionsWrapper {
        id: transactionsWrapper
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
                if (transactionsWrapper.currentAccountId() !== "") {
                    refreshTransactions()
                }
            }
        }
    }

    Component.onCompleted: {
        accountsComboBox.model = myTradingAccounts.model
        listView.model = transactionsWrapper.model

        if (fpCore.authState === Fpx.AuthenticatedState) {
            updateAccounts()
            if (transactionsWrapper.currentAccountId() !== "") {
                refreshTransactions()
            }
        }
    }

    accountsComboBox.onActivated: {
        var accountId = myTradingAccounts.model.getAccountId(accountsComboBox.currentIndex)
        var notifier = transactionsWrapper.loadTransactions(accountId)
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            decrementBusyIndicatorVisibility()
            transactionsEmpty = listView.count == 0
        })
    }

    menuButton.onClicked: {
        appDrawer.open()
    }

    filterButton.onClicked: {
        transactionQueryPopup.open()
    }

    listView.onAtYEndChanged: {
        if (listView.atYEnd) {
            fetchMoreResults()
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

    function refreshTransactions() {
        var notifier = transactionsWrapper.refreshTransactions()
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            decrementBusyIndicatorVisibility()
            transactionsEmpty = listView.count == 0
        })
    }

    function fetchMoreResults() {
        if (!transactionsWrapper.canFetchMore())
            return

        var notifier = transactionsWrapper.getNextPage()
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
        })
    }

    Dialog {
        id: transactionQueryPopup
        modal: true
        focus: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        title: qsTr("Filter Transactions")

        GridLayout {
            columns: 2

            Label {
                text: qsTr("First Date")
            }

            TextField {
                id: startDateInput
                placeholderText: "yyyy-M-d"
                selectByMouse: true
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Last Date")
            }

            TextField {
                id: endDateInput
                placeholderText: "yyyy-M-d"
                selectByMouse: true
                Layout.fillWidth: true
            }
        }

        onAccepted: {
            var start = transactionsWrapper.makeDateFromString(startDateInput.text, "yyyy-M-d")
            var end = transactionsWrapper.makeDateFromString(endDateInput.text, "yyyy-M-d")
            transactionsWrapper.setDateRangeLocal(start, end)
            refreshTransactions()
        }
    }
}
