import QtQuick 2.0

Item {
    property real neuronValue: 0
    property int netId: 0
    property int neuronID: {
        if(typeId===def.outputType||typeId===def.hiddenType){
            return netListVisu.getNeuronID(netId,typeId,type)
        }
        return typeId
    }
    property int typeId:0
    property int type: 0
    property int absId: {
        if(typeId===def.input){
            return typeId
        }
        if(typeId===def.hiddenType){
            return typeId+netListVisu.getInputs()
        }
        if(typeId===def.outputType){
            return typeId+netListVisu.getHiddenX(netId)*netListVisu.getHiddenX(netId)+netListVisu.getInputs()

        }
        if(typeId===def.bias){
            return typeId+netListVisu.getHiddenX(netId)*netListVisu.getHiddenX(netId)+netListVisu.getOutputs()+netListVisu.getInputs()
        }
        return typeId
    }

    function updateValue(){
        neuronValue=netListVisu.getNeuronValue(netId,typeId,type)
    }

}
