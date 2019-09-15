import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
Rectangle{
    id: netClassic
    x:0
    y:0
    Repeater{
        model:ui.startNeuron.length
        Connection{
            xStart: net.layerPositionX[ui.startNeuron[index]]*net.distanceX+net.diameter+100
            yStart: net.layerPositionY[ui.startNeuron[index]]*net.distanceY+(net.diameter/2)+100
            xEnd: net.layerPositionX[ui.endNeuron[index]]*net.distanceX+100
            yEnd: net.layerPositionY[ui.endNeuron[index]]*net.distanceY+(net.diameter/2)+100
            weight: ui.connectionWeight[index]
            value: ui.neuronValueVect[ui.startNeuron[index]]
        }

    }
    Repeater{
        id:inputs
        model:ui.input
        Neuron{
            neuronid: index
            x:100
            y:100+(net.distanceY*index)
            diameter: net.diameter
            neuronValue:{
                    net.layerPositionX[index]=0
                    net.layerPositionY[index]=index
                    return ui.neuronValueVect[index]}
            }
        }
    Repeater{
        id:hiddenY
        model:ui.hiddenY
        Neuron{
            neuronid: index
            x:100+net.distanceX
            diameter: net.diameter
            y:100+(net.distanceY*index)
            neuronValue:{
                net.layerPositionX[index+ui.input]=1
                net.layerPositionY[index+ui.input]=index
                return ui.neuronValueVect[index+ui.input]}
            }
    }
    Repeater{
        id:output
        model:ui.output
        Neuron{
            neuronid: index
            x:100+net.distanceX*(ui.hiddenX+1)
            y:100+(net.distanceY*index)
            diameter: net.diameter
            neuronValue:{
                net.layerPositionX[index+ui.input+ui.hiddenY]=2
                net.layerPositionY[index+ui.input+ui.hiddenY]=index
                return ui.neuronValueVect[index+ui.input]}
            }
    }
}
