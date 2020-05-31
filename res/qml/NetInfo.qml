import QtQuick 2.0

Item {
    property int netID: value
    property string netStatus: "running"
    property int updateTime:100
    property int hiddenNeuronX: netListVisu.getHiddenX(netID)
    property int hiddenNeuronY: netListVisu.getHiddenY(netID)
    property int inputNeuron: netListVisu.getInputs(netID)
    property int outputNeuron: netListVisu.getOutputs(netID)
    property bool bias: netListVisu.getBias(netID)
    property real biasValue: netListVisu.getBiasValue(netID)

    Connections {
                   target: netListVisu
                   onStopUpdateSignal: netStatus="stop"
                   onStartUpdateSignal:{
                       netStatus="running"
                       updateStructur()}
                   onSetUpdateTimeSignal:updateTime=time
    }
    Column{
        anchors.leftMargin: 10
        anchors.fill: parent
        Text{
            text: "Net ID: "+netID+"\n"
            font.pixelSize: 20
        }

        Grid{
            anchors.topMargin: 20
            spacing:5
            columns:2
            Text {text: "status "}
            Text{
                text: netStatus
                color: if(netStatus==="running") return "green"
                       else if(netStatus==="stop")return "red"
                }
            Text {text: "Bias "}
            Text {text: bias}
            Text {text: "BiasValue "; visible: bias}
            Text {text: biasValue; visible: bias}
            Text {text: "\nInputs "}
            Text {text: inputNeuron}
            Text {text: "Outputs "}
            Text {text: outputNeuron}
            Text {text: "HiddenX "}
            Text {text: hiddenNeuronX}
            Text {text: "HiddenY "}
            Text {text: hiddenNeuronX}

        }
    }

    function updateStructur(){
        //console.debug("update Struc")
        inputNeuron=netListVisu.getInputs(netID)
        outputNeuron=netListVisu.getOutputs(netID)
        hiddenNeuronX=netListVisu.getHiddenX(netID)
        hiddenNeuronY=netListVisu.getHiddenY(netID)

        hiddenIDs=netListVisu.getHiddenID(netID)
        outputIds=netListVisu.getOutputID(netID)

        bias=netListVisu.getBias(netID)
    }
}
