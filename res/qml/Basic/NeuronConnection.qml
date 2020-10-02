import QtQuick 2.0
import QtQuick.Shapes 1.14
import "../../js/VisuFunction.js" as VisuFunction

Shape {
    id:con
    property int conId: 0
    property int netId: 0
    property real weight:1
    property int conWidth: 4
    property real minWidth: 0.2
    property real maxWidth: 5*d
    property real d: 50
    property int transparent: 100
    property int connType: 0

    property variant start: Qt.point(0,0)
    property variant end: Qt.point(100,400)
    property alias sourceData: source
    property alias destinationData: destination
    NeuronData{
        id:destination
        netId:con.netId
        typeId:0
        type:netListVisu.getConDestinationType(con.netId,con.conId,con.connType)
    }
    NeuronData{
        id:source
        netId:con.netId
        typeId:0
        type:netListVisu.getConSourceType(con.netId,con.conId,con.connType)
    }

    ShapePath {
           strokeColor: if(netItem.visuNeuronModus===def.functionVisu) return VisuFunction.color(transparent,"#daa520")
                        else if(weight<0) return VisuFunction.color(transparent,"ff0000")
                        else if(weight>0) return VisuFunction.color(transparent,"008000")
                        else return "white"

           strokeWidth:if(netItem.visuNeuronModus===def.functionVisu) return con.d*0.1
                        else if(Math.abs(weight)*con.d*0.1>maxWidth) return maxWidth
                        else return Math.abs(weight)*con.d*0.1

           startX:con.start.x
           startY:con.start.y
           PathLine {
               id:path
               x: con.end.x
               y: con.end.y
           }
         }
    function updateValue(){
        weight=netListVisu.getConWeight(netId,conId)
    }
    function updateNeurons(){
        setDesinationId()
        setSourceId()
    }

    function setDesinationId(){
                    var destinationId=netListVisu.getConDestinationID(con.netId,con.conId,con.connType)
                    if(destination.type==def.outputType){
                        destination.typeId= destinationId-(netListVisu.getHiddenX(netId)*netListVisu.getHiddenY(netId))
                    }
                    else{
                        destination.typeId=destinationId
                    }
                    }
    function setSourceId(){
        if(source.type==def.biasType){
               source.typeId= Math.floor(destination.neuronID/netListVisu.getHiddenX(netId))
        }
        else{
            source.typeId=netListVisu.getConSourceID(con.netId,con.conId,con.connType)
        }
        }
}
