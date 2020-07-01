import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
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
            text:"save Netlayout"
            onClicked: {

                netListVisu.saveRelPos(mainNet.xRel,mainNet.yRel,mainNet.netID)
            }
        }
        Button{
            text:"save Netlayout as"
            onClicked: {
                saveFileDialog.open()
                netListVisu.saveRelPos(mainNet.xRel,mainNet.yRel,mainNet.netID)
            }
        }
        Button{
            text:"load Netlayout"
            onClicked: {
                var x=[]
                var y=[]
                x=netListVisu.getRelX(mainNet.netID)
                y=netListVisu.getRelY(mainNet.netID)
                if(x.length>0&&y.length>0){
                    mainNet.xRel=netListVisu.getRelX(mainNet.netID)
                    mainNet.yRel=netListVisu.getRelY(mainNet.netID)
                    mainNet.loadRelPos()
                }
            }
        }
        Button{
            text:"load Netlayout from"
            onClicked: {
                var x=[]
                var y=[]
                loadFileDialog.open()
                x=netListVisu.getRelX(mainNet.netID)
                y=netListVisu.getRelY(mainNet.netID)
                if(x.length>0&&y.length>0){
                    mainNet.xRel=netListVisu.getRelX(mainNet.netID)
                    mainNet.yRel=netListVisu.getRelY(mainNet.netID)
                    mainNet.loadRelPos()
                }
            }
        }
        FileDialog{
            id:loadFileDialog
            title:  "Load Netlayout from"
            folder: shortcuts.documents
            defaultSuffix : "db"
            selectExisting :true
            nameFilters : "db files (*.db )"
        }
        FileDialog{
            id:saveFileDialog
            title:  "Save Netlayout at"
            defaultSuffix : "db"
            selectExisting :false
            nameFilters : "db files (*.db )"
            selectFolder :true
            folder: 	shortcuts.documents
        }

    }
}
