import QtQuick 2.0
import QtQuick 2.13
Rectangle {

    id: layer
    width: ui.distanceX
    height: parent.height
    property int neurons: 6
    property int nextLayerY: 6

    Repeater{
        id: repeater
        property int neurons: layer.neurons
        model: neurons
        width: ui.diameter
        height: ui.height

        Neuron{
            id: neuron
            i: index
            }
    }
    Canvas
    {
        id: drawingCanvas
        x:ui.diameter
        width: layer.width-ui.diameter
        height:layer.height
        onPaint:
        {
            var ctx = getContext("2d")

            ctx.fillStyle = "white"
            ctx.lineWidth = 2;
            ctx.strokeStyle = "green"
            for(var j=0;j<layer.neurons;++j){
            var x1=0
            var y1= j*ui.distanceY+((layer.height-neurons*ui.distanceY)/2)+(ui.diameter/2)
            for(var i=0;i<layer.nextLayerY;++i){
                var x2=width
                var y2=i*ui.distanceY+((layer.height-layer.nextLayerY*ui.distanceY)/2)+(ui.diameter/2)
                ctx.beginPath()
                ctx.moveTo(x1,y1)
                ctx.lineTo(x2,y2)
                ctx.stroke()
                        }
            }

        }
    }

}
