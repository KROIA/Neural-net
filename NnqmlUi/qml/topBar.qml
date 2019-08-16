import QtQuick.Controls 2.0
import QtQuick 2.12
import QtQuick.Controls 1.4

Rectangle {
id:topBar
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
    onClicked: {ui.zoom=ui.zoom-0.1}
    text: "-"
    width: topBar.height
}
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
}
}
