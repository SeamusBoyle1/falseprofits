import QtQuick 2.4
import QtQuick.Controls 2.2

import com.example.fpx 1.0

DetailedQuotePageForm {
    signal tradeButtonClicked(string symbol)

    property string currentSymbol
    property int busyIndicatorVisibility: 0

    orderButton.onClicked: {
        if (currentSymbol !== "") {
            tradeButtonClicked(currentSymbol)
        }
    }

    onCurrentSymbolChanged: {
        clearQuote()
        symbolText = currentSymbol
        updateQuote()
    }

    Dialog {
        id: errorDialog
        parent: appNavStack
        title: qsTr("Error")
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        implicitHeight: 160
        implicitWidth: parent.width * 0.9

        Label {
            id: errorDialogText
            wrapMode: Text.WordWrap
        }
    }

    function clearQuote() {
        companyNameText = ""
        industryText = ""
        lastPriceLabel.text = ""
    }

    function updateQuote() {
        incrementBusyIndicatorVisibility()
        var quoteResp = fpCore.getQuotes(currentSymbol)
        quoteResp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            if (!quoteResp.hasError()) {
                var quotes = fpType.makeJsonQuotes(quoteResp.payload())
                var quote = quotes.find(currentSymbol)
                if (quote.isValid) {
                    lastPriceLabel.text = quote.last
                }
            } else {
                errorDialogText.text = quoteResp.httpStatusReason()
                errorDialog.open()
            }
        })

        incrementBusyIndicatorVisibility()
        var detailsResp = fpCore.getShareDetails(currentSymbol)
        detailsResp.onFinished.connect(function() {
            decrementBusyIndicatorVisibility()
            if (!detailsResp.hasError()) {
                companyNameText = detailsResp.companyName
                industryText = detailsResp.industry
            } else {
                errorDialogText.text = detailsResp.httpStatusReason()
                errorDialog.open()
            }
        })
    }

    function incrementBusyIndicatorVisibility() {
        busyIndicator.visible = true
        busyIndicatorVisibility = busyIndicatorVisibility + 1
    }

    function decrementBusyIndicatorVisibility() {
        busyIndicatorVisibility = busyIndicatorVisibility - 1
        if (busyIndicatorVisibility == 0) {
            busyIndicator.visible = false
        }
    }
}
