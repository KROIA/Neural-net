import QtQuick 2.0
import QtQuick.Controls 2.4
Item {
    width: 100
    height: 100
    property real buttonBar: 0.1
    property int maxNet: netListVisu.getNetCount()
    property int netId: spinNetId.value
    Connections {
                   target: netListVisu
                   onStopUpdateSignal: timerSingelNet.running=false
                   onStartUpdateSignal:timerSingelNet.running=true
    }
    Timer {
            id:timerSingelNet
            interval: updateTime; running: false; repeat: true
            onTriggered: updateMultiNet()
        }
    Net{
        id:net
        x:0
        y:0
        width:parent.width
        height: parent.height*(1-buttonBar)
        netID:  parent.netId
    }
    SpinBox{
        id:spinNetId
        from: 0
        to: maxNet-1
        editable:true
        inputMethodHints: Qt.ImhDigitsOnly
        font.pixelSize: xAxis.height*0.4
        value:0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.top: net.bottom
    }
    function updateMultiNet(){
               maxNet= netListVisu.getNetCount()
            }
}
