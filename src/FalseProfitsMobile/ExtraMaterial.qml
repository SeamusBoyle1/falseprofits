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

    // https://material.io/guidelines/style/color.html#color-usability
    // Dark text on light backgrounds
    // - most important text [primary] 87%
    // - secondary text 54%
    // - text hints and disabled text 38%
    //
    // Light text on dark backgrounds
    // - most important text [primary] 100%
    // - secondary text 70%
    // - text hints and disabled text 50%
    property color primaryTextColor: Qt.rgba(0, 0, 0, .87)
    property color hintTextColor: Qt.rgba(0, 0, 0, .38)

    property double secondaryTextOpacity: .54

    property color errorHintTextColor: Material.color(Material.Red, Material.Shade300)
}
