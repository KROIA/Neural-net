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
    property real maxWidth: con.d*0.3

    property int connType: 0
    property int destinationId: netListVisu.getConDestinationID(con.netId,con.conId,con.connType)
    property int destinationType: netListVisu.getConDestinationType(con.netId,con.conId,con.connType)
    property int sourceId: netListVisu.getConSourceID(con.netId,con.conId,con.connType)
    property int sourceType:netListVisu.getConSourceType(con.netId,con.conId,con.connType)
    property variant start: Qt.point(0,0)
    property variant end: Qt.point(100,400)
    property alias destinationData: destination
    property alias sourceData: source
    NeuronData{
        id:destination
        netId:con.netId
        typeId:{
            if(destinationType==def.outputType){
                return destinationId-(netListVisu.getHiddenX(netId)*netListVisu.getHiddenY(netId))
            }
            return destinationId
            }
        type:destinationType
    }
    NeuronData{
        id:source
        netId:con.netId
        typeId:{
            if(sourceType==def.biasType){
                   return Math.floor(destinationId/netListVisu.getHiddenX(netId))
            }
            return sourceId
            }
        type:sourceType
    }
    property real d: 0.5//0.00001
    property int transparent: 100
    visible:netItem.visuNeuronModus===def.functionVisu||(Math.abs(weight)*d*0.2)>minWidth


    ShapePath {
           strokeColor: if(netItem.visuNeuronModus===def.functionVisu) return VisuFunction.color(transparent,"#daa520")
                        else if(weight<0) return VisuFunction.color(transparent,"ff0000")
                        else if(weight>0) return VisuFunction.color(transparent,"008000")
                        else return "white"

           strokeWidth:if(netItem.visuNeuronModus===def.functionVisu) return con.d*0.05
                        else if(Math.abs(weight)*con.d*0.1>maxWidth) return maxWidth
                        else return Math.abs(weight)*con.d*0.1

           startX:start.x
                    /*var xtemp=0
                   if(def.inputType===sourceType){
                       if(0<=inputConXOutput[sourceId]){
                       xtemp= inputConXOutput[sourceId]}
                   }
                   else if(def.hiddenType===sourceType){
                       if(0<=hiddenConXOutput[sourceId]){
                       xtemp= hiddenConXOutput[sourceId]}

                   else if(def.biasType===sourceType){
                       if(Math.floor(destinationId/hiddenNeuronY)<biasConXOutput.length){
                            if(0<=biasConXOutput[Math.floor(destinationId/hiddenNeuronY)]){
                                xtemp= biasConXOutput[Math.floor(destinationId/hiddenNeuronY)]}
                       }
                       else{
                           xtemp= biasConXOutput[biasConXOutput.length-1]
                       }

                   }

                   return xtemp
               }*/

           startY:start.y
               /*{var ytemp=0
                  if(def.inputType===sourceType){
                      if(0<=inputConYOutput[sourceId]){
                      ytemp= inputConYOutput[sourceId]}
                  }
                  else if(def.hiddenType===sourceType){
                      if(0<=hiddenConYOutput[sourceId]){
                      ytemp= hiddenConYOutput[sourceId]}
                  }
                  else if(def.biasType===sourceType){
                      if(Math.floor(destinationId/hiddenNeuronY)<biasConYOutput.length){
                            if(0<=biasConYOutput[Math.floor(destinationId/hiddenNeuronY)]){
                                ytemp= biasConYOutput[Math.floor(destinationId/hiddenNeuronY)]}
                      }
                      else{
                          ytemp= biasConYOutput[biasConYOutput.length-1]
                      }
                  }
                  return ytemp
                }*/
           PathLine {
               id:path
               x: end.x
                   /*var xtemp=0
                   if(def.outputType===destinationType){
                       if(0<=outputConXInput[destinationId-hiddenNeuronX*hiddenNeuronY]){
                            xtemp= outputConXInput[destinationId-hiddenNeuronX*hiddenNeuronY]
                       }
                   }
                   else if(def.hiddenType===destinationType){
                       if(0<=hiddenConXInput[destinationId]){
                            xtemp= hiddenConXInput[destinationId]
                           //console.debug("con",xtemp)
                       }
                   }

                   return xtemp
               }*/
               y: end.y
                  /*if(def.outputType===destinationType){
                      if(0<=outputConXInput[destinationId-hiddenNeuronX*hiddenNeuronY]){
                            return outputConYInput[destinationId-hiddenNeuronX*hiddenNeuronY]
                      }
                  }
                  else  if(def.hiddenType===destinationType){
                      if(0<=hiddenConYInput[destinationId]){
                            return hiddenConYInput[destinationId]
                      }
                  }
                  return 0
              }*/
           }
         }
    function updateValue(){
        weight=netListVisu.getConWeight(netId,conId)
    }
}
