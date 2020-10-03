import QtQuick 2.0
import "../../js/VisuFunction.js" as VisuFunction
Item {
    property int netId: 0
    onNetIdChanged: {
        updateStructur()
    }


    property bool forceTimer: false

    property int hiddenNeuronX: 0
    property int hiddenNeuronY: 0
    property int totalHidden: 0
    property int inputNeuron: 0
    property int outputNeuron: 0
    property int totalConns: 0
    property int clickedNeuronID: -1
    property int clickedNeuronType: 0

    property bool bias: false
    signal updateStructurSignal()
    Component.onCompleted: {
        updateStructur()
    }

    function updateStructur(){
        inputNeuron=netListVisu.getInputs(netId)
        outputNeuron=netListVisu.getOutputs(netId)
        hiddenNeuronX=netListVisu.getHiddenX(netId)
        hiddenNeuronY=netListVisu.getHiddenY(netId)
        totalHidden=hiddenNeuronX*hiddenNeuronY
        bias=netListVisu.getBias(netId)
        totalConns= netListVisu.getConns(netId)
        updateStructurSignal()
    }
    function getTypeString(type){
        if(type===def.noneType){
            return "noneType"}
        else if(type===def.inputType){
            return "inputType"}
        else if(type===def.hiddenType){
            return "hiddenType"}
        else if(type===def.outputType){
            return "outputType"}
        else if(type===def.biasType){
            return "biasType"}
        else return ""
    }
    function chekArrayForValues(idArr,typeArr,id,type,conIdArr){
        var orgId
        orgId=id
        if(type===def.outputType){
            id +=totalHidden
        }
        else if(type===def.biasType){
            id=0
        }
        var res=[]
        //console.debug(arr1,arr2,val1,val2)
        for(var i=0;i<idArr.length;i++){
            if(idArr[i]===id&&typeArr[i]===type){
                if(type!==def.biasType||Math.floor(conIdArr[i]/hiddenNeuronY)===orgId){
                    res.push(i)}
            }

        }
        return res
    }
}
