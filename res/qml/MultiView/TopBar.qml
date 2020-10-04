import QtQuick 2.14
import QtQuick.Controls 2.4
import "../BasicTemplate" as Template

Template.TopBar {
    id: topBar

    property int totalNet: 0

    Row{
        spacing: 10
        padding:10
        anchors.top: topBar.top
        anchors.bottom: topBar.bottom

        ComboBox {
            id:modus
            model: ["Value", "Function", "Combined"]
            anchors.verticalCenter: parent.verticalCenter
            currentIndex: generalVariable.visuNeuronModus
            onCurrentIndexChanged: {
                generalVariable.visuNeuronModus=currentIndex
            }
        }
        ComboBox {
            id:updateTimer
            model: [ "interval","real time", "pause"]
            anchors.verticalCenter: parent.verticalCenter
            currentIndex: generalVariable.updateModus
            onCurrentIndexChanged: {
                generalVariable.updateModus=currentIndex
            }
        }
        SpinBox{
            id:updateIntervalSpinBox
            anchors.verticalCenter: parent.verticalCenter
            editable :true
            visible: generalVariable.updateModus==def.interval
            from: 0
            to: 10000
            value:generalVariable.updateInterval
            onValueChanged: {
                generalVariable.updateInterval=value
            }
        }
        Label{
            text: "ms"
            font.pixelSize:  15
            verticalAlignment:Text.AlignVCenter
            visible: generalVariable.updateModus==def.interval
            anchors.verticalCenter: parent.verticalCenter
        }

        Label{
            text: "rows "
            font.pixelSize:  15
            verticalAlignment:Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        SpinBox{
            id:xAxisItem
            editable :true
            from: 1
            to: 10
            anchors.verticalCenter: parent.verticalCenter
            value:generalVariable.netXAxis
            onValueChanged: {
                updateMultiNet()
                generalVariable.netXAxis=value
            }
        }
        Label{
            text: "columns "
            font.pixelSize:  15
            verticalAlignment:Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        SpinBox{
            id:yAxisItem
            editable :true
            from: 1
            to: 10
            value:generalVariable.netYAxis
            anchors.verticalCenter: parent.verticalCenter
            onValueChanged: {
                updateMultiNet()
                generalVariable.netYAxis=value
            }
        }
        Label{
            text: "rows "
            font.pixelSize:  15
            verticalAlignment:Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        SpinBox{
            id:tabItem
            editable :true
            from: 0
            to: Math.floor(generalVariable.totalNet/(yAxisItem.value*xAxisItem.value))
            anchors.verticalCenter: parent.verticalCenter
            onValueChanged: {
                updateMultiNet()
                generalVariable.tabId=value
            }
        }
    }
        /*
    Row {
        anchors.fill:parent
        Template.TopbarSpinBox{
            id:
            height: itemTopBar*0.8
            width:spinBoxSize*2
            text:
            from: 1
            to: 10
            spinBoxWidth: spinBoxSize
            onValueChanged: updateMultiNet()
        }
        Template.TopbarSpinBox{
            id:yAxisItem
            height: itemTopBar*0.8
            width:spinBoxSize*2
            text: "max nets\ncolumn  "
            from: 1
            to: 10
            spinBoxWidth: spinBoxSize
            onValueChanged: updateMultiNet()
        }
        Template.TopbarSpinBox{
            id:timerItem
            height: itemTopBar*0.8
            width:spinBoxSize*2.5
            text: "update\nTimer  "
            from: 10
            to: 10000
            defaultValue: 200
            spinBoxWidth: spinBoxSize*1.5
        }
        Template.TopbarSpinBox{
            id:tabItem
            height: itemTopBar*0.8
            width:spinBoxSize*2
            text: "side:  "
            from: 0
            to: 100//Math.floor(itemTopBar.totalNet/(yAxisItem.value*xAxisItem.value))
            spinBoxWidth: spinBoxSize
        }
    }*/

    Connections {
                   target: netListVisu
                   function onStartUpdateSignal(){

                       updateMultiNet()}
    }
    function updateMultiNet(){
               totalNet= netListVisu.getNetCount()
            }
}
