pragma Singleton
import QtQuick 2.7

QtObject {
    property int appWindowWidth: 360
    property int appWindowHeight: 520

    property string iconAccent: "white"
    property string iconPrimary: "black"

    property int passwordEchoMode: TextInput.PasswordEchoOnEdit
}
