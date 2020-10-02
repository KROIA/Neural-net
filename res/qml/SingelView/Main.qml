import QtQuick 2.0
import QtQuick.Controls 1.4
import "../Basic"
import "../"

Item {
    id:singelWorkArea
    width: 100
    height: 100
    property int updateTime: 200
    Net{
        id:mainNet
        x:0
        y:0
        anchors.right:singelWorkArea.right// sidebar.left
        anchors.left: singelWorkArea.left
        anchors.top: singelTopBar.bottom
        anchors.bottom: singelWorkArea.bottom
        visuNeuronModus:singelTopBar.visuNeuronModus
        moveable:singelTopBar.moveable
        zoomEnable:true
        visible: parent.visible
        Component.onCompleted: {
            console.debug("load Net new")
        }
    }


    TopBar{
        id:singelTopBar
        anchors.top: singelWorkArea.top
        anchors.right:singelWorkArea.right
        anchors.left:singelWorkArea.left
        height: 100
        visible: parent.visible
    }
/*
    SideBar{
        id:sidebar
        anchors.right:singelWorkArea.right
        anchors.top: singelTopBar.bottom
        showWidth: singelWorkArea.width*0.2
        anchors.bottom: singelWorkArea.bottom
        visible: parent.visible
        //clickedNeuronID: mainNet.clickedNeuronID
        //clickedNeuronType: mainNet.clickedNeuronType
    }*/
}
