import QtQuick 2.9
import Mainwindowclass 1.0
import QtQuick 2.13

Item {
    id: window
    visible: true
    width: 640
    height: 480
    Ui{
        id:ui
        width: window.width
        height: window.height
        property int diameter: (height/3)/hiddenY
        property int distanceY: (height/2)/hiddenY
        property int hiddenX: 3
        property int hiddenY: 8
        property int output: 4
        property int input: 5
        property int distanceX: width/(hiddenX+4)

        Layer{
            id: input
            neurons: ui.input
            nextLayerY: ui.hiddenY
            x: ui.distanceX
        }
        Repeater{
            id: hiddenrepeater
            model:ui.hiddenX
            Layer{
                id: hidden
                x: (index+2)*ui.distanceX
                neurons: ui.hiddenY
                nextLayerY:{ if(index<ui.hiddenX-1){
                                nextLayerY=ui.hiddenY
                            }
                            else{
                                nextLayerY=ui.output
                            }
                }
            }
        }
        Layer{
            id: output
            x: ui.distanceX*(ui.hiddenX+2)
            neurons: ui.output
            nextLayerY: 0
        }
    }
}
