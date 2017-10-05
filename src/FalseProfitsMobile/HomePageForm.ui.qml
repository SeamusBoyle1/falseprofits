import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Page {
    width: 400
    height: 400
    property alias watchlistPage: watchlistPage
    property alias portfolioPage: portfolioPage
    property alias swipeView: swipeView

    SwipeView {
        id: swipeView
        interactive: false
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        PortfolioPage {
            id: portfolioPage
        }

        WatchlistPage {
            id: watchlistPage
        }
    }

    header: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("Portfolio")
        }
        TabButton {
            text: qsTr("Watchlist")
        }
    }
}
