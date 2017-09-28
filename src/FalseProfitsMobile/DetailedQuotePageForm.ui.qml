import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

Page {
    id: page
    width: 400
    height: 400
    property string symbolText
    property string companyNameText
    property string industryText
    property string bidText
    property string askText
    property int direction: 0
    property string changePriceText
    property string changePercentText
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
                Layout.topMargin: 8
                Layout.leftMargin: FpStyle.screenEdgeLeftMargin
                Layout.rightMargin: FpStyle.screenEdgeRightMargin

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
                    contentItem: Image {
                        fillMode: Image.Pad
                        horizontalAlignment: Image.AlignHCenter
                        verticalAlignment: Image.AlignVCenter
                        source: "qrc:/images/" + FpStyle.iconPrimary + "/gavel.png"
                    }
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

                            RowLayout {
                                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                                Layout.rowSpan: 5

                                Label {
                                    id: changeArrow
                                    text: direction == 1 ? '▲' : direction == -1 ? '▼' : '='
                                    color: direction == 1 ? "#0b893e" : direction
                                                            == -1 ? "#bf1722" : "black"
                                    font.pixelSize: 32
                                }

                                Label {
                                    id: lastPriceLabel
                                    font.pixelSize: 32
                                }
                            }

                            Label {
                                text: qsTr("Daily Change")
                                font.pixelSize: 12
                                font.capitalization: Font.AllUppercase
                                Layout.columnSpan: 2
                            }

                            RowLayout {
                                Layout.columnSpan: 2
                                spacing: 12

                                Label {
                                    text: changePriceText
                                    Layout.fillWidth: true
                                    font.pixelSize: 14
                                }
                                Label {
                                    text: changePercentText
                                    Layout.fillWidth: true
                                    font.pixelSize: 14
                                    horizontalAlignment: Text.AlignRight
                                }
                            }

                            Label {
                                text: qsTr("Bid")
                                font.pixelSize: 12
                                font.capitalization: Font.AllUppercase
                            }
                            Label {
                                text: qsTr("Ask")
                                font.pixelSize: 12
                                font.capitalization: Font.AllUppercase
                            }

                            Label {
                                text: bidText
                                font.pixelSize: 14
                            }
                            Label {
                                text: askText
                                font.pixelSize: 14
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
