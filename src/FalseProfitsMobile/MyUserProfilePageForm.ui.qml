import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

Page {
    id: page
    width: 400
    height: 400
    property string originalDisplayName
    property string originalEmail
    property alias cancelButton: cancelButton
    property alias saveButton: saveButton
    property alias headlineGreeting: headlineGreeting
    property alias userDetails: userDetails
    property alias busyIndicator: busyIndicator
    property bool _inDesktopMode: FpStyle.selector === "desktop"

    ScrollView {
        id: flickable
        anchors.fill: parent
        contentWidth: mlay.width
        contentHeight: mlay.height
        clip: true

        ColumnLayout {
            id: mlay
            width: page.width

            ColumnLayout {
                spacing: 0
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.maximumWidth: _inDesktopMode ? 480 : parent.width

                Label {
                    id: headlineGreeting
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    font.pixelSize: 24
                    Layout.fillWidth: true
                    Layout.topMargin: 32
                    Layout.bottomMargin: 32
                    Layout.leftMargin: FpStyle.screenEdgeLeftMargin
                    Layout.rightMargin: FpStyle.screenEdgeRightMargin
                }

                MyUserProfile {
                    id: userDetails
                    Layout.fillWidth: true
                    Layout.leftMargin: FpStyle.screenEdgeLeftMargin
                    Layout.rightMargin: FpStyle.screenEdgeRightMargin
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    Button {
                        id: cancelButton
                        text: qsTr("Cancel")
                        enabled: userDetails.userDisplayNameText != originalDisplayName
                                 || userDetails.userEmailText != originalEmail
                    }

                    Button {
                        id: saveButton
                        text: qsTr("Save changes")
                        enabled: (userDetails.userDisplayNameText != originalDisplayName
                                  && userDetails.displayNameAcceptableInput)
                                 || (userDetails.userEmailText != originalEmail
                                     && userDetails.emailAcceptableInput)
                    }
                }
            }
        }
    }

    VeryBusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
