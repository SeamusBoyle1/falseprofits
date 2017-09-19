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

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
