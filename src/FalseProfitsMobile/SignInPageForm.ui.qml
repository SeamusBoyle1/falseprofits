import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane {
    width: 400
    height: 400
    property alias busyIndicator: busyIndicator
    property alias signupPageButton: signupPageButton
    property alias signInButton: signInButton
    property alias passwordField: passwordField
    property alias emailField: emailField

    ColumnLayout {
        spacing: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        TextField {
            id: emailField
            Layout.fillWidth: true
            placeholderText: qsTr("Email address")
            selectByMouse: true
            inputMethodHints: Qt.ImhEmailCharactersOnly
            validator: RegExpValidator {
                // RegExp source: https://stackoverflow.com/a/16148388
                regExp: /\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/
            }

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
            selectByMouse: true
            echoMode: TextInput.PasswordEchoOnEdit

            Label {
                text: qsTr("Password")
                visible: parent.text
                anchors.bottom: parent.top
            }
        }

        Button {
            id: signInButton
            text: qsTr("Sign in")
            enabled: emailField.text.length > 0 && emailField.acceptableInput
                     && passwordField.text.length > 0
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        Button {
            id: signupPageButton
            text: qsTr("Create an account")
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            flat: true
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
