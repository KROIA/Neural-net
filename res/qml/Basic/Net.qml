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


    ////--------Costumizing Net Property--------////

    //Timer Propery

    signal updateValue();
    property int updateModus: def.interval
    property int updateInterval:100

    //Visual Option
    property bool moveable: false
    property bool neuronClickEnable: true
    property int netAlignment: Net.NetAlignment.Top

    enum NetAlignment{
        Top=0,
        Bottom,
        Center,
        Costum
    }
    signal loadRelPos();
    property int visuNeuronModus: 0

    property bool enableMousArea: false
    property real zoom: 1
    property bool zoomEnable: false
    property bool showId: true

    // Clickevent/properties

    signal clickedNet(var id)
    signal clickedNeuronID(var absId)
    property int clickedneuronID: -1
    property int clickedNeurontype: 0
    signal showConnectedNeuron(var absId)
    signal showAll();

    //Position

    property variant yRel: []
    property variant xRel: []
    property real yOffSet: (1/(maxYNeuron+1))/2
    property real xOffSet: (1/(hiddenNeuronX+2))/2
    property int yBiasPos: if(bias) return 0.1
                        else return 0

    property var inputDockingPoint : [
                Qt.point(0,0),
                Qt.point(100,100)
            ]
    property var outputDockingPoint : [
                Qt.point(0,0),
                Qt.point(100,100)
            ]

    //Neuron auxiliary properties

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

    ///////////////////////////////////////////////////////////////////////////

    Component.onCompleted: {
        updateStructur()
        neuronLoader.active = true
        connectionLoader.active = true
    }

    Timer {
        id:timerNet
            interval: netItem.updateInterval
            running: true
            repeat: true
            onTriggered: {
                if(def.interval==netItem.updateModus){
                    updateNetData()
                }
            }
        }
   Connections {
                   target: netListVisu
                   function onStopUpdateSignal(){
                       timerNet.running=false}
                   function onStartUpdateSignal(){
                       timerNet.running=true
                      }
                    function onSetUpdateTimeSignal(){
                        if(!forceTimer) netItem.updateInterval=100}
                    function onNewValues(){
                        if(updateModus==def.realTime){
                        updateNetData()}
                    }

                    function onUpdateNetStruc(){
                        neuronLoader.active = false
                        connectionLoader.active = false
                        inputDockingPoint=[]
                        outputDockingPoint=[]
                        netItem.updateStructur()
                        /*console.debug("new Config:")
                        console.debug("input ",netItem.inputNeuron)
                        console.debug("output ",netItem.outputNeuron)
                        console.debug("hiddenX", netItem.hiddenNeuronX)
                        console.debug("hiddenY", netItem.hiddenNeuronY)*/
                        neuronLoader.active = true
                        connectionLoader.active = true
                    }
    }
    MouseArea{
        id:mous
        anchors.fill: parent
        enabled: enableMousArea
        onClicked: {clickedNet(netId)
            }
        ScrollView{
                id:scroll
                ScrollBar.vertical.policy:Qt.ScrollBarAlwaysOn
                ScrollBar.horizontal.policy:Qt.ScrollBarAlwaysOn
                anchors.fill: parent
                contentHeight: scroll.height*zoom
                contentWidth: scroll.width*zoom
                ScrollBar.vertical.position:0.8
                Rectangle{
                    id:totalNet
                    width: scroll.contentWidth
                    height:scroll.contentHeight
                    x:0
                    y:0

                    MouseArea{
                            anchors.fill: parent
                            hoverEnabled:true
                            property bool zoomActiv: true
                            onClicked: showAll()
                            onWheel: {
                                if(zoomEnable){
                                      if(zoomActiv) {
                                         if (wheel.angleDelta.y < 0)
                                         {
                                             zoom =0.95*zoom
                                         }
                                         else
                                         {
                                             zoom=1.05*zoom
                                         }
                                        }
                                }
                            }

                        }

                property real dRelationship: 0.8
                property variant yRelInput: []
                property variant yRelHidden: []
                property variant yRelOutput: []
                property int d:{
                        var w
                        if((totalNet.width-(xOffSet*2*totalNet.width))/(hiddenNeuronX+2)>
                            (totalNet.height-(yOffSet*2*totalNet.height))/netItem.maxYNeuron){
                            w=(totalNet.height-(yOffSet*2*totalNet.height))/netItem.maxYNeuron
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
                Loader{
                   id:connectionLoader
                   anchors.fill: parent
                   sourceComponent: connectionComp
                   active: false
                }
                Loader{
                    id:neuronLoader
                    anchors.fill: parent
                    sourceComponent: neuronComp
                    active: false
                }
                Component{
                    id: connectionComp


                Repeater{
                    model: totalConns
                    NeuronConnection{

                        id:connection
                        conId: index
                        transparent: 100//conTransparent[index]
                        d:totalNet.d
                        netId:netItem.netId
                        anchors.fill: parent
                        start:netItem.inputDockingPoint[connection.sourceData.absId];
                        end:netItem.outputDockingPoint[connection.destinationData.absId];
                        Component.onCompleted: {
                                                updateNeurons()
                                           }
                        Connections{
                            target: netItem
                            function onUpdateValue(){
                                connection.updateValue()}

                            function onClickedNeuronID(absID){
                                if(absID===connection.sourceData.absId){
                                    transparent=100
                                    netItem.showConnectedNeuron(connection.destinationData.absId)
                                }
                                else if(absID===connection.destinationData.absId){
                                    transparent=100
                                    netItem.showConnectedNeuron(connection.sourceData.absId)
                                }
                                else{
                                    transparent=50
                                }
                            }
                            function onShowAll(){
                                transparent=100
                            }
                        }
                        Connections {
                                       target: netListVisu
                                        function onUpdateNetStruc(){
                                            updateNeurons()
                                        }
                            }
                        }
                    }
                }
                Component{
                    id: neuronComp
                    Item{

                        id:neuronItem
                        property int lastId: 0
                        property int loadedNeuron: 0
                        property bool finishedLoading: loadedNeuron>=totalHidden+outputNeuron+inputNeuron+netItem.hiddenNeuronX+1
                        Repeater{
                            id:biasLayer
                            model: netItem.hiddenNeuronX+1
                            NetNeuron{
                                xRel:{
                                    var i=index
                                    return calculateXRelPos(i,dataNeuron.type)}
                                yRel: yOffSet+yBiasPos
                                dataNeuron.type:def.biasType
                                lastNeuron:{
                                    //console.debug(lastId,dataNeuron.absId,"||",!finishedLoading,lastId==dataNeuron.absId||!finishedLoading)
                                    return lastId==dataNeuron.absId||!finishedLoading}
                                dataNeuron.typeId: index
                                visible: netItem.bias

                            }
                        }
                        Repeater{
                            id:inputLayer
                            model:netItem.inputNeuron
                            NetNeuron{
                                xRel:{
                                    var i=0
                                    return calculateXRelPos(i,dataNeuron.type)}
                                yRel:{
                                    var i=inputNeuron
                                    return calculateYRelPos(index,i)
                                }
                                dataNeuron.type:def.inputType
                                dataNeuron.typeId: index
                                lastNeuron:{
                                    //console.debug(lastId,dataNeuron.absId,"||",!finishedLoading,lastId==dataNeuron.absId||!finishedLoading)
                                    return lastId==dataNeuron.absId||!finishedLoading}
                            }
                        }

                        Repeater{
                            id:hiddenXLayer
                            model:netItem.hiddenNeuronX
                            Repeater{
                                id:hiddenYLayer
                                model:netItem.hiddenNeuronY
                                property int indexX: index
                                NetNeuron{
                                    xRel:{
                                        var i=indexX
                                        return calculateXRelPos(i,dataNeuron.type)}
                                    yRel:{
                                        var i=hiddenNeuronY
                                        return calculateYRelPos(index,i)
                                    }
                                    dataNeuron.typeId: index+(indexX*hiddenNeuronY)
                                    dataNeuron.type:def.hiddenType
                                    lastNeuron:{
                                        //console.debug(lastId,dataNeuron.absId,"||",!finishedLoading,lastId==dataNeuron.absId||!finishedLoading)
                                        return lastId==dataNeuron.absId||!finishedLoading}
                                }
                            }
                        }
                        Repeater{
                            id:outputLayer
                            model:netItem.outputNeuron
                            NetNeuron{
                                xRel:{
                                    var i=0
                                    return calculateXRelPos(i,dataNeuron.type)}
                                yRel:{
                                    var i=outputNeuron
                                    return calculateYRelPos(index,i)
                                }
                                dataNeuron.type:def.outputType
                                dataNeuron.typeId: index
                                lastNeuron:{
                                    //console.debug(lastId,dataNeuron.absId,"||",!finishedLoading,lastId==dataNeuron.absId||!finishedLoading)
                                    return lastId==dataNeuron.absId||!finishedLoading}
                            }
                        }

                    }
                }

                anchors.fill:parent
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

    function updateNetData(){
        if(!(visuNeuronModus===def.functionVisu)){
            netListVisu.displayUpdatNetTimer(netId)
            updateValue()
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
            //console.debug("y",pos,inARow,(((1-yOffSet-(yBiasPos))/(netItem.maxYNeuron))*(pos+1))+yOffSet+yBiasPos)
            if(bias===true){
                return (((1-yOffSet-(yBiasPos))/(netItem.maxYNeuron))*(pos+1))+yOffSet+yBiasPos
            }
            else{
                return (((1-yOffSet-(yBiasPos))/(netItem.maxYNeuron))*(pos))+yOffSet+yBiasPos
            }
        }
        else if(netAlignment===Net.NetAlignment.Bottom){
            return (((1-yOffSet-(yBiasPos))/(netItem.maxYNeuron+1))*(netItem.maxYNeuron-pos))+yOffSet
        }
        return 0
    }
}
