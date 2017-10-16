import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import io.material.xtra 1.0

Page {
    width: 400
    height: 400
    property alias userEmailText: userEmailField.text
    property alias userDisplayNameText: userDisplayNameField.text
    property alias userLevelText: userLevelField.text

    GridLayout {
        columnSpacing: 12
        rowSpacing: 20
        anchors.fill: parent
        columns: 1

        TextField {
            id: userEmailField
            readOnly: true
            font.pixelSize: 14
            placeholderText: qsTr("Email address")
            selectByMouse: true
            Layout.fillWidth: true

            Label {
                text: qsTr("Email")
                color: ExtraMaterial.hintTextColor
                font.pixelSize: 12
                visible: parent.text
                anchors.bottom: parent.top
            }
        }

        TextField {
            id: userDisplayNameField
            readOnly: true
            font.pixelSize: 14
            placeholderText: qsTr("Name")
            selectByMouse: true
            Layout.fillWidth: true

            Label {
                text: qsTr("Name")
                color: ExtraMaterial.hintTextColor
                font.pixelSize: 12
                visible: parent.text
                anchors.bottom: parent.top
            }
        }

        TextField {
            id: userLevelField
            readOnly: true
            font.pixelSize: 14
            placeholderText: qsTr("Level")
            selectByMouse: true
            Layout.fillWidth: true

            Label {
                text: qsTr("Level")
                color: ExtraMaterial.hintTextColor
                font.pixelSize: 12
                visible: parent.text
                anchors.bottom: parent.top
            }
        }
    }
}
