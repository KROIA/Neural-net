import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window{
    id: item
    visible: true
    width: 640
    height: 480
    title: "Net Übersicht"
    Connections {
                     target: netVisu
                     onIncreaseOne: txtCount.text =ms
                     }

    Net{
        id: prenet

    }

    /*Text {
    id:txtCount
    text: "0 Hits"
    x:200
    y:200
        }
    Button{
        id: btnUpdate
        text: "Just Click Me"
        onClicked: netVisu.callMeFromQml()
    }*/
}
