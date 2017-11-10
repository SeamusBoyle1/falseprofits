import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import io.material.xtra 1.0

Page {
    width: 400
    height: 400
    property bool zeroSearchResults: false
    property alias busyIndicator: busyIndicator
    property alias listView: listView
    property alias queryString: queryString
    property alias searchButton: searchButton

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: 8
            Layout.leftMargin: FpStyle.screenEdgeLeftMargin
            Layout.rightMargin: FpStyle.screenEdgeRightMargin

            TextField {
                id: queryString
                Layout.fillWidth: true
                rightPadding: searchButton.implicitWidth + 6
                inputMethodHints: Qt.ImhUppercaseOnly
                selectByMouse: true
                placeholderText: qsTr("Symbol/Company")

                Button {
                    id: searchButton
                    anchors.bottom: parent.bottom
                    anchors.right: queryString.right
                    contentItem: Image {
                        fillMode: Image.Pad
                        horizontalAlignment: Image.AlignHCenter
                        verticalAlignment: Image.AlignVCenter
                        source: "qrc:/images/" + FpStyle.iconPrimary + "/search.png"
                        opacity: parent.enabled ? ExtraMaterial.activeIconOnPrimaryOpacity : ExtraMaterial.inactiveIconOnPrimaryOpacity
                    }
                    flat: true
                }
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 300
            Layout.minimumWidth: 300
            Layout.preferredWidth: 600
            contentWidth: listView.width

            SymbolSearchListView {
                id: listView

                BusyIndicator {
                    id: busyIndicator
                    visible: false
                    anchors.centerIn: parent
                }

                Label {
                    text: qsTr("No companies")
                    visible: !busyIndicator.visible && zeroSearchResults
                    anchors.centerIn: parent
                }
            }
        }
    }
}
