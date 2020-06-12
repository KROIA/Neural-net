import QtQuick 2.0
import "../js/VisuFunction.js" as VisuFunction
Rectangle {
    id:neuron
    property int d: 20
    property real neuronValue: 0
    property int neuronID: 0
    property int typeId:0
    property int type: 0
    property bool lastNeuron: false
    property real xRel: 0
    property real yRel: 0
    x: {
        //if(type===def.hiddenType) //console.debug(xRel+" * "+netItem.xDistance+" = "+xRel*netItem.xDistance)
        return xRel*totalNet.xDistance}
    y: yRel*totalNet.yDistance

    onXChanged:{
        //if(type===def.hiddenType) console.debug("hiddenx changed: "+x)
        dockingXPoint()
        if(Drag.active||lastNeuron){ updateXPos()
        }}
    onYChanged:{dockingYPoint()
        if(Drag.active||lastNeuron) {updateYPos()
        }}
    onDChanged:{
        dockingPoint()
        if(lastNeuron) {
            updateXPos()
            updateYPos()
        }
        xOffset=0.2*d
        }

    Drag.active: mouseArea.drag.active
    property real xOffset: 0.2*d

    function updatePos(){

        updateXPos()
        updateYPos()
    }
    function updateXPos(){

        if(type===def.inputType){
            inputConXOutput=VisuFunction.updateArray(inputConXOutput)
        }
        else if(type===def.outputType){
            outputConXInput=VisuFunction.updateArray(outputConXInput)
        }
        else if(type===def.hiddenType){
            hiddenConXInput=VisuFunction.updateArray(hiddenConXInput)
            hiddenConXOutput=VisuFunction.updateArray(hiddenConXOutput)
        }
        else if(type===def.biasType){
            biasConXOutput=VisuFunction.updateArray(biasConXOutput)
        }
    }
    function updateYPos(){
            //console.debug("update Y")
            if(type===def.inputType){
                inputConYOutput=VisuFunction.updateArray(inputConYOutput)
            }
            else if(type===def.outputType){
                outputConYInput=VisuFunction.updateArray(outputConYInput)
            }
            else if(type===def.hiddenType){
                hiddenConYInput=VisuFunction.updateArray(hiddenConYInput)
                hiddenConYOutput=VisuFunction.updateArray(hiddenConYOutput)
            }
            else if(type===def.biasType){
                biasConYOutput=VisuFunction.updateArray(biasConYOutput)
            }
        }
    function dockingPoint(){
        dockingYPoint()
        dockingXPoint()
    }
    function dockingXPoint(){
        var xInput
        var xOutput
        //console.debug(neuron.dockingPointOffset)
        var offSet=neuron.dockingPointOffset
        //console.debug(offSet)
        xInput=x+(0.2*d)
        xOutput=d+x-(0.2*d)

        if(type===def.inputType){
                    inputConXOutput[typeId]=xOutput
                }
                else if(type===def.biasType){
                    biasConXOutput[typeId]=xOutput

                    if(lastNeuron){
                        var tempBiasConXOutput=[]
                        tempBiasConXOutput=biasConXOutput
                        biasConXOutput=tempBiasConXOutput
                   }

                }
                else if(type===def.hiddenType){
                    hiddenConXInput[typeId]=xInput
                    hiddenConXOutput[typeId]=xOutput
                    //console.debug(hiddenConXOutput)

                    if(lastNeuron){
                        var tempHiddenConXOutput=[]
                        var tempHiddenConXInput=[]
                        tempHiddenConXInput=hiddenConXInput
                        tempHiddenConXOutput=hiddenConXOutput
                        hiddenConXInput=tempHiddenConXInput
                        hiddenConXOutput=tempHiddenConXOutput
                    }
                }
                    else if(type===def.outputType){
                        outputConXInput[typeId]=xInput
                }

    }

    function dockingYPoint(){
        var yInput
        var yOutput
        yInput=(d/2)+y
        yOutput=(d/2)+y
        if(type===def.inputType){
                    inputConYOutput[typeId]=yOutput
                    if(lastNeuron){
                        var tempInputConYOutput=[]
                        tempInputConYOutput=inputConYOutput
                        inputConYOutput=tempInputConYOutput
                    }
                }
                else if(type===def.biasType){

                    biasConYOutput[typeId]=yOutput
                    if(lastNeuron){
                        var tempBiasConYOutput=[]
                        tempBiasConYOutput=biasConYOutput
                        biasConYOutput=tempBiasConYOutput
                   }

                }
                else if(type===def.hiddenType){
                    hiddenConYInput[typeId]=yInput
                    hiddenConYOutput[typeId]=yOutput
                    //console.debug(hiddenConYOutput)

                    if(lastNeuron){
                        var tempHiddenConYOutput=[]
                        var tempHiddenConYInput=[]
                        tempHiddenConYInput=hiddenConYInput
                        tempHiddenConYOutput=hiddenConYOutput
                        hiddenConYInput=tempHiddenConYInput
                        hiddenConYOutput=tempHiddenConYOutput
                    }
                }
                    else if(type===def.outputType){
                        outputConYInput[typeId]=yInput
                        if(lastNeuron){
                            var tempOutputConYInput=[]
                            tempOutputConYInput=outputConYInput
                            outputConYInput=tempOutputConYInput
                    }
                }
    }
    property int transparent: 100
    property string neuronColor: {
                if(netItem.visuNeuronModus===def.functionVisu) return VisuFunction.color(transparent,def.getTypeColor(type))
                else{
                    if(neuronValue<0) return VisuFunction.color(transparent,"8b0000")
                    else return VisuFunction.color(transparent,"006400")}
                }
    width: d
    height: d
    border.width: netItem.visuNeuronModus===def.combinedVisu ?  d*0.1:d*0.05
    border.color: if(netItem.visuNeuronModus===def.combinedVisu) return VisuFunction.color(transparent,def.getTypeColor(type))
                      else return VisuFunction.color(transparent,"000000")
    radius: d/2
    color:
        if(netItem.visuNeuronModus===def.functionVisu) return neuronColor
        else neuronValue===0 ? VisuFunction.color(transparent,"f5f5f5"):Qt.lighter(neuronColor,(1-Math.abs(neuronValue-0.1))*5)

    Text {
        visible: (parent.d>10)


        font.pixelSize: parent.d*0.2
        horizontalAlignment: Text.AlignHCenter
        text: if(type>def.noneType) return  "ID:"+neuronID+"\nValue: \n"+Math.round(neuronValue*10000)/10000
                else return "Value: \n"+Math.round(neuronValue*10000)/10000
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color:Math.abs(neuronValue)>0.75&&netItem.visuIndex!=def.functionVisu ? "white":"black"
    }
    signal clickedNeuron(var id,var type)
    property bool movable: netItem.moveable

    MouseArea{
        id:mouseArea
        anchors.fill: parent
        enabled: netItem.neuronClickEnable
        onClicked: {clickedNeuron(typeId,type)
        setNetHighlight(typeId,type)
        }
        drag {
                target: if(movable) return parent
                maximumX: totalNet.width-d
                maximumY: totalNet.height-d
                minimumX: 0
                minimumY: 0

            }
        onDragActiveChanged: if(drag){
                                 xRel=parent.x/xDistance
                                 yRel=parent.y/yDistance}

        property bool dragActive: drag.active
    }
}
