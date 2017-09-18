import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 360
    height: 520
    title: qsTr("Hello World")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        SignupPage {
        }

        SignInPage {
        }

        SignOutPage {
        }
    }

    header: ColumnLayout {
        ToolBar {
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent
                ToolButton {
                    text: qsTr("...")
                    onClicked: {
                        drawer.open()
                    }
                }
                Label {
                    text: "Account"
                    elide: Label.ElideRight
                    verticalAlignment: Qt.AlignVCenter
                    Layout.fillWidth: true
                    font.pixelSize: 16
                    font.bold: true
                }
            }
        }
        TabBar {
            id: tabBar
            currentIndex: swipeView.currentIndex
            Layout.fillWidth: true
            TabButton {
                text: qsTr("Signup")
            }
            TabButton {
                text: qsTr("Sign in")
            }
            TabButton {
                text: qsTr("Sign out")
            }
        }
    }

    Drawer {
        id: drawer
        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
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

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }

    Connections {
        target: fpCore
        onAuthStateChanged: {
            tabBar.setCurrentIndex(1 /* index of sign in page, demo testing only */)
        }
    }
}
