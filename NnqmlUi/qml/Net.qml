import QtQuick.Controls 2.0
import QtQuick 2.12
import QtQuick.Controls 1.4
import "jsSource.js" as Source
Item {
    id: netItem
    anchors.fill: parent
    ScrollView{
        anchors.fill:parent
        Rectangle{


            id: net
            x:50
            y:50
            width: (ui.hiddenX+2)*distanceX
            height: (ui.hiddenY+2)*distanceY
            property bool newConStartSet: false
            property int newConStart: 0
            property int diameter: 100*ui.zoom
            property int distanceY: 200*ui.zoom
            property int distanceX: 400*ui.zoom
            property bool onNeuronClicked: false
            property int choosenNeuron: 0
            property variant neuronVisible:{
                                            var x= [0]
                                            for(var i=0;i<net.neurons;++i){
                                                x[i]=1;
                                            }
                                            return x
            }
            property variant connectionVisible:{
                var x= [0]
                for(var i=0;i<ui.connectionWeight.length;++i){
                    x[i]=1;
                }
                return x
            }
            property variant layerPositionX: [0]
            property variant layerPositionY: [0]
            property variant connectionStartXPoint:[0]
            property variant connectionStartYPoint:[0]
            property variant connectionEndXPoint:[0]
            property variant connectionEndYPoint:[0]
            property int neurons: ui.hiddenX*ui.hiddenY+ui.input+ui.output

            NetInCircel{
                visible: if(ui.ansicht==0){
                                return true}
                    else
                        return false
                }

            NetClassic{
                visible: if(ui.ansicht==1)
                                return true
                    else
                        return false

            }
            /*function neuronClicked(index){
                /*if(ui.setNewCon==true){
                    net.neuronVisible=Source.showAllNeuron(net.neuronVisible);
                    net.connectionVisible=Source.showAllCon(net.connectionVisible)
                    if(net.newConStartSet==false&&index>ui.input){
                        net.newConStart=index;
                        net.newConStartSet=true;
                    }
                    if(net.newConStartSet==true&&index>ui.input){
                        ui.addConnection(net.newConStart-ui.input,index-ui.input);
                        ui.setNewCon=false;
                        net.newConStartSet=false;
                        net.connectionVisible[net.connectionVisible.length]=true
                    }
                }
                else{
                    if(index!==net.choosenNeuron){
                        net.onNeuronClicked=true
                        net.connectionVisible=Source.connectionClicked(net.connectionVisible,ui.endNeuron,index)
                        net.neuronVisible=Source.neuronClicked(net.neuronVisible,index)
                        net.choosenNeuron=index;
                    }
                    else{

                        net.neuronVisible=Source.showAllNeuron(net.neuronVisible)
                        net.connectionVisible=Source.showAllCon(net.connectionVisible)
                        net.choosenNeuron=net.neurons+1
                    }
                }
            }
            function neuronReleased(index){
                net.onNeuronClicked=false
            }*/

        }



    }
}
