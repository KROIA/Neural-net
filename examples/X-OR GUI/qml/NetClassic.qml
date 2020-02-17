import QtQuick 2.12
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
Rectangle{

    id: netClassic
    x:0
    y:0
    Repeater{
        model:net.startNeuron.length
        Connection{
            xStart: net.layerPositionX[net.startNeuron[index]]*net.distanceX+net.diameter+100
            yStart: net.layerPositionY[net.startNeuron[index]]*net.distanceY+(net.diameter/2)+100
            xEnd: net.layerPositionX[net.endNeuron[index]]*net.distanceX+100
            yEnd: net.layerPositionY[net.endNeuron[index]]*net.distanceY+(net.diameter/2)+100
            weight: net.connectionWeight[index]
            value: net.neuronValueVect[net.startNeuron[index]]
            connectionVisibilaty: net.connectionVisible[index]
        }

    }
    Repeater{
        id:inputs
        model:net.input
        Neuron{
            neuronid: index
            x:100
            y:100+(net.distanceY*index)
            diameter: net.diameter
            neuronValue:{
                    net.layerPositionX[index]=0
                    net.layerPositionY[index]=index
                    return net.neuronValueVect[index]}
            onClicked:{
                net.neuronClicked(index)
            }
            onReleased: {
                net.neuronReleased(index)
            }
            }
        }
    Repeater{
        id:hiddenY
        model:net.hiddenY
        Neuron{
            neuronid: index
            x:100+net.distanceX
            diameter: net.diameter
            y:100+(net.distanceY*index)
            neuronValue:{
                net.layerPositionX[index+net.inputs]=1
                net.layerPositionY[index+net.inputs]=index
                return net.neuronValueVect[index+net.inputs]}
            onClicked:{
                net.neuronClicked(index)
            }
            onReleased: {
                net.neuronReleased(index)
            }
        }
    }
    Repeater{
        id:output
        model:net.output
        Neuron{
            neuronid: index
            x:100+net.distanceX*(net.hiddenX+1)
            y:100+(net.distanceY*index)
            diameter: net.diameter
            neuronValue:{
                net.layerPositionX[index+net.input+net.hiddenY]=2
                net.layerPositionY[index+net.input+net.hiddenY]=index
                return net.neuronValueVect[index+net.input]}
            onClicked:{
                net.neuronClicked(index)
            }
            onReleased: {
                net.neuronReleased(index)
            }
            }
    }
}
