import QtQuick 2.0
import "../../js/VisuFunction.js" as VisuFunction
Neuron {
    x: xRel*totalNet.width-(totalNet.d/2)
    y: yRel*totalNet.height-(totalNet.d/2)
    d:totalNet.d
    visuModus:netItem.visuNeuronModus
    dataNeuron.netId:netItem.netId
    onClickedNeuron: {
        netItem.clickedNeuronID(dataNeuron.absId)
    }
    transparent: 100// biasTransparent[index]
    Component.onCompleted: {
        if(!finishedLoading){
            lastId=dataNeuron.absId
            loadedNeuron++
        }
        netItem.xRel[dataNeuron.absId]=xRel
        netItem.yRel[dataNeuron.absId]=yRel
        dockingPoint()
    }
    onXChanged:{
        dockingPoint()
    }
    onYChanged:{
        dockingPoint()
    }
    //onDChanged:dockingPoint()
    onXRelChanged: {
        dockingPoint()
    }
    onYRelChanged:{
        dockingPoint()
    }
    Connections{
        target: netItem
        function onUpdateValue(){
            dataNeuron.updateValue()
        }
        function onShowAll(){
            transparent=100
        }

        function onClickedNeuronID(absID){
            if(absID===dataNeuron.absId){
                transparent=100
            }
            else{
                transparent=50
            }
        }
        function onShowConnectedNeuron(absID){
            if(absID===dataNeuron.absId){
                transparent=100
            }
        }
        function onLoadRelPos(){
            xRel=netItem.xRel[dataNeuron.absId]
            yRel=netItem.yRel[dataNeuron.absId]
            dockingPoint()
        }

    }
    function dockingPoint(){
        netItem.inputDockingPoint[dataNeuron.absId]=Qt.point(d+x-(0.2*d),(d/2)+y)
        netItem.outputDockingPoint[dataNeuron.absId]=Qt.point(x+(0.2*d),(d/2)+y)
        if(Drag.active||lastNeuron){
            netItem.inputDockingPoint=VisuFunction.updateArray(netItem.inputDockingPoint)
            netItem.outputDockingPoint=VisuFunction.updateArray(netItem.outputDockingPoint)
        }
    }
}
