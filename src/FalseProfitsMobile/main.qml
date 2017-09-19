import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

ApplicationWindow {
    id: appWindow
    visible: true
    width: 360
    height: 520
    title: qsTr("Hello World")

    StackView {
        id: appNavStack
        anchors.fill: parent
        initialItem: MyUserProfileNavigation{}
    }

    Drawer {
        id: appDrawer
        width: Math.min(appWindow.width, appWindow.height) / 3 * 2
        height: appWindow.height

        property bool enableDrawer: true

        onEnableDrawerChanged: {
            // Drawer doesn't have an enable property, fake one
            dragMargin = !enableDrawer ? 0 : Qt.styleHints.startDragDistance
        }
    }

    Dialog {
        id: errorDialog
        title: qsTr("Error")
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        implicitHeight: 160
        implicitWidth: parent.width * 0.9

        Label {
            id: errorDialogText
            wrapMode: Text.WordWrap
        }
    }

    Dialog {
        id: signedOutDialog
        title: "Signed Out"
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        Label {
            wrapMode: Text.WordWrap
            text: "You're now signed out"
        }
    }

    function showSignInScreen() {
        appNavStack.push("qrc:/SignInNavigation.qml", {"objectName": "SignInNavigation"})
        appDrawer.close()
        appDrawer.enableDrawer = false
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
            if (appNavStack.depth > 1) {
                appNavStack.pop()
            } else {
                appNavStack.clear() // limit depth
                appNavStack.push("qrc:/MyUserProfileNavigation.qml")
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
