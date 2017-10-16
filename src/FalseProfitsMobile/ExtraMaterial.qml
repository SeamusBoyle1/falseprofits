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
}
