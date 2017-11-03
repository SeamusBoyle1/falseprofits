import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import "MaterialComponents"

ApplicationWindow {
    id: appWindow
    visible: true
    width: FpStyle.appWindowWidth
    height: FpStyle.appWindowHeight

    Shortcut {
        sequence: StandardKey.Quit
        context: Qt.ApplicationShortcut
        onActivated: Qt.quit()
    }

    StackView {
        id: appNavStack
        anchors.fill: parent
        initialItem: QuotesNavigation{}
    }

    Drawer {
        id: appDrawer
        width: Math.min(FpStyle.navBarMaxWidth, appWindow.width - FpStyle.appBarHeight)
        height: appWindow.height

        property bool enableDrawer: true

        onEnableDrawerChanged: {
            // Drawer doesn't have an enable property, fake one
            dragMargin = !enableDrawer ? 0 : Qt.styleHints.startDragDistance
        }

        ScrollView {
            anchors.fill: parent
            contentWidth: appDrawerContentsLayout.width
            contentHeight: appDrawerContentsLayout.height
            clip: true

            ColumnLayout {
                id: appDrawerContentsLayout
                width: appDrawer.width
                spacing: 0

                DrawerUserProfile {
                    Layout.fillWidth: true
                }

                HorizontalDivider {
                    Layout.fillWidth: true
                }

                ListView {
                    id: listView

                    focus: true
                    currentIndex: -1
                    implicitHeight: childrenRect.height
                    interactive: false
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    Layout.topMargin: 8

                    delegate: ItemDelegate {
                        width: parent.width
                        height: 48
                        text: model.title
                        font.pixelSize: 14
                        font.weight: Font.Medium
                        opacity: 0.87
                        highlighted: ListView.isCurrentItem
                        onClicked: {
                            listView.currentIndex = index
                            if (model.source === "qrc:/SignOutPage.qml") {
                                showSignOutScreen()
                                return
                            }
                            appNavStack.clear() // limit depth
                            appNavStack.push(model.source)
                            appDrawer.close()
                        }
                    }

                    model: ListModel {
                        ListElement{ title: qsTr("Quotes"); source: "qrc:/QuotesNavigation.qml" }
                        ListElement{ title: qsTr("Leaderboard"); source: "qrc:/LeaderboardPage.qml" }
                        ListElement{ title: qsTr("Transactions"); source: "qrc:/TransactionsPage.qml" }
                        ListElement{ title: qsTr("Profile"); source: "qrc:/MyUserProfileNavigation.qml" }
                        ListElement{ title: qsTr("Sign Out"); source: "qrc:/SignOutPage.qml" }
                    }
                }
                HorizontalDivider {
                    Layout.fillWidth: true
                }

                ItemDelegate {
                    text: qsTr("About")
                    Layout.fillWidth: true
                    opacity: 0.87
                    onClicked: {
                        listView.currentIndex = -1
                        appNavStack.push("qrc:/AboutPage.qml")
                        appDrawer.close()
                    }
                }
            }
        }
    }

    function showSignInScreen() {
        listView.currentIndex = -1
        appNavStack.push("qrc:/SignInNavigation.qml", {"objectName": "SignInNavigation"})
        appDrawer.close()
        appDrawer.enableDrawer = false
    }

    function showSignOutScreen() {
        listView.currentIndex = -1
        appNavStack.push("qrc:/SignOutPage.qml", {"objectName": "SignOutPage"})
        appDrawer.close()
    }

    /*!
       Push the sign in screen if not authenticated,
       Pop the sign in screen if authenticated and the
       stack is not empty, otherwise show the default
       screen.
     */
    function getBackToWork() {
        if (fpCore.authState === Fpx.NotAuthenticatedState) {
            showSignInScreen()
        } else if (appNavStack.currentItem
                   && appNavStack.currentItem.objectName === "SignInNavigation"){
            appDrawer.enableDrawer = true
            var didGetBackToWork = false
            if (appNavStack.depth > 1) {
                appNavStack.pop()
                if (appNavStack.currentItem.objectName === "SignOutPage") {
                    if (appNavStack.depth > 1) {
                        appNavStack.pop()
                        didGetBackToWork = true
                    }
                } else {
                    didGetBackToWork = true
                }
            }

            if (!didGetBackToWork) {
                listView.currentIndex = 0
                appNavStack.clear() // limit depth
                appNavStack.push("qrc:/QuotesNavigation.qml")
            }
        }
    }

    Connections {
        target: fpCore
        onLaunchSettingsLoaded: {
            getBackToWork()
        }
        onAuthStateChanged: {
            getBackToWork()
        }
    }
}
