import QtQuick 2.0
import QtQuick.Controls 1.4
import "../Basic"
import "../"

FocusScope {
    id:singelWorkArea
    width: 100
    height: 100
    property int updateTime: 200
    visible:!singelTopBar.multiView
    Connections {
                   target: netListVisu
                   function onStartUpdateSignal(){updateMultiNet()}

    }
    Net{
        id:mainNet
        x:0
        y:0
        anchors.right: sidebar.left
        anchors.left: singelWorkArea.left
        anchors.top: singelTopBar.bottom
        anchors.bottom: singelWorkArea.bottom
        visuNeuronModus:singelTopBar.visuNeuronModus
        moveable:singelTopBar.moveable
        zoomEnable:true
    }


    TopBar{
        id:singelTopBar
        anchors.top: singelWorkArea.top
        anchors.right:singelWorkArea.right
        anchors.left:singelWorkArea.left
        height: 100
    }

    SideBar{
        id:sidebar
        anchors.right:singelWorkArea.right
        anchors.top: singelTopBar.bottom
        showWidth: singelWorkArea.width*0.2
        anchors.bottom: singelWorkArea.bottom
        clickedNeuronID: mainNet.clickedNeuronID
        clickedNeuronType: mainNet.clickedNeuronType
    }
}
