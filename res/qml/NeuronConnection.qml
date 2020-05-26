import QtQuick 2.0
import QtQuick.Shapes 1.14

Shape {
    property int conID: 0
    property real weight:1
    property int conWidth: 4
    property real minWidth: 1
    property real maxWidth: d*0.3
    property real d: 0.2
    ShapePath {
           strokeColor: if(weight>0) return "green"
                        else if(weight<0) return "red"
                        else return "white"
           strokeWidth: if(Math.abs(weight)*d*0.2<minWidth) return minWidth
                        else if(Math.abs(weight)*d*0.2>maxWidth) return maxWidth
                        else return Math.abs(weight)*d*0.2

           startX:{
                   if(inputType===conSourceType[conID]){
                       if(0<=inputConXOutput[conSourceID[conID]]){
                       return inputConXOutput[conSourceID[conID]]}
                   }
                   else if(hiddenType===conSourceType[conID]){
                       if(0<=hiddenConXOutput[conSourceID[conID]]){
                       return hiddenConXOutput[conSourceID[conID]]}
                   }
                   else if(biasType===conSourceType[conID]){
                       if(0<=biasConXOutput[Math.floor(conDestinationID[conID]/hiddenNeuronY)]){
                       return biasConXOutput[Math.floor(conDestinationID[conID]/hiddenNeuronY)]}
                   }
                   return 0
               }

           startY:{
                  if(inputType===conSourceType[conID]){
                      if(0<=inputConYOutput[conSourceID[conID]]){
                      return inputConYOutput[conSourceID[conID]]}
                  }
                  else if(hiddenType===conSourceType[conID]){
                      if(0<=hiddenConYOutput[conSourceID[conID]]){
                      return hiddenConYOutput[conSourceID[conID]]}
                  }
                  else if(biasType===conSourceType[conID]){
                      if(0<=biasConYOutput[Math.floor(conSourceID[conID]/hiddenNeuronY)]){
                      return biasConYOutput[Math.floor(conSourceID[conID]/hiddenNeuronY)]}
                  }
                  return 0
                }
           PathLine {
               x: {
                   if(outputType===conDestinationType[conID]){
                       if(0<=outputConXInput[conDestinationID[conID]-hiddenNeuronX*hiddenNeuronY]){
                            return outputConXInput[conDestinationID[conID]-hiddenNeuronX*hiddenNeuronY]
                       }
                   }
                   else if(hiddenType===conDestinationType[conID]){
                       if(0<=hiddenConXInput[conDestinationID[conID]]){
                            return hiddenConXInput[conDestinationID[conID]]
                       }
                   }
                   return 0
               }
               y: {
                  if(outputType===conDestinationType[conID]){
                      if(0<=outputConXInput[conDestinationID[conID]-hiddenNeuronX*hiddenNeuronY]){
                            return outputConYInput[conDestinationID[conID]-hiddenNeuronX*hiddenNeuronY]
                      }
                  }
                  else  if(hiddenType===conDestinationType[conID]){
                      if(0<=hiddenConYInput[conDestinationID[conID]]){
                            return hiddenConYInput[conDestinationID[conID]]
                      }
                  }
                  return 0
              }
           }
         }
}
