import QtQuick 2.4

import com.example.fpx 1.0

MyUserProfilePageForm {

    getUserDetailsButton.onClicked: {
        if (fpCore.authState === Fpx.AuthenticatedState) {
            busyIndicator.visible = true
            var userProfileResp = fpCore.getUserProfile()
            userProfileResp.onFinished.connect(function() {
                busyIndicator.visible = false
                if (!userProfileResp.hasError()) {
                    var userDetailsDat = fpType.makeJsonUserDetails(userProfileResp.payload())
                    userDetails.userIdText = userDetailsDat.id
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

    Connections {
        target: fpCore
        onAuthStateChanged: {
            updateAuthenticationDisplay()
        }
    }

    function clearUserProfileDisplay()
    {
        userDetails.userIdText = ""
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
}
