import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    ColumnLayout {
        anchors.centerIn: parent

        Button {
            text: qsTr("Sign out")

            Component {
                id: me
                Dialog {
                    id: me2
                }
            }

            onClicked: {
                fpCore.signOut()
                signedOutDialog.visible = true
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
