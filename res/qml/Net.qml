import QtQuick 2.0

Item {
    id:netItem
    width: 600
    height: 600
    property int netID: 0
    property int updateTime:100
    property variant inputNeuron: [0]
    property variant outputNeuron: [0]
    property int hiddenNeuronX: 1
    property int hiddenNeuronY: 1
    property variant hiddenValue: [0]
    property int maxYNeuron: {
        var b
        if(bias===true)b=1
        else b=0
        if(hiddenNeuronY>=outputNeuron.length&&hiddenNeuronY>=inputNeuron.length){
            return hiddenNeuronY+b
        }
        else if(hiddenNeuronY<=outputNeuron.length&&outputNeuron.length>=inputNeuron.length){
            return outputNeuron.length+b
        }
        else if(hiddenNeuronY<=inputNeuron.length&&outputNeuron.length<=inputNeuron.length){
            return inputNeuron.length+b
        }
        return 0
    }
    property real dRelationship: 0.4
    property int xDistance: {
        var w
        if(((width)-xOffSet*2)/(hiddenNeuronX+1)*dRelationship>((height)/(maxYNeuron-1+yOffSet*2))*dRelationship){
            w= ((width)-xOffSet*2)/(hiddenNeuronX+1)*dRelationship}
        else w= ((height)/(maxYNeuron-1+yOffSet*2))*dRelationship
        return (width-w)/(hiddenNeuronX+1+yOffSet*2)
    }
    property int yDistance: {
        var w
        if(((width)-xOffSet*2)/(hiddenNeuronX+1)*dRelationship>((height)/(maxYNeuron-1+yOffSet*2))*dRelationship){
            w= ((width)-xOffSet*2)/(hiddenNeuronX+1)*dRelationship}
        else w= ((height)/(maxYNeuron-1+yOffSet*2))*dRelationship
        return (height-w)/(maxYNeuron-1+yOffSet*2)
    }
    property int d:
        if(yDistance<xDistance) return yDistance*dRelationship
        else return xDistance*dRelationship
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

    property variant hiddenIDs: [0,1,2,3]
    property variant outputIds: [0]

    property variant conSourceID: [0]
    property variant conDestinationID: [0]

    property variant conSourceType: [0]
    property variant conDestinationType: [0]

    property variant conWeight: [0]
    property int noneType: 1
    property int inputType: 1
    property int hiddenType: 2
    property int outputType: 3
    property int biasType: 5

    property bool bias: false
    property real biasValue: 0
    property real yOffSet: 0.1
    property real xOffSet: 0.1
    property int yBiasPos: if(bias) return yDistance
                        else return 0
    Connections {
                   target: netVisu
                   onStopUpdateSignal: timerNet.running=false
                   onStartUpdateSignal:timerNet.running=true
    }
    Timer {
        id:timerNet
            interval: updateTime; running: true; repeat: true
            onTriggered: updateValue()
        }
    Repeater{
        model: conWeight.length
        NeuronConnection{
            conID: index
            weight:conWeight[index]
            d:netItem.d
        }
    }
    Repeater{
        id:biasLayer

        model: hiddenNeuronX+1
        visible: bias
        Neuron{
            x:(xOffSet+index)*xDistance
            y: yOffSet*yDistance
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
            x:xOffSet*xDistance
            y:(index+yOffSet)*yDistance+yBiasPos
            d:netItem.d
            neuronValue: if(inputNeuron.length>typeId) return inputNeuron[typeId]
                            else return 0
            typeId: index
            type:inputType
        }
    }

    Repeater{
        id:hiddenXLayer
        model:hiddenNeuronX
        Repeater{
            id:hiddenYLayer
            model:hiddenNeuronY
            property int indexX: index
            Neuron{
                x:xDistance*(indexX+xOffSet+1)
                y:(index+yOffSet)*yDistance+yBiasPos
                d:netItem.d
                typeId: index+(indexX*hiddenNeuronY)
                neuronID:if(0<hiddenIDs[typeId]) return 0<hiddenIDs[typeId]
                            else return typeId
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
            x:(xOffSet+1+hiddenNeuronX)*xDistance
            y:(index+yOffSet)*yDistance+yBiasPos
            d:netItem.d
            typeId: index
            neuronID: if(outputIds.length>typeId) return outputIds[typeId]
                        else return typeId+hiddenNeuronX*hiddenNeuronY
            type:outputType
            neuronValue: if(outputNeuron.length) outputNeuron[index]
            else return 0
        }
    }

    function updateValue(){

        hiddenNeuronX=netVisu.getHiddenX(netItem.netID)
        hiddenNeuronY=netVisu.getHiddenY(netItem.netID)
        inputNeuron=netVisu.getInputsValue(netItem.netID)
        outputNeuron=netVisu.getOutputsValue(netItem.netID)
        hiddenValue=netVisu.getHiddenValue(netItem.netID)
        hiddenIDs=netVisu.getHiddenID(netItem.netID)
        outputIds=netVisu.getOutputID(netItem.netID)
        conSourceID=netVisu.getConSourceID(netItem.netID)
        conSourceType=netVisu.getConSourceType(netItem.netID)
        conDestinationID=netVisu.getConDestinationID(netItem.netID)
        conDestinationType=netVisu.getConDestinationType(netItem.netID)
        conWeight=netVisu.getConWeight(netItem.netID)
        bias=netVisu.getBias(netItem.netID)
        biasValue=netVisu.getBiasValue(netItem.netID)
    }
}
