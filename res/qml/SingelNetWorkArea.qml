import QtQuick 2.0
import QtQuick.Controls 1.4
Item {
    id:singelWorkArea
    width: 100
    height: 100
    property int updateTime: 200
    Connections {
                   target: netListVisu
                   onStartUpdateSignal:updateMultiNet()

    }
    Net{
        id:mainNet
        anchors.right: sidebar.left
        anchors.left: singelWorkArea.left
        anchors.top: singelWorkArea.top
        anchors.bottom: singelWorkArea.bottom
    }

    SingelNetSideBar{
        id:sidebar
        anchors.right:singelWorkArea.right
        anchors.top: singelWorkArea.top
        showWidth: singelWorkArea.width*0.2
        height: singelWorkArea.height
    }
}
