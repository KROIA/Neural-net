import QtQuick 2.0

Rectangle {
    id:neuron
    property int d: 20
    property real neuronValue: 0
    property int neuronID: 0
    property int typeId:0
    property int type: 0
    property bool lastNeuron: false
    onXChanged: {
        dockingXPoint()
    }
    onYChanged: {      
        dockingYPoint()
    }
    onDChanged: {
        dockingPoint()
    }
    property real xOffset: 0.2*d
    function dockingPoint(){
        dockingYPoint()
        dockingXPoint()
    }
    function dockingXPoint(){
        var xInput
        var xOutput
        xInput=x+xOffset
        xOutput=d+x-xOffset
        if(type===inputType){

                    inputConXOutput[typeId]=xOutput
                    if(lastNeuron){
                        var tempInputConXOutput=[]
                        tempInputConXOutput=inputConXOutput
                        inputConXOutput=tempInputConXOutput
                        //console.debug(inputConXOutput)
                    }
                }
                else if(type===biasType){
                    biasConXOutput[typeId]=xOutput

                    if(lastNeuron){
                        var tempBiasConXOutput=[]
                        tempBiasConXOutput=biasConXOutput
                        biasConXOutput=tempBiasConXOutput
                   }

                }
                else if(type===hiddenType){
                    hiddenConXInput[typeId]=xInput
                    hiddenConXOutput[typeId]=xOutput
                    //console.debug(hiddenConXOutput)

                    if(lastNeuron){
                        var tempHiddenConXOutput=[]
                        var tempHiddenConXInput=[]
                        tempHiddenConXInput=hiddenConXInput
                        tempHiddenConXOutput=hiddenConXOutput
                        hiddenConXInput=tempHiddenConXInput
                        hiddenConXOutput=tempHiddenConXOutput
                    }
                }
                    else if(type===outputType){
                        outputConXInput[typeId]=xInput
                        if(lastNeuron){
                            var tempOutputConXInput=[]
                            tempOutputConXInput=outputConXInput
                            outputConXInput=tempOutputConXInput
                    }
                }

    }

    function dockingYPoint(){
        var yInput
        var yOutput
        yInput=(d/2)+y
        yOutput=(d/2)+y
        if(type===inputType){
                    inputConYOutput[typeId]=yOutput
                    if(lastNeuron){
                        var tempInputConYOutput=[]
                        tempInputConYOutput=inputConYOutput
                        inputConYOutput=tempInputConYOutput
                    }
                }
                else if(type===biasType){

                    biasConYOutput[typeId]=yOutput
                    console.debug( biasConYOutput[0], biasConYOutput[1])
                    if(lastNeuron){
                        var tempBiasConYOutput=[]
                        tempBiasConYOutput=biasConYOutput
                        biasConYOutput=tempBiasConYOutput
                   }

                }
                else if(type===hiddenType){
                    hiddenConYInput[typeId]=yInput
                    hiddenConYOutput[typeId]=yOutput
                    //console.debug(hiddenConYOutput)

                    if(lastNeuron){
                        var tempHiddenConYOutput=[]
                        var tempHiddenConYInput=[]
                        tempHiddenConYInput=hiddenConYInput
                        tempHiddenConYOutput=hiddenConYOutput
                        hiddenConYInput=tempHiddenConYInput
                        hiddenConYOutput=tempHiddenConYOutput
                    }
                }
                    else if(type===outputType){
                        outputConYInput[typeId]=yInput
                        if(lastNeuron){
                            var tempOutputConYInput=[]
                            tempOutputConYInput=outputConYInput
                            outputConYInput=tempOutputConYInput
                    }
                }
    }






        //

    property string neuronColor:if(neuronValue<0){
                                          return"darkred";
                                            }
                                      else
                                            {
                                          return"darkgreen";
                                            }
    width: d
    height: d
    border.width: d*0.05
    border.color: "black"
    radius: d/2
    color:if(neuronValue!=0){
                return Qt.lighter(neuronColor,(1-Math.abs(neuronValue))*4);
         }
         else{
             return "whitesmoke"
         }
    Text {
        visible: (parent.d>10)
        font.pixelSize: parent.d*0.2
        horizontalAlignment: Text.AlignHCenter
        text: if(type>noneType) return  "ID:"+neuronID+"\nValue: \n"+Math.round(neuronValue*10000)/10000
                else return "Value: \n"+Math.round(neuronValue*10000)/10000
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        color:if(Math.abs(neuronValue)>0.6){
                              return "white"
                          }
                          else{
                              return "black"
                          }
    }
}
