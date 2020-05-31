import QtQuick 2.14
import QtQuick.Controls 2.4

NetData {
    id:netItem
    width: 600
    height: 600
    property bool enableMousArea: false
    property int clickedNeuronID: -1
    property int clickedNeuronType: 0
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


    property real yOffSet: 0.1
    property real xOffSet: 0.1
    property int yBiasPos: if(bias) return yDistance
                        else return 0

    property bool showId: true
    signal clickedNet(var id)

    MouseArea{
        anchors.fill: parent
        enabled: enableMousArea
        onClicked: clickedNet(netID)
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
            lastNeuron: (index==(biasLayer.model-1))
            onClickedNeuron: {
                clickedNeuronID= typeId
                clickedNeuronType= type
            }
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
            onClickedNeuron: {
                clickedNeuronID= typeId
                clickedNeuronType= type
            }
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
                onClickedNeuron: {
                    clickedNeuronID= typeId
                    clickedNeuronType= type
                }
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
            onClickedNeuron: {
                clickedNeuronID= typeId
                clickedNeuronType= type
            }
        }
    }


    Text {
        anchors.top:parent.top
        anchors.right: parent.right
        anchors.rightMargin: netItem.width*xOffSet

        anchors.topMargin: yOffSet
        topPadding: yOffSet
        text: "net Id: "+netID
        font.pixelSize: parent.d*0.8<20 ? parent.d*0.8:20

        visible:showId
        horizontalAlignment: Text.AlignRight
    }

}
