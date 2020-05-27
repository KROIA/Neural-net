import QtQuick 2.0

Item {
    id:netItem
    width: 600
    height: 600
    property int netID: 0
    property int updateTime:100
    property int hiddenNeuronX: netVisu.getHiddenX(netItem.netID)
    property int hiddenNeuronY: netVisu.getHiddenY(netItem.netID)
    property int inputNeuron: netVisu.getInputs(netItem.netID)
    property int outputNeuron: netVisu.getOutputs(netItem.netID)

    property variant hiddenValue: netVisu.getHiddenValue(netItem.netID)
    property variant outputValue:netVisu.getOutputsValue(netItem.netID)
    property variant inputValue:netVisu.getInputsValue(netItem.netID)
    property int maxYNeuron: {
        var b
        if(bias===true)b=1
        else b=0
        if(hiddenNeuronY>=outputNeuron&&hiddenNeuronY>=inputNeuron){
            return hiddenNeuronY+b
        }
        else if(hiddenNeuronY<=outputNeuron&&outputNeuron>=inputNeuron){
            return outputNeuron+b
        }
        else if(hiddenNeuronY<=inputNeuron&&outputNeuron<=inputNeuron){
            return inputNeuron+b
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

    property variant hiddenIDs: netVisu.getHiddenID(netItem.netID)
    property variant outputIds: netVisu.getOutputID(netItem.netID)

    property variant conSourceID: netVisu.getConSourceID(netItem.netID)
    property variant conDestinationID: netVisu.getConDestinationID(netItem.netID)

    property variant conSourceType: netVisu.getConSourceType(netItem.netID)
    property variant conDestinationType: netVisu.getConDestinationType(netItem.netID)

    property variant conWeight: netVisu.getConWeight(netItem.netID)

    property int noneType: 1
    property int inputType: 1
    property int hiddenType: 2
    property int outputType: 3
    property int biasType: 5

    property bool bias: netVisu.getBias(netItem.netID)
    property real biasValue: netVisu.getBiasValue(netItem.netID)
    property real yOffSet: 0.1
    property real xOffSet: 0.1
    property int yBiasPos: if(bias) return yDistance
                        else return 0

    Connections {
                   target: netVisu
                   onStopUpdateSignal: timerNet.running=false
                   onStartUpdateSignal:{timerNet.running=true
                       updateStructur()}
    }
    Timer {
        id:timerNet
            interval: updateTime; running: true
             repeat: true
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
        model:inputNeuron
        Neuron{
            x:xOffSet*xDistance
            y:(index+yOffSet)*yDistance+yBiasPos
            d:netItem.d
            neuronValue: if(inputValue.length>typeId) return inputValue[typeId]
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
        model:outputNeuron
        Neuron{
            x:(xOffSet+1+hiddenNeuronX)*xDistance
            y:(index+yOffSet)*yDistance+yBiasPos
            d:netItem.d
            typeId: index
            neuronID: if(outputIds.length>typeId) return outputIds[typeId]
                        else return typeId+hiddenNeuronX*hiddenNeuronY
            type:outputType
            neuronValue: if(outputValue.length>typeId) outputValue[index]
            else return 0
        }
    }

    function updateValue(){
        hiddenValue=netVisu.getHiddenValue(netItem.netID)
        inputValue=netVisu.getInputsValue(netItem.netID)
        outputValue=netVisu.getOutputsValue(netItem.netID)
        biasValue=netVisu.getBiasValue(netItem.netID)
        conWeight=netVisu.getConWeight(netItem.netID)



        /*
        */
    }
    function updateStructur(){
        console.debug("update Struc")
        inputNeuron=netVisu.getInputs(netItem.netID)
        outputNeuron=netVisu.getOutputs(netItem.netID)
        hiddenNeuronX=netVisu.getHiddenX(netItem.netID)
        hiddenNeuronY=netVisu.getHiddenY(netItem.netID)

        hiddenIDs=netVisu.getHiddenID(netItem.netID)
        outputIds=netVisu.getOutputID(netItem.netID)
        conSourceID=netVisu.getConSourceID(netItem.netID)
        conSourceType=netVisu.getConSourceType(netItem.netID)
        conDestinationID=netVisu.getConDestinationID(netItem.netID)
        conDestinationType=netVisu.getConDestinationType(netItem.netID)

        bias=netVisu.getBias(netItem.netID)
    }
}
