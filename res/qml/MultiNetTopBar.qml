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
    Flow {
        Text {
            leftPadding: itemTopBar.width*0.02
            text: "max nets\n row  "
            font.pixelSize: itemTopBar.height*0.3
            rightPadding: itemTopBar.width*0.02
        }
        SpinBox{
            id:xAxis
            from: 1
            to: 10
            editable:true
            inputMethodHints: Qt.ImhDigitsOnly
            font.pixelSize: xAxis.height*0.4
            value:3
        }
        Text {
            leftPadding: itemTopBar.width*0.02
            text: "max nets\n column  "
            font.pixelSize: itemTopBar.height*0.3
            rightPadding: itemTopBar.width*0.02
        }

        SpinBox{
            id:yAxis
            from: 1
            editable:true
            inputMethodHints: Qt.ImhDigitsOnly
            font.pixelSize: height*0.4
            to: 10
            value:3
        }
        Text {
            leftPadding: itemTopBar.width*0.02
            text: "update\n Timer  "
            font.pixelSize: itemTopBar.height*0.3
            rightPadding: itemTopBar.width*0.02
        }

        SpinBox{
            id:timer
            editable:true
            from: 10
            to: 10000
            inputMethodHints: Qt.ImhDigitsOnly
            font.pixelSize: height*0.4
            stepSize: 50
            value:200
        }
    }
}
