import QtQuick 2.0
import "../../js/VisuFunction.js" as VisuFunction
Rectangle {
    id:neuron
    property int d: 20
    property int parentWidth: totalNet.width
    property int parentHeight: totalNet.height
    property int visuModus: 0
    property bool lastNeuron: false
    property real xRel: 0
    property real yRel: 0
    NeuronData{
        id:dataNeuron
    }
    property alias dataNeuron: dataNeuron
    Connections{
        target: netItem
        function onLoadRelPos(){
            neuron.xRel=netItem.xRel[absId]
            neuron.yRel=netItem.yRel[absId]
            dockingPoint()
        }
    }

    onXChanged:{
        //if(type===def.hiddenType) console.debug("hiddenx changed: "+x)
        dockingPoint()
       /*if(Drag.active||lastNeuron){ //updatePos()
        }*/}
    onYChanged:{
        dockingPoint()
        /*if(Drag.active||lastNeuron) {updatePos()
        }*/}
    onDChanged:{
        dockingPoint()
        /*if(lastNeuron) {
            updatePos()
        }*/
        xOffset=0.2*d
        }

    Drag.active: mouseArea.drag.active
    property real xOffset: 0.2*d

    /*function updatePos(){
        input=netListVisu.getDockingPointInput(dataNeuron.netId,dataNeuron.absId)
        output=netListVisu.getDockingPointOutput(dataNeuron.netId,dataNeuron.absId)
        //updateXPos()
        //updateYPos()
    }
/*
    function updateXPos(){

        if(dataNeuron.type===def.inputType){
            inputConXOutput=VisuFunction.updateArray(inputConXOutput)
        }
        else if(dataNeuron.type===def.outputType){
            outputConXInput=VisuFunction.updateArray(outputConXInput)
        }
        else if(dataNeuron.type===def.hiddenType){
            hiddenConXInput=VisuFunction.updateArray(hiddenConXInput)
            hiddenConXOutput=VisuFunction.updateArray(hiddenConXOutput)
        }
        else if(dataNeuron.type===def.biasType){
            biasConXOutput=VisuFunction.updateArray(biasConXOutput)
        }
    }
    function updateYPos(){
            console.debug("update Y")
            if(dataNeuron.type===def.inputType){
                inputConYOutput=VisuFunction.updateArray(inputConYOutput)
            }
            else if(dataNeuron.type===def.outputType){
                outputConYInput=VisuFunction.updateArray(outputConYInput)
            }
            else if(dataNeuron.type===def.hiddenType){
                hiddenConYInput=VisuFunction.updateArray(hiddenConYInput)
                hiddenConYOutput=VisuFunction.updateArray(hiddenConYOutput)
            }
            else if(dataNeuron.type===def.biasType){
                biasConYOutput=VisuFunction.updateArray(biasConYOutput)
            }
        }*/
    function dockingPoint(){
        netListVisu.setDockingPointInput(dataNeuron.netId,dataNeuron.absId,Qt.point(x+(0.2*d),(d/2)+y),0)//Drag.active||
        netListVisu.setDockingPointOutput(dataNeuron.netId,dataNeuron.absId,Qt.point(d+x-(0.2*d),(d/2)+y),Drag.active||lastNeuron)
    }
    /*
    function dockingXPoint(){
        var xInput
        var xOutput
        //console.debug(neuron.dockingPointOffset)
        var offSet=neuron.dockingPointOffset
        //console.debug(offSet)
        xInput=x+(0.2*d)
        xOutput=d+x-(0.2*d)

        if(dataNeuron.type===def.inputType){
                    inputConXOutput[dataNeuron.typeId]=xOutput
                }
                else if(dataNeuron.type===def.biasType){
                    biasConXOutput[dataNeuron.typeId]=xOutput
                    if(lastNeuron){
                        var tempBiasConXOutput=[]
                        tempBiasConXOutput=biasConXOutput
                        biasConXOutput=tempBiasConXOutput
                   }
                }
                else if(dataNeuron.type===def.hiddenType){
                    hiddenConXInput[dataNeuron.typeId]=xInput
                    hiddenConXOutput[dataNeuron.typeId]=xOutput
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
                    else if(dataNeuron.type===def.outputType){
                        outputConXInput[dataNeuron.typeId]=xInput
                }

    }

    function dockingYPoint(){
        var yInput
        var yOutput
        yInput=(d/2)+y
        yOutput=(d/2)+y
        if(dataNeuron.type===def.inputType){
                    inputConYOutput[dataNeuron.typeId]=yOutput
                    //console.debug(,inputConYOutput)
                    if(lastNeuron){
                        var tempInputConYOutput=[]
                        tempInputConYOutput=inputConYOutput
                        inputConYOutput=tempInputConYOutput
                   }
                }
                else if(dataNeuron.type===def.biasType){

                    biasConYOutput[dataNeuron.typeId]=yOutput
                    if(lastNeuron){
                        var tempBiasConYOutput=[]
                        tempBiasConYOutput=biasConYOutput
                        biasConYOutput=tempBiasConYOutput
                   }

                }
                else if(dataNeuron.type===def.hiddenType){
                    hiddenConYInput[dataNeuron.typeId]=yInput
                    hiddenConYOutput[dataNeuron.typeId]=yOutput
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
                    else if(dataNeuron.type===def.outputType){
                        outputConYInput[dataNeuron.typeId]=yInput
                        if(lastNeuron){
                            var tempOutputConYInput=[]
                            tempOutputConYInput=outputConYInput
                            outputConYInput=tempOutputConYInput
                    }
                }
    }

*/
    property int transparent: 100
    property string neuronColor: {
                if(visuModus===def.functionVisu) return VisuFunction.color(transparent,def.getTypeColor(dataNeuron.type))
                else{
                    if(dataNeuron.neuronValue<0) return VisuFunction.color(transparent,"8b0000")
                    else return VisuFunction.color(transparent,"006400")}
                }
    width: d
    height: d
    border.width: visuModus===def.combinedVisu ?  d*0.1:d*0.05
    border.color: if(dataNeuron.visuModus===def.combinedVisu) return VisuFunction.color(transparent,def.getTypeColor(dataNeuron.type))
                      else return VisuFunction.color(transparent,"000000")
    radius: d/2
    color:
        if(visuModus===def.functionVisu) return neuronColor
        else dataNeuron.neuronValue===0 ? VisuFunction.color(transparent,"f5f5f5"):Qt.lighter(neuronColor,(1-Math.abs(dataNeuron.neuronValue-0.1))*5)

    Text {
        visible: (parent.d>10)


        font.pixelSize: parent.d*0.2
        horizontalAlignment: Text.AlignHCenter
        text: if(dataNeuron.type>def.noneType) return  "ID:"+dataNeuron.neuronID+"\nValue: \n"+Math.round(dataNeuron.neuronValue*10000)/10000
                else return "Value: \n"+Math.round(dataNeuron.neuronValue*10000)/10000
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color:Math.abs(dataNeuron.neuronValue)>0.75&&netItem.visuIndex!=def.functionVisu ? "white":"black"
    }
    signal clickedNeuron(var id,var type)
    property bool movable: netItem.moveable
    Component.onCompleted: {
        netItem.xRel[dataNeuron.absId]=xRel
        netItem.yRel[dataNeuron.absId]=yRel
    }
    MouseArea{
        id:mouseArea
        anchors.fill: parent
        enabled: netItem.neuronClickEnable
        onClicked: {clickedNeuron(dataNeuron.typeId,dataNeuron.type)
        setNetHighlight(dataNeuron.typeId,dataNeuron.type)
        }
        drag {
                target: if(movable) return parent
                maximumX: totalNet.width-d
                maximumY: totalNet.height-d
                minimumX: 0
                minimumY: 0

            }
        onDragActiveChanged: if(drag){
                                 neuron.xRel=(dropArea.drag.x+(d/2))/parentWidth
                                 neuron.yRel=(dropArea.drag.y+(d/2))/parentHeight
                                 var provX=netItem.xRel
                                 provX[dataNeuron.absId]=neuron.xRel
                                 netItem.xRel=provX
                                 //console.debug(absId,neuron.xRel,netItem.xRel[absId])
                                 var provY=netItem.yRel
                                 provY[dataNeuron.absId]=neuron.yRel
                                 netItem.yRel=provY

                                 ///console.debug(absId,neuron.yRel,netItem.yRel[absId])
                                 netItem.xRel=VisuFunction.updateArray(netItem.xRel)
                                 netItem.yRel=VisuFunction.updateArray(netItem.yRel)
                                 //console.debug(neuron.xRel,dropArea.drag.x,netItem.xRel)
                             }

        property bool dragActive: drag.active
    }

}
