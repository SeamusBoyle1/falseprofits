import QtQuick 2.7

SymbolSearchPageForm {
    signal symbolClicked(string symbol)

    Component.onCompleted: {
        listView.symbolClicked.connect(symbolClicked)
    }

    searchButton.onClicked: {
        doSearch()
    }

    queryString.onAccepted: {
        doSearch()
    }

    listView.onAtYEndChanged: {
        if (listView.atYEnd) {
            fetchMoreResults()
        }
    }

    function doSearch() {
        symbolSearchWrapper.clearSearchResults()
        var notifier = symbolSearchWrapper.doSearch(queryString.text)
        if (notifier) {
            busyIndicator.visible = true;
            notifier.onFinished.connect(function() {
                busyIndicator.visible = false;
                zeroSearchResults = listView.count == 0
            })
        }
    }

    function fetchMoreResults() {
        if (!symbolSearchWrapper.canFetchMore())
            return

        var notifier = symbolSearchWrapper.getNextPage()
        if (notifier) {
            busyIndicator.visible = true;
            notifier.onFinished.connect(function() {
                busyIndicator.visible = false;
            })
        }
    }
}
