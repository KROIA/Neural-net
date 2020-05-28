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
    Connections {
                   target: netListVisu
                   onStartUpdateSignal:updateMultiNet()
    }

    signal updateNet()


                Repeater{
                    model:{
                        console.debug(totalNet,tabIndex*(yNetPerTab*xNetPerTab),yNetPerTab*xNetPerTab)
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
                        netID: 0//index+tabIndex*(yNetPerTab*xNetPerTab)
                        updateTime:multiNet.updateTime
                        }
                    }
    function updateMultiNet(){
               totalNet= netListVisu.getNetCount()
                console.debug(totalNet)
            }
}


