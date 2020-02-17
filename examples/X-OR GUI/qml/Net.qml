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
            property int zoom: 1
            width: (net.hiddenX()+2)*distanceX
            height: (net.hiddenY()+2)*distanceY
            property bool newConStartSet: false
            property int newConStart: 0
            property int diameter: 100*zoom
            property int distanceY: 200*zoom
            property int distanceX: 400*zoom
            property bool onNeuronClicked: false
            property int choosenNeuron: 0

            //property int hiddenX: netVisu.getHiddenX()
            //property int hiddenY: netVisu.getHiddenY()
            property int inputs: netVisu.getInputs()
            property int outputs: netVisu.getOutputs()
            /*

            /*property variant getStartNeuron:{
                var x= [0]
                for(var i=0;i<netVisu.StartNeuron.length;++i){
                    x[i]=netVisu.StartNeuron[i];
                }
                return x
            }
            property variant getEndNeuron:{
                var x= [0]
                for(var i=0;i<netVisu.EndNeuron.length;++i){
                    x[i]=netVisu.EndNeuron[i];
                }
                return x
            }

            property variant neuronTyp:{
                var x= [0]
                for(var i=0;i<netVisu.NeuronTyp.length;++i){
                    x[i]=netVisu.NeuronTyp[i];
                }
                return x
            }
            property variant connectionWeight:
            {
                            var x= [0]
                            for(var i=0;i<netVisu.ConnectionWeight.length;++i){
                                x[i]=netVisu.ConnectionWeight[i];
                            }
                            return x
                        }
            property variant  neuronValueVect:
            {
                            var x= [0]
                            for(var i=0;i<netVisu.NeuronValueVect.length;++i){
                                x[i]=netVisu.NeuronValueVect[i];
                            }
                            return x
                        }


            property variant neuronVisible:{
                                            var x= [0]
                                            for(var i=0;i<net.neurons;++i){
                                                x[i]=1;
                                            }
                                            return x
            }
            property variant connectionVisible:{
                var x= [0]
                for(var i=0;i<net.connectionWeight.length;++i){
                    x[i]=1;
                }
                return x
            }*/
                   /*
            property variant layerPositionX: [0]
            property variant layerPositionY: [0]
            property variant connectionStartXPoint:[0]
            property variant connectionStartYPoint:[0]
            property variant connectionEndXPoint:[0]
            property variant connectionEndYPoint:[0]
            property int neurons: net.hiddenX*net.hiddenY+net.inputs+net.outputs
           /* NetClassic{

            }*/
            /*function neuronClicked(index){
                if(ui.setNewCon==true){
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
