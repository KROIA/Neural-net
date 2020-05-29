import QtQuick 2.0
import QtQuick.Controls 1.4
Item {
    id:singelWorkArea
    width: 100
    height: 100
    Connections {
                   target: netListVisu
                   onStartUpdateSignal:updateMultiNet()
    }
    Net{
        id:mainNet
        anchors.right: leftbar.left
        anchors.left: singelWorkArea.left
        anchors.top: singelWorkArea.top
        anchors.bottom: singelWorkArea.bottom
    }
    ScrollView{
        id:leftbar
        anchors.right:singelWorkArea.right
        anchors.top: singelWorkArea.top
        width: singelWorkArea.width*0.3
        height: singelWorkArea.height
        clip: true
        MultiNet{
            x:0
            y:0
            width: leftbar.width
            height: singelWorkArea.height*0.3*totalNet
            xNetPerTab:1
            yNetPerTab:totalNet
            updateTime: 1000
            clickedNetId:{
                clickedNetId=0
            }

            onClickedNetIdChanged: mainNet.netID=clickedNetId
        }
    }

}
