import QtQuick 2.7
import QtQuick.Controls 2.2

SignupPageForm {
    signal showSignInScreenRequested

    signupButton.onClicked: doMakeUser()

    signInPageButton.onClicked: {
        showSignInScreenRequested()
    }

    displayNameField.onAccepted: doMakeUser()

    emailField.onAccepted: doMakeUser()

    passwordField.onAccepted: doMakeUser()

    Dialog {
        id: errorDialog
        title: qsTr("Error")
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        Label {
            id: errorDialogText
            anchors.fill: parent
            wrapMode: Text.WordWrap
        }
    }

    function doMakeUser() {
        if (!signupButton.enabled) {
            return
        }

        var newUser = fpType.makeNewUserDetails()
        newUser.displayName = displayNameField.text
        newUser.email = emailField.text
        newUser.password = passwordField.text

        busyIndicator.visible = true
        var resp = fpCore.createNewUser(newUser)
        resp.onFinished.connect(function() {
            busyIndicator.visible = false
            if (!resp.hasError()) {
            } else {
                errorDialogText.text = resp.errorMessage()
                errorDialog.open()
            }
        })
    }
}
