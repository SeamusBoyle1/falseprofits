import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    width: 400
    height: 400
    property alias userIdText: userIdField.text
    property alias userEmailText: userEmailField.text
    property alias userDisplayNameText: userDisplayNameField.text
    property alias userLevelText: userLevelField.text

    GridLayout {
        anchors.fill: parent
        columns: 2

        Label {
            text: qsTr("ID")
        }

        Label {
            id: userIdField
            Layout.fillWidth: true
            wrapMode: Text.WrapAnywhere
        }

        Label {
            text: qsTr("email")
        }

        Label {
            id: userEmailField
        }

        Label {
            text: qsTr("displayName")
        }

        Label {
            id: userDisplayNameField
        }

        Label {
            text: qsTr("Level")
        }

        Label {
            id: userLevelField
        }
    }
}
