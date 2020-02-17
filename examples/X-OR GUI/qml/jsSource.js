
function neuronClicked(neuronArray,pos) {
    neuronArray=showAllNeuron(neuronArray);
    neuronArray[pos]=1;
    return neuronArray;
}
function connectionClicked(connectionArray,endArray,pos) {
    for(var i=0;i<connectionArray.length;++i){
        if(endArray[i]===pos){
            connectionArray[i]=1
        }
        else{
            connectionArray[i]=0
        }
    }
    return connectionArray
}
function showAllNeuron(neuronArray){
    for(var i=0;i<neuronArray.length;++i){
        neuronArray[i]=2;
    }
    return neuronArray
}
function showAllCon(connectionArray){
    for(var i=0;i<connectionArray.length;++i){
            connectionArray[i]=1;
    }
    return connectionArray;
}
