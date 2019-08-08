import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.13
import QtQml 2.13
Dialog {
    id:newNet
    width: 500
    height: 400
    property variant outputSet: [0]
    property variant trainingSet: [0]
    standardButtons: StandardButton.Save | StandardButton.Cancel
    onAccepted: {
        ui.hiddenX=hiddenX.value
        ui.hiddenY=hiddenY.value
        ui.maxLearningSteps=maxSteps.value
        ui.bias=biosCheckbox.checked
        ui.creatNew(maxError.value/100,maxSteps.value)
        for(var i=0;i<4;++i){
            ui.trainingSetId=i
            for(var j = 0;j<2;++j){
                ui.trainingSet[j]=newNet.trainingSet[j+(i*3)]
            }
            ui.outputSet[i]=newNet.outputSet[i*2]

        }
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
        }
        Text {
            text:   "HiddenY"
        }
        SpinBox{
            id: hiddenY
            minimumValue: 1
            decimals: 0
        }
        Text {
            text:   "MaxError"
        }
        SpinBox{
            id: maxError
            minimumValue: 0
            decimals: 4
            suffix: "%"
        }
        Text {
            text:   "MaxSteps"
        }
        SpinBox{
            id: maxSteps
            minimumValue: 1
            maximumValue: 1000000000
        }
        CheckBox{
            id:biosCheckbox
            text: "Bios"
        }
        SpinBox{
            id: bios
            minimumValue: 0
            decimals: 0
            enabled: if(biosCheckbox.checked==true){
                         return true
                     }
                     else{
                         return false
                     }
        }
    }
    GridLayout{
        columns:3
        Repeater{
            model:3*4
            id: repeater
                TextField {
                    id: textField
                    placeholderText: {
                        ui.trainingSetId=index/3
                        var textF ="error"
                        if(index%3==2){
                            newNet.outputSet[Math.round(index/3)]=ui.outputSet[0]
                            textF=ui.outputSet[0]
                        }
                        if(index%3==0||index%3==1){
                            newNet.trainingSet[index%3+(Math.round(index/3)*3)]=ui.trainingSet[index%3]
                            textF=ui.trainingSet[index%3]
                        }
                        return textF
                    }
                    onEditingFinished:{
                        if(index%3==2){
                            newNet.outputSet[Math.round(index/3)]=textField.text
                        }
                        else{
                            newNet.trainingSet[index%3+(Math.round(index/3)*3)]=textField.text
                        }

                    }

                    font.bold: true
                    validator: DoubleValidator {
                            bottom: 1
                            top: 100
                        }
                    inputMethodHints: Qt.ImhDigitsOnly
            }
        }
    }
    }
}
