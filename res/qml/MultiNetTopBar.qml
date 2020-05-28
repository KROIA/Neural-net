import QtQuick 2.14
import QtQuick.Controls 2.4


Rectangle {
    id:itemTopBar
    width: 100
    height: 100
    color: "lightgrey"
    property int netXAxis: xAxis.value
    property int netYAxis: yAxis.value
    property int updateTimer: timer.value
    property int totalNet: 0
    property int tabId: tab.value
    Flow {
        anchors.fill:parent
        Text {
            leftPadding: itemTopBar.width*0.02
            text: "max nets\n row  "
            font.pixelSize: itemTopBar.height*0.1
            rightPadding: itemTopBar.width*0.02
        }
        SpinBox{
            id:xAxis
            from: 1
            to: 10
            editable:true
            inputMethodHints: Qt.ImhDigitsOnly
            font.pixelSize: xAxis.height*0.3
            value:3
            width:parent.width*0.2

        }
        Text {
            leftPadding: itemTopBar.width*0.02
            text: "max nets\n column  "
            font.pixelSize: itemTopBar.height*0.1
            rightPadding: itemTopBar.width*0.02
        }

        SpinBox{
            id:yAxis
            from: 1
            editable:true
            inputMethodHints: Qt.ImhDigitsOnly
            font.pixelSize: height*0.3
            to: 10
            value:3
            width:parent.width*0.2
        }
        Text {
            leftPadding: itemTopBar.width*0.02
            text: "update\n Timer  "
            font.pixelSize: itemTopBar.height*0.1
            rightPadding: itemTopBar.width*0.02
        }

        SpinBox{
            id:timer
            editable:true
            from: 10
            to: 10000
            inputMethodHints: Qt.ImhDigitsOnly
            font.pixelSize: height*0.3
            stepSize: 50
            value:200
            width:parent.width*0.2
        }
        Text {
            leftPadding: itemTopBar.width*0.02
            text: "side:  "
            font.pixelSize: itemTopBar.height*0.1
            rightPadding: itemTopBar.width*0.02
        }

        SpinBox{
            id:tab
            from: 0
            editable:true
            inputMethodHints: Qt.ImhDigitsOnly
            font.pixelSize: height*0.3
            to: Math.floor(totalNet/(yAxis.value*xAxis.value))
            value:0
            width:parent.width*0.2
        }
    }
    Connections {
                   target: netListVisu
                   onStartUpdateSignal:updateMultiNet()
    }
    function updateMultiNet(){
               totalNet= netListVisu.getNetCount()
                console.debug("new totalNet " +totalNet)

            }
}
