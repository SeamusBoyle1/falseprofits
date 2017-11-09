import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4 as OldControl
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

TransactionsPageForm {
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
        busyIndicator.incrementVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            busyIndicator.decrementVisibility()
            transactionsEmpty = listView.count == 0
        })
    }

    menuButton.onClicked: {
        appDrawer.open()
    }

    filterButton.onClicked: {
        transactionQueryPopup.open()
    }

    refreshButton.onClicked: {
        updateAccounts()
        refreshTransactions()
    }

    listView.onAtYEndChanged: {
        if (listView.atYEnd) {
            fetchMoreResults()
        }
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

    function refreshTransactions() {
        var notifier = transactionsWrapper.refreshTransactions()
        busyIndicator.incrementVisibility()
        notifier.onFinished.connect(function() {
            // TODO(seamus): Handle errors
            busyIndicator.decrementVisibility()
            transactionsEmpty = listView.count == 0
        })
    }

    function fetchMoreResults() {
        if (!transactionsWrapper.canFetchMore())
            return

        var notifier = transactionsWrapper.getNextPage()
        busyIndicator.incrementVisibility()
        notifier.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
        })
    }

    Dialog {
        id: transactionQueryPopup

        // The minimum height required by the Dialog. Added to contentHeight
        // value to determine total required height for forced resize.
        property int dialogBaseHeight: 115

        focus: true
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: visible

        x: (parent.width - width) / 2 //parent.width - width - 5
        y: (parent.height - height) / 2

        title: qsTr("Filter Transactions")

        // Forces resize of Dialog - required due to lack of automatic resize
        // under certain conditions in combination with calendar visibility toggle.
        onContentHeightChanged: {
            height = contentHeight + dialogBaseHeight
        }

        onAboutToHide: {
            startCalendar.visible = false;
            endCalendar.visible = false
        }

        ColumnLayout {

            Row {
                Label {
                    text: qsTr("Start Date")
                    topPadding: 8
                    rightPadding: 10
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            startCalendar.visible = true;
                            endCalendar.visible = false
                        }
                    }
                }

                TextField {
                    id: startdateText
                    text: Qt.formatDate(startCalendar.selectedDate, "dd/MM/yyyy")
                    font.pixelSize: 14
                    inputMask: "99/99/9999"

                    onEditingFinished: {
                        var newDate = new Date();
                        newDate.setDate(text.substr(0, 2));
                        newDate.setMonth(text.substr(3, 2) - 1);
                        newDate.setFullYear(text.substr(6, 4));
                        startCalendar.selectedDate = newDate;
                        startCalendar.visible = false
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            startCalendar.visible = true;
                            endCalendar.visible = false
                        }

                    }
                }
            }

            Row
            {
                Label {
                    text: qsTr("End Date  ")
                    rightPadding: 10
                    topPadding: 8
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            startCalendar.visible = false;
                            endCalendar.visible = true
                        }

                    }
                }

                TextField {
                    id: endDateText
                    text: Qt.formatDate(endCalendar.selectedDate, "dd/MM/yyyy")
                    font.pixelSize: 14
                    inputMask: "99/99/9999"

                    onEditingFinished: {
                        var newDate = new Date();
                        newDate.setDate(text.substr(0, 2));
                        newDate.setMonth(text.substr(3, 2) - 1);
                        newDate.setFullYear(text.substr(6, 4));
                        endCalendar.selectedDate = newDate;
                        endCalendar.visible = false
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            startCalendar.visible = false;
                            endCalendar.visible = true
                        }
                    }
                }
            }

            Row
            {
                id: calendarRow

                OldControl.Calendar
                {
                    id: startCalendar
                    visible: false
                    width: 230
                    height: width

                    focus: visible
                    onClicked: visible = false
                    Keys.onBackPressed: {
                        event.accepted = true;
                        visible = false
                    }

                    style: FpCalendarStyle {
                    }
                }

                OldControl.Calendar {
                    id: endCalendar
                    visible: false
                    width: 230
                    height: width

                    focus: visible
                    onClicked: visible = false
                    Keys.onBackPressed: {
                        event.accepted = true;
                        visible = false
                    }

                    style: FpCalendarStyle {
                    }
                }
            }

            CheckBox {
                id: showTradesOnly
                text: qsTr("Show trades only")
            }

        }

        onAccepted: {
            var start = startCalendar.selectedDate
            var end = endCalendar.selectedDate
            transactionsWrapper.setDateRangeLocal(start, end)
            transactionsWrapper.setShowTradesOnly(showTradesOnly.checked)
            refreshTransactions()
        }
    }
}
