import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import io.material.xtra 1.0

Pane {
    width: 400
    height: 400
    property alias busyIndicator: busyIndicator
    property alias signInPageButton: signInPageButton
    property alias signupButton: signupButton
    property alias passwordField: passwordField
    property alias emailField: emailField
    property alias displayNameField: displayNameField
    property bool _inDesktopMode: FpStyle.selector === "desktop"

    ColumnLayout {
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        ColumnLayout {
            spacing: 37
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.maximumWidth: _inDesktopMode ? 480 : parent.width

            ColumnLayout {
                visible: _inDesktopMode
                spacing: 20
                Layout.bottomMargin: 20

                Label {
                    text: qsTr("Create an account")
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 26
                    Layout.fillWidth: true
                }
            }

            TextField {
                id: displayNameField
                Layout.fillWidth: true
                placeholderText: qsTr("Name")
                selectByMouse: true
                validator: RegExpValidator {
                    regExp: /^.{5,30}$/
                }

                Label {
                    text: qsTr("Name")
                    color: displayNameField.acceptableInput ? ExtraMaterial.hintTextColor : ExtraMaterial.errorHintTextColor
                    font.pixelSize: 12
                    visible: parent.text
                    anchors.bottom: parent.top
                }

                Label {
                    text: qsTr("Name must be between 5 and 30 characters")
                    color: ExtraMaterial.errorHintTextColor
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    visible: parent.text.length > 0 && !parent.acceptableInput
                    anchors.top: parent.bottom
                }
            }

            TextField {
                id: emailField
                Layout.fillWidth: true
                placeholderText: qsTr("Email address")
                selectByMouse: true
                maximumLength: 100
                validator: RegExpValidator {
                    // RegExp source: https://stackoverflow.com/a/16148388
                    regExp: /\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/
                }
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                                  | Qt.ImhEmailCharactersOnly

                Label {
                    text: qsTr("Email")
                    color: emailField.acceptableInput ? ExtraMaterial.hintTextColor : ExtraMaterial.errorHintTextColor
                    font.pixelSize: 12
                    visible: parent.text
                    anchors.bottom: parent.top
                }

                Label {
                    text: qsTr("Email must be valid and not already used")
                    color: ExtraMaterial.errorHintTextColor
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    visible: parent.text.length > 0 && !parent.acceptableInput
                    anchors.top: parent.bottom
                }
            }

            TextField {
                id: passwordField
                Layout.fillWidth: true
                placeholderText: qsTr("Password")
                selectByMouse: true
                echoMode: FpStyle.passwordEchoMode
                validator: RegExpValidator {
                    regExp: /^.{8,30}$/
                }

                Label {
                    text: qsTr("Password")
                    color: passwordField.acceptableInput ? ExtraMaterial.hintTextColor : ExtraMaterial.errorHintTextColor
                    font.pixelSize: 12
                    visible: parent.text
                    anchors.bottom: parent.top
                }

                Label {
                    text: qsTr("Password must be between 8 and 30 characters")
                    color: ExtraMaterial.errorHintTextColor
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    visible: parent.text.length > 0 && !parent.acceptableInput
                    anchors.top: parent.bottom
                }
            }

            TextField {
                id: passwordConfirmationField
                Layout.fillWidth: true
                placeholderText: qsTr("Confirm your password")
                selectByMouse: true
                echoMode: FpStyle.passwordEchoMode

                property bool passwordsMatch: passwordConfirmationField.text == passwordField.text

                Label {
                    text: qsTr("Confirm your password")
                    color: passwordConfirmationField.passwordsMatch ? ExtraMaterial.hintTextColor : ExtraMaterial.errorHintTextColor
                    font.pixelSize: 12
                    visible: parent.text
                    anchors.bottom: parent.top
                }

                Label {
                    text: qsTr("These passwords don't match. Try again?")
                    color: ExtraMaterial.errorHintTextColor
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    visible: parent.text.length > 0 && !passwordConfirmationField.passwordsMatch
                    anchors.top: parent.bottom
                }
            }

            Button {
                id: signupButton
                text: qsTr("Create account")
                enabled: displayNameField.acceptableInput
                         && emailField.acceptableInput
                         && passwordField.acceptableInput
                         && passwordConfirmationField.passwordsMatch
                Layout.fillWidth: true
            }

            Button {
                id: signInPageButton
                text: qsTr("Already a member? Sign in")
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                flat: true
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
