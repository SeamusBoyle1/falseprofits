import QtQuick 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.1
import com.example.fpx 1.0

// fpCalendarStyle is modifing the calendar navigationBar to add Material Left/Right Arrow icons.
// All other content is purely to enable that function.
CalendarStyle {
    navigationBar: Rectangle {
        color: "#f9f9f9"
        height: dateText.height * 1.8

        ToolButton {
            id: previousMonth
            width: parent.height
            height: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            onClicked: control.showPreviousMonth()

            // ToolButton, in Controls 2.2, does not have an iconSource property, so we use
            // contentItem to include an icon instead.
            contentItem: Image {
                source: "qrc:/images/" + FpStyle.iconPrimary + "/arrow_back.png"
            }
        }
        Label {
            id: dateText
            text: styleData.title
            font.pixelSize: 13
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            fontSizeMode: Text.Fit
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: previousMonth.right
            anchors.leftMargin: 2
            anchors.right: nextMonth.left
            anchors.rightMargin: 2
        }
        ToolButton {
            id: nextMonth
            width: parent.height
            height: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            onClicked: control.showNextMonth()

            // ToolButton, in Controls 2.2, does not have an iconSource property, so we use
            // contentItem to include an icon instead.
            contentItem: Image {
                source: "qrc:/images/" + FpStyle.iconPrimary + "/arrow_forward.png"
            }
        }
    }
}
