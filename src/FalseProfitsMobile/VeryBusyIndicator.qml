import QtQuick 2.0
import QtQuick.Controls 2.2

BusyIndicator {
    property int busyIndicatorVisibility: 0

    function incrementVisibility() {
        visible = true
        busyIndicatorVisibility = busyIndicatorVisibility + 1
    }

    function decrementVisibility() {
        busyIndicatorVisibility = busyIndicatorVisibility - 1
        if (busyIndicatorVisibility == 0) {
            visible = false
        }
    }
}
