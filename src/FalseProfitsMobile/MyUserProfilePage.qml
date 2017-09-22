import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

MyUserProfilePageForm {

    Component.onCompleted: {
        reloadUserProfile()
    }

    deleteMyAccountButton.onActivated: {
        busyIndicator.visible = true
        var resp = fpCore.deleteUser()
        resp.onFinished.connect(function() {
            busyIndicator.visible = false
            if (!resp.hasError()) {
                infoDialog.title = qsTr("User Deleted")
                infoDialogText.text = qsTr("Your user account has been annihilated.")
                infoDialog.open()
            } else {
                errorDialogText.text = resp.httpStatusReason()
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
            wrapMode: Text.WordWrap
        }
    }

    Connections {
        target: fpCore
        onAuthStateChanged: {
            updateAuthenticationDisplay()
        }
    }

    function clearUserProfileDisplay()
    {
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
            busyIndicator.visible = true
            var userProfileResp = fpCore.getUserProfile()
            userProfileResp.onFinished.connect(function() {
                busyIndicator.visible = false
                if (!userProfileResp.hasError()) {
                    var userDetailsDat = fpType.makeJsonUserDetails(userProfileResp.payload())
                    userDetails.userEmailText = userDetailsDat.email
                    userDetails.userDisplayNameText = userDetailsDat.displayName
                    userDetails.userLevelText = userDetailsDat.level
                } else {
                    clearUserProfileDisplay()
                    errorDialogText.text = userProfileResp.httpStatusReason()
                    errorDialog.open()
                }
            })
        }
    }
}
