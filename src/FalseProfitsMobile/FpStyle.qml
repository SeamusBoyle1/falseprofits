pragma Singleton
import QtQuick 2.7

QtObject {
    property string selector: "touch"

    property int appWindowWidth: 360
    property int appWindowHeight: 520

    property int screenEdgeLeftMargin: 16
    property int screenEdgeRightMargin: 16

    property string iconAccent: "white"
    property string iconPrimary: "black"

    property int passwordEchoMode: TextInput.PasswordEchoOnEdit
}
