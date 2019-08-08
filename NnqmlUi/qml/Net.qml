import QtQuick.Controls 2.0
import QtQuick 2.13

Rectangle {
    id: net
    width: (ui.hiddenX+2)*distanceX
    height: (ui.hiddenY+2)*distanceY
    property int diameter: 100*ui.zoom
    property int distanceY: 200*ui.zoom
    property int distanceX: 400*ui.zoom
    property real zoom: 1

    Repeater{
        id: hiddenrepeater
        model:ui.hiddenX
        Layer{
            id: hidden
            x: (1+index*net.distanceX)+net.diameter
            y:0
            neurons: ui.hiddenY
            layerid: {

                ui.layerId=index+1
                ui.neuronId=0
                return index+1
                 }
            lastLayerY: {

                        if(index==0){
                            return ui.input
                        }
                        else{
                            return ui.hiddenY
                        }
            }
        }
    }
    Repeater{
        id: inputOutput
        model:2
        Layer{
            id: inputOutputLayer
            neurons: {if(index==0){
                        return ui.input}
                      else{
                        return ui.output
                    }
                }
            lastLayerY: index*ui.hiddenY
            x: (ui.hiddenX*net.distanceX+net.diameter)*index
            y:0
            layerid: (ui.hiddenX+1)*index
            width: if(index==0){
                       return net.diameter}
                     else{
                       return net.distanceX
                   }
        }
    }
}
