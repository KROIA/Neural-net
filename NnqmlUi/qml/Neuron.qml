import QtQuick.Controls 2.0
import QtQuick 2.12

Rectangle {
    id: neuron
    property int i: 1
    property int totalHeight: parent.height
    property int totalWidth: parent.width
    property int layerX: 0
    property int neuronid: 0
    property int connectionInputX: 0
    property int connectionInputY: height/2
    property int connectionOutputX: width
    property int connectionOutputY: height/2
    property real neuronValue:{
        var neuronValue=2
        ui.layerId=layerX
        ui.neuronId = i
        neuronValue=ui.neuronValue

        //if(i==0){
         //   drawingCanvas.requestPaint()}
        return neuronValue
    }

    property string neuronColor:if(neuronValue<0){
                                      return"darkred";
                                        }
                                  else
                                        {
                                      return"darkgreen";
                                        }

    width: net.diameter
    height: net.diameter
    border.color: "black"
    color:{
        if(neuronValue!=0){
               return Qt.lighter(neuronColor,(1-Math.abs(neuronValue))*4);
                }
        else{
            return "white";
        }

    }

    border.width: 1
    radius: net.diameter*0.5
    Text {
        id: text
            anchors.fill: parent
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            height: parent.textHeight
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 10*ui.zoom
            text: neuron.neuronid+ " id \n"+Math.round(neuron.neuronValue*1000000)/1000000
            color:if(Math.abs(neuron.neuronValue)>0.8){
                      return "white"
                  }
                  else{
                      return "black"
                  }
        }
    Rectangle{
        x: neuron.connectionInputX
        y: neuron.connectionInputY-5
        width:10
        height: 10
        color: "blue"
    }
    Rectangle{
        x: neuron.connectionOutputX
        y: neuron.connectionOutputY-5
        width:10
        height: 10
        color: "yellow"
    }
}
