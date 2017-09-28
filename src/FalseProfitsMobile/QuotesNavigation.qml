import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

Page {
    FpSymbolSearchWrapper {
        id: symbolSearchWrapper
        coreClient: fpCore
    }

    ToolBar {
        id: toolBarH
        RowLayout {
            anchors.fill: parent
            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: navPan.depth > 1 ? "qrc:/images/" + FpStyle.iconAccent
                                               + "/arrow_back.png"
                                             : "qrc:/images/" + FpStyle.iconAccent + "/menu.png"
                }
                onClicked: {
                    if (navPan.depth > 1) {
                        navPan.pop()
                        listView.currentIndex = -1
                    } else {
                        appDrawer.open()
                    }
                }
            }
            Label {
                text: qsTr("Quotes")
                elide: Label.ElideRight
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
                font.pixelSize: 16
                font.bold: true
            }
            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/" + FpStyle.iconAccent + "/search.png"
                }
                enabled: visible
                visible: symbolSearchPageLoader.item ?
                             navPan.currentItem !== symbolSearchPageLoader.item : true
                onClicked: showQuoteLookup()
            }
            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/" + FpStyle.iconAccent + "/refresh.png"
                }
                enabled: visible
                visible: watchlistPageLoader.item ?
                             navPan.currentItem === watchlistPageLoader.item : false
                onClicked: watchlistPageLoader.item.refreshWatchlist()
            }
        }
    }

    header: toolBarH

    StackView {
        id: navPan
        anchors.fill: parent

        Loader {
            id: symbolSearchPageLoader
            active: false
            source: "qrc:/SymbolSearchPage.qml"
            anchors.fill: parent
            onLoaded: {
                item.listView.model = symbolSearchWrapper.model
                item.onSymbolClicked.connect(showCompanyInfo)
                if (navPan.currentItem && !(navPan.currentItem == item)) {
                    navPan.push(item)
                } else {
                    navPan.replace(item)
                }
            }
        }

        Loader {
            id: watchlistPageLoader
            active: true
            source: "qrc:/WatchlistPage.qml"
            anchors.fill: parent
            onLoaded: {
                item.onSymbolClicked.connect(showCompanyInfo)
                if (navPan.currentItem && !(navPan.currentItem == item)) {
                    navPan.push(item)
                } else {
                    navPan.replace(item)
                }
            }
        }

        Loader {
            id: companyInfoLoader
            property string tickerSymbol
            active: false
            source: "qrc:/DetailedQuotePage.qml"
            anchors.fill: parent
            onLoaded: {
                item.currentSymbol = tickerSymbol
                item.onTradeButtonClicked.connect(showOrderTicket)
                navPan.push(item)
            }
        }

        Loader {
            id: orderTicketLoader
            property string tickerSymbol
            active: false
            source: "qrc:/OrderTicketPage.qml"
            anchors.fill: parent
            onLoaded: {
                item.currentSymbol = tickerSymbol
                navPan.push(item)
            }
        }
    }

    function showCompanyInfo(ticker)
    {
        if (companyInfoLoader.active) {
            companyInfoLoader.item.currentSymbol = ticker
            var index = companyInfoLoader.item.StackView.index
            if (index >= 0) {
                navPan.pop(companyInfoLoader.item)
            } else {
                navPan.push(companyInfoLoader.item)
            }
        } else {
            companyInfoLoader.tickerSymbol = ticker
            companyInfoLoader.active = true
        }
    }

    function showOrderTicket(ticker)
    {
        if (orderTicketLoader.active) {
            orderTicketLoader.item.currentSymbol = ticker
            var index = orderTicketLoader.item.StackView.index
            if (index >= 0) {
                navPan.pop(orderTicketLoader.item)
            } else {
                navPan.push(orderTicketLoader.item)
            }
        } else {
            orderTicketLoader.tickerSymbol = ticker
            orderTicketLoader.active = true
        }
    }

    function showQuoteLookup()
    {
        if (symbolSearchPageLoader.active) {
            var index = symbolSearchPageLoader.item.StackView.index
            if (index >= 0) {
                navPan.pop(symbolSearchPageLoader.item)
            } else {
                navPan.push(symbolSearchPageLoader.item)
            }
        } else {
            symbolSearchPageLoader.active = true
        }
    }
}
