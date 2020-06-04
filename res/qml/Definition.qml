import QtQuick 2.0

QtObject {
    readonly property int noneType: 0
    readonly property int inputType: 1
    readonly property int hiddenType: 2
    readonly property int outputType: 3
    readonly property int biasType: 5
    readonly property font info: Qt.font({
            family: 'Encode Sans',
            weight: Font.Black,
            italic: false,
            pointSize: 24
        })

}
