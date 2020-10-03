import QtQuick 2.0

QtObject {
    readonly property int noneType: 0
    readonly property int inputType: 1
    readonly property int hiddenType: 2
    readonly property int outputType: 3
    readonly property int biasType: 5

    readonly property string noneTypeColor: "#fffafa"
    readonly property string inputTypeColor: "#ff6347"
    readonly property string hiddenTypeColor: 	"#ffa500"
    readonly property string outputTypeColor:"#dda0dd"
    readonly property string biasTypeColor: 	"#48d1cc"


    readonly property int valueVisu: 0
    readonly property int functionVisu: 1
    readonly property int combinedVisu: 2
    readonly property font info: Qt.font({
            family: 'Encode Sans',
            weight: Font.Black,
            italic: false,
            pointSize: 24
        })


    readonly property int interval:0
    readonly property int realTime:1
    readonly property int pause:2

    function getTypeColor(type){
        if(type===noneType) return noneTypeColor
        else if(type===inputType)return inputTypeColor
        else if(type===outputType)return outputTypeColor
        else if(type===hiddenType)return hiddenTypeColor
        else if(type===biasType)return biasTypeColor
    }
}
