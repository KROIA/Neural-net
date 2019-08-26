import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
Item {
    id:net
    anchors.fill:parent
    property variant connectionStartXPoint:[0]
    property variant connectionStartYPoint:[0]
    property variant connectionEndXPoint:[0]
    property variant connectionEndYPoint:[0]
    property int diameter: 100*zoom
    property int neurons: ui.hiddenX*ui.hiddenY+ui.input+ui.output
    property int zoom: 1
    Repeater{
        model: net.neurons
        Connection{
            xStart: net.connectionStartXPoint[index]
            yStart: net.connectionStartYPoint[index]
            xEnd: net.connectionEndXPoint[0]
            yEnd: net.connectionEndYPoint[0]
            height: 5
        }
    }
    PathView {
        id:pathView
        width: 400*net.zoom
        height: 400*net.zoom
        x:200
        y:200
        model: neurons
        delegate: Neuron{
            id:neuron
            connectionInputX: {
                net.connectionStartXPoint[index]=Math.cos(((360/neurons)*index+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2)+neuron.x+pathView.x
                net.connectionEndXPoint[index]=net.connectionStartXPoint[index]
                return Math.cos(((360/neurons)*index+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2)}
            connectionInputY: {
                net.connectionStartYPoint[index]=Math.sin(((360/neurons)*index+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2)+neuron.y+pathView.y
                net.connectionEndYPoint[index]=net.connectionStartYPoint[index]
                return Math.sin(((360/neurons)*index+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2)}
            connectionOutputX: connectionInputX
            connectionOutputY: connectionInputY
        }

        path: Path {
            id: path
            startX: pathView.width/2; startY: 0

            PathArc {
                x: pathView.width/2; y: pathView.height
                radiusX: pathView.width/2; radiusY: pathView.height/2
                useLargeArc: true
            }
            PathArc {
                x: pathView.width/2; y:0
                radiusX: pathView.width/2; radiusY: pathView.height/2
                useLargeArc: true
            }
        }
    }

}
