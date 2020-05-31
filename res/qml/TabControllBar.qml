import QtQuick 2.2
import QtQuick.Extras 1.4
import QtQuick.Controls 2.5
import QtQuick.Controls 2.13
Rectangle {
    id:tabControllBar
    property variant showContent: chooseTab(0)
    property variant texts: ["",""]
    width: 800
    height: 25
    color: "black"
    property int show: hideButton.checked
    Row{
        x:0
        y:0
        ToggleButton{
            id:hideButton
            text:hideButton.checked ? ">" : "<"

            height: tabControllBar.height
            width: tabControllBar.height
            style: TabBarSideButton{
                textRotation:-90
            }
        }
        TabBar{
            contentHeight:tabControllBar.height
            height: tabControllBar.height
            contentWidth:100
            width: tabControllBar.width
            Repeater{
                model:texts.length
                TabButton{
                    id:multiNetButton
                    checked: showContent[index]
                    onPressedChanged: {chooseTab(index)
                        hideButton.checked=true
                    }
                    height: tabControllBar.height
                    width: 110
                    contentItem: Text{
                        color: "black"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: texts[index]
                        font.pixelSize: 13

                    }

                    background:
                        Rectangle{
                            Rectangle{
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                width: 2

                            }
                            anchors.fill: parent
                            color: parent.checked ? "silver" : "darkgrey"
                            border.width: 0
                    }


                }
            }
        }
    }
    function chooseTab(index){
        var temp=[]
        for(var i=0;i<texts.length;i++){
            if(index===i)temp[i]=true
            else temp[i]=false
        }

        showContent=temp
    }
}
