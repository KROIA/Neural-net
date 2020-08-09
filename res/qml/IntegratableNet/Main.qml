import QtQuick 2.0
import QtQuick.Controls 2.4
import "../Basic"
import "../"
Item {
    id:uiIntegratableNet
    width: 100
    height: 100
    property real buttonBar: 0.1
    property int maxNet: netListVisu.getNetCount()
    property int netId: spinnetId.value
    property int updateTime: 10
    Component.onCompleted: updateNet();
    Connections {
                   target: netListVisu
                   function onStartUpdateSignal(){updateNet()}
                   }
    Net{
        id:net
        x:0
        y:0
        width:parent.width
        height: parent.height*(1-buttonBar)
        netId:  parent.netId
        updateTime:uiIntegratableNet.updateTime
    }
    SpinBox{
        id:spinnetId
        from: 0
        to: maxNet-1
        editable:true
        inputMethodHints: Qt.ImhDigitsOnly
        font.pixelSize: height*0.4
        value:0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.top: net.bottom
    }
    function updateNet(){
               maxNet= netListVisu.getNetCount()
            }
    Definition{
        id:def
    }
}
