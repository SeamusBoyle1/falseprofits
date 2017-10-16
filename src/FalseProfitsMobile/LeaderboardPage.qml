import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

LeaderboardPageForm {
    property int busyIndicatorVisibility: 0

    FpLeaderboardWrapper {
        id: leaderboardWrapper
        coreClient: fpCore
    }

    Connections {
        target: fpCore
        onAuthStateChanged: {
            if (fpCore.authState === Fpx.AuthenticatedState) {
                refreshLeaderboard()
            } else {
                gotCurrentUserRank = false
            }
        }
    }

    Component.onCompleted: {
        listView.model = leaderboardWrapper.model

        if (fpCore.authState === Fpx.AuthenticatedState) {
            refreshLeaderboard()
        }
    }

    menuButton.onClicked: {
        appDrawer.open()
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

    function updateCurrentUserRank() {
        var notifier = leaderboardWrapper.refreshLeaderboardMe()
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            gotCurrentUserRank = leaderboardWrapper.gotCurrentUserRank()
            if (leaderboardWrapper.gotCurrentUserRank()) {
                currentUserDisplayNameValue = leaderboardWrapper.currentUserDisplayName()
                currentUserRankNumber = leaderboardWrapper.currentUserRank()

                var profit = leaderboardWrapper.currentUserProfit()
                currentUserProfitNumber = profit ? parseFloat(profit).toFixed(2) : ""

                var profitPct = leaderboardWrapper.currentUserProfitPercent()
                currentUserProfitPercentNumber = profitPct ?
                            qsTr("%1%").arg(parseFloat(profitPct).toFixed(2)) : ""
            }
        })
    }

    function refreshLeaderboard() {
        var notifier = leaderboardWrapper.refreshLeaderboard()
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            decrementBusyIndicatorVisibility()
            leaderboardEmpty = listView.count == 0

            // refresh current user ranking when leaderboard refresh completed
            updateCurrentUserRank()
        })
    }

    function fetchMoreResults() {
        if (!leaderboardWrapper.canFetchMore())
            return

        var notifier = leaderboardWrapper.getNextPage()
        incrementBusyIndicatorVisibility()
        notifier.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
        })
    }

    Behavior on currentUserRank.y {
        NumberAnimation { duration: 300 }
    }
}
