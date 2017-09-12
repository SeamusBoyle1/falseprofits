import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane {
    width: 400
    height: 400
    property alias signupStatusField: signupStatusField
    property alias signupButton: signupButton
    property alias passwordField: passwordField
    property alias emailField: emailField
    property alias displayNameField: displayNameField

    ColumnLayout {
        anchors.fill: parent

        TextField {
            id: displayNameField
            Layout.fillWidth: true
            placeholderText: qsTr("Name")

            Label {
                text: qsTr("Name")
                visible: parent.text
                anchors.bottom: parent.top
            }
        }

        TextField {
            id: emailField
            Layout.fillWidth: true
            placeholderText: qsTr("Email address")

            Label {
                text: qsTr("Email")
                visible: parent.text
                anchors.bottom: parent.top
            }
        }

        TextField {
            id: passwordField
            Layout.fillWidth: true
            placeholderText: qsTr("Password")
            echoMode: TextInput.PasswordEchoOnEdit

            Label {
                text: qsTr("Password")
                visible: parent.text
                anchors.bottom: parent.top
            }
        }

        Button {
            id: signupButton
            text: qsTr("Create account")
            Layout.fillWidth: true
        }

        Label {
            text: qsTr("sign up status:")
        }

        Label {
            id: signupStatusField
            text: qsTr("no status yet")
        }
    }
}
