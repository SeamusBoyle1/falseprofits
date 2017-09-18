import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
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
                    text: qsTr("Account")
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

    Connections {
        target: fpCore
        onAuthStateChanged: {
            tabBar.setCurrentIndex(1 /* index of sign in page, demo testing only */)
        }
    }
}
