import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    ColumnLayout {
        anchors.centerIn: parent

        Label {
            text: qsTr("Really sign out?")
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        Item {
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            spacing: 20

            Button {
                text: qsTr("No")

                onClicked: {
                    if (appNavStack.depth > 1) {
                        appNavStack.pop()
                        listView.currentIndex = -1
                    } else {
                        appDrawer.open()
                    }
                }
            }

            Button {
                text: qsTr("Yes")

                onClicked: {
                    fpCore.signOut()
                    signedOutDialog.visible = true
                }
            }
        }
    }

    Dialog {
        id: signedOutDialog
        parent: appNavStack
        title: qsTr("Signed Out")
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        Label {
            anchors.fill: parent
            wrapMode: Text.WordWrap
            text: qsTr("You're now signed out")
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
