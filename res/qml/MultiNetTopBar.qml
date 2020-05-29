import QtQuick 2.14
import QtQuick.Controls 2.4


Rectangle {
    id:itemTopBar
    width: 100
    height: 100
    color: "lightgrey"
    property int netXAxis: xAxisItem.value
    property int netYAxis: yAxisItem.value
    property int updateTimer: timerItem.value
    property int totalNet: 0
    property int tabId: tabItem.value
    property int fontSize: itemTopBar.height*0.2
    property int spinBoxSize: 150//if(width*0.2<150) return width*0.2
                               // else return 150
    Flow {
        anchors.fill:parent
        TopbarSpinBox{
            id:xAxisItem
            height: itemTopBar*0.8
            width:spinBoxSize*2
            text: "max nets\nrow  "
            from: 1
            to: 10
            spinBoxWidth: spinBoxSize
        }
        TopbarSpinBox{
            id:yAxisItem
            height: itemTopBar*0.8
            width:spinBoxSize*2
            text: "max nets\ncolumn  "
            from: 1
            to: 10
            spinBoxWidth: spinBoxSize
        }
        TopbarSpinBox{
            id:timerItem
            height: itemTopBar*0.8
            width:spinBoxSize*2.5
            text: "update\nTimer  "
            from: 10
            to: 10000
            defaultValue: 200
            spinBoxWidth: spinBoxSize*1.5
        }
        TopbarSpinBox{
            id:tabItem
            height: itemTopBar*0.8
            width:spinBoxSize*2
            text: "side:  "
            from: 0
            to: Math.floor(totalNet/(yAxisItem.value*xAxisItem.value))
            spinBoxWidth: spinBoxSize
        }
    }

    Connections {
                   target: netListVisu
                   onStartUpdateSignal:updateMultiNet()
    }
    function updateMultiNet(){
               totalNet= netListVisu.getNetCount()
            }
}
