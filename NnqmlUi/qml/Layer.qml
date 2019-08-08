import QtQuick 2.0
import QtQuick 2.13

Rectangle {
    id: layer
    width: net.distanceX
    height: parent.height
    property int neurons: 0
    property double lastLayerY: 0
    property int layerid:0
    property double neuronpos: width-net.diameter
    Repeater{
        id: repeater
        model: layer.neurons
        width: parent.width
        height: net.height
        property int layerid: 0
        Neuron{
            id: neuron
            i: index
            y: i*net.distanceY+((parent.height-neurons*net.distanceY)/2)
            neuronid: index
            x:neuronpos
            layerX: layerid
        }
    }
    Neuron{
        id: bias
        y:0
        x:neuronpos
        color: "yellow"
        visible: {
            if(ui.bias==true&&layer.layerid<=ui.hiddenX){
                return true
            }
            else{
                return false
            }
        }
        neuronValue:0
        i:0
    }
    Canvas
    {
        id: drawingCanvas
        width: layer.width-net.diameter
        height:layer.height
        onPaint:
        {
            var ctx = getContext("2d")

            ctx.fillStyle = "white"
            ctx.fillRect(0,0,drawingCanvas.width,drawingCanvas.height)
            for(var j=0;j<layer.neurons;++j){
                var x1=layer.neuronpos
                var y1= j*net.distanceY+((layer.height-neurons*net.distanceY)/2)+(net.diameter/2)
                neuronid: repeater.layerid+j
                ui.layerId=layer.layerid
                ui.neuronId=j
                for(var i=0;i<layer.lastLayerY;++i){
                      ui.connectionId=i
                      ctx.strokeStyle = "black"
                      if(ui.connectionStrength<0){
                           ctx.strokeStyle = "red"
                      }
                      else if(ui.connectionStrength>0){
                           ctx.strokeStyle = "green"
                      }
                      ctx.lineWidth= (Math.abs(ui.connectionStrength)*5+2)*ui.zoom
                      var x2=0
                      var y2=i*net.distanceY+((layer.height-layer.lastLayerY*net.distanceY)/2)+(net.diameter/2)
                      ctx.beginPath()
                      ctx.moveTo(x1,y1)
                      ctx.lineTo(x2,y2)
                      ctx.stroke()
                      }
            }
            if(layer.layerid>0&&ui.bias==true)
            for(j=0;j<layer.neurons;++j){
                x1=layer.neuronpos
                ctx.lineWidth=4
                ctx.strokeStyle = "black"
                y1= j*net.distanceY+((layer.height-neurons*net.distanceY)/2)+(net.diameter/2)
                x2=0
                y2=net.diameter/2
                ctx.beginPath()
                ctx.moveTo(x1,y1)
                ctx.lineTo(x2,y2)
                ctx.stroke()
            }

        }
    }

}
