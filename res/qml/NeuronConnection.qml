import QtQuick 2.0
import QtQuick.Shapes 1.14

Shape {
    property int conID: 0
    property real weight:1
    property int conWidth: 4
    property int minWidth: 1
    ShapePath {
           strokeColor: if(weight>0) return "green"
                        else if(weight<0) return "red"
                        else return "white"
           strokeWidth: if(Math.abs(weight)*netItem.d*0.2>minWidth)return Math.abs(weight)*netItem.d*0.2
                        else return minWidth

           startX:{
                   if(inputType===conSourceType[conID]){
                       return inputConXOutput[conSourceID[conID]]
                   }
                   else if(hiddenType===conSourceType[conID]){
                       return hiddenConXOutput[conSourceID[conID]]
                   }
                   else if(biasType===conSourceType[conID]){
                       return biasConXOutput[Math.floor(conDestinationID[conID]/hiddenNeuronY)]
                   }
                   return 0
               }

           startY:{
                  if(inputType===conSourceType[conID]){
                      return inputConYOutput[conSourceID[conID]]
                  }
                  else if(hiddenType===conSourceType[conID]){
                      return hiddenConYOutput[conSourceID[conID]]
                  }
                  else if(biasType===conSourceType[conID]){
                      return biasConYOutput[Math.floor(conSourceID[conID]/hiddenNeuronY)]
                  }
                  return 0
                }
           PathLine {
               x: {
                   if(outputType===conDestinationType[conID]){
                       return outputConXInput[conDestinationID[conID]-hiddenNeuronX*hiddenNeuronY]
                   }
                   else if(hiddenType===conDestinationType[conID]){
                       return hiddenConXInput[conDestinationID[conID]]
                   }
                   return 0
               }
               y: {
                  if(outputType===conDestinationType[conID]){
                      return outputConYInput[conDestinationID[conID]-hiddenNeuronX*hiddenNeuronY]
                  }
                  else  if(hiddenType===conDestinationType[conID]){
                      return hiddenConYInput[conDestinationID[conID]]
                  }
                  return 0
              }
           }
         }
}
