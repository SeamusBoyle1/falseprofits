import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

HomePageForm {
    signal symbolClicked(string symbol)

    property bool firstView: true

    Component.onCompleted: {
        portfolioPage.symbolClicked.connect(symbolClicked)
        portfolioPage.portfolioLoaded.connect(onPortfolioLoaded)
        watchlistPage.symbolClicked.connect(symbolClicked)
    }

    function refreshView() {
        if (portfolioPage.SwipeView.isCurrentItem) {
            portfolioPage.refreshPositions()
        } else if (watchlistPage.SwipeView.isCurrentItem) {
            watchlistPage.refreshWatchlist()
        }
    }

    function onRefreshTriggered() {
        refreshView()
    }

    function onPortfolioLoaded(positionCount) {
        if (firstView && !(positionCount > 0)) {
            firstView = false
            swipeView.setCurrentIndex(watchlistPage.SwipeView.index)
        }
    }
}
