import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

Page {
    FpTradingAccounts {
        id: myTradingAccounts
        coreClient: fpCore
    }

    TradingAccountsListView {
        id: accountsListView
        anchors.fill: parent
        anchors.topMargin: 8
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }

    Connections {
        target: fpCore
        onAuthStateChanged: {
            if (fpCore.authState === Fpx.AuthenticatedState) {
                updateAccounts()
            }
        }
    }

    Component.onCompleted: {
        accountsListView.model = myTradingAccounts.model

        if (fpCore.authState === Fpx.AuthenticatedState) {
            updateAccounts()
        }
    }

    function updateAccounts() {
        var notifier = myTradingAccounts.updateAccounts()
        busyIndicator.visible = true
        notifier.onFinished.connect(function() {
            busyIndicator.visible = false
        })
    }
}
