import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

Page {
    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        MyUserProfilePage {
            id: userProfilePage
        }

        TradingAccountsPage {
            id: tradingAccountsPage
        }
    }

    header: ColumnLayout {
        ToolBar {
            implicitHeight: FpStyle.appBarHeight
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent
                ToolButton {
                    contentItem: Image {
                        fillMode: Image.Pad
                        horizontalAlignment: Image.AlignHCenter
                        verticalAlignment: Image.AlignVCenter
                        source: "qrc:/images/" + FpStyle.iconAccent + "/menu.png"
                    }
                    onClicked: {
                        appDrawer.open()
                    }
                }
                Label {
                    text: qsTr("Profile and Accounts")
                    elide: Label.ElideRight
                    verticalAlignment: Qt.AlignVCenter
                    Layout.fillWidth: true
                    font.pixelSize: 16
                    font.bold: true
                }
                ToolButton {
                    id: refreshButton
                    contentItem: Image {
                        fillMode: Image.Pad
                        horizontalAlignment: Image.AlignHCenter
                        verticalAlignment: Image.AlignVCenter
                        source: "qrc:/images/" + FpStyle.iconAccent + "/refresh.png"
                    }
                    enabled: visible
                    onClicked: refreshView()
                }
            }
        }
        TabBar {
            id: tabBar
            currentIndex: swipeView.currentIndex
            Layout.fillWidth: true
            TabButton {
                text: qsTr("Profile")
            }

            TabButton {
                text: qsTr("Accounts")
            }
        }
    }

    function refreshView() {
        userProfilePage.reloadUserProfile()
        userProfilePage.updateAccounts()
        tradingAccountsPage.updateAccounts()
    }
}
