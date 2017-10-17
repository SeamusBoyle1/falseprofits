pragma Singleton
import QtQuick 2.0
import QtQuick.Controls.Material 2.2

Item {
    // Workaround: No other way to access the predefined colors
    // used by Material.accent and Material.primary
    property var primaryColor: Material.Indigo
    property var accentColor: Material.Pink

    property var lighterAccentColor: Material.color(accentColor, Material.ShadeA200)
    property color textOnLighterAccentColor: "#fff"

    // https://material.io/guidelines/components/dividers.html
    // 12% black for light theme or 12% white for dark theme
    property color dividerColor: Qt.rgba(0, 0, 0, 1)
    property double dividerOpacity: .12
}
