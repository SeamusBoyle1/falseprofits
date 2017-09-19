import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    SignOutPage {
        anchors.fill: parent
    }

    header: ToolBar {
        RowLayout {
            ToolButton {
                text: qsTr("...")
                onClicked: {
                    appDrawer.open()
                }
            }
            Label {
                text: qsTr("Sign Out")
                elide: Label.ElideRight
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
                font.pixelSize: 16
                font.bold: true
            }
        }
    }
}
