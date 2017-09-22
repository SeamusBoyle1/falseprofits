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
                text: navPan.depth > 1 ? qsTr("‹") : qsTr("...")
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
        }
    }

    header: toolBarH

    StackView {
        id: navPan
        anchors.fill: parent

        Loader {
            id: symbolSearchPageLoader
            active: true
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
        // TODO(seamus): If loader is already active reuse item
        companyInfoLoader.active = false
        companyInfoLoader.tickerSymbol = ticker
        companyInfoLoader.active = true
    }

    function showOrderTicket(ticker)
    {
        // TODO(seamus): If loader is already active reuse item
        orderTicketLoader.active = false
        orderTicketLoader.tickerSymbol = ticker
        orderTicketLoader.active = true
    }
}
