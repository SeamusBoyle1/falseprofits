import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

MyUserProfilePageForm {
    property int busyIndicatorVisibility: 0

    Connections {
        target: fpCore
        onAuthStateChanged: {
            reloadUserProfile()
        }
    }

    Component.onCompleted: {
        reloadUserProfile()
    }

    cancelButton.onClicked: {
        userDetails.userDisplayNameText = originalDisplayName
        userDetails.userEmailText = originalEmail
    }

    saveButton.onClicked: {
        var newProfile = fpType.makeEditUserArgs()

        if (originalDisplayName != userDetails.userDisplayNameText) {
            newProfile.displayName = userDetails.userDisplayNameText
        }

        if (originalEmail != userDetails.userEmailText) {
            newProfile.email = userDetails.userEmailText
        }

        incrementBusyIndicatorVisibility()
        var resp = fpCore.editUserProfile(newProfile)
        resp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            if (!resp.hasError()) {
                reloadUserProfile()
            } else {
                errorDialogText.text = resp.errorMessage()
                errorDialog.open()
            }
        })
    }

    deleteMyAccountButton.onActivated: {
        incrementBusyIndicatorVisibility()
        var resp = fpCore.deleteUser()
        resp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            if (!resp.hasError()) {
                infoDialog.title = qsTr("User Deleted")
                infoDialogText.text = qsTr("Your user account has been annihilated.")
                infoDialog.open()
            } else {
                errorDialogText.text = resp.errorMessage()
                errorDialog.open()
            }
        })
    }

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

    Dialog {
        id: infoDialog
        parent: appNavStack
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        implicitHeight: 160
        implicitWidth: parent.width * 0.9

        Label {
            id: infoDialogText
            anchors.fill: parent
            wrapMode: Text.WordWrap
        }
    }

    Connections {
        target: fpCore
        onAuthStateChanged: {
            updateAuthenticationDisplay()
        }
    }

    function incrementBusyIndicatorVisibility() {
        busyIndicator.visible = true
        busyIndicatorVisibility = busyIndicatorVisibility + 1
    }

    function decrementBusyIndicatorVisibility() {
        busyIndicatorVisibility = busyIndicatorVisibility - 1
        if (busyIndicatorVisibility == 0) {
            busyIndicator.visible = false
        }
    }

    function clearUserProfileDisplay()
    {
        originalDisplayName = ""
        originalEmail = ""

        headlineGreeting.text = "Hello"
        userDetails.userEmailText = ""
        userDetails.userDisplayNameText = ""
        userDetails.userLevelText = ""
    }

    function updateAuthenticationDisplay()
    {
        if (fpCore.authState === Fpx.NotAuthenticatedState) {
            clearUserProfileDisplay()
        }
    }

    function reloadUserProfile()
    {
        if (fpCore.authState === Fpx.AuthenticatedState) {
            incrementBusyIndicatorVisibility()
            var userProfileResp = fpCore.getUserProfile()
            userProfileResp.onFinished.connect(function() {
                decrementBusyIndicatorVisibility()
                if (!userProfileResp.hasError()) {
                    var userDetailsDat = fpType.makeJsonUserDetails(userProfileResp.payload())
                    originalDisplayName = userDetailsDat.displayName
                    originalEmail = userDetailsDat.email

                    headlineGreeting.text = qsTr("Hi %1!").arg(userDetailsDat.displayName)
                    userDetails.userEmailText = userDetailsDat.email
                    userDetails.userDisplayNameText = userDetailsDat.displayName
                    userDetails.userLevelText = userDetailsDat.level
                } else {
                    clearUserProfileDisplay()
                    errorDialogText.text = userProfileResp.errorMessage()
                    errorDialog.open()
                }
            })
        }
    }
}
