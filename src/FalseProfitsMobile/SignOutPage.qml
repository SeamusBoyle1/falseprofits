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

        DelayButton {
            text: qsTr("Delete my account")

            onActivated: {
                busyIndicator.visible = true
                var resp = fpCore.deleteUser()
                resp.onFinished.connect(function() {
                    busyIndicator.visible = false
                    if (!resp.hasError()) {
                        deleteUserStatusField.text = qsTr("User deleted")
                    } else {
                        errorDialogText.text = resp.httpStatusReason()
                        errorDialog.open()
                        deleteUserStatusField.text = resp.httpStatusReason()
                    }
                })
            }
        }

        Label {
            text: qsTr("delete user status:")
        }

        Label {
            id: deleteUserStatusField
            text: qsTr("no status")
        }
    }
}
