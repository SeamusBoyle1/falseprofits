import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0
import io.material.xtra 1.0

Page {
    id: page
    width: 400
    height: 400
    property alias chartFullScreenButton: chartFullScreenButton
    property alias pricePredictionChartButton: pricePredictionChartButton
    property alias rangeButtonGroup: rangeButtonGroup
    property alias priceLineChart: priceLineChart
    property alias dividendHistoryChart: dividendHistoryChart
    property alias starButton: starButton
    property string symbolText
    property string companyNameText
    property string industryText
    property string bidText
    property string bidSizeText
    property string askText
    property string askSizeText
    property int direction: 0
    property string changePriceText
    property string changePercentText
    property string dayLowText
    property string dayHighText
    property bool starred: false
    property bool maybeHasChartData: true
    property alias lastPriceLabel: lastPriceLabel
    property alias orderButton: orderButton
    property alias newsFeedPage: newsFeedPage
    property alias busyIndicator: busyIndicator

    property string fundamental_marketCap
    property string fundamental_volume
    property string fundamental_averageDailyVolume
    property string fundamental_previousClose
    property string fundamental_low52Weeks
    property string fundamental_high52Weeks
    property string fundamental_dividendShare
    property string fundamental_dividendYield
    property string fundamental_exDividendDate
    property string fundamental_dividendPayDate
    property string fundamental_peRatio
    property string fundamental_bookValue
    property string fundamental_priceBook
    property string fundamental_earningsShare
    property string fundamental_movingAverage200Days
    property string fundamental_movingAverage50Days
    property string fundamental_symbol
    property string fundamental_name
    property string fundamental_industry

    ButtonGroup2 {
        id: rangeButtonGroup
    }

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
                        font.pixelSize: 16
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }

                    Label {
                        id: industryLabel
                        text: industryText
                        font.pixelSize: 14
                        opacity: ExtraMaterial.secondaryTextOpacity
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }
                }

                RowLayout {
                    Layout.alignment: Qt.AlignRight | Qt.AlignTop
                    spacing: 0

                    ToolButton {
                        id: orderButton
                        contentItem: Image {
                            fillMode: Image.Pad
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
                            source: "qrc:/images/" + FpStyle.iconPrimary + "/gavel.png"
                            opacity: parent.enabled ? ExtraMaterial.activeIconOnPrimaryOpacity : ExtraMaterial.inactiveIconOnPrimaryOpacity
                        }
                        flat: true
                        hoverEnabled: true

                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Place a buy or sell order")
                    }

                    ToolButton {
                        id: starButton
                        checkable: true
                        checked: starred
                        contentItem: Image {
                            fillMode: Image.Pad
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
                            source: starred ? "qrc:/images/" + FpStyle.iconPrimary
                                              + "/star.png" : "qrc:/images/"
                                              + FpStyle.iconPrimary + "/star_border.png"
                            opacity: parent.enabled ? ExtraMaterial.activeIconOnPrimaryOpacity : ExtraMaterial.inactiveIconOnPrimaryOpacity
                        }
                        flat: true
                        hoverEnabled: true

                        ToolTip.visible: hovered
                        ToolTip.text: starred ? qsTr("Remove from watchlist") : qsTr(
                                                    "Add to watchlist")
                    }
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
                TabButton {
                    text: qsTr("News")
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
                            columns: 4

                            Label {
                                id: lastPriceLabel
                                // Workaround for Light weighted font not displaying
                                // correct values on iOS. The UI design had this label at
                                // size 45sp with Light weight, this workaround reduces the
                                // font size and uses default (probably Normal) weight.
                                // TODO(seams): Change pixelSize back to 45 and font.weight
                                // back to Light when Roboto font is used on iOS.
                                font.pixelSize: 34
                                //font.weight: Font.Light
                            }

                            Label {
                                id: changeArrow
                                text: direction == 1 ? '▲' : direction == -1 ? '▼' : '='
                                color: direction == 1 ? "#0b893e" : direction
                                                        == -1 ? "#bf1722" :
                                                                ExtraMaterial.primaryTextColor
                                font.pixelSize: 32
                                anchors.baseline: lastPriceLabel.baseline
                            }

                            Label {
                                text: changePriceText
                                font.pixelSize: 16
                                font.weight: Font.Medium
                                color: direction == 1 ? "#0b893e" : direction
                                                        == -1 ? "#bf1722" :
                                                                ExtraMaterial.primaryTextColor
                                anchors.baseline: lastPriceLabel.baseline
                            }

                            Label {
                                text: changePercentText
                                font.pixelSize: 16
                                font.weight: Font.Medium
                                color: direction == 1 ? "#0b893e" : direction
                                                        == -1 ? "#bf1722" :
                                                                ExtraMaterial.primaryTextColor
                                horizontalAlignment: Text.AlignRight
                                anchors.baseline: lastPriceLabel.baseline
                            }
                        }

                        ColumnLayout {
                            spacing: 0

                            Flickable {
                                id: flickable2
                                Layout.fillWidth: true
                                Layout.minimumHeight: rangeButtons.implicitHeight
                                contentWidth: rangeButtons.implicitWidth
                                contentHeight: rangeButtons.implicitHeight
                                clip: true

                                RowLayout {
                                    id: rangeButtons
                                    Layout.bottomMargin: 0
                                    spacing: 0

                                    ToolButton {
                                        text: qsTr("1d")
                                        checkable: true
                                        checked: true
                                        ButtonGroup.group: rangeButtonGroup
                                    }
                                    ToolButton {
                                        text: qsTr("5d")
                                        checkable: true
                                        ButtonGroup.group: rangeButtonGroup
                                    }
                                    ToolButton {
                                        text: qsTr("1mo")
                                        checkable: true
                                        ButtonGroup.group: rangeButtonGroup
                                    }
                                    ToolButton {
                                        text: qsTr("6mo")
                                        checkable: true
                                        ButtonGroup.group: rangeButtonGroup
                                    }
                                    ToolButton {
                                        text: qsTr("ytd")
                                        checkable: true
                                        ButtonGroup.group: rangeButtonGroup
                                    }
                                    ToolButton {
                                        text: qsTr("1y")
                                        checkable: true
                                        ButtonGroup.group: rangeButtonGroup
                                    }
                                    ToolButton {
                                        text: qsTr("5y")
                                        checkable: true
                                        ButtonGroup.group: rangeButtonGroup
                                    }
                                    ToolButton {
                                        text: qsTr("max")
                                        checkable: true
                                        ButtonGroup.group: rangeButtonGroup
                                    }

                                    ToolSeparator {
                                    }

                                    ToolButton {
                                        id: chartFullScreenButton
                                        contentItem: Image {
                                            fillMode: Image.Pad
                                            horizontalAlignment: Image.AlignHCenter
                                            verticalAlignment: Image.AlignVCenter
                                            source: "qrc:/images/" + FpStyle.iconPrimary
                                                    + "/fullscreen.png"
                                            opacity: parent.enabled ? ExtraMaterial.activeIconOnPrimaryOpacity : ExtraMaterial.inactiveIconOnPrimaryOpacity
                                        }
                                        flat: true

                                        ToolTip.visible: hovered
                                        ToolTip.text: qsTr("Fullscreen")
                                    }
                                }

                                ScrollIndicator.horizontal: ScrollIndicator {
                                }
                            }

                            FpLineChartWidget {
                                id: priceLineChart
                                Layout.minimumHeight: 260
                                Layout.maximumWidth: parent.width < 1260 ? -1 : 840
                                Layout.fillWidth: true
                                backgroundRoundness: 0
                                lineSeries.visible: maybeHasChartData
                            }

                            Label {
                                text: qsTr("No chart data")
                                visible: !busyIndicator.visible
                                         && !maybeHasChartData
                                anchors.centerIn: priceLineChart
                            }
                        }

                        ToolButton {
                            id: pricePredictionChartButton
                            text: qsTr("Price Prediction")
                            flat: true

                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("View price prediction")
                        }
                    }
                }

                Page {
                    ColumnLayout {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.topMargin: 4

                        GridLayout {
                            rowSpacing: 20
                            Layout.fillWidth: true
                            Layout.leftMargin: 16
                            Layout.rightMargin: 16
                            columns: parent.width < 500 ? 1 : 2
                            columnSpacing: 32

                            // Unused fundamental fields
                            //Label { text: fundamental_bookValue }
                            //Label { text: fundamental_symbol }
                            //Label { text: fundamental_name }
                            //Label { text: fundamental_industry }
                            GridLayout {
                                columns: 2

                                Label {
                                    text: qsTr("Trading")
                                    font.pixelSize: 16
                                    font.weight: Font.Medium
                                    Layout.columnSpan: 2
                                    Layout.bottomMargin: 8
                                }

                                Label {
                                    text: qsTr("Previous Close")
                                }
                                Label {
                                    text: fundamental_previousClose
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("Day's Range")
                                }
                                Label {
                                    text: dayLowText + " - " + dayHighText
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("Volume")
                                }
                                Label {
                                    text: fundamental_volume
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("Bid")
                                }
                                Label {
                                    text: bidText + " x " + bidSizeText
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("Ask")
                                }
                                Label {
                                    text: askText + " x " + askSizeText
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                            }

                            GridLayout {
                                columns: 2
                                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

                                Label {
                                    text: qsTr("Valuation")
                                    font.pixelSize: 16
                                    font.weight: Font.Medium
                                    Layout.columnSpan: 2
                                    Layout.bottomMargin: 8
                                }

                                Label {
                                    text: qsTr("Market Cap")
                                }
                                Label {
                                    text: fundamental_marketCap
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("P/E Ratio")
                                }
                                Label {
                                    text: fundamental_peRatio
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("Price/Book")
                                }
                                Label {
                                    text: fundamental_priceBook
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("Earnings/Share")
                                }
                                Label {
                                    text: fundamental_earningsShare
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                            }

                            GridLayout {
                                columns: 2

                                Label {
                                    text: qsTr("Stock Market History")
                                    font.pixelSize: 16
                                    font.weight: Font.Medium
                                    Layout.columnSpan: 2
                                    Layout.bottomMargin: 8
                                }

                                Label {
                                    text: qsTr("52 Week Range")
                                }
                                Label {
                                    text: fundamental_low52Weeks + " - " + fundamental_high52Weeks
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("50-Day Moving Average")
                                }
                                Label {
                                    text: fundamental_movingAverage50Days
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("200-Day Moving Average")
                                }
                                Label {
                                    text: fundamental_movingAverage200Days
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("Average Daily Volume")
                                }
                                Label {
                                    text: fundamental_averageDailyVolume
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                            }

                            GridLayout {
                                columns: 2

                                Label {
                                    text: qsTr("Dividends")
                                    font.pixelSize: 16
                                    font.weight: Font.Medium
                                    Layout.columnSpan: 2
                                    Layout.bottomMargin: 8
                                }

                                Label {
                                    text: qsTr("Dividend Share")
                                }
                                Label {
                                    text: fundamental_dividendShare
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("Dividend Yield")
                                }
                                Label {
                                    text: fundamental_dividendYield
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("Ex-Dividend Date")
                                }
                                Label {
                                    text: fundamental_exDividendDate
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Label {
                                    text: qsTr("Dividend Pay Date")
                                }
                                Label {
                                    text: fundamental_dividendPayDate
                                    Layout.fillWidth: true
                                    horizontalAlignment: Text.AlignRight
                                }
                            }
                        }

                        FpDividendsColumnChartWidget {
                            id: dividendHistoryChart
                            Layout.minimumHeight: 260
                            Layout.maximumWidth: parent.width < 1260 ? -1 : 840
                            Layout.fillWidth: true
                            Layout.bottomMargin: 38
                            backgroundRoundness: 0
                        }
                    }
                }

                SymbolNewsFeedPage {
                    id: newsFeedPage
                    enableReloadTimer: SwipeView.isCurrentItem
                    enableRelativeTimeUpdates: SwipeView.isCurrentItem
                }
            }
        }

        ScrollIndicator.vertical: ScrollIndicator {
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }
}
