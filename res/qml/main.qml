import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
Window {
    id:window
    width: 800
    height: 600
    visible: true/*
    MultiNet{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        xNetPerTab: 3
        yNetPerTab: 3
        width: window.width*0.9
        height: window.height*0.9
    }*/
    Net{
        anchors.fill: parent
        netID: 0
    }
    Connections {
                     target: netVisu

                     }
}
