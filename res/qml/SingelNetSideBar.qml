import QtQuick 2.0
import QtQuick.Extras 1.4

Rectangle{
    id:sidebar
    color: "silver"
    width: 0
    property int showWidth: 100
    property variant showContent: [false,false]
    states: [
            State {
                name: "show"
                PropertyChanges { target: sidebarContent; anchors.leftMargin: -showWidth }
                PropertyChanges { target: sidebar; width: showWidth }
                PropertyChanges { target: sidebar; showContent: [true,false]}

            }
        ]
    Column{
        anchors.top: sidebar.top
        anchors.bottom: sidebar.bottom
        anchors.right: sidebarContent.left
        ToggleButton{
            id:hideButton
            text:hideButton.checked ? "<" : ">"
            onCheckedChanged: checked ? sidebar.state= 'show':sidebar.state='*'
            height: width
            onPressedChanged: checked=true
            style: LeftSideButton{

            }
        }
        ToggleButton{
            id:multiNetButton
            checked: showContent[0]
            onPressedChanged: showContent=[true,false]
            style: LeftSideButton{

            }
            text: "all Nets"
        }
        ToggleButton{
            id:netInfoButton
            checked: showContent[1]
            onPressedChanged: showContent=[false,true]
            style: LeftSideButton{

            }
            text: "Net info"
        }
    }
    Item{
        id:sidebarContent
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.right
        anchors.leftMargin: 0
        width: showWidth
        MultiNet{
            id:multiNet
            anchors.fill: parent
            xNetPerTab:1
            yNetPerTab:3
            visible: multiNetButton.checked
            updateTime: singelWorkArea.updateTime
            clickedNetId:{
                clickedNetId=0
            }
            enableUpdateTimer: false
            onClickedNetIdChanged: mainNet.netID=clickedNetId
        }
    }
}

