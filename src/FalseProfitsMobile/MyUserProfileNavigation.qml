import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import io.material.xtra 1.0

Page {
    MyUserProfilePage {
        anchors.fill: parent
        id: userProfilePage
    }

    header: ToolBar {
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
                    opacity: parent.enabled ? ExtraMaterial.activeIconOnAccentOpacity :
                                              ExtraMaterial.inactiveIconOnAccentOpacity
                }
                onClicked: {
                    appDrawer.open()
                }
            }
            Label {
                text: qsTr("Profile")
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
                    opacity: parent.enabled ? ExtraMaterial.activeIconOnAccentOpacity :
                                              ExtraMaterial.inactiveIconOnAccentOpacity
                }
                enabled: visible
                onClicked: refreshView()
            }

            ToolButton {
                id: overflowMenuButton
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/" + FpStyle.iconAccent + "/more_vert.png"
                    opacity: parent.enabled ? ExtraMaterial.activeIconOnAccentOpacity :
                                              ExtraMaterial.inactiveIconOnAccentOpacity
                }
                enabled: visible
                onClicked: menu.open()
            }
        }
    }

    Menu {
        id: menu
        x: parent.width - width
        y: -FpStyle.appBarHeight + 8

        MenuItem {
            text: "Reset Account..."
            onTriggered: resetDialog.open()
        }
        MenuItem {
            text: "Delete User..."
            onTriggered: deleteDialog.open()
        }
    }

    function refreshView() {
        userProfilePage.reloadUserProfile()
        userProfilePage.updateAccounts()
    }

    Dialog {
        id: resetDialog
        title: qsTr("Danger")
        modal: true
        standardButtons: Dialog.Yes | Dialog.No
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        Label {
            text: qsTr("Really reset your trading account?")
            anchors.fill: parent
            wrapMode: Text.WordWrap
        }

        onAccepted: userProfilePage.resetMyAccount()
    }

    Dialog {
        id: deleteDialog
        title: qsTr("Danger")
        modal: true
        standardButtons: Dialog.Yes | Dialog.No
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        Label {
            text: qsTr("Really delete your user account?")
            anchors.fill: parent
            wrapMode: Text.WordWrap
        }

        onAccepted: userProfilePage.deleteMyAccount()
    }
}
