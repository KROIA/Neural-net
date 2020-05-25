import QtQuick 2.0

Item {
    id:netItem
    width: 600
    height: 600
    property int netID: 0

    property variant inputNeuron: [1]
    property variant outputNeuron: [1]
    property int hiddenNeuronX: 1
    property int hiddenNeuronY: 1
    property variant hiddenValue: [0]
    property int maxYNeuron: {
        if(hiddenNeuronY>outputNeuron.length&&hiddenNeuronY>inputNeuron.length){
            return hiddenNeuronY
        }
        if(hiddenNeuronY<outputNeuron.length&&outputNeuron.length>inputNeuron.length){
            return outputNeuron.length
        }
        if(hiddenNeuronY<inputNeuron.length&&outputNeuron.length<inputNeuron.length){
            return inputNeuron.length
        }
        return 0
    }
    property int xDistance: {
        return width/(hiddenNeuronX+3)
    }
    property int yDistance: {
        return height/(hiddenNeuronY+1+yOffSet-0.5)
    }
    property int d:
        if(yDistance<xDistance) return yDistance*0.4
        else return xDistance*0.4
    property variant hiddenConXInput: [0]
    property variant hiddenConYInput: [0]

    property variant hiddenConXOutput: [0]
    property variant hiddenConYOutput: [0]

    property variant outputConXInput: [0]
    property variant outputConYInput: [0]

    property variant inputConXOutput: [0]
    property variant inputConYOutput: [0]

    property variant biasConXOutput: [0]
    property variant biasConYOutput: [0]

    property variant hiddenIDs: [0]
    property variant outputIds: [0]

    property variant conSourceID: [0]
    property variant conDestinationID: [0]

    property variant conSourceType: [0]
    property variant conDestinationType: [0]

    property int noneType: 1
    property int inputType: 1
    property int hiddenType: 2
    property int outputType: 3
    property int biasType: 5

    property bool bias: false
    property real biasValue: 0
    property real yOffSet: if(bias) return 1.5
                            else return 0.5
    Repeater{
        model: conSourceID.length
        NeuronConnection{
            conID: index
        }
    }
    Repeater{
        id:biasLayer
        model: hiddenNeuronX+1
        visible: bias
        Neuron{
            x:(0.5+index)*xDistance
            y: 0.5*yDistance
            d:netItem.d
            neuronValue: biasValue
            typeId: index
            type:biasType
        }
    }

    Repeater{
        id:inputLayer
        model:inputNeuron.length
        Neuron{
            x:0.5*xDistance
            y:(index+yOffSet)*yDistance
            d:netItem.d
            neuronValue: if(inputNeuron.length) return inputNeuron[index]
                            else return 0
            typeId: index
            type:inputType
        }
    }
    Timer {
            interval: 100; running: true; repeat: true
            onTriggered: updateValue()
        }
    Repeater{
        id:hiddenXLayer
        model:hiddenNeuronX
        Repeater{
            id:hiddenYLayer
            model:hiddenNeuronY
            property int indexX: index
            Neuron{
                x:xDistance*(indexX+1.5)
                y:(index+yOffSet)*yDistance
                d:netItem.d
                typeId: index+(indexX*hiddenNeuronY)
                neuronID: hiddenIDs[typeId]
                neuronValue: if(index+(indexX*hiddenNeuronY)<hiddenValue.length){
                                         return hiddenValue[typeId]}
                             else return 0
                type:hiddenType
            }
        }
    }
    Repeater{
        id:outputLayer
        model:outputNeuron.length
        Neuron{
            x:(1.5+hiddenNeuronX)*xDistance
            y:(index+yOffSet)*yDistance
            d:netItem.d
            typeId: index
            neuronID: outputIds[typeId]
            type:outputType
            neuronValue: if(outputNeuron.length) outputNeuron[index]
            else return 0
        }
    }

    function updateValue(){
        hiddenNeuronX=netVisu.getHiddenX()
        hiddenNeuronY=netVisu.getHiddenY()
        inputNeuron=netVisu.getInputsValue()
        outputNeuron=netVisu.getOutputsValue()
        hiddenValue=netVisu.getHiddenValue()
        hiddenIDs=netVisu.getHiddenID()
        outputIds=netVisu.getOutputID()
        conSourceID=netVisu.getConSourceID()
        conSourceType=netVisu.getConSourceType()
        conDestinationID=netVisu.getConDestinationID()
        conDestinationType=netVisu.getConDestinationType()
        bias=netVisu.getBias()
        biasValue=netVisu.getBiasValue()
    }
}
