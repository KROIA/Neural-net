import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    id:netCircle
    anchors.fill:parent
    property int bigDiameter: 800*ui.zoom
    Repeater{
        model: ui.startNeuron.length
        Connection{
            xStart: //(Math.cos(((360/net.neurons)*ui.startNeuron[index]+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2))+
                    (Math.sin(((360/net.neurons)*ui.startNeuron[index])*(Math.PI/180))*(netCircle.bigDiameter/2)+(netCircle.bigDiameter/2)+200)
            yStart: //(Math.cos(((360/net.neurons)*ui.startNeuron[index]+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2))+
                    (Math.sin(((360/net.neurons)*ui.startNeuron[index]-90)*(Math.PI/180))*(netCircle.bigDiameter/2)+(netCircle.bigDiameter/2)+200)
            xEnd: //(Math.cos(((360/net.neurons)*ui.endNeuron[index]+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2))+
                  (Math.sin(((360/net.neurons)*ui.endNeuron[index])*(Math.PI/180))*(netCircle.bigDiameter/2)+(netCircle.bigDiameter/2)+200)
            yEnd: //(Math.sin(((360/net.neurons)*ui.endNeuron[index]+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2))+
                  (Math.sin(((360/net.neurons)*ui.endNeuron[index]-90)*(Math.PI/180))*(netCircle.bigDiameter/2)+(netCircle.bigDiameter/2)+200)
            weight: ui.connectionWeight[index]
            value: ui.neuronValueVect[ui.startNeuron[index]]
            connectionVisibilaty: net.connectionVisible[index]
        }
    }
    PathView {
        id:pathView
        width:netCircle.bigDiameter
        height: netCircle.bigDiameter
        x:200
        y:200
        model: net.neurons

        delegate: Neuron{
            id:neuron
            neuronid: index
            diameter: net.diameter
            neuronValue: ui.neuronValueVect[index]
            connectionInputX: Math.cos(((360/net.neurons)*index+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2)
            connectionInputY: Math.sin(((360/net.neurons)*index+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2)
            connectionOutputX: connectionInputX
            connectionOutputY: connectionInputY
            //neuronVisibilaty: net.neuronVisible[index]
            onClicked: {
                net.neuronClicked(index)
            }
            onReleased: {
                net.neuronReleased(index)
            }

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
                radiusX: pathView.width/2
                radiusY: pathView.height/2
                useLargeArc: true
            }

        }
    }
}
    /*PathView {
        id:biaspathView
        width: 640*net.zoom
        height: 640*net.zoom
        x:80
        y:80
        model: neurons
        delegate: Neuron{
            id:biasneuron
            neuronid: index
            neuronValue: ui.neuronValueVect[index]
            connectionInputX: {
                net.connectionStartXPoint[index]=Math.cos(((360/neurons)*index+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2)+biasneuron.x+pathView.x
                net.connectionEndXPoint[index]=net.connectionStartXPoint[index]
                return Math.cos(((360/neurons)*index+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2)}
            connectionInputY: {
                net.connectionStartYPoint[index]=Math.sin(((360/neurons)*index+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2)+biasneuron.y+pathView.y
                net.connectionEndYPoint[index]=net.connectionStartYPoint[index]
                return Math.sin(((360/neurons)*index+90)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2)}
            connectionOutputX: connectionInputX
            connectionOutputY: connectionInputY
            biasx: {return Math.sin(((360/neurons)*index)*(Math.PI/180))*(net.diameter/2)+(net.diameter/2)}

            }
        }
}
        /*path: Path {
            id: biaspath
            startX: biaspathView.width/2; startY: 0

            PathArc {
                x: biaspathView.width/2; y: biaspathView.height
                radiusX: biaspathView.width/2; radiusY: biaspathView.height/2
                useLargeArc: true
            }
            PathArc {
                x: biaspathView.width/2; y:0
                radiusX: biaspathView.width/2; radiusY: biaspathView.height/2
                useLargeArc: true
            }
        }
    }

//}*/
