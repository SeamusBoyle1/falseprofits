import QtQuick 2.4
import QtQuick.Controls 2.2

import com.example.fpx 1.0

SignInPageForm {
    signal showSignupScreenRequested

    signInButton.onClicked: doSignIn()

    signupPageButton.onClicked: {
        showSignupScreenRequested()
    }

    emailField.onAccepted: doSignIn()

    passwordField.onAccepted: doSignIn()

    Dialog {
        id: errorDialog
        parent: appNavStack
        title: qsTr("Error")
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        implicitHeight: 160
        implicitWidth: parent.width * 0.9

        Label {
            id: errorDialogText
            anchors.fill: parent
            wrapMode: Text.WordWrap
        }
    }

    function doSignIn() {
        if (!signInButton.enabled) {
            return
        }

        busyIndicator.visible = true
        var resp = fpCore.authenticate(emailField.text, passwordField.text)
        resp.onFinished.connect(function() {
            busyIndicator.visible = false
            if (!resp.hasError()) {
            } else {
                if (resp.httpStatusCode() === 401 /* Authentication failed */) {
                    errorDialogText.text = qsTr("Your email or password is incorrect.")
                } else {
                    errorDialogText.text = resp.errorMessage()
                }
                errorDialog.open()
            }
        })
    }
}
