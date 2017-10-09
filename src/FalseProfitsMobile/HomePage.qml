import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

HomePageForm {
    signal symbolClicked(string symbol)

    Component.onCompleted: {
        portfolioPage.symbolClicked.connect(symbolClicked)
        watchlistPage.symbolClicked.connect(symbolClicked)
    }

    function refreshView() {
        if (portfolioPage.SwipeView.isCurrentItem) {
            portfolioPage.refreshPositions()
        } else if (watchlistPage.SwipeView.isCurrentItem) {
            watchlistPage.refreshWatchlist()
        }
    }
}
