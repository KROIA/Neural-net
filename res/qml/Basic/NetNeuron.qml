import QtQuick 2.0

Neuron {
    x: xRel*totalNet.width-(totalNet.d/2)
    y: yRel*totalNet.height-(totalNet.d/2)
    /*onXChanged: {
       console.debug("yRel",yRel,"height",totalNet.height,"width",totalNet.width,"d",totalNet.d)
    }*/
    d:totalNet.d
    visuModus:netItem.visuNeuronModus
    dataNeuron.netId:netItem.netId
    layoutId: netItem.layoutId
    onClickedNeuron: {
        netItem.clickedNeuronID(dataNeuron.absId)
    }
    transparent: 100// biasTransparent[index]

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
    }
}
