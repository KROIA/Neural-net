import QtQuick 2.0
import QtQuick.Shapes 1.14
import "../js/VisuFunction.js" as VisuFunction

Shape {
    property int conID: 0
    property real weight:1
    property int conWidth: 4
    property real minWidth: 0.2
    property real maxWidth: d*0.3
    property int destinationId: conDestinationID[conID]
    property int destinationType: conDestinationType[conID]
    property int sourceId: conSourceID[conID]
    property int sourceType:conSourceType[conID]
    property real d: 0.2
    property int transparent: 100
    visible: (Math.abs(weight)*d*0.2)>minWidth
    ShapePath {
           strokeColor: if(weight<0) return VisuFunction.color(transparent,"ff0000")
                        else if(weight>0) return VisuFunction.color(transparent,"008000")
                        else return "white"
           strokeWidth:if(Math.abs(weight)*d*0.2>maxWidth) return maxWidth
                        else return Math.abs(weight)*d*0.2

           startX:{
                   if(inputType===sourceType){
                       if(0<=inputConXOutput[sourceId]){
                       return inputConXOutput[sourceId]}
                   }
                   else if(hiddenType===sourceType){
                       if(0<=hiddenConXOutput[sourceId]){
                       return hiddenConXOutput[sourceId]}
                   }
                   else if(biasType===sourceType){

                       if(0<=biasConXOutput[Math.floor(destinationId/hiddenNeuronY)]){
                       return biasConXOutput[Math.floor(destinationId/hiddenNeuronY)]}
                   }
                   return 0
               }

           startY:{
                  if(inputType===sourceType){
                      if(0<=inputConYOutput[sourceId]){
                      return inputConYOutput[sourceId]}
                  }
                  else if(hiddenType===sourceType){
                      if(0<=hiddenConYOutput[sourceId]){
                      return hiddenConYOutput[sourceId]}
                  }
                  else if(biasType===sourceType){
                      if(0<=biasConYOutput[Math.floor(destinationId/hiddenNeuronY)]){

                      return biasConYOutput[Math.floor(destinationId/hiddenNeuronY)]}
                  }
                  return 0
                }
           PathLine {
               x: {
                   if(outputType===destinationType){
                       if(0<=outputConXInput[destinationId-hiddenNeuronX*hiddenNeuronY]){
                            return outputConXInput[destinationId-hiddenNeuronX*hiddenNeuronY]
                       }
                   }
                   else if(hiddenType===destinationType){
                       if(0<=hiddenConXInput[destinationId]){
                            return hiddenConXInput[destinationId]
                       }
                   }
                   return 0
               }
               y: {
                  if(outputType===destinationType){
                      if(0<=outputConXInput[destinationId-hiddenNeuronX*hiddenNeuronY]){
                            return outputConYInput[destinationId-hiddenNeuronX*hiddenNeuronY]
                      }
                  }
                  else  if(hiddenType===destinationType){
                      if(0<=hiddenConYInput[destinationId]){
                            return hiddenConYInput[destinationId]
                      }
                  }
                  return 0
              }
           }
         }
}
