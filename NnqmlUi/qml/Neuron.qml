import QtQuick.Controls 2.0
import QtQuick 2.12

Item {
    id: neuron
    signal clicked()
    signal released()
    width: neuron.diameter
    height: neuron.diameter
    property int i: 1
    property int diameter: 50
    property int totalHeight: parent.height
    property int totalWidth: parent.width
    property int layerX: 0
    property int neuronid: 0
    property int neuronVisibilaty: 1
    property int connectionInputX: 0
    property int connectionInputY: height/2
    property int connectionOutputX: width
    property int connectionOutputY: height/2
    property int biasx: 0
    property int biasy: height/2
    property real neuronValue:0
    property string neuronColor:if(neuronValue<0){
                                      return"darkred";
                                        }
                                  else
                                        {
                                      return"darkgreen";
                                        }
    Rectangle{
        id:neuronRect
        anchors.fill: parent

        border.color: if(neuron.neuronVisibilaty==1){
                          return "black"
                      }
                      else if(neuron.neuronVisibilaty==2){
                                  return "grey"
                              }
                     else{
                          return "white"
                      }
        visible: if(neuron.neuronVisibilaty==0){
                     return 0
                 }
                else{
                     return 1
                 }
        color:
            if(neuron.neuronVisibilaty!=0&&neuron.neuronValue!=0){
                   return Qt.lighter(neuron.neuronColor,(1-Math.abs(neuron.neuronValue))*4);
            }
            else{
                return "white"
            }

        border.width: 4
        radius: neuron.diameter*0.5
    }
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
    MouseArea{
        id: mouseArea
        z:1
        anchors.fill: parent
        onClicked: {
                neuron.clicked();
        }
        onReleased: {
            neuron.released();
        }
    }

}
