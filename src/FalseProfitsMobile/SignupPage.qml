import QtQuick 2.7
import QtQuick.Controls 2.2

SignupPageForm {
    signupButton.onClicked: {
        var newUser = fpType.makeNewUserDetails()
        newUser.displayName = displayNameField.text
        newUser.email = emailField.text
        newUser.password = passwordField.text

        busyIndicator.visible = true
        var resp = fpCore.createNewUser(newUser)
        resp.onFinished.connect(function() {
            busyIndicator.visible = false
            if (!resp.hasError()) {
                signupStatusField.text = qsTr("Signed up")
            } else {
                errorDialogText.text = resp.httpStatusReason()
                errorDialog.open()
                signupStatusField.text = resp.httpStatusReason()
            }
        })
    }

    Dialog {
        id: errorDialog
        title: qsTr("Error")
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        implicitHeight: 160
        implicitWidth: parent.width * 0.9

        Label {
            id: errorDialogText
        }
    }
}
