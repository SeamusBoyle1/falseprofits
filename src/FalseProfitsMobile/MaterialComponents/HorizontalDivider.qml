import QtQuick 2.0
import QtQuick.Controls.Material 2.2

import io.material.xtra 1.0

Item {
    height: 1
    anchors.left: parent.left
    anchors.right: parent.right

    Rectangle {
        width: parent.width
        height: parent.height
        color: ExtraMaterial.dividerColor
        opacity: ExtraMaterial.dividerOpacity
    }
}
