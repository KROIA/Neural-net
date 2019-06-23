import QtQuick 2.0
import QtQuick 2.13
Rectangle {
    id: neuron
    property int i: 1
    property int totalHeight: parent.height
    property int totalWidth: parent.width
    property int neurons: repeater.neurons
    property int layerX: 0
    width: ui.diameter
    height: ui.diameter
    y: i*ui.distanceY+((parent.height-neurons*ui.distanceY)/2)
    color: "red"
    border.color: "black"
    border.width: 1
    radius: ui.diameter*0.5
}
