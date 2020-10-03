import QtQuick 2.0
import QtQuick.Controls 2.4
import "../Basic"
import "../"
Item {
    id:uiIntegratableNet
    width: 100
    height: 100
    property int maxNet: netListVisu.getNetCount()
    property int netId: spinnetId.value
    Component.onCompleted: updateNet();
    Connections {
                   target: netListVisu
                   function onStartUpdateSignal(){
                       updateNet()}
                   }

    Net{
        id:net
        x:0
        y:0
        width:parent.width
        anchors.bottom: miniMenu.top

        netId:  parent.netId
        updateInterval:updateIntervalSpinBox.value
    }
    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.button === Qt.RightButton){
                    contextMenu.popup()
                }

            }
            onPressAndHold: {
                    if (mouse.source === Qt.MouseEventNotSynthesized)
                        contextMenu.popup()
                }
            Menu {
                    id: contextMenu
                    MenuItem { text: miniMenu.visible? "hide Minimenu": "show Minimenu"
                            onTriggered: {
                                miniMenu.visible=!miniMenu.visible
                                if(miniMenu.visible)
                                    miniMenu.height=50
                                else
                                    miniMenu.height=0
                                }
                        }
                    MenuSeparator { }
                    Menu {
                            title: "Update Settings"
                            MenuItem { text: "Interval"
                                    onTriggered: {
                                        net.updateModus=def.interval
                                    }
                            }
                            MenuItem { text: "Realtime"
                                    onTriggered: {
                                               net.updateModus=def.realTime
                                           }
                            }
                            MenuItem { text: "Paus"
                                    onTriggered: {
                                            net.updateModus=def.pause
                                           }
                            }
                        }

                }
    }
    Row{
        id:miniMenu
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 3
        height: 50
        spacing: 5
        Label{
            text: "Net ID"
            font.pixelSize:  15
            verticalAlignment:Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        SpinBox{
            id:spinnetId
            from: 0
            to: maxNet-1
            editable:true
            inputMethodHints: Qt.ImhDigitsOnly
            font.pixelSize: height*0.4
            value:0
            anchors.verticalCenter: parent.verticalCenter
        }
        Label{
            text: "Update Interval"
            font.pixelSize:  15
            verticalAlignment:Text.AlignVCenter
            visible: net.updateModus==def.interval
            anchors.verticalCenter: parent.verticalCenter
        }
        SpinBox{
            id:updateIntervalSpinBox
            editable :true
            visible: net.updateModus==def.interval
            value:500
            from: 0
            to: 10000//Math.floor(itemTopBar.totalNet/(yAxisItem.value*xAxisItem.value))
            anchors.verticalCenter: parent.verticalCenter
        }


    }
    function updateNet(){
               maxNet= netListVisu.getNetCount()
            }
    Definition{
        id:def
    }
}
