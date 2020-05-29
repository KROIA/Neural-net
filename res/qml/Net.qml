import QtQuick 2.0

Item {
    id:netItem
    width: 600
    height: 600
    property int netID: 0
    property int updateTime:100
    property int hiddenNeuronX: netListVisu.getHiddenX(netItem.netID)
    property int hiddenNeuronY: netListVisu.getHiddenY(netItem.netID)
    property int inputNeuron: netListVisu.getInputs(netItem.netID)
    property int outputNeuron: netListVisu.getOutputs(netItem.netID)
    property variant hiddenValue: netListVisu.getHiddenValue(netItem.netID)
    property variant outputValue:netListVisu.getOutputsValue(netItem.netID)
    property variant inputValue:netListVisu.getInputsValue(netItem.netID)
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
    property real dRelationship: 0.8
    property int xDistance: {
        var w
        if(((width)-xOffSet*2)/(hiddenNeuronX+1)*dRelationship<((height)/(maxYNeuron-1+yOffSet*2))*dRelationship){
            w= ((width)-xOffSet*2)/(hiddenNeuronX+1)*dRelationship}
        else w= ((height)/(maxYNeuron-1+yOffSet*2))*dRelationship
        return (width-w)/(hiddenNeuronX+1+yOffSet*2)
    }
    property int yDistance: {
        var w
        if(((width)-xOffSet*2)/(hiddenNeuronX+1)*dRelationship<((height)/(maxYNeuron-1+yOffSet*2))*dRelationship){
            w= ((width)-xOffSet*2)/(hiddenNeuronX+1)*dRelationship}
        else w= ((height)/(maxYNeuron-1+yOffSet*2))*dRelationship
        return (height-w)/(maxYNeuron-1+yOffSet*2)
    }
    property int d:{
        var dP
        if(yDistance<xDistance) dP =yDistance*dRelationship
        else dP= xDistance*dRelationship
        return dP
    }
    property variant hiddenConXInput: []
    property variant hiddenConYInput: []

    property variant hiddenConXOutput: []
    property variant hiddenConYOutput: []

    property variant outputConXInput: []
    property variant outputConYInput: []

    property variant inputConXOutput: []
    property variant inputConYOutput: []

    property variant biasConXOutput: []
    property variant biasConYOutput: []

    property variant hiddenIDs: netListVisu.getHiddenID(netItem.netID)
    property variant outputIds: netListVisu.getOutputID(netItem.netID)

    property variant conSourceID: netListVisu.getConSourceID(netItem.netID)
    property variant conDestinationID: netListVisu.getConDestinationID(netItem.netID)

    property variant conSourceType: netListVisu.getConSourceType(netItem.netID)
    property variant conDestinationType: netListVisu.getConDestinationType(netItem.netID)

    property variant conWeight: netListVisu.getConWeight(netItem.netID)

    property int noneType: 1
    property int inputType: 1
    property int hiddenType: 2
    property int outputType: 3
    property int biasType: 5

    property bool bias: netListVisu.getBias(netItem.netID)
    property real biasValue: netListVisu.getBiasValue(netItem.netID)
    property real yOffSet: 0.1
    property real xOffSet: 0.1
    property int yBiasPos: if(bias) return yDistance
                        else return 0
    Connections {
                   target: netListVisu
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
            lastNeuron: (index<(biasLayer.model-1))
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
            lastNeuron: (index==(inputLayer.model-1))
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
                neuronID:if(0<hiddenIDs[typeId]){
                             return hiddenIDs[typeId]}
                            else return typeId
                neuronValue: if(index+(indexX*hiddenNeuronY)<hiddenValue.length){
                                         return hiddenValue[typeId]}
                             else return 0
                type:hiddenType
                lastNeuron: (indexX==(hiddenXLayer.model-1)&&index===(hiddenYLayer.model-1))
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
            lastNeuron: (index==(outputLayer.model-1))
        }
    }

    function updateValue(){
        hiddenValue=netListVisu.getHiddenValue(netItem.netID)
        inputValue=netListVisu.getInputsValue(netItem.netID)
        outputValue=netListVisu.getOutputsValue(netItem.netID)
        biasValue=netListVisu.getBiasValue(netItem.netID)
        conWeight=netListVisu.getConWeight(netItem.netID)
        /*
        */
    }
    function updateStructur(){
        //console.debug("update Struc")
        inputNeuron=netListVisu.getInputs(netItem.netID)
        outputNeuron=netListVisu.getOutputs(netItem.netID)
        hiddenNeuronX=netListVisu.getHiddenX(netItem.netID)
        hiddenNeuronY=netListVisu.getHiddenY(netItem.netID)

        hiddenIDs=netListVisu.getHiddenID(netItem.netID)
        outputIds=netListVisu.getOutputID(netItem.netID)
        conSourceID=netListVisu.getConSourceID(netItem.netID)
        conSourceType=netListVisu.getConSourceType(netItem.netID)
        conDestinationID=netListVisu.getConDestinationID(netItem.netID)
        conDestinationType=netListVisu.getConDestinationType(netItem.netID)

        bias=netListVisu.getBias(netItem.netID)
    }
}
