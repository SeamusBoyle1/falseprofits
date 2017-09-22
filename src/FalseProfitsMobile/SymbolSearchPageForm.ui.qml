import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane {
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

            TextField {
                id: queryString
                Layout.fillWidth: true
                rightPadding: searchButton.implicitWidth + 6
                inputMethodHints: Qt.ImhUppercaseOnly
                selectByMouse: true
                placeholderText: qsTr("Symbol/Company")

                Button {
                    id: searchButton
                    anchors.right: queryString.right
                    text: qsTr("Search")
                    flat: true
                }
            }
        }

        SymbolSearchListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollIndicator.vertical: ScrollIndicator {
            }

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