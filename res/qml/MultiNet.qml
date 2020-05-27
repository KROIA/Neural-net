import QtQuick 2.0
import QtQuick.Controls 1.4

Item{
    id:multiNet
    width: 600
    height: 600
    property int xNetPerTab: 3
    property int yNetPerTab: 3
    property int totalNet: 0
    property int updateTime: 100
    property bool updateTimerEnable: false
    Connections {
                   target: netVisu
                   onStopUpdateSignal: timerMultiNet.running=false
                   onStartUpdateSignal:timerMultiNet.running=true
    }
    Timer {
            id:timerMultiNet
            interval: updateTime; running: false; repeat: true
            onTriggered: updateMultiNet()
        }
    signal updateNet()
    TabView {
        anchors.fill:parent
        Repeater{
            model:Math.ceil(totalNet/(yNetPerTab*xNetPerTab))
            Tab{
                id:tab
                title:index
                property int tabIndex:index
                Repeater{
                    model:{
                        if(totalNet-tabIndex*(yNetPerTab*xNetPerTab)>=yNetPerTab*xNetPerTab) return yNetPerTab*xNetPerTab
                        else return totalNet-tabIndex*(yNetPerTab*xNetPerTab)
                    }
                    Net{
                        id:net
                        x: (index%xNetPerTab)*width
                        y: Math.floor(index/xNetPerTab)*height
                        width: tab.width/xNetPerTab
                        height: tab.height/yNetPerTab
                        netID: index+tabIndex*(yNetPerTab*xNetPerTab)
                        updateTime:multiNet.updateTime
                        }
                    }

                }
            }
        }

    function updateMultiNet(){
               totalNet= netVisu.getNetCount()
            }
}


