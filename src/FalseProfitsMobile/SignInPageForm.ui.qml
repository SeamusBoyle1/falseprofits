import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane {
    width: 400
    height: 400
    property alias getUserDetailsButton: getUserDetailsButton
    property alias userDetails: userDetails
    property alias signInStatusField: signInStatusField
    property alias signInButton: signInButton
    property alias passwordField: passwordField
    property alias emailField: emailField

    ColumnLayout {
        anchors.fill: parent

        TextField {
            id: emailField
            Layout.fillWidth: true
            placeholderText: qsTr("Email address")
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

        Label {
            text: qsTr("sign in status:")
        }

        Label {
            id: signInStatusField
            text: qsTr("no status yet")
        }

        Button {
            id: getUserDetailsButton
            text: qsTr("Get my details")
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        GroupBox {
            title: qsTr("Your user details")
            Layout.fillWidth: true
            GridLayout {
                columns: 2
                id: userDetails
                Layout.fillWidth: true

                property alias userIdText: userIdField.text
                property alias userEmailText: userEmailField.text
                property alias userDisplayNameText: userDisplayNameField.text
                property alias userLevelText: userLevelField.text

                Label {
                    text: "ID"
                }
                Label {
                    id: userIdField
                    Layout.fillWidth: true
                    wrapMode: Text.WrapAnywhere
                }
                Label {
                    text: "email"
                }
                Label {
                    id: userEmailField
                }
                Label {
                    text: "displayName"
                }
                Label {
                    id: userDisplayNameField
                }
                Label {
                    text: "Level"
                }
                Label {
                    id: userLevelField
                }
            }
        }
    }
}
