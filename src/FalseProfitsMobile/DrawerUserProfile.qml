import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.2

import com.example.fpx 1.0

Page {
    property var gravatarUrl

    //height: 147
    implicitHeight: mainLay.implicitHeight
    Material.background: Qt.rgba(1, 1, 1, .12)

    Connections {
        target: fpCore
        onAuthStateChanged: {
            if (fpCore.authState === Fpx.AuthenticatedState) {
                reloadUserProfile()
            }
        }
        onUserProfileChanged: {
            if (fpCore.authState === Fpx.AuthenticatedState) {
                reloadUserProfile()
            }
        }
    }

    GridLayout {
        id: mainLay

        columns: 1

        anchors.fill: parent
        anchors.leftMargin: FpStyle.screenEdgeLeftMargin
        anchors.rightMargin: FpStyle.screenEdgeRightMargin

        Image {
            source: gravatarUrl ? gravatarUrl + "&s=" +
                                  Math.ceil(65 * Screen.devicePixelRatio) : ""
            Layout.rowSpan: 2
            Layout.preferredWidth: 65
            Layout.preferredHeight: 65
            fillMode: Image.PreserveAspectCrop
            Layout.topMargin: 16
            Layout.bottomMargin: 16
        }

        Label {
            id: nameLabel
            font.pixelSize: 14
            font.weight: Font.Medium
            Material.foreground: Qt.rgba(0, 0, 0, .87)
        }

        Label {
            id: emailLabel
            font.pixelSize: 14
            font.weight: Font.Normal
            Material.foreground: Qt.rgba(0, 0, 0, .54)
            Layout.bottomMargin: 16
        }
    }

    VeryBusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }

    function clearUserProfileDisplay()
    {
        nameLabel.text = ""
        emailLabel.text = ""
        gravatarUrl = undefined
    }

    function reloadUserProfile()
    {
        busyIndicator.incrementVisibility()
        var userProfileResp = fpCore.getUserProfile()
        clearUserProfileDisplay()
        userProfileResp.onFinished.connect(function() {
            busyIndicator.decrementVisibility()
            if (!userProfileResp.hasError()) {
                var userDetailsDat = fpType.makeJsonUserDetails(userProfileResp.payload())

                nameLabel.text = userDetailsDat.displayName
                emailLabel.text = userDetailsDat.email
                gravatarUrl = userDetailsDat.gravatarUrl
            }
        })
    }
}
