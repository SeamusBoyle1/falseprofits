import QtQuick 2.7
import QtQuick.XmlListModel 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    property string currentSymbol

    /* newsFeedModel
        Defining XML file of current Symbol to be created as List element.  */
    XmlListModel {
        id: newsFeedModel

        source: "https://feeds.finance.yahoo.com/rss/2.0/headline?s=" + currentSymbol + ".AX"
        query: "/rss/channel/item"

        XmlRole { name: "title"; query: "title/string()" }
        XmlRole { name: "link"; query: "link/string()" }
        XmlRole { name: "pubDate"; query: "pubDate/string()" } // Publish date of article.

        // Removing any links from the description
        XmlRole { name: "description"; query: "fn:replace(description/string(), '\&lt;a href=.*\/a\&gt;', '')" }
    }

    /* Creating list from newsFeedModel specifications, using Repeater. */
    Column {
        id: listView
        anchors.left: parent.left
        anchors.right: parent.right

        clip: true
        Repeater {
            anchors.fill: parent
            model: newsFeedModel

            delegate: Column {
                id: delegate
                width: parent.width - 30 // Width minus padding for either side, creates right-side padding
                padding: 15 // left side padding.
                spacing: 8

                Item { height: 8; width: delegate.width }

                Label {
                    id: titleBox
                    text: title
                    width: delegate.width
                    wrapMode: Text.WordWrap
                    font.pixelSize: 13
                    font.bold: true
                }

                Label {
                    id: pubDateBox
                    text: pubDate + " (<a href=\"" + link + "\">Link</a>)"
                    width: delegate.width
                    wrapMode: Text.WordWrap
                    font.pixelSize: 12
                    font.italic: true

                    onLinkActivated: { Qt.openUrlExternally(link) }
                }

                Label {
                    id: descriptionBox
                    text: description
                    width: parent.width
                    wrapMode: Text.WordWrap
                    font.pixelSize: 12
                    textFormat: Text.StyledText
                }
            }
        }
    }
}

