import QtQuick 2.0
import "../BasicTemplate"as Template
import QtQuick.Controls 2.13

Template.TopBar{
    id:topbar
    width: 100
    height: 100
    property bool moveable: moveSwitch.checked
    property int netId: 0
    Row {
        anchors.fill:parent
        anchors.margins: 10
        spacing: 15
        Switch{id:moveSwitch
            text: "moveable"

        }
        Column{
            spacing: 10
            anchors.verticalCenter: parent.verticalCenter
            Button{
                text: "remove Input"
                onClicked: {
                    netListVisu.removeInput(netId)
                }
            }
            Button{
                text: "add Input"
                onClicked: {
                    netListVisu.addInput(netId)
                }
            }
        }
        Column{
            spacing: 10
            anchors.verticalCenter: parent.verticalCenter
            Button{
                text: "remove HiddenX"
                onClicked: {
                    netListVisu.removeHiddenX(netId)
                }
            }
            Button{
                text: "add HiddenX"
                onClicked: {
                    netListVisu.addHiddenX(netId)
                }
            }
        }
        Column{
            spacing: 10
            anchors.verticalCenter: parent.verticalCenter
            Button{
                text: "remove HiddenY"
                onClicked: {
                    netListVisu.removeHiddenY(netId)
                }
            }
            Button{
                text: "add HiddenY"
                onClicked: {
                    netListVisu.addHiddenY(netId)
                }
            }
        }
        Column{
            spacing: 10
            anchors.verticalCenter: parent.verticalCenter
            Button{
                text: "remove Output"
                onClicked: {
                    netListVisu.removeOutput(netId)
                }
            }
            Button{
                text: "add Output"
                onClicked: {
                    netListVisu.addOutput(netId)
                }
            }
        }
        Column{
            spacing: 10
            anchors.verticalCenter: parent.verticalCenter
            Switch{
                id:biasEnable
                text: "Bias"
                onCheckedChanged: {
                    if(checked){
                       netListVisu.addBias(topbar.netId)
                    }
                    else{
                       netListVisu.removeBias(topbar.netId)

                    }
                }
            }
            TextField{
                id:biasValue
                enabled: biasEnable.checked
                width: 80
                text: "1 "
                validator: DoubleValidator  {bottom: -10000; top: 10000}
                onTextChanged:{
                    netListVisu.changeBias(topbar.netId,parseFloat(text))
                }
            }

        }

    }
}

