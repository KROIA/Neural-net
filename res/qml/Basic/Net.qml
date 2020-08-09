//import QtQuick 2.4
//import QtQuick.Controls 1.4
import QtQuick 2.14
//import QtQuick.Controls 2.4
import QtQuick.Controls 2.14
import QtQuick.Window 2.2
import "../../js/VisuFunction.js" as VisuFunction
NetData {
    id:netItem
    width: 600
    height: 600

    enum NetAlignment{
        Top=0,
        Bottom,
        Center,
        Costum
    }
    signal loadRelPos();
    property int netAlignment: Net.NetAlignment.Top
    property int visuNeuronModus: 0
    property bool enableUpdateTimer: visuNeuronModus===def.functionVisu ? false:true
    property int visu: 0
    property bool enableMousArea: false
    property int clickedneuronID: -1
    property int clickedNeurontype: 0
    property bool moveable: false

    property variant yRel: []
    property variant xRel: []
    property variant hiddenConXInput: []
    property variant hiddenConYInput: []

    property variant hiddenConXOutput: []
    property variant hiddenConYOutput: []

    property variant outputConXInput: []
    property variant outputConYInput: []

    property variant inputConXOutput: []
    property variant inputConYOutput: []

    property variant biasConXOutput: []
    property variant biasConYOutput: []
    property variant hiddenTransparent:  []
    property variant biasTransparent:  []
    property variant inputTransparent:  []
    property variant outputTransparent:  []
    property variant conTransparent:  []

    property bool neuronClickEnable: true
    property real yOffSet: 0.1
    property real xOffSet: 0.1
    property real zoom: 1

    property bool zoomEnable: false
    property int yBiasPos: if(bias) return 0.1
                        else return 0
    property bool showId: true
    signal clickedNet(var id)
    signal updateValue();
    onUpdateStructurSignal:{
        netItem.hiddenTransparent=new Array(totalHidden).fill(100)
        netItem.biasTransparent= new Array(hiddenNeuronX+1).fill(100)
        netItem.inputTransparent= new Array(inputNeuron).fill(100)
        netItem.outputTransparent= new Array(outputNeuron).fill(100)
        netItem.conTransparent= new Array(totalConns).fill(100)
    }
    Component.onCompleted: {
        updateStructur()
    }

    Timer {
        id:timerNet
            interval: updateTime; running: enableUpdateTimer
             repeat: true
            onTriggered: {
                netListVisu.displayUpdatNetTimer(netId)
                updateValue()
            }
        }
    Connections {
                   target: netListVisu
                   function onStopUpdateSignal(){ timerNet.running=false}
                   function onStartUpdateSignal(){timerNet.running=true
                       netItem.updateStructur()}
                    function onSetUpdateTimeSignal(){if(!forceTimer) updateTime=100}
                    function onUpdateVisu(){
                        netListVisu.displayUpdatNetTimer(netId)
                        updateValue()
                    }
    }
    MouseArea{
        id:mous
        anchors.fill: parent
        enabled: enableMousArea
        onClicked: {clickedNet(netId)}
        ScrollView{
                id:scroll
                ScrollBar.vertical.policy:Qt.ScrollBarAlwaysOn
                ScrollBar.horizontal.policy:Qt.ScrollBarAlwaysOn
                anchors.fill: parent
                contentHeight: scroll.height*zoom
                contentWidth: scroll.width*zoom
                ScrollBar.vertical.position:0.8
                Item{
                    id:totalNet
                    width: scroll.contentWidth
                    height:scroll.contentHeight
                    x:0
                    y:0
                    MouseArea{
                            anchors.fill: parent
                            onClicked: setTransparancy(100)
                            hoverEnabled:true
                            property bool zoomActiv: true
                            onWheel: {
                                if(zoomEnable){
                                //console.debug(zoom)
                                        if(zoomActiv) {
                                        //console.debug("scroll")

                                         if (wheel.angleDelta.y < 0)
                                         {
                                             //console.debug("DOWN")
                                             zoom =0.95*zoom

                                         }
                                         else
                                         {
                                             //console.debug("UP")
                                             zoom=1.05*zoom

                                         }

                                         //console.debug(mouseY,scroll.height)
                                          //  console.debug(mouseY/scroll.height)
                                        }
                                         //scroll.ScrollBar.vertical.position=0.5//mouseY/scroll.height
                                         //scroll.ScrollBar.horizontal.position=0.5//mouseX/scroll.width
                                         //totalNet.x:mouseX
                                         //console.debug(zoom)
                                }
                            }

                        }


                property int maxYNeuron: {
                    var b
                    if(bias===true)b=1
                    else b=0
                    if(hiddenNeuronY>=outputNeuron&&hiddenNeuronY>=inputNeuron){
                        return hiddenNeuronY+b
                    }
                    else if(hiddenNeuronY<=outputNeuron&&outputNeuron>=inputNeuron){
                        return outputNeuron+b
                    }
                    else if(hiddenNeuronY<=inputNeuron&&outputNeuron<=inputNeuron){
                        return inputNeuron+b
                    }
                    return 0
                }
                property real dRelationship: 0.8
                property variant yRelInput: []
                property variant yRelHidden: []
                property variant yRelOutput: []
                property int d:{
                        var w
                        if((totalNet.width-(xOffSet*2*totalNet.width))/(hiddenNeuronX+2)>
                            (totalNet.height-(yOffSet*2*totalNet.height))/totalNet.maxYNeuron){
                            w=(totalNet.height-(yOffSet*2*totalNet.height))/totalNet.maxYNeuron
                        }
                        else{
                            w=(totalNet.width-(xOffSet*2*totalNet.width))/(hiddenNeuronX+2)
                        }
                        return w*0.7
                    }
                function calculateYDistance(maxYNeuronRow){
                    if(netAlignment===Net.NetAlignment.Center){
                        return (height-(netItem.yOffSet*height))/(maxYNeuronRow) }//-1+yOffSet*2
                    else{
                        return (height-(xOffset*height))/(maxYNeuron-1+yOffSet*2) }
                }

                DropArea {
                    id:dropArea
                    anchors.fill: parent
                }
                Repeater{
                    model: {
                        return totalConns
                    }
                    NeuronConnection{
                        id:connection
                        conId: index
                        transparent: 100//conTransparent[index]
                        d:totalNet.d
                        netId:netItem.netId
                        Connections{
                            target: netItem
                            function onUpdateValue(){
                                connection.updateValue()
                            }
                        }
                    }
                }
                Repeater{
                    id:biasLayer
                    model: hiddenNeuronX+1
                    visible: bias
                    Neuron{
                        x: xRel*totalNet.width-(d/2)
                        y: yRel*totalNet.height-(d/2)
                        xRel:{
                            var i=index
                            calculateXRelPos(i,dataNeuron.type)}
                        yRel: yOffSet+yBiasPos
                        d:totalNet.d
                        visuModus:visuNeuronModus

                        dataNeuron.typeId: index
                        dataNeuron.type:def.biasType
                        dataNeuron.netId:netItem.netId

                        lastNeuron: (index==(biasLayer.model-1))
                        onClickedNeuron: {
                            clickedNeuronID= dataNeuron.typeId
                            clickedNeuronType= data.type
                        }
                        transparent: 100// biasTransparent[index]
                        Connections{
                            target: netItem
                            function onUpdateValue(){
                                dataNeuron.updateValue()
                            }
                        }
                    }
                }

                Repeater{
                    id:inputLayer
                    model:inputNeuron
                    Neuron{
                        x: xRel*totalNet.width-(d/2)
                        y: yRel*totalNet.height-(d/2)
                        xRel:{
                            var i=0
                            calculateXRelPos(i,dataNeuron.type)}
                        yRel:{
                            var i=inputNeuron
                            return calculateYRelPos(index,i)
                        }
                        d:totalNet.d
                        visuModus:visuNeuronModus
                        dataNeuron.typeId: index
                        dataNeuron.type:def.inputType
                        dataNeuron.netId:netItem.netId

                        lastNeuron: (index==(inputLayer.model-1))
                        onClickedNeuron: {
                            clickedNeuronID= dataNeuron.typeId
                            clickedNeuronType= dataNeuron.type
                        }
                        transparent: 100//inputTransparent[index]
                        Connections{
                            target: netItem
                            function onUpdateValue(){
                                dataNeuron.updateValue()
                            }
                        }
                    }
                }

                Repeater{
                    id:hiddenXLayer
                    model:hiddenNeuronX
                    Repeater{
                        id:hiddenYLayer
                        model:netItem.hiddenNeuronY
                        property int indexX: index
                        Neuron{
                            x: xRel*totalNet.width-(d/2)
                            y: yRel*totalNet.height-(d/2)
                            xRel:{
                                var i=indexX
                                calculateXRelPos(i,dataNeuron.type)}
                            yRel:{
                                var i=hiddenNeuronY
                                return calculateYRelPos(index,i)
                            }
                            visuModus:visuNeuronModus
                            d:totalNet.d
                            dataNeuron.typeId: index+(indexX*hiddenNeuronY)
                            dataNeuron.type:def.hiddenType
                            dataNeuron.netId:netItem.netId

                            lastNeuron: (indexX==(hiddenXLayer.model-1)&&index===(hiddenYLayer.model-1))
                            onClickedNeuron: {
                                clickedNeuronID= dataNeuron.typeId
                                clickedNeuronType= dataNeuron.type
                            }
                            transparent: 100// hiddenTransparent[dataNeuron.typeId]
                            Connections{
                                target: netItem
                                function onUpdateValue(){
                                    dataNeuron.updateValue()
                                }
                            }
                        }
                    }
                }
                Repeater{
                    id:outputLayer
                    model:outputNeuron
                    Neuron{
                        x: xRel*totalNet.width-(d/2)
                        y: yRel*totalNet.height-(d/2)
                        xRel:{
                            var i=0
                            calculateXRelPos(i,dataNeuron.type)}
                        yRel:{
                            var i=outputNeuron
                            return calculateYRelPos(index,i)
                        }
                        visuModus:visuNeuronModus
                        d:totalNet.d
                        dataNeuron.typeId: index
                        dataNeuron.type:def.outputType
                        dataNeuron.netId:netItem.netId

                        lastNeuron: (index==(outputLayer.model-1))
                        onClickedNeuron: {
                            clickedNeuronID= dataNeuron.typeId
                            clickedNeuronType= dataNeuron.type
                        }
                        transparent: 100// outputTransparent[index]
                        Connections{
                            target: netItem
                            function onUpdateValue(){
                                dataNeuron.updateValue()
                            }
                        }

                    }
                }


                Text {
                    anchors.top:parent.top
                    anchors.right: parent.right
                    anchors.rightMargin: totalNet.width*xOffSet
                    anchors.topMargin: yOffSet
                    topPadding: yOffSet
                    text: "net Id: "+netId
                    font.pixelSize: yOffSet
                    visible:showId
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

    }
    function loadLayout(){
        var x=[]
        var y=[]
        x=netListVisu.getRelX(netId)
        y=netListVisu.getRelY(netId)
        if(x.length>0&&y.length>0){
            xRel=netListVisu.getRelX(netId)
            yRel=netListVisu.getRelY(netId)
            loadRelPos()
        }
        else{
            console.debug("found no Layout")
        }
    }

    function calculateXRelPos(pos,type){
        if(type===def.hiddenType){
            pos+=1
        }
        if(type===def.outputType){
            pos+=hiddenNeuronX
            pos+=1
        }
        return (((1-(xOffSet*2))/(hiddenNeuronX+1))*(pos))+xOffSet
    }

    function calculateYRelPos(pos,inARow){
        if(netAlignment===Net.NetAlignment.Center){
            //console.debug((1-yOffSet-(yBiasPos*2)))
            return (((1-yOffSet-(yBiasPos))/(inARow+1))*(pos+1))+yOffSet+(yBiasPos)
        }
        else if(netAlignment===Net.NetAlignment.Top){
            return (((1-yOffSet-(yBiasPos))/(totalNet.maxYNeuron))*(pos+1))+yOffSet+yBiasPos
        }
        else if(netAlignment===Net.NetAlignment.Bottom){
            return (((1-yOffSet-(yBiasPos))/(totalNet.maxYNeuron+1))*(totalNet.maxYNeuron-pos))+yOffSet
        }
        return 0
    }
    function setTransparancy(trans){
        hiddenTransparent= new Array(totalHidden).fill(trans)
        biasTransparent= new Array(hiddenNeuronX+1).fill(trans)
        inputTransparent= new Array(inputNeuron).fill(trans)
        outputTransparent= new Array(outputNeuron).fill(trans)
        conTransparent= new Array(totalConns).fill(trans)

    }
    function updateTransparancy(){
        hiddenTransparent=VisuFunction.updateArray(hiddenTransparent)
        outputTransparent=VisuFunction.updateArray(outputTransparent)
        inputTransparent=VisuFunction.updateArray(inputTransparent)
        biasTransparent=VisuFunction.updateArray(biasTransparent)
        conTransparent=VisuFunction.updateArray(conTransparent)
    }

    function setHighlight(id,type,highlightValue){
        if(type===def.hiddenType){
            hiddenTransparent[id]=highlightValue
        }
        else if(type===def.outputType){
            outputTransparent[id-totalHidden]=highlightValue
        }
        else if(type===def.inputType){
            inputTransparent[id]=highlightValue
        }
        else if(type===def.biasType){
            biasTransparent[id]=highlightValue
        }
    }

    function setNetHighlight(id,type){
        /*var highlightValue=100
        setTransparancy(20)
        if(type===def.outputType) id+=totalHidden
        setHighlight(id,type,highlightValue)
        if(type===def.outputType) id-=totalHidden
        var arrConId=[]
        var sType
        var sId
        arrConId=getConSourceID(id,type)
        for(var i=0;i<arrConId.length;i++){

            sId=conDestinationID[arrConId[i]]
            sType=conDestinationType[arrConId[i]]
            conTransparent[arrConId[i]]=highlightValue
            setHighlight(sId,sType,highlightValue)
        }
        arrConId=getConDestination(id,type)
        for(i=0;i<arrConId.length;i++){
            sId=conSourceID[arrConId[i]]
            sType=conSourceType[arrConId[i]]
            if(sType===def.biasType){
                if(type===def.outputType)sId=hiddenNeuronX
                else sId=Math.floor(id/hiddenNeuronY)
            }
            conTransparent[arrConId[i]]=highlightValue
            setHighlight(sId,sType,highlightValue)
        }
        updateTransparancy()*/
    }
}
