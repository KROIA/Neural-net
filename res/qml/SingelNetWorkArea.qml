import QtQuick 2.0
import QtQuick.Controls 1.4
Item {
    id:singelWorkArea
    width: 100
    height: 100
    property int updateTime: 200
    visible:!singelTopBar.multiView
    Connections {
                   target: netListVisu
                   onStartUpdateSignal:updateMultiNet()

    }
    Net{
        id:mainNet
        anchors.right: sidebar.left
        anchors.left: singelWorkArea.left
        anchors.top: singelTopBar.bottom
        anchors.bottom: singelWorkArea.bottom
        visuNeuronModus:singelTopBar.visuNeuronModus
        moveable:singelTopBar.moveable

    }
    SingelNetTopBar{
        id:singelTopBar
        anchors.top: singelWorkArea.top
        anchors.right:singelWorkArea.right
        anchors.left:singelWorkArea.left
        height: 100
    }

    SingelNetSideBar{
        id:sidebar
        anchors.right:singelWorkArea.right
        anchors.top: singelTopBar.bottom
        showWidth: singelWorkArea.width*0.2
        height: singelWorkArea.height
        clickedNeuronID: mainNet.clickedNeuronID
        clickedNeuronType: mainNet.clickedNeuronType
    }
}
