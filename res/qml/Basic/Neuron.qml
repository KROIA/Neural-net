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
    property variant input: Qt.point(0,0)
    property variant output: Qt.point(0,0)
    property int layoutId: 0
    NeuronData{
        id:dataNeuron
    }
    property alias dataNeuron: dataNeuron

    onXChanged:dockingPoint()
    onYChanged:dockingPoint()
    onDChanged:dockingPoint()
    onXRelChanged: dockingPoint()
    onYRelChanged:dockingPoint()
    Drag.active: mouseArea.drag.active
    property real xOffset: 0.2*d
    function dockingPoint(){
        netItem.inputDockingPoint[dataNeuron.absId]=Qt.point(neuron.x+(0.2*neuron.d),(neuron.d/2)+neuron.y)
        netItem.outputDockingPoint[dataNeuron.absId]=Qt.point(neuron.d+neuron.x-(0.2*neuron.d),(neuron.d/2)+neuron.y)
        if(Drag.active||lastNeuron){
           netItem.inputDockingPoint=VisuFunction.updateArray(netItem.inputDockingPoint)
            netItem.outputDockingPoint=VisuFunction.updateArray(netItem.outputDockingPoint)
        }
    }

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
    border.color: if(visuModus===def.combinedVisu) return VisuFunction.color(transparent,def.getTypeColor(dataNeuron.type))
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

    MouseArea{
        id:mouseArea
        anchors.fill: parent
        enabled: netItem.neuronClickEnable
        onClicked: {clickedNeuron(dataNeuron.typeId,dataNeuron.type)

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
