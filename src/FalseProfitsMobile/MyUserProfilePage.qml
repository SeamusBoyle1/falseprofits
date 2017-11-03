import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

MyUserProfilePageForm {
    FpTradingAccounts {
        id: myTradingAccounts
        coreClient: fpCore
    }

    Connections {
        target: fpCore
        onAuthStateChanged: {
            reloadUserProfile()

            if (fpCore.authState === Fpx.AuthenticatedState) {
                updateAccounts()
            }
        }
    }

    Component.onCompleted: {
        reloadUserProfile()

        if (fpCore.authState === Fpx.AuthenticatedState) {
            updateAccounts()
        }
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

        busyIndicator.incrementVisibility()
        var resp = fpCore.editUserProfile(newProfile)
        resp.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
            if (!resp.hasError()) {
                reloadUserProfile()
            } else {
                errorDialogText.text = resp.errorMessage()
                errorDialog.open()
            }
        })
    }

    function resetMyAccount() {
        // Only handling resetting the first account
        // since all users only have one account
        var accountId = myTradingAccounts.model.getAccountId(0)
        if (accountId === "")
            return;

        busyIndicator.incrementVisibility()
        var resp = fpCore.resetAccount(accountId)
        resp.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
            if (!resp.hasError()) {
                infoDialog.title = qsTr("Account Reset")
                infoDialogText.text = qsTr("Your trading account has been reset, enjoy.")
                infoDialog.open()
            } else {
                errorDialogText.text = resp.errorMessage()
                errorDialog.open()
            }
        })
    }

    function deleteMyAccount() {
        busyIndicator.incrementVisibility()
        var resp = fpCore.deleteUser()
        resp.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
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
            busyIndicator.incrementVisibility()
            var userProfileResp = fpCore.getUserProfile()
            userProfileResp.onFinished.connect(function() {
                busyIndicator.decrementVisibility()
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

    function updateAccounts() {
        var notifier = myTradingAccounts.updateAccounts()
        busyIndicator.incrementVisibility()
        notifier.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
        })
    }
}
