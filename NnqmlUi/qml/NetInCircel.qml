import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
Item {
    id:net
    anchors.fill:parent
    property int diameter: 50*zoom
    property int neurons: ui.hiddenX*ui.hiddenY+ui.input+ui.output
    property int zoom: 1
    PathView {
        id:pathView
        width: 200*net.zoom
        height: 200*net.zoom
        anchors.centerIn: parent
        model: neurons
        delegate: Neuron{
            connectionInputY: (net.diameter/2)*Math.sin(((360/neurons*index)+90)*(Math.PI/180))
            connectionInputX: net.diameter-(net.diameter/2)*Math.cos(((360/neurons*index)+90)*(Math.PI/180))
            connectionOutputX: connectionInputX
            connectionOutputY: connectionInputY
        }

        path: Path {
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
