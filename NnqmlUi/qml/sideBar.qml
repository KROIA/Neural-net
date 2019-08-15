import QtQuick.Controls 2.0
import QtQuick 2.13
import QtQuick.Controls 1.4
import QtCharts 2.3

Rectangle{
    id:sideBar
    width:100
    height: 800
    Column{
        anchors.fill: parent
        anchors.margins: 4
        Text {
            id:error
            width: 150
            text: "Error "+Math.round(ui.averageError*10000)/100+"%"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 18
        }

        Text {
            id:steps
            width: 150
            text: "Steps "+ui.learningSteps
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 18
        }
        ProgressBar {
            id: stepsProgressbar
            maximumValue: ui.maxLearningSteps
            value: ui.learningSteps
            width: parent.width-10

            }
        Text {
            id: test
            text: {var textF=""
                for(var i=0; i<4; ++i){
                    ui.trainingSetId=i
                    for(var j=0; j<ui.trainingSet.length; ++j){
                        textF=textF+" "+ui.trainingSet[j]}
                    textF=textF+":\t"
                    for(j=0; j<ui.outputSet.length; ++j){
                        textF=textF+" "+ui.outputSet[j]}
                    textF=textF+"\n"
            }

        }
        }

    }


}
