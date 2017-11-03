import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

LeaderboardPageForm {
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

    refreshButton.onClicked: {
        // updateCurrentUserRank() is called inside refreshLeaderboard()
        refreshLeaderboard()
    }

    listView.onAtYEndChanged: {
        if (listView.atYEnd) {
            fetchMoreResults()
        }
    }

    function updateCurrentUserRank() {
        var notifier = leaderboardWrapper.refreshLeaderboardMe()
        busyIndicator.incrementVisibility()
        gotCurrentUserRank = false
        notifier.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
            gotCurrentUserRank = leaderboardWrapper.gotCurrentUserRank()
            if (leaderboardWrapper.gotCurrentUserRank()) {
                currentUserDisplayNameValue = leaderboardWrapper.currentUserDisplayName()
                currentUserRankNumber = fpLocale.toIntString(leaderboardWrapper.currentUserRank())

                var profitPct = leaderboardWrapper.currentUserProfitPercent()
                currentUserProfitPercentNumber = profitPct ?
                            qsTr("%1%").arg(fpLocale.toDecimalString(parseFloat(profitPct), 2)) : ""
            }
        })
    }

    function refreshLeaderboard() {
        var notifier = leaderboardWrapper.refreshLeaderboard()
        busyIndicator.incrementVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            busyIndicator.decrementVisibility()
            leaderboardEmpty = listView.count == 0

            // refresh current user ranking when leaderboard refresh completed
            updateCurrentUserRank()
        })
    }

    function fetchMoreResults() {
        if (!leaderboardWrapper.canFetchMore())
            return

        var notifier = leaderboardWrapper.getNextPage()
        busyIndicator.incrementVisibility()
        notifier.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
        })
    }

    Behavior on currentUserRank.y {
        NumberAnimation { duration: 300 }
    }
}
