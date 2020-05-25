import QtQuick 2.0
import QtQuick.Shapes 1.14
Shape {
    property int conID: 0
    property int conWidth: 4
    ShapePath {
           strokeColor: "green"
           strokeWidth: conWidth
           startX:{
                   if(inputType===conSourceType[conID]){
                       return inputConXOutput[conSourceID[conID]]
                   }
                   if(hiddenType===conSourceType[conID]){
                       return hiddenConXOutput[conSourceID[conID]]
                   }
                   if(biasType===conSourceType[conID]){
                       console.debug("connection to bias "+Math.floor(conDestinationID[conID]/hiddenNeuronX)+" From "+ conID)
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
