pragma Singleton
import QtQuick 2.7

QtObject {
    property int appWindowWidth: 1024
    property int appWindowHeight: 600

    property string iconAccent: "white"
    property string iconPrimary: "black"

    property int passwordEchoMode: TextInput.Password
}
