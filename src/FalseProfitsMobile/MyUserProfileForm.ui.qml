import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import io.material.xtra 1.0

Page {
    width: 400
    height: 400
    property alias displayNameAcceptableInput: userDisplayNameField.acceptableInput
    property alias emailAcceptableInput: userEmailField.acceptableInput
    property alias userEmailText: userEmailField.text
    property alias userDisplayNameText: userDisplayNameField.text
    property alias userLevelText: userLevelField.text

    GridLayout {
        columnSpacing: 12
        rowSpacing: 37
        anchors.fill: parent
        columns: 1

        TextField {
            id: userEmailField
            font.pixelSize: 14
            placeholderText: qsTr("Email address")
            selectByMouse: true
            maximumLength: 100
            validator: RegExpValidator {
                // RegExp source: https://stackoverflow.com/a/16148388
                regExp: /\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/
            }
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                              | Qt.ImhEmailCharactersOnly
            Layout.fillWidth: true

            Label {
                text: qsTr("Email")
                color: userEmailField.acceptableInput ? ExtraMaterial.hintTextColor : ExtraMaterial.errorHintTextColor
                font.pixelSize: 12
                visible: parent.text
                anchors.bottom: parent.top
            }

            Label {
                text: qsTr("Email must be valid and not already used")
                color: ExtraMaterial.errorHintTextColor
                font.pixelSize: 12
                wrapMode: Text.WordWrap
                visible: parent.text.length > 0 && !parent.acceptableInput
                anchors.top: parent.bottom
            }
        }

        TextField {
            id: userDisplayNameField
            font.pixelSize: 14
            placeholderText: qsTr("Name")
            selectByMouse: true
            validator: RegExpValidator {
                regExp: /^.{5,30}$/
            }
            Layout.fillWidth: true

            Label {
                text: qsTr("Name")
                color: userDisplayNameField.acceptableInput ? ExtraMaterial.hintTextColor : ExtraMaterial.errorHintTextColor
                font.pixelSize: 12
                visible: parent.text
                anchors.bottom: parent.top
            }

            Label {
                text: qsTr("Name must be between 5 and 30 characters")
                color: ExtraMaterial.errorHintTextColor
                font.pixelSize: 12
                wrapMode: Text.WordWrap
                visible: parent.text.length > 0 && !parent.acceptableInput
                anchors.top: parent.bottom
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
