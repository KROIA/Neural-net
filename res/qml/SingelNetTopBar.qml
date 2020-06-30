import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
TopBar {
    property int visuNeuronModus: modus.currentIndex
    property bool moveable: moveSwitch.checked

    Row{
        spacing: 10
        padding:10
        Switch{id:moveSwitch
            text: "moveable"
        }
        ComboBox {
            id:modus
            model: ["Value", "Function", "Combined"]
        }
        Button{
            text:"save Netpos"
            onClicked: {
                console.debug("x",mainNet.xRel)
                console.debug("y",mainNet.yRel)
                netListVisu.saveRelPos(mainNet.xRel,mainNet.yRel,mainNet.netID)
            }
        }
        Button{
            text:"load Netpos"
            onClicked: {
                var x=[]
                var y=[]
                x=netListVisu.getRelX(mainNet.netID)
                y=netListVisu.getRelY(mainNet.netID)
                if(x.length>0&&y.length>0){
                    console.debug("x",x)
                    console.debug("y",y)
                    mainNet.xRel=netListVisu.getRelX(mainNet.netID)
                    mainNet.yRel=netListVisu.getRelY(mainNet.netID)
                    mainNet.loadRelPos()
                }
            }
        }

    }
}
