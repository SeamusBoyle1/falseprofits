import QtQuick 2.9
import QtQuick.Controls 2.2

ButtonGroup {
    id: group
    signal clicked2(AbstractButton button)
    onClicked: {
        // Workaround: For ".onClicked" is not available due to component versioning.
        group.clicked2(button)
    }
}
