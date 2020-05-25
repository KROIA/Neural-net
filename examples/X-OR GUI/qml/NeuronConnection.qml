import QtQuick 2.0
import QtQuick.Shapes 1.14
Shape {
    property int conID: 0
    property real weight:1
    property int conWidth: 4
    ShapePath {
           strokeColor: if(weight>0) return "green"
                        else if(weight<0) return "red"
                        else return "white"
           strokeWidth: Math.abs(weight)*netItem.d*0.2
           startX:{
                   if(inputType===conSourceType[conID]){
                       return inputConXOutput[conSourceID[conID]]
                   }
                   if(hiddenType===conSourceType[conID]){
                       return hiddenConXOutput[conSourceID[conID]]
                   }
                   if(biasType===conSourceType[conID]){
                       return biasConXOutput[Math.floor(conDestinationID[conID]/hiddenNeuronX)]
                   }
                   return 0
               }

           startY:{
                  if(inputType===conSourceType[conID]){
                      return inputConYOutput[conSourceID[conID]]
                  }
                  if(hiddenType===conSourceType[conID]){
                      return hiddenConYOutput[conSourceID[conID]]
                  }
                  if(biasType===conSourceType[conID]){
                      return biasConYOutput[conDestinationID[conID]/hiddenNeuronX]
                  }
                  return 0
                }
           PathLine {
               x: {
                   if(outputType===conDestinationType[conID]){
                       return outputConXInput[conDestinationID[conID]-hiddenIDs.length]
                   }
                   else if(hiddenType===conDestinationType[conID]){
                       return hiddenConXInput[conDestinationID[conID]]
                   }
                   return 0
               }
               y: {
                  if(outputType===conDestinationType[conID]){
                      return outputConYInput[conDestinationID[conID]-hiddenIDs.length]
                  }
                  else  if(hiddenType===conDestinationType[conID]){
                      return outputConYInput[conDestinationID[conID]]
                  }
                  return 0
              }
           }
         }
}
