function color(transparent,color) {
    var res=[]
    color=color.replace("#","")
    transparent=transparent/100*255
    transparent=Math.round(transparent)
    return "#"+transparent.toString(16)+color
}

function updateArray(arr){
    /*var temp=[]
    temp=arr
    arr=temp*/
    return arr
}

function getAbsIdFromTypeId(id,type,netId){
    if(type===def.input){
        return id
    }
    if(type===def.hiddenType){
        return id+netListVisu.getInputs(netId)
    }
    if(type===def.outputType){
        return id+netListVisu.getHiddenX(netId)*netListVisu.getHiddenX(netId)+netListVisu.getInputs(netId)
    }
    if(type===def.bias){
        return id+netListVisu.getHiddenX(netId)*netListVisu.getHiddenX(netId)+netListVisu.getOutputs(netId)+netListVisu.getInputs(netId)
    }
    return id
}

function getAbsIdFromNeuronId(id,type){
    if(type===def.input){
        return id
    }
    if(type===def.hiddenType||type===def.outputType){
        return id+netListVisu.getInputs(netId)
    }
    if(type===def.bias){
        return id+netListVisu.getHiddenX(netId)*netListVisu.getHiddenX(netId)+netListVisu.getOutputs(netId)+netListVisu.getInputs(netId)
    }
    return id
}

