import QtQuick.Controls 2.0
import QtQuick 2.12
import QtQuick.Controls 1.4
ScrollView{
    anchors.fill:parent

    Rectangle{
        id: net
        x:50
        y:50
        width: (ui.hiddenX+2)*distanceX
        height: (ui.hiddenY+2)*distanceY
        property int diameter: 100*ui.zoom
        property int distanceY: 200*ui.zoom
        property int distanceX: 400*ui.zoom
        property variant layerPositionX: [0]
        property variant layerPositionY: [0]
        property variant connectionStartXPoint:[0]
        property variant connectionStartYPoint:[0]
        property variant connectionEndXPoint:[0]
        property variant connectionEndYPoint:[0]
        property int neurons: ui.hiddenX*ui.hiddenY+ui.input+ui.output

    NetInCircel{
        visible: if(ui.ansicht==0){
                        return true}
            else
                return false
        }

    NetClassic{
        visible: if(ui.ansicht==1)
                        return true
            else
                return false

    }
    }
}
