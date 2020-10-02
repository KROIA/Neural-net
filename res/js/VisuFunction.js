function color(transparent,color) {
    var res=[]
    color=color.replace("#","")
    transparent=transparent/100*255
    transparent=Math.round(transparent)
    return "#"+transparent.toString(16)+color
}

function updateArray(arr){
    return arr
}

function getAbsIdFromTypeId(id,type,netId){
    var res
    if(type===def.inputType){
        res= id
    }
    else if(type===def.hiddenType){
        res= id+netListVisu.getInputs(netId)
    }
    else if(type===def.outputType){
        //console.debug(id,netListVisu.getHiddenX(netId)*netListVisu.getHiddenY(netId),netListVisu.getInputs(netId))
        res= id+(netListVisu.getHiddenX(netId)*netListVisu.getHiddenY(netId))+netListVisu.getInputs(netId)
    }

    else if(type===def.biasType){

        res= id+(netListVisu.getHiddenX(netId)*netListVisu.getHiddenY(netId))+netListVisu.getOutputs(netId)+netListVisu.getInputs(netId)
    }
    else{
      res= id
    }
    //console.debug(id)
    return res

}

function getAbsIdFromNeuronId(id,type){
    if(type===def.input){
        return id
    }
    if(type===def.hiddenType||type===def.outputType){
        return id+netListVisu.getInputs(netId)
    }
    if(type===def.bias){
        return id+(netListVisu.getHiddenX(netId)*netListVisu.getHiddenY(netId))+netListVisu.getOutputs(netId)+netListVisu.getInputs(netId)
    }
    return id
}

