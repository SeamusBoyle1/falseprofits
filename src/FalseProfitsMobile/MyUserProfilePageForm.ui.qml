import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: page
    width: 400
    height: 400
    property alias getUserDetailsButton: getUserDetailsButton
    property alias userDetails: userDetails
    property alias busyIndicator: busyIndicator

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: mlay.width
        contentHeight: mlay.height
        clip: true

        ColumnLayout {
            id: mlay
            width: page.width

            Button {
                id: getUserDetailsButton
                text: qsTr("Get my details")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            GroupBox {
                title: qsTr("Your user details")
                Layout.fillWidth: true

                MyUserProfile {
                    id: userDetails
                    anchors.fill: parent
                }
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
