import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.12
import QtQml 2.12
Dialog {
    id:newNet
    width: 500
    height: 400
    property int trainingExamples: 4
    property int inputs: 2
    property int outputs: 1
    property variant outputSet: [0]
    property variant trainingSet: [0]
    standardButtons: StandardButton.Save | StandardButton.Cancel
    onAccepted: {
        for(var i=0;i<4;++i){
            ui.trainingSetId=i
            for(var j = 0;j<newNet.inputs;++j){
                ui.trainingSet[j]=newNet.trainingSet[j+(i*newNet.inputs)]
            }
            for(j = 0;j<newNet.outputs;++j){
                ui.outputSet[j]=newNet.outputSet[j+(i*newNet.outputs)]
            }
            //console.debug("creat:"+newNet.trainingSet[i*2]+" "+newNet.trainingSet[i*2+1]+ " ="+newNet.outputSet[i])
        }
        ui.hiddenX=hiddenX.value
        ui.hiddenY=hiddenY.value
        ui.maxLearningSteps=maxSteps.value
        ui.bias=biosCheckbox.checked
        ui.creatNew(maxError.value/100,maxSteps.value)
        window.update()
        }
    GridLayout{
        anchors.fill: parent
        anchors.margins: 4
        columns:2
    GridLayout {
        id:a
        anchors.margins: 4
        columns:2
        Text {
            text:   "HiddenX"
        }
        SpinBox{
            id: hiddenX
            minimumValue: 1
            decimals: 0
            value: ui.hiddenX
        }
        Text {
            text:   "HiddenY"
        }
        SpinBox{
            id: hiddenY
            minimumValue: 1
            decimals: 0
            value: ui.hiddenY
        }
        Text {
            text:   "MaxError"
        }
        SpinBox{
            id: maxError
            minimumValue: 0
            decimals: 4
            suffix: "%"
            value: 5
        }
        Text {
            text:   "MaxSteps"
        }
        SpinBox{
            id: maxSteps
            minimumValue: 1
            value: ui.maxLearningSteps
            maximumValue: 1000000000
        }
        CheckBox{
            id:biosCheckbox
            text: "Bios"
            checked: true
        }
        ComboBox{
                            id: activationFunc
            model: ListModel {
                ListElement { text: "Linear"}
                ListElement { text: "ReLu"}
                ListElement { text: "Binary"}
                ListElement { text: "Gaussian"}
                ListElement { text: "Sigmoid"}
            }
            currentIndex: 0
        }
        /*SpinBox{
            id: bios
            minimumValue: 0
            decimals: 0

            enabled: if(biosCheckbox.checked==true){
                         return true
                     }
                     else{
                         return false
                     }
        }*/
    }
    Row{

        Column{
            Repeater{
                id: rows
                model: newNet.trainingExamples
                Flow{
                    id: flow
                    property int i: index
                    Repeater{
                        model:newNet.inputs
                        TextField {
                            id: input
                            placeholderText: {
                                ui.trainingSetId=flow.i
                                newNet.trainingSet[index+(flow.i*newNet.inputs)]=ui.trainingSet[index]
                                return newNet.trainingSet[index+(flow.i*newNet.inputs)]
                            }
                            onEditingFinished:{
                                    newNet.trainingSet[index+(flow.i*newNet.inputs)]=parseFloat(input.text)
                            }
                            font.bold: true
                            validator: DoubleValidator {
                                    bottom: 1
                                    top: 100
                                }
                            inputMethodHints: Qt.ImhDigitsOnly
                            width: 50
                        }
                    }
                    Repeater{
                        model:newNet.outputs
                            TextField {
                                id: output
                                placeholderText: {
                                    ui.trainingSetId=flow.i
                                    newNet.outputSet[index+(flow.i*newNet.outputs)]=ui.outputSet[index]
                                    return newNet.outputSet[index+(flow.i*newNet.outputs)]
                                }
                                onEditingFinished:{
                                        newNet.outputSet[index+(flow.i*newNet.outputs)]=parseFloat(output.text)
                                }
                                font.bold: true
                                validator: DoubleValidator {
                                        bottom: -100
                                        top: 100
                                    }
                                inputMethodHints: Qt.ImhDigitsOnly
                                width: 50
                        }
                    }
                }
            }
        }

    }
    }
}
