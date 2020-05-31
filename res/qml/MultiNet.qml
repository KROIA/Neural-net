import QtQuick 2.0
import QtQuick.Controls 1.4

Item{
    id:multiNet
    width: 600
    height: 600
    property int xNetPerTab: 3
    property int yNetPerTab: 3
    property int totalNet: 0
    property int tabIndex:0
    property int updateTime: 100
    property bool enableUpdateTimer: true
    property bool forceTimer: false
    property int maxTab: Math.ceil(totalNet/(xNetPerTab*yNetPerTab))
    property int minTab: 0
    Component.onCompleted: updateMultiNet()
    property int clickedNetId: 0

    MouseArea{
        anchors.fill: parent
        onWheel: {
                if (wheel.angleDelta.y > 0)
                {
                    if(multiNet.tabIndex>multiNet.minTab)
                        multiNet.tabIndex--
                }
                else
                {
                    if(multiNet.tabIndex<multiNet.maxTab-1) multiNet.tabIndex++

                }
        }
    }
    Connections {
                   target: netListVisu
                   onStartUpdateSignal:updateMultiNet()
    }
                Repeater{
                    model:{
                        if(totalNet-tabIndex*(yNetPerTab*xNetPerTab)>=yNetPerTab*xNetPerTab) {
                            return yNetPerTab*xNetPerTab}
                        else return totalNet-tabIndex*(yNetPerTab*xNetPerTab)
                    }
                    Net{
                        id:net
                        x: (index%xNetPerTab)*width
                        y: Math.floor(index/xNetPerTab)*height
                        width: multiNet.width/xNetPerTab
                        height: multiNet.height/yNetPerTab
                        netID: index+tabIndex*(yNetPerTab*xNetPerTab)
                        updateTime:multiNet.updateTime
                        enableMousArea: true
                        onClickedNet: clickedNetId=id
                        enableUpdateTimer:multiNet.enableUpdateTimer
                        forceTimer:multiNet.forceTimer
                        }
                    }
    function updateMultiNet(){
               totalNet= netListVisu.getNetCount()
            }

}


