import QtQuick 2.0

Rectangle {
    property int d: 20
    property real neuronValue: 0
    property int neuronID: 0
    property int typeId:0
    property int type: 0
    onXChanged: {
        dockingPoint()
    }
    onYChanged: {      
        dockingPoint()
    }
    onDChanged: {
        dockingPoint()
    }
    property real xOffset: 0.2*d
    function dockingPoint(){
        var xInput
        var yInput
        var xOutput
        var yOutput
        xInput=x+xOffset
        xOutput=d+x-xOffset
        yInput=(d/2)+y
        yOutput=(d/2)+y
        //
        if(type===inputType){
            var tempInputConXOutput=[]
            var tempInputConYOutput=[]
            tempInputConXOutput=inputConXOutput
            tempInputConYOutput=inputConYOutput

            tempInputConXOutput[typeId]=xOutput
            tempInputConYOutput[typeId]=yOutput

            inputConXOutput=tempInputConXOutput
            inputConYOutput=tempInputConYOutput
        }
        else if(type===biasType){
            var tempBiasConXOutput=[]
            var tempBiasConYOutput=[]
            tempBiasConXOutput=biasConXOutput
            tempBiasConYOutput=biasConYOutput

            tempBiasConXOutput[typeId]=xOutput
            tempBiasConYOutput[typeId]=yOutput

            biasConXOutput=tempBiasConXOutput
            biasConYOutput=tempBiasConYOutput
        }
        else if(type===hiddenType){
            var tempHiddenConXInput=[]
            var tempHiddenConYInput=[]
            tempHiddenConXInput=hiddenConXInput
            tempHiddenConYInput=hiddenConYInput

            tempHiddenConXInput[typeId]=xInput
            tempHiddenConYInput[typeId]=yInput

            hiddenConXInput=tempHiddenConXInput
            hiddenConYInput=tempHiddenConYInput
            var tempHiddenConXOutput=[]
            var tempHiddenConYOutput=[]
            tempHiddenConXOutput=hiddenConXOutput
            tempHiddenConYOutput=hiddenConYOutput

            tempHiddenConXOutput[typeId]=xOutput
            tempHiddenConYOutput[typeId]=yOutput

            hiddenConXOutput=tempHiddenConXOutput
            hiddenConYOutput=tempHiddenConYOutput
        }
        else if(type===outputType){
            var tempOutputConXInput=[]
            var tempOutputConYInput=[]
            tempOutputConXInput=outputConXInput
            tempOutputConYInput=outputConYInput

            tempOutputConXInput[typeId]=xInput
            tempOutputConYInput[typeId]=yInput

            outputConXInput=tempOutputConXInput
            outputConYInput=tempOutputConYInput
        }
    }
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
    color: if(neuronValue!=0){
                return Qt.lighter(neuronColor,(1-Math.abs(neuronValue))*4);
         }
         else{
             return "white"
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
