import QtQuick 2.0
import QtQuick.Extras 1.4
import QtQuick.Controls 2.13
Rectangle{
    id:sidebar
    color: "silver"
    width: 0
    property int showWidth: 100
    property variant showContent: [false,false]
    property int clickedNeuronID: -1
    property int clickedNeuronType:0
    states: [
            State {
                name: "show"
                PropertyChanges { target: sidebarContent; anchors.leftMargin: -showWidth }
                PropertyChanges { target: sidebar; width: showWidth }
                PropertyChanges { target: sidebar; showContent: [true,false]}

            }
        ]
    TabControllBar{
        id:tabBar
        onShowChanged: show ? sidebar.state= 'show':sidebar.state='*'
        width:sidebar.height
        anchors.right: sidebarContent.left

        anchors.top: sidebar.bottom
        transformOrigin: Item.BottomRight
        anchors.rightMargin: height
        rotation: 90
        texts: ["All nets","Net Info"]
    }

    Item{
        id:sidebarContent
        anchors.top: sidebar.top
        anchors.bottom: sidebar.bottom
        anchors.left: sidebar.right
        anchors.leftMargin: 0
        width: showWidth
        MultiNet{
            id:multiNet
            anchors.fill: sidebarContent
            xNetPerTab:1
            yNetPerTab:3
            visible: tabBar.showContent[0]
            updateTime: 1000//singelWorkArea.updateTime
            forceTimer: true
            clickedNetId:0
            onClickedNetIdChanged: mainNet.netID=clickedNetId
        }
        NetInfo{
            anchors.fill: sidebarContent
            visible: tabBar.showContent[1]
            netID: mainNet.netID
            clickedNeuronID:sidebar.clickedNeuronID
            clickedNeuronType: sidebar.clickedNeuronType
        }
    }


}

