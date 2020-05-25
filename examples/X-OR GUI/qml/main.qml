import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
Window {
    id:window
    width: 800
    height: 600
    visible: true
    Net{
        id:net
        anchors.fill: parent
    }

    Button{
        anchors.left: window.left
        anchors.top: window.top
        text: "aktualisieren"
        onClicked:{
            net.updateValue()
        }
    }

    Connections {
                     target: netVisu

                     }
}
