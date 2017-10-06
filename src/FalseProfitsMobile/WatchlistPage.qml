import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

WatchlistPageForm {
    signal symbolClicked(string symbol)
    signal triggerAddWatchlistSymbol(string watchlistId)

    property int busyIndicatorVisibility: 0

    FpSymbolSearchWrapper {
        id: symbolSearchWrapper
        coreClient: fpCore
    }

    Connections {
        target: fpCore
        onAuthStateChanged: {
            if (fpCore.authState === Fpx.AuthenticatedState) {
                updateAvailableWatchlists()
                if (watchlistWrapper.currentWatchlistId() !== "") {
                    refreshWatchlist()
                }
            }
        }
    }

    Component.onCompleted: {
        listView.symbolClicked.connect(symbolClicked)
        listView.triggerRemoveSymbol.connect(removeSymbol)

        watchlistsComboBox.model = watchlistsListWrapper.model
        listView.model = watchlistWrapper.model

        if (fpCore.authState === Fpx.AuthenticatedState) {
            updateAvailableWatchlists()
            if (watchlistWrapper.currentWatchlistId() !== "") {
                refreshWatchlist()
            }
        }
    }

    watchlistsComboBox.onActivated: {
        var watchlistId = watchlistsComboBox.model.getWatchlistId(watchlistsComboBox.currentIndex)
        var notifier = watchlistWrapper.loadWatchlist(watchlistId)
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            decrementBusyIndicatorVisibility()
            watchlistEmpty = listView.count == 0
        })
    }

    addButton.onClicked: {
        triggerAddWatchlistSymbol(watchlistWrapper.currentWatchlistId())

        symbolSearchPopup.open()
    }

    function updateAvailableWatchlists() {
        var notifier = watchlistsListWrapper.updateWatchlistList()
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            decrementBusyIndicatorVisibility()
            if (watchlistsComboBox.currentIndex === -1) {
                watchlistsComboBox.incrementCurrentIndex()
            }
        })
    }

    function refreshWatchlist() {
        var notifier = watchlistWrapper.refreshWatchlist()
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            decrementBusyIndicatorVisibility()
            watchlistEmpty = listView.count == 0
        })
    }

    function addWatchlistSymbol(symbol) {
        symbolSearchPopup.close()

        var existingRow = watchlistWrapper.model.getRowOfSymbol(symbol)
        if (existingRow >= 0) {
            // scroll to symbol in view if it already exists
            // is the watchlist
            listView.currentIndex = existingRow
        } else {
            var notifier = watchlistWrapper.addSymbol(symbol)
            notifier.onFinished.connect(function() {
                // TODO(seamus): Handle errors
                watchlistWrapper.refreshWatchlist()
            })

            var newRow = watchlistWrapper.model.getRowOfSymbol(symbol)
            // A placeholder item is available to scroll to
            listView.currentIndex = newRow
        }
    }

    function removeSymbol(symbol) {
        var notifier = watchlistWrapper.removeSymbol(symbol)
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
        })
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

    Dialog {
        id: symbolSearchPopup
        modal: true
        focus: true
        standardButtons: Dialog.Cancel

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        contentHeight: 300
        contentWidth: 500

        SymbolSearchPage {
            id: item
            anchors.fill: parent

            Component.onCompleted: {
                item.listView.model = symbolSearchWrapper.model
                item.onSymbolClicked.connect(addWatchlistSymbol)
            }
        }
    }
}
