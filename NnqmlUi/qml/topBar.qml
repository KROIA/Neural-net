import QtQuick.Controls 2.0
import QtQuick 2.12
import QtQuick.Controls 1.4

Rectangle {
    id:topBar
    TabView{
        anchors.fill: parent
        Tab{
            title: "View"
            Flow{
                anchors.fill: parent
                anchors.margins: 4
                spacing: 10
            Button{
                id:run
                text:"run"
                onClicked:ui.start();
            }
            Button{
                id:stop
                onClicked:ui.stop();
                text: "stop"
            }

            Button{
                id:reset
                onClicked: ui.reset()
                text: "reset"
            }
            Button{
                id:zoomIn
                onClicked: {ui.zoom=ui.zoom+0.1}
                text: "+"
                width: topBar.height
            }
            Button{
                id:zoomOut
                onClicked: {if(ui.zoom>0.2){ui.zoom=ui.zoom-0.1}}
                text: "-"
                width: topBar.height
            }
            ComboBox{
                model: ListModel {
                    id: netVisOp
                    ListElement { text: "Cicle"}
                    ListElement { text: "Classic"}
                }
                currentIndex: ui.ansicht
                onCurrentIndexChanged: {
                    ui.ansicht=currentIndex;
                }

            }

            }
        }
        Tab{
            title: "Edit"
            Flow{
                anchors.fill: parent
                anchors.margins: 4
                spacing: 10
                Button{
                    id:creatNew
                    onClicked:{
                        ui.stop();
                        var component = Qt.createComponent("CreatNewNet.qml");
                        window.win = component.createObject(window);
                        window.win.open();
                    }
                    text: "create new"
                }
                Button{
                    id:addCon
                    onClicked:  {
                        ui.setNewCon=true
                    }
                    text: "add Connection"
                }
            }
        }
    }


}
