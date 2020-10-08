import QtQuick 2.0
import QtQuick.Controls 1.4
import "../Basic"
import "../"
NetData {

    id: netData
    property string netStatus: "stop"
    property variant conDest:getConDestination(clickedNeuronID,clickedNeuronType)
    property variant conSource:getConSource(clickedNeuronID,clickedNeuronType)
    property int fontSize:10

    ScrollView {
        horizontalScrollBarPolicy :Qt.ScrollBarAlwaysOff
        verticalScrollBarPolicy :Qt.ScrollBarAsNeeded
        anchors.fill:parent
        Column{
            x:15
            Text{
                text: "Net ID: "+netId+"\n"
                font.pointSize: 20
            }

            Grid{
                anchors.topMargin: 20
                spacing:15
                columns:2
                Text {font.pointSize: fontSize;text: "status "}
                Text{
                    text: netStatus
                    color: if(netStatus==="running") return "green"
                           else if(netStatus==="stop")return "red"
                    }
                Text {font.pointSize: fontSize;text: "Bias "}
                Text {font.pointSize: fontSize;text: bias}
                Text {font.pointSize: fontSize;text: "BiasValue "; visible: bias}
                Text {font.pointSize: fontSize;text: biasValue; visible: bias}
                Text {font.pointSize: fontSize;text: "\nInputs "}
                Text {font.pointSize: fontSize;text: "\n"+inputNeuron}
                Text {font.pointSize: fontSize;text: "Outputs "}
                Text {font.pointSize: fontSize;text: outputNeuron}
                Text {font.pointSize: fontSize;text: "HiddenX "}
                Text {font.pointSize: fontSize;text: hiddenNeuronX}
                Text {font.pointSize: fontSize;text: "HiddenY "}
                Text {font.pointSize: fontSize;text: hiddenNeuronY}


            }
            Column{
                anchors.leftMargin: 10
                visible:clickedNeuronID!=-1
                Text {font.pointSize: fontSize;text: "\nNeuron Data";font.bold:true}
                Grid{
                    spacing:5
                    columns:2
                    Text {font.pointSize: fontSize;text: "Neuron Id "}
                    Text {font.pointSize: fontSize;text: clickedNeuronType===def.outputType ?clickedNeuronID:clickedNeuronID+totalHidden}
                    Text {font.pointSize: fontSize;text: "Neuron Type "}
                    Text {font.pointSize: fontSize;text: getTypeString(clickedNeuronType)}
                }
                Text {font.pointSize: fontSize;text: "\nConnection Data";font.bold:true}
                ConnectionList{
                    conArr: conDest
                    conArrID: conSourceID
                    conArrType: conSourceType
                    titel: "From"
                    fontSize: netData.fontSize
                }
                ConnectionList{
                    conArr: conSource
                    conArrID: conDestinationID
                    conArrType: conDestinationType
                    titel: "To"
                    fontSize: netData.fontSize
                }
            }
        }
    }
}

