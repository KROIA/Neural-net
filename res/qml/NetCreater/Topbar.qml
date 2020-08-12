import QtQuick 2.0
import "../BasicTemplate"as Template
import QtQuick.Controls 2.13

Template.TopBar{
    id:topbar
    width: 100
    height: 100
    property int visuNeuronModus: modus.currentIndex
    property bool moveable: moveSwitch.checked
    property int netId: 0
    Row {
        anchors.fill:parent
        anchors.margins: 10
        spacing: 10
        Switch{id:moveSwitch
            text: "moveable"
            anchors.verticalCenter: parent.verticalCenter
        }
        Button{
            anchors.verticalCenter: parent.verticalCenter
            text: "add Input"
            onClicked: {
                netListVisu.addNewNeuron(netId,def.inputType)
            }
        }
        Button{
            anchors.verticalCenter: parent.verticalCenter
            text: "add Hidden"
            onClicked: {
                netListVisu.addNewNeuron(netId,def.hiddenType)
            }
        }
        Button{
            anchors.verticalCenter: parent.verticalCenter
            text: "add Output"
            onClicked: {
                netListVisu.addNewNeuron(netId,def.outputType)
            }
        }
    }
}

