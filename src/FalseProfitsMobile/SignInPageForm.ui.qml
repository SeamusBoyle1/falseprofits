import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import io.material.xtra 1.0

Pane {
    width: 400
    height: 400
    property alias busyIndicator: busyIndicator
    property alias signupPageButton: signupPageButton
    property alias signInButton: signInButton
    property alias passwordField: passwordField
    property alias emailField: emailField
    property bool _inDesktopMode: FpStyle.selector === "desktop"

    ColumnLayout {
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        Image {
            fillMode: Image.Pad
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            source: "qrc:/images/logo/AppLogo.png"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        ColumnLayout {
            spacing: 20
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.maximumWidth: _inDesktopMode ? 480 : parent.width

            ColumnLayout {
                visible: _inDesktopMode
                spacing: 20
                Layout.bottomMargin: 20

                Label {
                    text: qsTr("Sign in to continue")
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 26
                    Layout.fillWidth: true
                }
            }

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
                    color: emailField.acceptableInput ? ExtraMaterial.hintTextColor : ExtraMaterial.errorHintTextColor
                    font.pixelSize: 12
                    visible: parent.text
                    anchors.bottom: parent.top
                }
            }

            TextField {
                id: passwordField
                Layout.fillWidth: true
                placeholderText: qsTr("Password")
                selectByMouse: true
                echoMode: FpStyle.passwordEchoMode

                Label {
                    text: qsTr("Password")
                    color: ExtraMaterial.hintTextColor
                    visible: parent.text
                    anchors.bottom: parent.top
                }
            }

            Button {
                id: signInButton
                text: qsTr("Sign in")
                enabled: emailField.text.length > 0
                         && emailField.acceptableInput
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
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
