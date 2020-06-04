function color(transparent,color) {
    var res=[]
    color=color.replace("#","")
    transparent=transparent/100*255
    transparent=Math.round(transparent)
    return "#"+transparent.toString(16)+color
}

function updateArray(arr){
    var temp=[]
    temp=arr
    arr=temp
    return arr
}
