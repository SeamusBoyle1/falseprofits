import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import com.example.fpx 1.0

Column {
    anchors.fill: parent

    Image {
        width: parent.width
        source: "qrc:/images/logo/AppLogo.png"
        horizontalAlignment: Image.AlignHCenter
        fillMode: Image.Pad
    }

    Label {
        width: parent.width
        padding: 10
        text: "FalseProfits is a stockmarket simulation game using data collected " +
              "from the Yahoo Stock API. Created by students of the " +
              "<a href=\"https://www.rmit.edu.au/\">Royal Melbourne Institute of Technology</a> " +
              "via <a href=\"https://www.open.edu.au/\">Open Universities Australia,</a> as part " +
              "of the 2017 Programming Project."
        wrapMode: Text.WordWrap

        onLinkActivated: { Qt.openUrlExternally(link) }

        // Adds the hoveredLink hand when hovering over urls. Effective on desktop only.
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton // MouseArea does not process clicks
            cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
        }
    }


    Column {
        padding: 10

        Label {
            text: "Development Team"
            font.bold: true
            wrapMode: Text.WordWrap
        }

        Label {
            topPadding: 5
            text: "Seamus Boyle \nPascal Enz \nLeonard Marq \nJoshua Ashby"
        }
    }
}
