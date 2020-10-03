import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import "../Basic"
import "../"
import "../BasicTemplate" as Template
Template.TopBar {
    id: topBar
    property int visuNeuronModus: modus.currentIndex
    property bool moveable: moveSwitch.checked
    property int updateModus: updateTimer.currentIndex
    property int updateInterval: updateIntervalSpinBox.value
    Row{
        spacing: 10
        padding:10
        anchors.top: topBar.top
        anchors.bottom: topBar.bottom
        Switch{id:moveSwitch
            text: "moveable"
            anchors.verticalCenter: parent.verticalCenter
        }
        ComboBox {
            id:modus
            model: ["Value", "Function", "Combined"]
            anchors.verticalCenter: parent.verticalCenter
        }
        Button{
            text:"save Netlayout"
            onClicked: {

                netListVisu.saveRelPos(mainNet.xRel,mainNet.yRel,mainNet.netID)
            }
            anchors.verticalCenter: parent.verticalCenter
        }
        Button{
            text:"save Netlayout as"
            onClicked: {
                saveFileDialog.open()
                netListVisu.saveRelPos(mainNet.xRel,mainNet.yRel,mainNet.netID)
            }
            anchors.verticalCenter: parent.verticalCenter
        }
        Button{
            text:"load Netlayout"
            onClicked: {
                mainNet.loadLayout()
            }
            anchors.verticalCenter: parent.verticalCenter
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
            anchors.verticalCenter: parent.verticalCenter
        }

        ComboBox {
            id:updateTimer
            model: [ "interval","real time", "pause"]
            anchors.verticalCenter: parent.verticalCenter
        }
        SpinBox{
            id:updateIntervalSpinBox
            editable :true
            value:500
            visible: updateTimer.currentIndex==def.interval
            from: 0
            to: 10000//Math.floor(itemTopBar.totalNet/(yAxisItem.value*xAxisItem.value))
            anchors.verticalCenter: parent.verticalCenter
        }
        Label{
            text: "ms"
            font.pixelSize:  15
            verticalAlignment:Text.AlignVCenter
            visible: updateTimer.currentIndex==def.interval
            anchors.verticalCenter: parent.verticalCenter
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
            selectMultiple  :true
            folder: 	shortcuts.documents
        }

    }
}
