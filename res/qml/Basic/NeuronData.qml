import QtQuick 2.0
import "../../js/VisuFunction.js" as Logic
Item {
    property real neuronValue: 0
    property int netId: 0
    property int neuronID: {
        if(type===def.outputType||type===def.hiddenType){
            return netListVisu.getNeuronID(netId,typeId,type)
        }
        return typeId
    }
    property int typeId:0
    property int type: 0
    property int absId: {
        return Logic.getAbsIdFromTypeId(typeId,type,netId)
    }

    function updateValue(){
        neuronValue=netListVisu.getNeuronValue(netId,typeId,type)
    }
}
