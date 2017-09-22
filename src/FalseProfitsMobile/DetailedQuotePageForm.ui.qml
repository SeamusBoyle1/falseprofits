import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: page
    width: 400
    height: 400
    property string symbolText
    property string companyNameText
    property string industryText
    property string bidText
    property string askText
    property string dayLowText
    property string dayHighText
    property alias lastPriceLabel: lastPriceLabel
    property alias orderButton: orderButton
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

            RowLayout {
                spacing: 20
                Layout.fillWidth: true
                Layout.leftMargin: 16
                Layout.rightMargin: 16

                ColumnLayout {
                    Layout.fillWidth: true

                    Label {
                        id: symbolLabel
                        text: symbolText
                        font.pixelSize: 32
                        Layout.fillWidth: true
                    }

                    Label {
                        id: companyNameLabel
                        text: companyNameText
                        font.pixelSize: 12
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }

                    Label {
                        id: industryLabel
                        text: industryText
                        font.pixelSize: 11
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }
                }

                Button {
                    id: orderButton
                    text: qsTr("Trade")
                    flat: true
                }
            }

            TabBar {
                id: tabBar
                Layout.fillWidth: true
                currentIndex: swipeView.currentIndex
                TabButton {
                    text: qsTr("Summary")
                }
                TabButton {
                    text: qsTr("Key stats")
                }
            }

            SwipeView {
                id: swipeView
                Layout.fillWidth: true
                Layout.minimumHeight: currentItem ? currentItem.implicitHeight : 1
                currentIndex: tabBar.currentIndex

                Page {
                    ColumnLayout {
                        anchors.fill: parent

                        GridLayout {
                            columnSpacing: 20
                            Layout.fillWidth: true
                            Layout.leftMargin: 16
                            Layout.rightMargin: 16
                            columns: 3

                            Label {
                                id: lastPriceLabel
                                Layout.rowSpan: 2
                                font.pixelSize: 32
                            }

                            Label {
                                text: qsTr("Bid")
                                font.capitalization: Font.AllUppercase
                            }
                            Label {
                                text: qsTr("Ask")
                                font.capitalization: Font.AllUppercase
                            }

                            Label {
                                text: bidText
                            }
                            Label {
                                text: askText
                            }
                        }
                    }
                }

                Page {
                    ColumnLayout {
                        anchors.fill: parent

                        GridLayout {
                            rowSpacing: 20
                            Layout.fillWidth: true
                            Layout.leftMargin: 16
                            Layout.rightMargin: 16
                            columns: 2

                            Label {
                                text: qsTr("Day High")
                            }
                            Label {
                                text: dayHighText
                            }

                            Label {
                                text: qsTr("Day Low")
                            }
                            Label {
                                text: dayLowText
                            }

                            Label {
                                text: qsTr("Bid")
                            }
                            Label {
                                text: bidText
                            }

                            Label {
                                text: qsTr("Ask")
                            }
                            Label {
                                text: askText
                            }
                        }
                    }
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
